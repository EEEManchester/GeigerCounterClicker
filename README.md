# GeigerCounterClicker
ROS Compatible hardware to make Geiger counter-like clicks

## Hardware
This projects uses an Arduino Uno to interface with the Robot Operating System (ROS), producing a clicking sound with a piezoelectric buzzer to sound like a Geiger counter.  This provides sonfification for ionising radiation instruments on remote robots during inspection.

## Software
Currently, the hardware subscribes to ROS [radiation_msgs/DoseRate](https://github.com/EEEManchester/radiation_msgs) messages.  Please ensure this package is installed, or modify the code here to use an existing message type.

Remember, once new messages have been generated you will need to [update the ROS header file](http://wiki.ros.org/rosserial_arduino/Tutorials/Adding%20Custom%20Messages) for [ROSSerial](http://wiki.ros.org/rosserial).

## Demo
TODO

## Circuit
TODO
