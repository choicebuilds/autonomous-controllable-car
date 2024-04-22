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

#### Step 1: Create the car using the Shadow chassis, wheels, motor, motor driver, and battery holder.*
<img src="https://github.com/choicebuilds/ece4180/blob/main/images/shadow-chassis.jpg" alt="Shadow Chassis" width="200"/>

*The instructions on how to assemble the car can be found [here][car-assembly].


#### Step 2: Connect the Motor and Motor Driver
<img src="https://github.com/choicebuilds/ece4180/blob/main/images/motor.jpg" alt="Motor" width="200"/>  <img src="https://github.com/choicebuilds/ece4180/blob/main/images/motor-wiring.png" alt="Motor Wiring" width="400"/>

#### Step 3: Connect the Bluetooth Sensor
<img src="https://github.com/choicebuilds/ece4180/blob/main/images/bluetooth-sensor.jpg" alt="Bluetooth Sensor" width="200"/>  <img src="https://github.com/choicebuilds/ece4180/blob/main/images/bluetooth-wiring.png" alt="Bluetooth Wiring" width="200"/>  

#### Step 4: Connect the Speaker along with the Audio Amp
<img src="https://github.com/choicebuilds/ece4180/blob/main/images/audio-amp.jpg" alt="Audio Amp" width="200"/>  <img src="https://github.com/choicebuilds/ece4180/blob/main/images/speaker-wiring.png" alt="Speaker Wiring" width="200"/>  


#### Step 5: Connect the lidar sensor
<img src="https://github.com/choicebuilds/ece4180/blob/main/images/lidar-sensor.jpg" alt="Lidar Sensor" width="200"/>  <img src="https://github.com/choicebuilds/ece4180/blob/main/images/lidar-wiring.png" alt="Lidar Wiring" width="200"/>  

#### Step 6: Connect the uLCD screen
<img src="https://github.com/choicebuilds/ece4180/blob/main/images/uLCD.jpg" alt="uLCD" width="200"/>  <img src="https://github.com/choicebuilds/ece4180/blob/main/images/uLCD-wiring.png" alt="uLCD Wiring" width="200"/>  


#### Step 7: Connect the servo
<img src="https://github.com/choicebuilds/ece4180/blob/main/images/servo.jpg" alt="Servo" width="200"/>  <img src="https://github.com/choicebuilds/ece4180/blob/main/images/servo-wiring.png" alt="Servo Wiring" width="200"/> 

##### Add overall picture along with wiring of entire thing

## Software Overview

Here is where all the magic happens. This section explains how all the different hardware parts are implemented and work together to create a complete robot:

#### Bluetooth Manual vs. Automatic
1. We use this code to set up the bluetooth. We can then use the switch statements to control the various functionalities and direction of the car.  
![alt text][bt1]

2. To enable/disable the automatic/manual modes, we will use button 1.  
![alt text][bt2]

3. To control the speed of the robot, we will use button 3 to increase and button 4 to decrease.  
![alt text][bt3]

4. To control the direction of the robot, we will configure it with cases 5, 6, 7, and 8.  
![alt text][bt4]

#### Speaker Functionality
1. The speaker plays a note everytime the speed of the robot is increased or decreased in the following case statements.
<img src="https://github.com/choicebuilds/ece4180/blob/main/images/speakerBTcode.png" alt="Speaker" width="400"/>

2. The speaker will also play notes of varying tones depending on the robots proximity to an object detected by the lidar sensor.
<img src="https://github.com/choicebuilds/ece4180/blob/main/images/SpeakerDistance.png" alt="SpeakerDist" width="400"/>

#### LiDaR Functionality
1. The lidar thread gathers the distance data for the distance variable
<img src="https://github.com/choicebuilds/ece4180/blob/main/images/lidarFunct.png" alt="LiDaR" width="400"/>

2. The move thread uses the distance data from the lidar to determine when the robot is close enough to an object to turn as well as play varying notes from the speaker.
<img src="https://github.com/choicebuilds/ece4180/blob/main/images/moveFunct.png" alt="Move" width="400"/>

#### uLCD Functionality
1. The LCD is used within the main function to display a green progross bar depending on the distance the robot is to an object, the closer the taller the bar.
<img src="https://github.com/choicebuilds/ece4180/blob/main/images/LCDbar.png" alt="bar" width="400"/>

2. The LCD also displays which mode the robot is in, either "Manual" or "Automatic".
<img src="https://github.com/choicebuilds/ece4180/blob/main/images/LCDmode.png" alt="mode" width="400"/>

#### Speedometer Functionality
1. The servo motor mimics the functionality of a speedomoter. When the speed variable is 0 (lowest speed) it points to the left and at 1.0 (highest speed) it points right.
<img src="https://github.com/choicebuilds/ece4180/blob/main/images/servoCode.png" alt="SpeakerDist" width="400"/>

## Running the Car
1. Connect your car to the power sources
2. Download the BluefruitConnect App (iOS/Android)
3. Select your car (Adafruit Bluefruit LE)
4. Go to "Controller"
5. Go to "Control Pad"

## Conclusion/Future Improvements?

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
[car-assembly]: https://www.youtube.com/watch?v=aJRYTqZu5OE
[bt1]: https://github.com/choicebuilds/ece4180/blob/main/images/bluetooth-1.png "Bluetooth Setup"
[bt2]: https://github.com/choicebuilds/ece4180/blob/main/images/bluetooth-2.png "Mode Control"
[bt3]: https://github.com/choicebuilds/ece4180/blob/main/images/bluetooth-3.png "Speed Control"
[bt4]: https://github.com/choicebuilds/ece4180/blob/main/images/bluetooth-4.png "Direction Control"
