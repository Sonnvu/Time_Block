# Time_Block
## Design Overview
**Description:** Time block is an IoT device that uses an MPU-6050 accelerometer to detect the orientation of a cube. The project is designed to help users keep track of time spent on different activities by associating each different side of the block with a corresponding activity. When the orientation of the block changes, the time block will send an API request to Clockify.com time tracking service using an ESP32 development board to start or stop a timer associated with the current activity that is represented by whichever side of the cube is facing up. The action of recording time is automatic depending on which side is facing upward, and to stop the current running timer, the user will give the time block a nice shake and another one to resume the time tracking process. There will be 2 LEDs for user indication. The white LED is lit up when the block is successfully connected to Wifi, or when the cube changed from stop recording to start recording. The red LED will be blinking when the cube is trying to connect to WiFi, or stay on when the cube is current in no-recording mode. 
<br>
<br>
**Original Design Concepts:** Initially, project Time Block consist of 3 component:
- Arduino IoT Device 
- Database
- Web Application
### Design 1 Diagram: Original Concept
<!-- ![image](https://user-images.githubusercontent.com/97687998/235034610-1ed8fc16-2aed-4788-9813-e8f56a0c9c95.png)
![image](https://user-images.githubusercontent.com/97687998/235016566-6aaba4b7-b96d-48fc-b577-fcef49c668a0.png) -->
![image](https://user-images.githubusercontent.com/97687998/235034385-78148da6-6298-4ac0-862a-45d4cafce68d.png)


**IoT Device:** At the early stage of development, the first component, which is the IoT device, consist of two main component: The microcontroller/WiFi module ESP32-WROOM-32 and the MPU-6050 accelerometer. For the first week, I was getting familiar with the two modules while also exploring different approaches in how to send and what to send in term of data to the database to achieve my goal. At the end of milestone 1, which is to get the hardware ready for data transfer to the cloud, I successfully programmed the accelerometer to track the orientation of the cube and recognize which side is facing up. Since the cube has 6 sides, I created 6 variables side1 to side6 and assign value of 1 to the sides that is up while 0 to the others. I bunch up the variable into a JSON file for transfer, and create a publish message function to send data to AWS IoT Core via MQTT messaging. Since constantly uploading data not only maintainable but also uses a large amount of resources that a free AWS account cannot provide, the IoT device is implemented so that it send data when and only when there is a change in side orientation. 

**AWS Database Services:** 
