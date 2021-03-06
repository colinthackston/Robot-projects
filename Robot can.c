#pragma config(Sensor, dgtl3,  QuadShoulder,   sensorQuadEncoder)
#pragma config(Sensor, dgtl5,  QuadElbow,   sensorQuadEncoder)
#pragma config(Sensor, dgtl7,  Sonar,          sensorSONAR_mm)
#pragma config(Sensor, dgtl9, bumpBack,   sensorTouch)
#pragma config(Sensor, dgtl10, bumpBottom,   sensorTouch)
#pragma config(Sensor, dgtl11, bumpUp,         sensorTouch)
#pragma config(Sensor, dgtl12, bumpDown,       sensorTouch)
#pragma config(Motor,  port2	,           RightShoulder, tmotorVex393, openLoop)
#pragma config(Motor,  port3,           LeftShoulder,  tmotorVex393, openLoop)
#pragma config(Motor,  port4,           Wrist,         tmotorServoContinuousRotation, openLoop)
#pragma config(Motor,  port5,           Elbow,         tmotorVex393, openLoop)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

/*******************************************************************************
*
* This file is meant as a guide to working with sensors.
*
*******************************************************************************/


/*******************************************************************************
*
*  MOTORS & SENSORS:
*
*    [I/O Port]          [Name]              [Type]                [Description]                     *|
*  Motor  - Port 2      Servo          VEX 3-wire module      servo
*  Motor  - Port 3      Motr           VEX 3-wire module      motor
*  Analog - Port 1      Light_sensor
*  Analog - Port 2      Potmeter_sensor
*  Digital- Port 1      Bump
*  Digital- Port 2      LimitSwitch
*  Digital- Port 3      RotEncoder    VEX Shaft Encoder     Left side
*  Digital- Port 4,5    QuadEncoder   VEX Shaft Encoder     Right side
*  Digital- Port 6,7    Sonar         VEX Sonar Sensor      Front mounted, facing forward
*  Digital- Port 12     Jumper
*******************************************************************************/

// Functions Prototypes
void WristMove(int servoPosition);
void ShoulderMove(int motorSpeed);
void ShoulderStop();
void ElbowMove(int motorSpeed);
void ElbowStop();
void moveButton();
void OpenClaw();
void CloseClaw();
void write(char* phrase, int value);
void armReset(int safe);
void moveShoulderTo(int angle);
void moveElbowTo(int angle);
void armMove(int angle);
void flingIt();
void ShoulderReset();
void ElbowReset();
int findCan();

// Declare Global Variables
int shoulderAngle = 0;
int elbowAngle = 0;

//+++++++++++++++++++++++++++++++++++++++++++++| MAIN |+++++++++++++++++++++++++++++++++++++++++++++++
task main()
{
// variables to be used:

	armReset(0);


  int test = 0;
  int run = 1;

  int angle = 0;

  while(test){
  	angle = findCan();
  	//armMove(41);
  	//armReset(1);
  	//ElbowMove(30);
  	//ShoulderMove(20);
  	//moveButton();
  	//ShoulderMove(-50);
	}
  while(run){
  		angle = findCan();
  		armMove(angle);
  		armReset(1);
			flingIt();
			armReset(1);
  }

}

int findCan(){
	//range is about 41-130 (about 10 inches on table)
	int distance = SensorValue(Sonar);
	int angle = 0;
	while(distance == -1 || distance > 485) {
		distance = SensorValue(Sonar);
		write("Sonar Value: ", distance);
	}
	write("Sonar Value: ", distance);
	angle = ((((float) distance)-278)/272) * 89 + 41;
	//let's write an equation that converts distance(inches) to angle
	return angle;
}



//moves the motors a small amount based on button input


void flingIt(){
		moveElbowTo(130);
		moveShoulderTo(35);
		moveElbowTo(200);
		moveShoulderTo(20);
		moveElbowTo(250);
		OpenClaw();
		moveElbowTo(150);
}

void moveShoulderTo(int angle) {
	SensorValue[QuadShoulder] = 0;
	int totalAngle = abs(shoulderAngle - angle) - 9;
	//write("orig Shoulder Angle ", shoulderAngle);
	//converts given angle into angle we need it to move
	if(angle < shoulderAngle) {
		write("Sensor Value ", SensorValue(QuadShoulder));
		write("Angle ", totalAngle);
		while(abs(SensorValue(QuadShoulder)) < totalAngle) {
			//writeDebugStreamLine("Quad Reading = %d",SensorValue(QuadShoulder));
			ShoulderMove(-50);
		}
	}
	else if(angle > shoulderAngle) {
		while(abs(SensorValue(QuadShoulder)) < angle) {
			ShoulderMove(56);
		}
	}
	ShoulderStop();
	shoulderAngle = angle;
	write("Shoulder Angle: ", shoulderAngle);
}

