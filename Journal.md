# Project Log: Face-Following Robot
**Date:** July 2nd  
**Time Invested:** 6 Hours  
**Author:** VATheProgrammer  

---

## Project Concept & Inspiration(30 Minutes)  
I got this project idea from a horror film with a creature that constantly tracks and follows you. I decided to make a robot similar to that. I knew from the very beggining that face recognition was the way to go with this idea, but I did not knowhow to go of this. After going to Youtube, I understood that a very simple and efficient drivetrian for such robots works as a 2 wheeled drivetrain with a bearing in the front for support without restricting movement. 
  
---

## Hardware & BOM (30 Minutes)  
Now that I knew what I wanted it to do and how I wanted it to work, I had to make everything as compact and cost-efficient as possible. To achieve this, I chose the following materials:  

I chose a XIAO ESP32 S3 Sense with a camera for the "brains" of this robot, as it was compact enough to fit in the dimensions I wanted while still having the proccessing capabilities to run a face detection model locally on the robot.  
<img width="818" height="712" alt="51hgdPB7atL _AC_SL1400_" src="https://github.com/user-attachments/assets/5c4473b3-6477-4c6c-becb-a9f19da4514f" />  
I'm using 2 wheels, both controlled by different motors for steering purposes.  
<img width="293" height="292" alt="image" src="https://github.com/user-attachments/assets/79b36bae-63cb-4735-ae60-c132a7418daa" />  
I'm using a bearing in the front to stabilize the whole robot without restricitng movement.  
<img width="1500" height="1500" alt="image" src="https://github.com/user-attachments/assets/03f7135c-4a1b-44e9-b3b7-08cfd4da34c8" />  
I used a motor driver so I could move both motors forwards and backwards.  
<img width="158" height="148" alt="image" src="https://github.com/user-attachments/assets/c845632c-7522-4104-ab24-32b1c950cfbf" />  

---

## Mechanical Frame & CAD(1.5 Hours)
After knowing what parts I was going to use, I used Fusion 360 to CAD a frame to hold all of the robot's parts in:  
<img width="231" height="155" alt="image" src="https://github.com/user-attachments/assets/7071f0fd-9b0b-43b6-b3ae-9abfd6517023" />
<img width="257" height="169" alt="image" src="https://github.com/user-attachments/assets/5bfa3782-3cd3-4fe5-948a-8b78ab504a7f" />
  
In the design, the XIAO ESP32 S3 is mounted in the very front, ensuring the camera gets the best view possible of its surroundings to detect a face. The wheels are in the very back and supported by a bearing in the front. I ensured that this robot has the minimum footprint possible, and I achieved this by wasting no space to place any items on. I have also decided that tape will be needed to hold down some of the parts, as screwing may add more weight to the robot.

---

## Firmware(3.5 Hours)
This firmware was designed with the following (pseudocode) algorithm in mind:  
  
Initialize  
While True:  
&nbsp;Get camera image  
&nbsp;If face is in frame:  
&ensp;If face area is small(face is not too close to robot)  
&emsp;Determine coordinates of center of detection box.  
&emsp;Find x offset from center coordinate to center of full camera frame.  
&emsp;Set acceleration to a constant value.  
&emsp;Set turn to a value proportionate to the x offset  
&ensp;Else:  
&emsp;Set turn and acceleration to 0  
&nbsp;Else:  
&emsp;Set turn and acceleration to 0  
&nbsp;leftmotor = acceleration+turn  
&nbsp;rightmotor = acceleration-turn  
  
  
Such an algorithm was used because I did not want to use any complex algorithms to be run as that would only decrease the frame rate of the camera's proccessing. It is very simple and ensures that the robot can turn towards the face and drive towards it. I used this becuase I was already familiar with how to work something like this after my experience in FTC using a similar algorithm for aligning a turret to an AprilTag using the tx data from a Limelight.  
I used some ESP 32 documentation alongside the SenseCraft documentation to write this code in Visual Studio. I leveraged the use of AI whenever I was stuck in writing the firmware. In the end, I had successfully written a proper code which is (probably) ready to be deployed on a ESP 32. This code can be seen on this GitHub repository.
