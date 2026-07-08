# Project Log: Face-Following Robot
**Date:** July 2nd  
**Time Invested:** 6 Hours  
**Author:** VATheProgrammer  

---

## Project Concept & Inspiration(30 Minutes)  
I got this project idea from a horror film with a creature that constantly tracks and follows you. I decided to make a robot similar to that. I knew from the very beggining that face recognition was the way to go with this idea, but I did not knowhow to go of this. After going to Youtube, I understood that a very simple and efficient drivetrian for such robots works as a 2 wheeled drivetrain with a bearing in the front for support without restricting movement. 
  
---

## Hardware & BOM (30 Minutes)  
Now that I knew what I wanted it to do and how I wanted it to work, I had to make everything as compact and cost-efficient as possible. To achieve this, I went to Amazon and chose the following materials:  

<img width="230" height="236" alt="image" src="https://github.com/user-attachments/assets/3ff4535f-618d-4006-af20-e67693a5ee7d" />  

I chose a XIAO ESP32 S3 Sense with a camera for the "brains" of this robot, as it was compact enough to fit in the dimensions I wanted while still having the proccessing capabilities to run a face detection model locally on the robot. As I said before, I used 2 wheels, both controlled by different motors for steering purposes, alongside a bearing for the drivetrain. I chose a bundle that included a motor driver to ensure I didn't have to buy that seperatley, so I could move both motors forwards and backwards. I chose a rechargable battery with a decent capacity to ensure I was able to keep this robot running for a decent amount of time while I was at Open Sauce.

---

## Mechanical Frame & CAD(1.5 Hours)
After knowing what parts I was going to use, I used Fusion 360 to CAD a frame to hold all of the robot's parts in:  
<img width="277" height="214" alt="image" src="https://github.com/user-attachments/assets/016a0b4e-5e7a-4b24-9145-749226b00337" />  <img width="257" height="169" alt="image" src="https://github.com/user-attachments/assets/5bfa3782-3cd3-4fe5-948a-8b78ab504a7f" />
  
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
I used some ESP 32 documentation alongside the SenseCraft documentation to write this code. I leveraged the use of AI whenever I was stuck in writing the firmware. In the end, I had successfully written a proper code which is ready to be deployed on a ESP 32. This code can be seen on this GitHub repository.
