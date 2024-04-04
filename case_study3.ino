
#include <ESP8266WiFi.h>
#include <Adafruit_MQTT.h>
#include <Adafruit_MQTT_Client.h>
#include <LiquidCrystal.h>

// WiFi credentials
#define WIFI_SSID "*******"
#define WIFI_PASS "********"

// Adafruit IO credentials
#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883
#define AIO_USERNAME  "**********"
#define AIO_KEY       "********************************"

WiFiClient client;
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);
Adafruit_MQTT_Subscribe toggleButton = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/toggle-light");
Adafruit_MQTT_Publish counter = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/light-intensity");

#define LED_PIN D4  
#define LDR_PIN A0 

LiquidCrystal lcd(D0, D1, D2, D3, D5, D6);

void MQTT_connect();

void setup() {
  Serial.begin(115200);
  lcd.begin(16, 2);
  lcd.print("Connecting...");

  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected");
  lcd.clear();
  lcd.print("WiFi Connected");

  mqtt.subscribe(&toggleButton);
  pinMode(LED_PIN, OUTPUT);
}

void loop() {
  MQTT_connect();
  if (mqtt.connected()) {
    Adafruit_MQTT_Subscribe *subscription;
    while ((subscription = mqtt.readSubscription(5000))) {
      if (subscription == &toggleButton) {
        Serial.print(F("Got: "));
        Serial.println((char *)toggleButton.lastread);
        int state = strcmp((char *)toggleButton.lastread, "ON") == 0 ? HIGH : LOW;
        digitalWrite(LED_PIN, state);
        lcd.clear();
        lcd.print("LED: ");
        lcd.print(state == HIGH ? "ON" : "OFF");

      // Check if the LCD prints "LED ON" and then turn on the LED
      if (strcmp((char *)toggleButton.lastread, "ON") == 0) {
        digitalWrite(LED_PIN, HIGH);
      } else {
        digitalWrite(LED_PIN, LOW); // Ensure LED is off if "LED ON" is not printed
      }
      }
    }

    // Read the light intensity value
    int lightResistance = analogRead(LDR_PIN);
    Serial.print(F("\nSending light intensity val "));
    Serial.print(lightResistance);
    Serial.print("...");
    
    if (!counter.publish(lightResistance)) {
      Serial.println(F("Failed"));
    } else {
      Serial.println(F("OK!"));
    }

    lcd.setCursor(0, 1);
    lcd.print("Light: ");
    lcd.print(lightResistance);

    if (!mqtt.ping()) {
      mqtt.disconnect();
    }
  }
  delay(3000);
}

void MQTT_connect() {
  int8_t ret;
  if (mqtt.connected()) {
    return;
  }

  uint8_t retries = 3;
  while ((ret = mqtt.connect()) != 0) {
    Serial.println(mqtt.connectErrorString(ret));
    Serial.println("Retrying MQTT connection in 5 seconds...");
    mqtt.disconnect();
    delay(3000);
    retries--;
    if (retries == 0) {
      while (1);
    }
  }
  Serial.println("MQTT Connected!");
}
