# Autonomous Controllable Car
Jaeyoung Choi, Shayahn Mirfendereski, Jubral Holloway, Seth Wobig
Georgia Tech ECE4180 Final Project

## Project Idea

The goal of this project is to create a self-driving car that has the ability to be driven independently when desired. The car will be operated using an mbed LPC1768 microcontroller and come with various features including:
1. Bluetooth Connection - allows user to switch between automatic/manual modes; on manual mode, the car can move in any direction with the ability to adjust speed.
2. Obstacle Detection - detects if an object is nearby via a lidar sensor and utilizes the speakers to beep louder as the car gets closer to an obstacle.
3. LCD Screen - displays the mode and creates a visual representation of how close the car is to an obstacle.
5. Speedometer - allows users to visually see the speed the car is going at.

## Parts List
* [mbed LPC1768][mbed]
* [Long Breadboard][LBB]*
* [Short Breadboard][SBB]*
* [Shadow Chassis][SC]
* [Shadow Chassis Motor][SCM]
* [Wheels][WH]
* 2x [Battery Holder][BH]
* [Motor Driver][MD]
* [Bluetooth Sensor][BS]
* [Speaker][SP]
* [Audio Amp][AA]
* [uLCD][ULCD]
* [Lidar Sensor][LS]
* [Servo][SE]
* 8x AA Batteries
* Wires

*Different breadboard sizes can be used, these two provided our team adequate space.

## Hardware Overview

Here is an overview of the parts we used along with the pin diagram of how they were plugged in into the mbed:

#### Step 1: Create the car using the Shadow chassis
![alt text][Shadow Chassis]

#### Step 2: Make the car move using the Motor
![alt text][Motor]

#### Step 3: Connect the Bluetooth Sensor
![alt text][Bluetooth Sensor]

#### Step 4: Connect the Speaker along with the Audio Amp
![alt text][Speaker]
![alt text][Audio Amp]

#### Step 5: Connect the lidar sensor
![alt text][Lidar]

#### Step 6: Connect the uLCD screen
![alt text][LCD]

##### Step 7: Connect the servo
![alt text][Servo]

##### Add overall picture along with wiring of entire thing

## Software Overview

##### Bluetooth Manual vs. Automatic

##### Speaker Functionality

##### LiDaR Functionality

##### uLCD Functionality

##### Cardinal Direction Functionality

## Running the Robot

Download bluetooth app + how to navigate to controls & what the buttons do.

[mbed]: https://www.digikey.com/en/products/detail/nxp-usa-inc/OM11043-598/2138502
[LBB]: https://www.sparkfun.com/products/12615
[SBB]: https://www.sparkfun.com/products/12002
[SC]: https://www.sparkfun.com/products/13301
[SCM]: https://www.sparkfun.com/products/13302
[WH]: https://www.sparkfun.com/products/
[BH]: https://www.sparkfun.com/products/552
[MD]: https://www.sparkfun.com/products/14450
[BS]: https://www.adafruit.com/product/2479
[SP]: https://www.sparkfun.com/products/11089
[AA]: https://www.sparkfun.com/products/11044
[ULCD]: https://www.sparkfun.com/products/11377
[LS]: https://www.adafruit.com/product/3317
[SE]: https://www.sparkfun.com/products/11884

[Shadow Chassis]: https://github.com/choicebuilds/ece4180/blob/main/images/shadow-chassis.jpg "Shadow Chassis"
[Motor]: https://github.com/choicebuilds/ece4180/blob/main/images/motor.jpg "Motor"
[Bluetooth Sensor]: https://github.com/choicebuilds/ece4180/blob/main/images/bluetooth-sensor.jpg "Bluetooth Sensor"
[Speaker]: https://github.com/choicebuilds/ece4180/blob/main/images/speaker.jpg "Speaker"
[Audio Amp]: https://github.com/choicebuilds/ece4180/blob/main/images/audio-amp.jpg "Audio Amp"
[Lidar]: https://github.com/choicebuilds/ece4180/blob/main/images/lidar-sensor.jpg "Lidar Sensor"
[LCD]: https://github.com/choicebuilds/ece4180/blob/main/images/uLCD.jpg "uLCD"
[Servo]: https://github.com/choicebuilds/ece4180/blob/main/images/servo.jpg "Servo"
