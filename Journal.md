# Project Log: Face-Following Robot
**Date:** July 2nd  
**Time Invested:** 6 Hours  
**Author:** VATheProgrammer  

---

## 1. Project Concept & Inspiration  
I got this project idea from a horror film with a creature that constantly tracks and follows you. I decided to make a robot similar to that. I decided it would be a 2 wheeled robot with a camera to detect peoples faces and turn towards them accordingly.
  
---

## 2. Hardware & Bill of Materials (BOM)  
To make everything as compact and efficient as possible, I chose the following materials:  

<img width="230" height="236" alt="image" src="https://github.com/user-attachments/assets/3ff4535f-618d-4006-af20-e67693a5ee7d" />  

---

## 3. Mechanical Frame & CAD
I made a chasis with CAD:  
<img width="277" height="214" alt="image" src="https://github.com/user-attachments/assets/016a0b4e-5e7a-4b24-9145-749226b00337" />  <img width="257" height="169" alt="image" src="https://github.com/user-attachments/assets/5bfa3782-3cd3-4fe5-948a-8b78ab504a7f" />

In the design, the XIAO ESP32 S3 is mounted in the fery front, ensuring it gets the best view possible. The wheels are in the very back and supported by a bearing in the front. I ensured that this robot has the minimum footprint possible. I have also decided that tape will be needed to hold down some of the parts.

---

## 4. Firmware
This firmware was designed with the following(pseudocode) algorithm in mind:

<img width="113" height="125" alt="image" src="https://github.com/user-attachments/assets/76ea5160-2d1b-4d3d-acd8-dd52c7372493" />
