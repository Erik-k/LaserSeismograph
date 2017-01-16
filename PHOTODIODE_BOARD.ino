
// KNOWN ISSUES:
// PD0 is connected to ch. 1, PD1 is connected to ch. 0 on board. 

// FUTURE IMPROVEMENTS:
// Accelerometer jostle detection
// Accelerometer leveling and calibration on startup, first for itself and then
//    for the PDs.
// Door-opened detection if all four PDs suddenly report increase in average light. 
//    Data is then invalid while ambient light is entering. This problably isn't
//    necessary since the laser light is so much stronger than ambient. 

////////
// Accelerometer includes
////////
#include <Wire.h>
#include "Adafruit_MMA8451.h"
#include "Adafruit_Sensor.h"

#include <SPI.h> // defines MOSI=D13, MISO=D12, SCLK=D14, SDA=D2

// Include the ESP8266 WiFi library. (Works a lot like the
// Arduino WiFi library.)
//#include <ESP8266WiFi.h>
//#include <Phant.h>

//////////////////////
// WiFi Definitions //
//////////////////////
const char WiFiSSID[] = "Korry_Guest";
const char WiFiPSK[] = "KEis#1inWA";

////////////////
// Phant Keys
// Data shows up at: https://data.sparkfun.com/streams/o8zpRqxZWpFx5OqMwMaY
////////////////
//const char PhantHost[] = "128.0.128.233";
const int httpPort = 8080;
const char PublicKey[] = "o8zpRqxZWpFx5OqMwMaY";
const char PrivateKey[] = "yzladNDYyaceBaygEgM7";

/////////////////
// Post Timing //
/////////////////
const unsigned long postRate = 60000;
unsigned long lastPost = 0;

///////////////
// MAIN      //
///////////////

const int CS = 16;   
const int EOC = 4;
const int LED_PIN = 5; // Thing's onboard, green LED

// MSB: [011]   [01]      [00]       [1]
//       ch3   scan 0-3  stay on  internal clk
const byte MAX1067_init = B01101001; // 0x69
// MSB: [011]   [00]          [00]       [1]
//       ch3   single chan  stay on  internal clk
//const byte MAX1067_init = B01101000; //0x68
const byte ADC_ch_0 = B00000000;
const byte ADC_ch_1 = B00100000;
const byte ADC_ch_2 = B01000000;
const byte ADC_ch_3 = B01100000;
const byte scan3fourx=B01111000;
const byte scan0fourx=B00011000;

unsigned int result = 0;  // This is 4 bytes large
unsigned int PD0_result = 0; 
unsigned int PD1_result = 0;
unsigned int PD2_result = 0;
unsigned int PD3_result = 0;

Adafruit_MMA8451 mma = Adafruit_MMA8451();

void setup() {
  Serial.begin(115200);
  SPI.begin();  // This is necessary as well as the .beginTransaction(x,y,z) func.  
  
  pinMode(CS, OUTPUT);
  digitalWrite(CS, HIGH); // so that we can set it low when we're ready
  pinMode(EOC, INPUT);

//  connectWiFi();
                                                                         
  Serial.println();
  Serial.println("Ending setup.");

}

void loop() {
  delay(200);
  Serial.println();
  readADC(MAX1067_init);

//  if(postToPhant(PD0_result, PD1_result, PD2_result, PD3_result)) {
//    Serial.println("Post succeeded");
//  }
//  else {
//    Serial.println("Post failed.");
//  }
}

///////////////
// FUNCTIONS //
///////////////

//void connectWiFi()
//{
//  byte ledStatus = LOW;
//  Serial.println();
//  Serial.println("Connecting to: " + String(WiFiSSID));
//  // Set WiFi mode to station (as opposed to AP or AP_STA)
//  WiFi.mode(WIFI_STA);
//
//  // WiFI.begin([ssid], [passkey]) initiates a WiFI connection
//  // to the stated [ssid], using the [passkey] as a WPA, WPA2,
//  // or WEP passphrase.
//  WiFi.begin(WiFiSSID, WiFiPSK);
//
//  // Use the WiFi.status() function to check if the ESP8266
//  // is connected to a WiFi network.
//  while (WiFi.status() != WL_CONNECTED)
//  {
//    // Blink the LED
//    digitalWrite(LED_PIN, ledStatus); // Write LED high/low
//    ledStatus = (ledStatus == HIGH) ? LOW : HIGH;
//
//    // Delays allow the ESP8266 to perform critical tasks
//    // defined outside of the sketch. These tasks include
//    // setting up, and maintaining, a WiFi connection.
//    delay(100);
//    // Potentially infinite loops are generally dangerous.
//    // Add delays -- allowing the processor to perform other
//    // tasks -- wherever possible.
//  }
//  Serial.println("WiFi connected");  
//  Serial.println("IP address: ");
//  Serial.println(WiFi.localIP());
//}

