// constants won't change. They're used here to set pin numbers:
#include <Servo.h> 
Servo bedtransform; 
Servo back_left_servo;
Servo back_right_servo;
//define joystick pins (Analog)
int joyX = 0;
int joyY = 1;
//variable to read the values from the analog pins   
int servoVal_x = 94;
int servoVal_y = 94;
int servoVal_left;
#define LEFT_DEFAULT_STOP 94
#define RIGHT_DEFAULT_STOP 94
const int buttonPin_1 = 2;     // the number of the pushbutton pin
const int buttonPin_2 = 3;
const int buttonPin_3 = 4;
const int buttonPin_4 = 6;
const int buttonPin_5 = 10;

const int Ahead = 22;

const int Right = 24;

const int Back = 26;

const int Left = 28;

const int StopRobot = 30;

const int relayPin[] = {13,12,8,7};
const int trigPin = 44;
const int echoPin = 46;

// variables will change:
int buttonState_1 = 0;         // variable for reading the pushbutton status
int buttonState_2 = 0;
int buttonState_3 = 0;
int buttonState_4;
int buttonState_5 = 0;
int GoAhead;

int GoRight;

int GoBack;

int GoLeft;

int GoStopRobot;
int   val[] = {0,0,0,0};
int relayStatus[] = {HIGH,HIGH,HIGH,HIGH};
long duration;
int distance;

void setup() {

  // initialize the pushbutton pin as an input:
  pinMode(buttonPin_1, INPUT);
  pinMode(buttonPin_2, INPUT);
  pinMode(buttonPin_3, INPUT);
  pinMode(buttonPin_4, INPUT);
  pinMode(buttonPin_5, INPUT);
  pinMode(Ahead, INPUT);
  
  pinMode(Right, INPUT);

  pinMode(Back, INPUT);
  
  pinMode(Left, INPUT);
  
  pinMode(StopRobot, INPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  //attaches servo on pins PWM 9-5
  bedtransform.attach(11);
  back_left_servo.attach(9);
  back_right_servo.attach(5);
  //Set servos to mid-range, ie, default stop
  back_left_servo.write(LEFT_DEFAULT_STOP);
  back_right_servo.write(RIGHT_DEFAULT_STOP);
  Serial.begin(9600);
  for(int i=0;i<4;i++)
  {
  pinMode(relayPin[i],OUTPUT);
  digitalWrite(relayPin[i],HIGH);
  }
  }
  

void loop() {
  //POWER
   buttonState_4 = digitalRead(buttonPin_4);
for(int i=0; i<4; i++){
  if(buttonState_4 == HIGH && relayStatus[i] == LOW){
    val[i] = 1- val[i];
    delay(100);
  }
  relayStatus[i] = buttonState_4;
  if(val[i] == 1){
    digitalWrite(relayPin[i],LOW);
  }else{
    digitalWrite(relayPin[i],HIGH);
  }
}
  // read the state of the pushbutton value:
  buttonState_1 = digitalRead(buttonPin_1);
  buttonState_2 = digitalRead(buttonPin_2);
  buttonState_3 = digitalRead(buttonPin_3);
  buttonState_5 = digitalRead(buttonPin_5);
  GoAhead = digitalRead(Ahead);

  GoRight = digitalRead(Right);

  GoBack = digitalRead(Back);

  GoLeft = digitalRead(Left);

  GoStopRobot = digitalRead(StopRobot);
  // check if the pushbutton is pressed. If it is, the buttonState is HIGH:
  if (buttonState_1 == HIGH && buttonState_2 == LOW && buttonState_5 == LOW) {
    // set the motor run smoothly
    bedtransform.write(85);

  } else if (buttonState_1 == LOW && buttonState_2 == HIGH && buttonState_3 == LOW) {
    bedtransform.write(103);

  } else {
    bedtransform.write(94);

  }
  servoVal_y = analogRead(joyY);//read the value of joystick(between 0-1023)
  servoVal_x = analogRead(joyX); //read the value of joystick(between 0-1023)
  servoVal_left = analogRead(joyY);
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin,LOW) ;
  duration = pulseIn(echoPin, HIGH);
  distance = duration*0.034/2;
  Serial.print("Distance: ");
  Serial.println(distance);
  //remote control:
  if(GoAhead == HIGH && buttonState_5 == HIGH && distance > 12){
    back_left_servo.write(98);
    back_right_servo.write(90);
  }
  else if(GoRight == HIGH && buttonState_5 == HIGH){
    back_left_servo.write(98);
    back_right_servo.write(98);
  }
  else if(GoBack == HIGH && buttonState_5 == HIGH){
    back_left_servo.write(90);
    back_right_servo.write(98);
  }
  else if(GoLeft == HIGH && buttonState_5 == HIGH){
    back_left_servo.write(90);
    back_right_servo.write(90);
  }
  else if(buttonState_5 == HIGH) {
  if(servoVal_x < 675 && servoVal_x > 348 && servoVal_y < 675 && servoVal_y > 348){
    back_left_servo.write(LEFT_DEFAULT_STOP);
    back_right_servo.write(RIGHT_DEFAULT_STOP);
  } else if (servoVal_x > 775 || servoVal_x < 248) {
  servoVal_x = map(servoVal_x, 0, 1023, 80, 100);//map the servo value from 0 to 180 degrees
  back_left_servo.write(servoVal_x);//set the servo speed and direction according to the joystick value
  back_right_servo.write(servoVal_x);//set the servo rotating speed and direction exactly the same on both sides so it can turn left or right
  } else if (distance > 12 || servoVal_left > 512){ //either there is no barricade ahead or model only move backwards
  servoVal_left = map (servoVal_left, 0, 1023, 100, 80);
  servoVal_y = map(servoVal_y, 0, 1023, 80, 100);
  back_right_servo.write(servoVal_y);//set the servo position according to the joystick value
  back_left_servo.write(servoVal_left);
  } else {
    back_left_servo.write(LEFT_DEFAULT_STOP);
    back_right_servo.write(RIGHT_DEFAULT_STOP);
  }
  } else {
    back_left_servo.write(LEFT_DEFAULT_STOP);
    back_right_servo.write(RIGHT_DEFAULT_STOP);
  }
}
