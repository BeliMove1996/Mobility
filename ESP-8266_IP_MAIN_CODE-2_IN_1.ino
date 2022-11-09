/* 
 * Mert Arduino and Tech - YouTube
 * WiFi Robot controlled by Phone (WiFibot/Android/IoT/ESP8266)
 * NodeMCU ESP8266 Tutorial 03
 * Please Subscribe for Support - https://goo.gl/RLT6DT
 */


/* include library */
#include <ESP8266WiFi.h>

/*
 * If you have no idea about this library, you can watch the tutorial below
 * NodeMCU ESP8266 Tutorial 01: Programming NodeMCU ESP-12E Using Arduino IDE
 * https://youtu.be/NtyFx1frYB0
 */

/* define port */
WiFiClient client;
WiFiServer server(80);

/* WIFI settings */
const char* ssid = "frank";
const char* password = "belimove";

/* data received from application */
String  data =""; 

/* define L298N or L293D motor control pins */
int Forward = 2;     /* GPIO2(D4)    */
int Backward = 15;   /* GPIO15(D8)   */
int Left = 0;    /* GPIO0(D3)    */
int Right = 13;  /* GPIO13(D7)   */
int Stop = 14; /* GPIO14(D5)  */

void setup()
{
  /* initialize motor control pins as output */
  pinMode(Forward, OUTPUT);
  pinMode(Backward, OUTPUT); 
  pinMode(Left, OUTPUT);  
  pinMode(Right, OUTPUT);
  pinMode(Stop, OUTPUT); 
  
  digitalWrite(Forward,LOW);
  digitalWrite(Backward,LOW);
  digitalWrite(Left,LOW);
  digitalWrite(Right,LOW);
  digitalWrite(Stop,LOW);
  Serial.begin(115200);
  connectWiFi();
  
  /* start server communication */
  server.begin();
}

void loop()
{
    /* If the server available, run the "checkClient" function */  
    client = server.available();
    if (!client) return; 
    data = checkClient ();

/************************ Run function according to incoming data from application *************************/

    /* If the incoming data is "forward", run the "MotorForward" function */
    if (data == "forward") MotorForward();
    /* If the incoming data is "backward", run the "MotorBackward" function */
    else if (data == "backward") MotorBackward();
    /* If the incoming data is "left", run the "TurnLeft" function */
    else if (data == "left") TurnLeft();
    /* If the incoming data is "right", run the "TurnRight" function */
    else if (data == "right") TurnRight();
    /* If the incoming data is "stop", run the "MotorStop" function */
    else if (data == "stop") MotorStop();
} 

/********************************************* FORWARD *****************************************************/
void MotorForward(void)   
{
  digitalWrite(Forward,HIGH);
  digitalWrite(Backward,LOW);
  digitalWrite(Left,LOW);
  digitalWrite(Right,LOW);
  digitalWrite(Stop,LOW);
}

/********************************************* BACKWARD *****************************************************/
void MotorBackward(void)   
{
  digitalWrite(Forward,LOW);
  digitalWrite(Backward,HIGH);
  digitalWrite(Left,LOW);
  digitalWrite(Right,LOW);
  digitalWrite(Stop,LOW);
}

/********************************************* TURN LEFT *****************************************************/
void TurnLeft(void)   
{
  digitalWrite(Forward,LOW);
  digitalWrite(Backward,LOW);
  digitalWrite(Left,HIGH);
  digitalWrite(Right,LOW);
  digitalWrite(Stop,LOW); 
}

/********************************************* TURN RIGHT *****************************************************/
void TurnRight(void)   
{
  digitalWrite(Forward,LOW);
  digitalWrite(Backward,LOW);
  digitalWrite(Left,LOW);
  digitalWrite(Right,HIGH);
  digitalWrite(Stop,LOW);
}

/********************************************* STOP *****************************************************/
void MotorStop(void)   
{
  digitalWrite(Forward,LOW);
  digitalWrite(Backward,LOW);
  digitalWrite(Left,LOW);
  digitalWrite(Right,LOW);
  digitalWrite(Stop,HIGH);
}

/********************************** RECEIVE DATA FROM the APP ******************************************/
String checkClient (void)
{
  while(!client.available()) delay(1); 
  String request = client.readStringUntil('\r');
  request.remove(0, 5);
  request.remove(request.length()-9,9);
  return request;
}

void connectWiFi()
{
  Serial.println("Connecting to WIFI");
  WiFi.begin(ssid, password);
  while ((!(WiFi.status() == WL_CONNECTED)))
  {
    delay(300);
    Serial.print("..");
  }
  delay(1100);
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("NodeMCU Local IP is : ");
  Serial.print((WiFi.localIP()));
}
