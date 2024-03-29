//https://docs.particle.io/reference/device-os/api/udp/beginpacket/
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <WiFi.h>
#include <WiFiUdp.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

#define OLED_RESET     -1 // Reset pin for the display
#define SCREEN_ADDRESS 0x3C // I2C address of the OLED display
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

const char *ssid = "ATT7SIBPfi"; // "and" "ATT7SIBPfi"
const char *password = "pgpn6j2dzy3p"; // "lunabotics" "pgpn6j2dzy3p"

const int ledPin25 = 25;
const int ledPin26 = 26;
const int ledPin27 = 27;

WiFiUDP udp;
unsigned int localPort = 3333; // local port to listen on
char packetBuffer[1000]; //buffer for incoming packets

void setup() {
  Serial.begin(115200);
  pinMode(25, OUTPUT);
  pinMode(26, OUTPUT);
  pinMode(27, OUTPUT);
  // Initialize the OLED display
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  display.clearDisplay();
  
  //connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to the WiFi network");

  //starting UDP
  udp.begin(localPort);
  Serial.printf("Now listening at IP %s, UDP port %d\n", WiFi.localIP().toString().c_str(), localPort);
}
void displayMessage() {
  //Initial Text
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.println("Hello world!");
  display.display();
  delay(2000);
  display.clearDisplay();

  // Display Inverted Text
  display.setTextColor(BLACK, WHITE); // 'inverted' text
  display.setCursor(0,28);
  display.println("Hello world!");
  display.display();
  delay(2000);
  display.clearDisplay();

  // Changing Font Size
  display.setTextColor(WHITE);
  display.setCursor(0,24);
  display.setTextSize(2);
  display.println("Hello!");
  display.display();
  delay(2000);
  display.clearDisplay();

  // Display Numbers
  display.setTextSize(1);
  display.setCursor(0,28);
  display.println("03/07/2024");
  display.display();
  delay(2000);
  display.clearDisplay();
/*
  // Specifying Base For Numbers
  display.setCursor(0,28);
  display.print("0x"); display.print(0xFF, HEX); 
  display.print("(HEX) = ");
  display.print(0xFF, DEC);
  display.println("(DEC)"); 
  display.display();
  delay(2000);
  display.clearDisplay();

  // Display ASCII Characters
  display.setCursor(0,24);
  display.setTextSize(2);
  display.write(3);
  display.display();
  delay(2000);
  display.clearDisplay();
*/
  // Scroll full screen
  display.setCursor(0,0);
  display.setTextSize(1);
  display.println("CSU Robotics Club");
  display.println("NASA");
  display.println("Lunabotics");
  display.display();
  display.startscrollright(0x00, 0x07);
  delay(2000);
  display.stopscroll();
  delay(1000);
  display.startscrollleft(0x00, 0x07);
  delay(2000);
  display.stopscroll();
  delay(1000);    
  display.startscrolldiagright(0x00, 0x07);
  delay(2000);
  display.startscrolldiagleft(0x00, 0x07);
  delay(2000);
  display.stopscroll();
  display.clearDisplay();

  // Scroll part of the screen
  display.setCursor(0,0);
  display.setTextSize(1);
  display.println("CSU Robotics Club");
  display.println("NASA");
  display.println("Lunabotics.");
  display.display();
  display.startscrollright(0x00, 0x00);
}
void turnDisplayOff() {
  display.clearDisplay(); //Clear the display buffer
  display.display(); //Update display to show the clear
}

void loop() {
  int size = udp.parsePacket();
  if (size) {
    //receive incoming UDP packets
    Serial.printf("Received %d bytes from %s, port %d\n", size, udp.remoteIP().toString().c_str(), udp.remotePort());
    int input = udp.read(packetBuffer, 1000);
    Serial.printf("UDP packet contents: %s\n", packetBuffer);
    switch (packetBuffer[0]) { //simple case statements to control GPIO pins via UDP inputs
      case '0': digitalWrite(25, LOW);  
      break; //turn OFF LED on GPIO 25
      case '1': digitalWrite(25, HIGH); 
      break; //turn ON LED on GPIO 25
      case '2': digitalWrite(26, LOW);  
      break; //turn OFF LED on GPIO 26
      case '3': digitalWrite(26, HIGH); 
      break; //turn ON LED on GPIO 26
      case '4': digitalWrite(27, LOW);  
      break; //turn OFF LED on GPIO 27
      case '5': digitalWrite(27, HIGH); 
      break; //turn ON LED on GPIO 27
      case '6': turnDisplayOff();
      break; //turn OLED display OFF
      case '7': displayMessage(); 
      break;  //turn OLED display ON
      default:  Serial.println("Invalid input!"); 
      break;
    }
    udp.endPacket();
  }
}