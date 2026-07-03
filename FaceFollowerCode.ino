#include <my_sensecraft_project.h>
#include "esp_camera.h"
#include "driver/ledc.h"

//Config
#define LEFT_IN1   D0
#define LEFT_IN2   D1
#define LEFT_PWM   D2

#define RIGHT_IN1  D3
#define RIGHT_IN2  D4
#define RIGHT_PWM  D5

#define STBY_PIN   D6

#define PWM_FREQ_HZ     5000
#define PWM_RESOLUTION  8
#define LEDC_CH_LEFT    0
#define LEDC_CH_RIGHT   1

//Tuning
#define BASE_SPEED           150   // constant forward speed (0-255) when tracking a face
#define TURN_GAIN            0.6   // how aggressively to turn per pixel of x offset
#define MAX_MOTOR_SPEED      255   // hard clamp for motor output
#define MIN_DETECTION_SCORE  0.6   // ignore low-confidence detections
#define MAX_FACE_AREA_RATIO  0.35  // 35% of frame area

#define PWDN_GPIO_NUM     -1
#define RESET_GPIO_NUM    -1
#define XCLK_GPIO_NUM     10
#define SIOD_GPIO_NUM     40
#define SIOC_GPIO_NUM     39
#define Y9_GPIO_NUM       48
#define Y8_GPIO_NUM       11
#define Y7_GPIO_NUM       12
#define Y6_GPIO_NUM       14
#define Y5_GPIO_NUM       16
#define Y4_GPIO_NUM       18
#define Y3_GPIO_NUM       17
#define Y2_GPIO_NUM       15
#define VSYNC_GPIO_NUM    38
#define HREF_GPIO_NUM     47
#define PCLK_GPIO_NUM     13


static uint8_t *snapshot_buf = nullptr;
static bool ei_camera_init(void);
static bool ei_camera_capture(uint32_t img_width, uint32_t img_height, uint8_t *out_buf);
static int ei_camera_get_data(size_t offset, size_t length, float *out_ptr);

//Motor control
void driveMotor(int in1Pin, int in2Pin, int ledcChannel, int speed) {
  speed = constrain(speed, -MAX_MOTOR_SPEED, MAX_MOTOR_SPEED);

  if (speed >= 0) {
    digitalWrite(in1Pin, HIGH);
    digitalWrite(in2Pin, LOW);
  } else {
    digitalWrite(in1Pin, LOW);
    digitalWrite(in2Pin, HIGH);
    speed = -speed;
  }
  ledcWrite(ledcChannel, speed);
}

void setMotors(int leftSpeed, int rightSpeed) {
  driveMotor(LEFT_IN1, LEFT_IN2, LEDC_CH_LEFT, leftSpeed);
  driveMotor(RIGHT_IN1, RIGHT_IN2, LEDC_CH_RIGHT, rightSpeed);
}

void stopMotors() {
  setMotors(0, 0);
}

void setupMotors() {
  pinMode(LEFT_IN1, OUTPUT);
  pinMode(LEFT_IN2, OUTPUT);
  pinMode(RIGHT_IN1, OUTPUT);
  pinMode(RIGHT_IN2, OUTPUT);

  if (STBY_PIN >= 0) {
    pinMode(STBY_PIN, OUTPUT);
    digitalWrite(STBY_PIN, HIGH);   
  }

  ledcSetup(LEDC_CH_LEFT, PWM_FREQ_HZ, PWM_RESOLUTION);
  ledcAttachPin(LEFT_PWM, LEDC_CH_LEFT);

  ledcSetup(LEDC_CH_RIGHT, PWM_FREQ_HZ, PWM_RESOLUTION);
  ledcAttachPin(RIGHT_PWM, LEDC_CH_RIGHT);

  stopMotors();
}

//Loop
void setup() {
  Serial.begin(115200);
  while (!Serial && millis() < 3000) { /* wait briefly for serial monitor */ }

  Serial.println("Face Following Robot - starting up");

  setupMotors();

  if (!ei_camera_init()) {
    Serial.println("Camera init failed - halting");
    while (true) { delay(1000); }
  }
  Serial.printf("Camera initialized. Model input size: %d x %d\n",
                EI_CLASSIFIER_INPUT_WIDTH, EI_CLASSIFIER_INPUT_HEIGHT);
}

