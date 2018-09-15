#pragma config(Sensor, in8,    light1,         sensorReflection)
#pragma config(Sensor, in7,    light2,         sensorReflection)
#pragma config(Sensor, dgtl11, bump1,          sensorNone)
#pragma config(Sensor, dgtl12, bump2,          sensorNone)
#pragma config(Sensor, dgtl7, bump3,          sensorNone) //left
#pragma config(Sensor, dgtl6, bump4,          sensorNone) //right
#pragma config(Motor,  port3,           leftMotor,    tmotorVex393, openLoop)
#pragma config(Motor,  port2,           rightMotor,     tmotorVex393, openLoop)
#pragma config(Motor,  port4,  Servo,               tmotorNormal, openLoop)
#pragma config(Sensor, dgtl8,  sonarSensor,         sensorSONAR_mm)
#pragma config(Sensor, in2,    Potmeter_sensor,     sensorPotentiometer)
#pragma config(Sensor, dgtl3, quadEncoder, sensorQuadEncoder)

//**//

/*******************************************************************************
*
*  File: main.c
*  Part of: Assignment 4
*  Authors: Colin Thackston, Josh Mullet, Kendall Ebley
*  Date Created: February 18, 2016
*  Last Modified:
*  Description:
*
*******************************************************************************/

/*******************************************************************************
*
*	main
*
*******************************************************************************/
#define LEFT 1
#define RIGHT 2

int sonar_value;
int isLeftWall = 0;
int isFrontWall = 0;

int bumpSense1; //front sensor
int bumpSense2; //butt sensor
int bumpSense3;
int bumpSense4; //side sensors

void ServoMove(int servoPosition){
  motor[Servo] = servoPosition;
  wait1Msec(80);
}

int angleToTime(int angle) {
	int time;
	time = (2900/360)*angle;
	return time;
}

void MotorStop() {
	motor[rightMotor] = 0;
  motor[leftMotor] = 0;
}

void MotorTurn(int angle, int direction){
  //writeDebugStream("Turn Angle = %d",angle);
  if(direction == RIGHT) {
  	motor[rightMotor] = 42;
  	motor[leftMotor] = 45;
  	wait1Msec(angleToTime(angle));
  }else if(direction == LEFT) {
  	motor[rightMotor] = -42;
  	motor[leftMotor] = -42;
  	wait1Msec(angleToTime(angle));
  }
  MotorStop();
  wait1Msec(500);
}

void MotorMove(int distance){
  motor[rightMotor] = -82;
  motor[leftMotor] = 75;
  wait1Msec(distance);
  MotorStop();
}

void MotorCreep(void){
  motor[rightMotor] = -38; //-38
  motor[leftMotor] = 29; //21
  wait1Msec(500);
}

void MotorReverse(void){
  motor[rightMotor] = 35;
  motor[leftMotor] = -21;
  wait1Msec(2000);
  MotorStop();
}

void Cruise(void){
  motor[rightMotor] = -52;
  motor[leftMotor] = 45;
}

void MoveBodyLength(void) {
	MotorStop();
	MotorCreep();
	wait1Msec(1600);
	MotorStop();
}




void SonarSearch(void) {
	ServoMove(-125);
	wait1Msec(400);
	sonar_value = SensorValue(sonarSensor);
	writeDebugStream("Left: %d\n",sonar_value);
	if(sonar_value > 575 || sonar_value == -1) { //made robot less senstive
		isLeftWall = 0;
	}else if(sonar_value < 80) { //corrects if robit is too close to wall
		MotorTurn(15,RIGHT);
		isLeftWall = 1;
	}else if(sonar_value > 400){ //correction
		isLeftWall=1;
		MotorTurn(20,LEFT);
	}
	else{
		isLeftWall = 1;
	}
	//looks straight and asks if there is a wall
	ServoMove(50);
	wait1Msec(300);
	sonar_value = SensorValue(sonarSensor);
	writeDebugStream("Forward: %d\n",sonar_value);
	if(sonar_value < 350) {
		isFrontWall = 1;
	}else{
		isFrontWall = 0;
	}
	wait1Msec(100);
}

void BumpCheck(void){
		bumpSense1 = SensorValue(bump1);
		bumpSense2 = SensorValue(bump2);
		bumpSense3 = SensorValue(bump3);
		bumpSense4 = SensorValue(bump4);
		if (bumpSense1 == 0) {
  		writeDebugStream("bump1: %d bump2: %d\n", bumpSense1, bumpSense2);
  		MotorStop();
  		MotorReverse();
  		SonarSearch();
  	}
  	if(bumpSense2 == 0){
  		MotorStop();
  		MotorCreep();
  		SonarSearch();
  }

  	if(!bumpSense3){ //stuck on left corner
  		MotorStop();
  		MotorReverse();
  		MotorTurn(30,RIGHT);
  		MotorCreep();
  		wait10Msec(150);
  	}

  	if(!bumpSense4){ //stuck on right conrner
  		MotorStop();
  		MotorReverse();
  		MotorTurn(30,LEFT);
  		MotorCreep();
  		wait10Msec(150);
  	}
	}

	void IntoTheMaze(void){
			MotorCreep();
			wait1Msec(1300);
	}

task main()
{

	wait1Msec(200);
	int testing = 0;
	int real = 1;



	while(testing){
		MotorTurn(90,LEFT);
		wait1Msec(100);
		MotorTurn(90,RIGHT);
		wait1Msec(2000);
		}
	IntoTheMaze();

  while(real)
  {
		BumpCheck();
  	SonarSearch();

  	//looks left and then asks if there is a wall
  	//decides to turn right, left, or continue straight
  	if(!isLeftWall) {
  		writeDebugStream("No left wall, let's turn left");
  		//MoveBodyLength();
  		//MoveForwardToWall();
  		wait1Msec(1000);
  		MotorTurn(90,LEFT);
  		MotorCreep();
  		BumpCheck();
  		wait1Msec(400);
  		SonarSearch();
  		if(isFrontWall) {
  			MotorTurn(90,LEFT);
  		}
  		MotorCreep();
  	}else if (isFrontWall) {
  		writeDebugStream("Front wall, turn right");
  		MotorStop();
  		MotorTurn(90,RIGHT);
  		BumpCheck();
			wait1Msec(500);
		}
		MotorCreep();
	}
}
