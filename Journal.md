# Project Log: Face-Following Robot
**Date:** July 2nd  
**Time Invested:** 6 Hours  
**Author:** VATheProgrammer  

---

## 1. Project Concept & Inspiration  
The foundational idea for this project stems from a horror film concept featuring a relentless creature that constantly tracks and follows its target. Translating this into a hardware prototype, the objective is to build an autonomous, two-wheeled mobile robot that leverages edge computer vision to detect human facial coordinates and dynamically adjust its drivetrain to pursue the subject.   
  
---

## 2. Hardware Architecture & Bill of Materials (BOM)  
To achieve a highly compact, self-contained system capable of on-device vision processing, the following hardware components were selected:  

<img width="230" height="236" alt="image" src="https://github.com/user-attachments/assets/3ff4535f-618d-4006-af20-e67693a5ee7d" />  

---

## 3. Mechanical Frame & CAD Design
I made a chasis with CAD:  
<img width="277" height="214" alt="image" src="https://github.com/user-attachments/assets/016a0b4e-5e7a-4b24-9145-749226b00337" />  <img width="257" height="169" alt="image" src="https://github.com/user-attachments/assets/5bfa3782-3cd3-4fe5-948a-8b78ab504a7f" />

* **Placement and Balancing of Components:** In the design, the computer vision model has the front as its mounting point for the main camera sensor (XIAO ESP32-S3), thereby ensuring that it gets an undistorted field of view. The actuation components (dual DC motors) have been located at the back of the system, and are balanced by the auxiliary support bearing on the bottom front of the robot.
* **Optimization of Volume Space:** This design optimizes the interior volume space by reducing the total footprint of the robot. Also, a particular ramp portion was designed to accommodate the batteries.
* **Rapid Prototyping Techniques:** In order to keep up with fast development pace, some allowance is made in this prototype. High strength tape will be employed to secure the battery and wiring bundles inside the robot before encasing them.

---

## 4. Firmware Architecture & Preliminary Code
A fundamental framework of the firmware was created within the development environment using official documentation:

* **Vision Pipeline:** Set up place holders to configure the registers of ESP32-S3 camera to make pixel format changes and adjust frame size so as to enable fast face matching at low latency.
* **Control Loop:** Created place holders of simple proportion control loop where face coordinates of ($X, Y$) from the camera center would translate to differential speed command to left/right motors PWM.
* **Physical Constraints:** The place holder framework has specific configurations to pass cables through physical wiring holes created within the CAD frame.

---

## 5. Documentation-Driven Firmware Setup (with AI)
Taking advantage of the official technical documentation in combination with artificial intelligence (AI), a working firmware placeholder was created that would work with the pre-trained SenseCraft face detection model.

* **Technical Documentation & AI Approach:** Technical guides describing how to use the hardware pins in XIAO ESP32-S3 Sense and implementation of the SenseCraft model were provided to an AI assistant to help generate the boilerplate code faster.
* **Model Implementation:** The firmware creates placeholders for the buffer that will be able to receive multidimensional array of data from the edge model.
* **Error Calculation Code:** The initial routines were created to analyze the received target points to enable the system to calculate error offsets relative to the lens center.