void loop() {
  snapshot_buf = (uint8_t *)malloc(EI_CLASSIFIER_INPUT_WIDTH * EI_CLASSIFIER_INPUT_HEIGHT * 3);
  if (!snapshot_buf) {
    Serial.println("Failed to allocate snapshot buffer");
    delay(500);
    return;
  }

  if (!ei_camera_capture(EI_CLASSIFIER_INPUT_WIDTH, EI_CLASSIFIER_INPUT_HEIGHT, snapshot_buf)) {
    Serial.println("Camera capture failed");
    free(snapshot_buf);
    stopMotors();
    return;
  }

  ei::signal_t signal;
  signal.total_length = EI_CLASSIFIER_INPUT_WIDTH * EI_CLASSIFIER_INPUT_HEIGHT;
  signal.get_data = &ei_camera_get_data;

  ei_impulse_result_t result = { 0 };
  EI_IMPULSE_ERROR err = run_classifier(&signal, &result, false /* debug */);
  free(snapshot_buf);

  if (err != EI_IMPULSE_OK) {
    Serial.print("Classifier failed with error code: ");
    Serial.println(err);
    stopMotors();
    return;
  }

  //Nearest face
  bool faceFound = false;
  long largestArea = 0;
  int nx = 0, ny = 0, nw = 0, nh = 0;

#if EI_CLASSIFIER_OBJECT_DETECTION == 1
  for (uint32_t i = 0; i < result.bounding_boxes_count; i++) {
    ei_impulse_result_bounding_box_t bb = result.bounding_boxes[i];
    if (bb.value < MIN_DETECTION_SCORE) continue;   // ignore low-confidence detections

    long area = (long)bb.width * (long)bb.height;
    if (area > largestArea) {
      largestArea = area;
      nx = bb.x;
      ny = bb.y;
      nw = bb.width;
      nh = bb.height;
      faceFound = true;
    }
  }
#else
  #error "This face-detection model export is not configured as an object detection model. Re-export from SenseCraft as an object-detection (FOMO) model."
#endif

  int acceleration = 0;
  int turn = 0;

  if (faceFound) {
    long frameArea = (long)EI_CLASSIFIER_INPUT_WIDTH * (long)EI_CLASSIFIER_INPUT_HEIGHT;
    float areaRatio = (float)largestArea / (float)frameArea;

    if (areaRatio < MAX_FACE_AREA_RATIO) {
      int faceCenterX = nx + (nw / 2);
      int frameCenterX = EI_CLASSIFIER_INPUT_WIDTH / 2;
      int xOffset = faceCenterX - frameCenterX;  

      acceleration = BASE_SPEED;
      turn = (int)(xOffset * TURN_GAIN);
    } else {
      acceleration = 0;
      turn = 0;
    }
  } else {
    acceleration = 0;
    turn = 0;
  }

  int leftSpeed  = acceleration + turn;
  int rightSpeed = acceleration - turn;

  setMotors(leftSpeed, rightSpeed);

  Serial.print("face=");
  Serial.print(faceFound);
  Serial.print(" accel=");
  Serial.print(acceleration);
  Serial.print(" turn=");
  Serial.print(turn);
  Serial.print(" L=");
  Serial.print(leftSpeed);
  Serial.print(" R=");
  Serial.println(rightSpeed);
}

static bool ei_camera_init(void) {
  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_2;  
  config.ledc_timer   = LEDC_TIMER_2;
  config.pin_d0       = Y2_GPIO_NUM;
  config.pin_d1       = Y3_GPIO_NUM;
  config.pin_d2       = Y4_GPIO_NUM;
  config.pin_d3       = Y5_GPIO_NUM;
  config.pin_d4       = Y6_GPIO_NUM;
  config.pin_d5       = Y7_GPIO_NUM;
  config.pin_d6       = Y8_GPIO_NUM;
  config.pin_d7       = Y9_GPIO_NUM;
  config.pin_xclk     = XCLK_GPIO_NUM;
  config.pin_pclk     = PCLK_GPIO_NUM;
  config.pin_vsync    = VSYNC_GPIO_NUM;
  config.pin_href     = HREF_GPIO_NUM;
  config.pin_sccb_sda = SIOD_GPIO_NUM;
  config.pin_sccb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn     = PWDN_GPIO_NUM;
  config.pin_reset    = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;
  config.frame_size   = FRAMESIZE_QVGA;   
  config.jpeg_quality = 12;
  config.fb_count     = 2;
  config.fb_location  = CAMERA_FB_IN_PSRAM;
  config.grab_mode    = CAMERA_GRAB_LATEST;

  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x\n", err);
    return false;
  }
  return true;
}

static bool ei_camera_capture(uint32_t img_width, uint32_t img_height, uint8_t *out_buf) {
  camera_fb_t *fb = esp_camera_fb_get();
  if (!fb) {
    Serial.println("Camera capture failed (no frame buffer)");
    return false;
  }

  bool converted = fmt2rgb888(fb->buf, fb->len, PIXFORMAT_JPEG, out_buf);
  esp_camera_fb_return(fb);

  if (!converted) {
    Serial.println("JPEG -> RGB888 conversion failed");
    return false;
  }

  return true;
}

static int ei_camera_get_data(size_t offset, size_t length, float *out_ptr) {
  size_t pixel_ix = offset * 3;
  for (size_t i = 0; i < length; i++) {
    out_ptr[i] = (snapshot_buf[pixel_ix + 2] << 16) +
                 (snapshot_buf[pixel_ix + 1] << 8) +
                 snapshot_buf[pixel_ix];
    pixel_ix += 3;
  }
  return 0;
}
