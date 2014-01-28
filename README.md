Arm Robot Control Firmware
===

For the robot made here http://www.thingiverse.com/thing:234482 

By sending command to firmware through serial, you could control the position of the robot.

There are 5 servos in this robot, number as 0-4 from top to bottom.

Current there are 3 commands supported:

A a0 a1 a2 a3 a4
V v0 v1 v2 v3 v4
P n AMin AMax VMin VMax ADef

V is to give the value of Arduino built-in Servo class, this command means:

	S[0].write(v0);
	S[1].write(v1);
	S[2].write(v2);
	S[3].write(v3);
	S[4].write(v4);

A is to give the angle you need, remember, due to the existence of assembly error, value given to V command may be bias to what you want, the A command is the angle after calibration. 

P command is for the calibration, n is servo number, AMin AMax is the angle bound of the servo specified by n, VMin VMax is the value for Servo class, a map() behind it doing the transformation from A to V. ADef is the default angle you want to put. Don't confused by the name, AMin and AMax doesn't necessarily means AMin<AMax, they just mark the boundry.

All the value in command should be integer, and only treated as valid when it is non-negtive. Any value less than 0 will be treated as invalid and will just skip it.

Values will write into EEPROM which means you don't need to calibrate again after reboot the robot.

Examples:
===
Rotate the servos to given value:
V 90 70 80 85 30

Rotate the servo 1 to value 90:
V -1 90 -1 -1 -1

Rotate the servos to specified angles:
A 0 90 90 90 0

Rotate the servo 2 to angle 90:
A -1 -1 90 -1 -1

Rotate the servo 2 to 90, 3 to 45:
A -1 -1 90 45 -1

Let Angle of servo 2 vary from 0 to 90 degree, where the coresponding value is 170 ~ 85
P 2 0 90 170 85

