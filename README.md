# Time_Block
## Design Overview
**Description:** Time block is an IoT device that uses an MPU-6050 accelerometer to detect the orientation of a cube. The project is designed to help users keep track of time spent on different activities by associating each different side of the block with a corresponding activity. When the orientation of the block changes, the time block will send an API request to Clockify.com time tracking service using an ESP32 development board to start or stop a timer associated with the current activity that is represented by whichever side of the cube is facing up. 