void moveElbowTo(int angle) {
	SensorValue[QuadElbow] = 0;
	write("Elbow Angle: ", elbowAngle);
	//converts given angle into angle we need it to move
	int elbowCorrection = 9;
	int totalAngle = abs(elbowAngle - angle) - elbowCorrection;
	if(elbowAngle < angle){
		while(abs(SensorValue(QuadElbow)/25) < totalAngle) {
			//write("Quad Elbow Value: ", SensorValue(QuadElbow)/25);
			ElbowMove(33);
		}
	}
	else{
		while(abs(SensorValue(QuadElbow)/25) < totalAngle) {
			//write("Quad Elbow Value: ", SensorValue(QuadElbow)/25);
			ElbowMove(-25);
		}
	}
	elbowAngle = angle;
	write("Quad Elbow Angle: ", elbowAngle);
	ElbowStop();
}



void ElbowReset(void){
  int bumpSenseBottom = SensorValue(bumpBottom);
  while(bumpSenseBottom != 1){
  	ElbowMove(-20);
  	bumpSenseBottom = SensorValue(bumpBottom);
	}
	while(bumpSenseBottom == 1){
		ElbowMove(20);
		bumpSenseBottom = SensorValue(bumpBottom);
	}
  ElbowStop();
  SensorValue[QuadElbow] = 0;
  elbowAngle = 41;
  wait1Msec(500);
}

void ShoulderReset(void){
	int bumpSenseBack = SensorValue(bumpBack);
	while(bumpSenseBack != 1){
		ShoulderMove(50);
		bumpSenseBack = SensorValue(bumpBack);
  }
  while(bumpSenseBack == 1){
		ShoulderMove(-40);
		bumpSenseBack = SensorValue(bumpBack);
  }
  ShoulderStop();
  SensorValue[QuadShoulder] = 0;
  shoulderAngle = 86;

  wait1Msec(200);
}

//gets the arms set into position
void armReset(int safe){
	if(safe){
		ShoulderReset();
		ElbowReset();
	}
	else{
		ElbowReset();
		ShoulderReset();
	}

}

void WristMove(int servoPosition){writeDebugStreamLine("Wrist Position = %d",servoPosition);motor[Wrist] = servoPosition;wait1Msec(50);}
void ShoulderMove(int motorSpeed){motor[LeftShoulder] = -motorSpeed;wait1Msec(30);}
void ElbowMove(int motorSpeed){motor[Elbow] = motorSpeed;wait1Msec(30);}
void ElbowStop(void){motor[Elbow] = 0;}
void ShoulderStop() {motor[RightShoulder] = 0;motor[LeftShoulder]= 0;}
void write(char* phrase, int value){writeDebugStreamLine("%s %d",phrase,value);}
void OpenClaw(){WristMove(127);wait1Msec(150);}
void CloseClaw(){WristMove(-125);wait1Msec(150);}
void armMove(int angle) {moveElbowTo(angle);moveShoulderTo((int) 90 - angle / 2);CloseClaw();}

void moveButton(){
	int bumpSenseUp; //up
	int bumpSenseDown; //down
	int bumpSenseBottom; //safety switch for shoulder
	int bumpSenseBack; // safety switch for back

  int	quadShoulderValue;

	bumpSenseUp = SensorValue(bumpUp);
	bumpSenseDown = SensorValue(bumpDown);
	bumpSenseBottom = SensorValue(bumpBottom);
	bumpSenseBack = SensorValue(bumpBack);

	//writeDebugStreamLine("Bump Position = %d",bumpSenseUp);
	//writeDebugStreamLine("Bump Position = %d",bumpSenseDown);
	quadShoulderValue = SensorValue(QuadShoulder);

	//write("QuadShoulder = ",quadShoulderValue);

	while (bumpSenseUp == 1 && bumpSenseBack != 1){
		ShoulderMove(-30);
		bumpSenseUp = SensorValue(bumpUp);
		bumpSenseBack = SensorValue(bumpBack);
		quadShoulderValue = SensorValue(QuadShoulder);
		writeDebugStreamLine("Quad1 Reading = %d",quadShoulderValue);
	}

	ShoulderStop();

	while(bumpSenseDown == 1 && bumpSenseBottom != 1){
		ShoulderMove(50);
		bumpSenseDown = SensorValue(bumpDown);
		bumpSenseBottom = SensorValue(bumpBottom);
		quadShoulderValue = SensorValue(QuadShoulder);
		write("Quad1 Reading = ",quadShoulderValue);
	}
	//if the should sensor is engaged, it resets the quadEncoder
	//and moves the arm up until the switch is no longer engaged.
	if(bumpSenseBottom == 1){
			SensorValue[QuadShoulder] = 35; // the new angle
			while(bumpSenseBottom == 1){
				ShoulderMove(30);
				bumpSenseBottom = SensorValue(bumpBottom);
			}
	}
	if(bumpSenseBack == 1){
			//SensorValue[QuadShoulder] = 0;
			while(bumpSenseBack == 1){
				ShoulderMove(50);
				bumpSenseBack = SensorValue(bumpBack);
			}
	}
	ElbowStop();
	ShoulderStop();
}
