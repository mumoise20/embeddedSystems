
#define BLYNK_PRINT Serial


#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#define smartWashRoom 2
// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "Nm50oLwWgA1yF5IiuHGpPMeQDe22w_7C";

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "Nokia 3";
char pass[] = "mynetwork";

void setup()
{
  // Debug console
  Serial.begin(9600);

  Blynk.begin(auth, ssid, pass);
  pinMode(smartWashRoom, OUTPUT);
}

void loop()
{
  Blynk.run();
}