//int postToPhant(int value1, int value2, int value3, int value4)
//{
//  // LED turns on when we enter, it'll go off when we 
//  // successfully post.
//  digitalWrite(LED_PIN, LOW);
//
//  // Declare an object from the Phant library - phant
//  Phant phant("data.sparkfun.com", PublicKey, PrivateKey);
//
//  // Do a little work to get a unique-ish name. Append the
//  // last two bytes of the MAC (HEX'd) to "Thing-":
////  uint8_t mac[WL_MAC_ADDR_LENGTH];
////  WiFi.macAddress(mac);
////  String macID = String(mac[WL_MAC_ADDR_LENGTH - 2], HEX) +
////                 String(mac[WL_MAC_ADDR_LENGTH - 1], HEX);
////  macID.toUpperCase();
////  String postedID = "EriksThingDev-" + macID;
//
//  // Add the four field/value pairs defined by our stream:
//  phant.add("PD0", value1);
//  phant.add("PD1", value2);
//  phant.add("PD2", value3);
//  phant.add("PD3", value4);
//
//  // Now connect to data.sparkfun.com, and post our data:
//  WiFiClient client;
//  Serial.print("Now trying to connect to ");
//  Serial.print("data.sparkfun.com");
//  Serial.print(":");
//  Serial.println(httpPort);
//  
//  if (!client.connect("data.sparkfun.com", httpPort)) 
//  {
//    // If we fail to connect, return 0.
//    return 0;
//  }
//  // If we successfully connected, print our Phant post:
//  client.print(phant.post());
//
//  // Read all the lines of the reply from server and print them to Serial
//  while(client.available()){
//    String line = client.readStringUntil('\r');
//    Serial.print(line); // Trying to avoid using serial
//  }
//
//  // Before we exit, turn the LED off.
//  digitalWrite(LED_PIN, HIGH);
//
//  return 1; // Return success
//}


byte OPCODE_parser(byte opcode){
  Serial.print("Currently using an Op-Code that specifies: ");
  
  byte channel_mask = B11100000 & opcode;
  switch (channel_mask){
    case B00000000:
      Serial.print("channel 0, ");
      break;
    case B00100000:
      Serial.print("channel 1, ");
      break;
    case B01000000:
      Serial.print("channel 2, ");
      break;
    case B01100000:
      Serial.print("channel 3, ");
  }
  
  byte scanmode_mask = B00011000 & opcode;
  switch (scanmode_mask){
    case B00000000:
      Serial.print("single channel no scan, ");
      break;
    case B00001000:
      Serial.print("sequentially scan channels 0-N, ");
      break;
    case B00010000:
      Serial.print("scan channels 2-N, ");
      break;
    case B00011000:
      Serial.print("scan channel N four times, ");
      break;
  }

  byte clock_mask = B00000001 & opcode;
  switch (clock_mask) {
    case B00000000:
      Serial.println("external clock.");
      break;
    case B00000001:
      Serial.println("internal clock.");
      break;
  }
  return opcode; //This lets me use this function within the call to readADC()
}

unsigned int DropTwoLSB(unsigned int inputData){
  // Drop lowest two bits and realign D14 through D0. 
  unsigned int keepMask = 0xFFFC; //four bytes long = 32 bits
  return ((keepMask & inputData) >> 2);
}

void EOCstatus(void){
  // Use this for scan mode 
  Serial.print("EOC is currently: ");
  if (digitalRead(EOC) == HIGH){
    Serial.println("HIGH.");
  }
  else{
    Serial.println("LOW.");
  }
}

unsigned int readADC(byte sendByte){
  
  byte inByte = 0; // buffer to hold incoming data
  result = 0;

  SPI.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE0));
  digitalWrite(CS, LOW);

  // Use this for scanning each channel individually
  SPI.transfer(sendByte);

  // Can't use EOC while using external clk. 
  while (digitalRead(EOC)) {
    yield();
  }
  
  inByte = SPI.transfer(sendByte);
  result = inByte;
  inByte = SPI.transfer(sendByte);
  result = result << 8;
  result = result | inByte;
  PD1_result = DropTwoLSB(result);
  //Serial.print("Result PD1: ");
  Serial.print(PD1_result);
  Serial.print(",");

  inByte = SPI.transfer(sendByte);
  result = inByte;
  inByte = SPI.transfer(sendByte);
  result = result << 8;
  result = result | inByte;
  PD0_result = DropTwoLSB(result);
  //Serial.print("Result PD0: ");
  Serial.print(PD0_result);
  Serial.print(",");

  inByte = SPI.transfer(sendByte);
  result = inByte;
  inByte = SPI.transfer(sendByte);
  result = result << 8;
  result = result | inByte;
  PD2_result = DropTwoLSB(result);
  //Serial.print("Result PD2: ");
  Serial.print(PD2_result);
  Serial.print(",");

  inByte = SPI.transfer(sendByte);
  result = inByte;
  inByte = SPI.transfer(sendByte);
  result = result << 8;
  result = result | inByte;
  PD3_result = DropTwoLSB(result);
  //Serial.print("Result PD3: ");
  Serial.print(PD3_result);
  Serial.print(",");
 
  SPI.endTransaction();
  digitalWrite(CS, HIGH);
  
  return (result);
}
