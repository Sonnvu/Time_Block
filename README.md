# Time_Block
## Design Overview
**Description:** Time block is an IoT device that uses an MPU-6050 accelerometer to detect the orientation of a cube. The project is designed to help users keep track of time spent on different activities by associating each different side of the block with a corresponding activity. When the orientation of the block changes, the time block will send an API request to Clockify.com time tracking service using an ESP32 development board to start or stop a timer associated with the current activity that is represented by whichever side of the cube is facing up. The action of recording time is automatic depending on which side is facing upward, and to stop the current running timer, the user will give the time block a nice shake and another one to resume the time tracking process. There will be 2 LEDs for user indication. The white LED is lit up when the block is successfully connected to Wifi, or when the cube changed from stop recording to start recording. The red LED will be blinking when the cube is trying to connect to WiFi, or stay on when the cube is current in no-recording mode. 
<br>
<br>
**Original Design Concepts:** Initially, project Time Block consist of 3 component:
- Arduino IoT Device 
- Database
- Web Application
### Design Diagram 1 - Original Concept
![image](https://user-images.githubusercontent.com/97687998/235016566-6aaba4b7-b96d-48fc-b577-fcef49c668a0.png)

At the early stage of development, the first component, which is the IoT device, consist of two main component: The microcontroller/WiFi module ESP32-WROOM-32 and the MPU-6050 accelerometer 
