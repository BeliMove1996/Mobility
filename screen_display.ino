#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <ESP8266WiFi.h>
WiFiClient client;
WiFiServer server(80);
const char* ssid = "frank";
const char* password = "belimove";
String  data =""; 
int Forward = 2;     /* GPIO2(D4)    */
int Backward = 15;   /* GPIO15(D8)   */
int Left = 0;    /* GPIO0(D3)    */
int Right = 13;  /* GPIO13(D7)   */
int Stop = 14; /* GPIO14(D5)  */

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
#define NUMFLAKES     10 // Number of snowflakes in the animation example
#define LOGO_HEIGHT   16
#define LOGO_WIDTH    16
static const unsigned char PROGMEM logo_bmp[] =
{ 0b00000000, 0b11000000,
  0b00000001, 0b11000000,
  0b00000001, 0b11000000,
  0b00000011, 0b11100000,
  0b11110011, 0b11100000,
  0b11111110, 0b11111000,
  0b01111110, 0b11111111,
  0b00110011, 0b10011111,
  0b00011111, 0b11111100,
  0b00001101, 0b01110000,
  0b00011011, 0b10100000,
  0b00111111, 0b11100000,
  0b00111111, 0b11110000,
  0b01111100, 0b11110000,
  0b01110000, 0b01110000,
  0b00000000, 0b00110000 };

void setup() {
  pinMode(Forward, OUTPUT);
  pinMode(Backward, OUTPUT); 
  pinMode(Left, OUTPUT);  
  pinMode(Right, OUTPUT);
  pinMode(Stop, OUTPUT); 
  Serial.begin(9600);
  
  digitalWrite(Forward,LOW);
  digitalWrite(Backward,LOW);
  digitalWrite(Left,LOW);
  digitalWrite(Right,LOW);
  digitalWrite(Stop,LOW);
  connectWiFi();
  server.begin();

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  // Show initial display buffer contents on the screen --
  // the library initializes this with an Adafruit splash screen.
  display.display();
  delay(2000); // Pause for 2 seconds

  // Clear the buffer
  display.clearDisplay();

  // Show the display buffer on the screen. You MUST call display() after
  // drawing commands to make them visible on screen!
  display.display();
  delay(2000);
  // display.display() is NOT necessary after every single drawing command,
  // unless that's what you want...rather, you can batch up a bunch of
  // drawing operations and then update the screen all at once by calling
  // display.display(). These examples demonstrate both approaches...
  testdrawbitmap();    // Draw a small bitmap image

  // Invert and restore display, pausing in-between
  display.invertDisplay(true);
  delay(1000);
  display.invertDisplay(false);
  delay(1000);
  writeword(WiFi.localIP().toString());
}

void loop() {
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


void testdrawbitmap(void) {
  display.clearDisplay();

  display.drawBitmap(
    (display.width()  - LOGO_WIDTH ) / 2,
    (display.height() - LOGO_HEIGHT) / 2,
    logo_bmp, LOGO_WIDTH, LOGO_HEIGHT, 1);
  display.display();
  delay(1000);
}

/********************************************* FORWARD *****************************************************/
void MotorForward(void)   
{
  digitalWrite(Forward,HIGH);
  digitalWrite(Backward,LOW);
  digitalWrite(Left,LOW);
  digitalWrite(Right,LOW);
  digitalWrite(Stop,LOW);
  display.clearDisplay();
  Serial.println("moving forward");
  writeword("moving    forward");
}

/********************************************* BACKWARD *****************************************************/
void MotorBackward(void)   
{
  digitalWrite(Forward,LOW);
  digitalWrite(Backward,HIGH);
  digitalWrite(Left,LOW);
  digitalWrite(Right,LOW);
  digitalWrite(Stop,LOW);
  Serial.println("moving backward");
  writeword("moving    backward");
}

/********************************************* TURN LEFT *****************************************************/
void TurnLeft(void)   
{
  digitalWrite(Forward,LOW);
  digitalWrite(Backward,LOW);
  digitalWrite(Left,HIGH);
  digitalWrite(Right,LOW);
  digitalWrite(Stop,LOW);
  Serial.println("turn left"); 
  writeword("turn      left");
}

/********************************************* TURN RIGHT *****************************************************/
void TurnRight(void)   
{
  digitalWrite(Forward,LOW);
  digitalWrite(Backward,LOW);
  digitalWrite(Left,LOW);
  digitalWrite(Right,HIGH);
  digitalWrite(Stop,LOW);
  Serial.println("turn right");
  writeword("turn      right");
}

/********************************************* STOP *****************************************************/
void MotorStop(void)   
{
  digitalWrite(Forward,LOW);
  digitalWrite(Backward,LOW);
  digitalWrite(Left,LOW);
  digitalWrite(Right,LOW);
  digitalWrite(Stop,HIGH);
  Serial.println("stop");
  writeword("stop");
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
  Serial.println((WiFi.localIP()));
}

void writeword (String wrd)
{
  display.clearDisplay();
  display.setTextSize(2);             // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE);        // Draw white text
  display.setCursor(0,0);             // Start at top-left corner
  display.println(wrd);
  display.display();
}
