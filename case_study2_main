#include <LiquidCrystal.h>

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
int temp;
int moisture; // Variable for soil moisture
String ssid     = "******";  // SSID to connect to
String password = "********"; // Our virtual wifi has no password 
String host     = "api.thingspeak.com"; // ThingSpeak API
const int httpPort   = 80;
String baseUri     = "/update?api_key=****************"; // Base URI for ThingSpeak

int setupESP8266(void) {
  // Start our ESP8266 Serial Communication
  Serial.begin(115200);   // Serial connection over USB to computer
  Serial.println("AT");   // Serial connection on Tx / Rx port to ESP8266
  delay(10);        // Wait a little for the ESP to respond
  if (!Serial.find("OK")) return 1;
    
  // Connect to 123D Circuits Simulator Wifi
  Serial.println("AT+CWJAP=\"" + ssid + "\",\"" + password + "\"");
  delay(10);        // Wait a little for the ESP to respond
  if (!Serial.find("OK")) return 2;
  
  // Open TCP connection to the host:
  Serial.println("AT+CIPSTART=\"TCP\",\"" + host + "\"," + httpPort);
  delay(50);        // Wait a little for the ESP to respond
  if (!Serial.find("OK")) return 3;
  
  return 0;
}

void anydata(void) {
  temp = map(analogRead(A0), 20, 358, -40, 125); // Read temperature
  moisture = analogRead(A1); // Read soil moisture from A1
  
  // Corrected HTTP call for soil moisture and temperature
  String httpPacket = "GET " + baseUri + "&field1=" + String(moisture) + "&field2=" + String(temp) + " HTTP/1.1\r\nHost: " + host + "\r\n\r\n";
  int length = httpPacket.length();
  
  Serial.print("AT+CIPSEND=");
  Serial.println(length);
  delay(10); // Wait a little for the ESP to respond
  Serial.print(httpPacket);
  delay(10); // Wait for the data to be sent
}

void setup() {
  setupESP8266();
  lcd.begin(16, 2);        
}

void loop() {
  anydata();
  
  // Update LCD with temperature and soil moisture
  lcd.setCursor(0, 0);
  lcd.print("Temp: ");
  lcd.print(temp);
  lcd.print(" C");
  
  lcd.setCursor(0, 1);
  lcd.print("Moisture: ");
  lcd.print(moisture);
  
  delay(20000); // Delay to respect ThingSpeak's 15-second rule and ensure stability
}
