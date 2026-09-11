// secrets.h contine BLYNK_TEMPLATE_ID / NAME / AUTH_TOKEN si WIFI_SSID / WIFI_PASS.
// Trebuie inclus INAINTE de <BlynkSimpleEsp32.h> (Blynk citeste define-urile la include).
#include "secrets.h"

#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include <ESP32Servo.h>

char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = WIFI_SSID;
char pass[] = WIFI_PASS;

int ledRosu = 23;
int ledVerde = 17;
int buzzerPin = 5;
int echoPin = 19;
int trigPin = 18;
float duration, distance;
bool alertaTrimisa = false;
int buttonPin = 21;
bool sistemArmat = false;
bool ultimaStareButon = HIGH;
void setup(){
  Serial.begin(115200);
  pinMode(ledRosu, OUTPUT);
  pinMode(ledVerde, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  // Configuram Servomotorul

  digitalWrite(ledVerde, HIGH);
  digitalWrite(ledRosu, LOW);
  Blynk.begin(auth, ssid, pass);
  
}

void loop() {
  Blynk.run();
  bool stareButonAcum = digitalRead(buttonPin);
  if(stareButonAcum == LOW && ultimaStareButon == HIGH)
  {
    sistemArmat = !sistemArmat;
    alertaTrimisa = false;
    delay(200);
  }
  if(sistemArmat)
    {
      digitalWrite(ledVerde, LOW);
      digitalWrite(ledRosu, HIGH);
      digitalWrite(trigPin, HIGH);
      delayMicroseconds(10);
      digitalWrite(trigPin, LOW);
      duration = pulseIn(echoPin, HIGH);
      distance = duration * 0.034/2;
      if (distance < 20)
      {
        tone(buzzerPin, 2000);
        if(alertaTrimisa == false)
        {
          Blynk.logEvent("esp32", "Atenție! Cineva este la ușă!");
          alertaTrimisa = true;
        }  
      }
      else
      {
        noTone(buzzerPin);
        alertaTrimisa = false;
      }
    }
    else
    {
      digitalWrite(ledVerde, HIGH);
      digitalWrite(ledRosu, LOW);
      noTone(buzzerPin);
      alertaTrimisa = false;
    }
  ultimaStareButon = stareButonAcum;
  delay(100); 
}
