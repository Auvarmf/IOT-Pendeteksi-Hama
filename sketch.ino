#define BLYNK_TEMPLATE_ID "TMPL6h9YGAJ4W"
#define BLYNK_TEMPLATE_NAME "deteksi serangga"
#define BLYNK_AUTH_TOKEN "b2-0K57-cj65Zx01wuEe_nVFNX2iXaY9"
#define BLYNK_PRINT Serial

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

char auth[] = BLYNK_AUTH_TOKEN; //Auth Token
char ssid[] = "Wokwi-GUEST"; //nama hotspot yang digunakan
char pass[] = ""; //password hotspot yang digunakan

BlynkTimer timer;

#define Echo 15
#define Trig 4
#define merah 14

int buzzerPin = 13;
float distance = 0;
int switchBuzz = 0;
int switchLed = 0;

void myTone(int pin){
  ledcAttachPin(pin, 0);
  ledcWriteNote(0, NOTE_F, 4);
}

void myNoTone(int pin){
  ledcDetachPin(pin);
}

BLYNK_WRITE(V2){
  int pinData = param.asInt();
  Serial.println(pinData);
  if (pinData == 1){
    switchBuzz = 1;
    switchLed = 1;
  }else{
    switchBuzz = 0;
    switchLed = 0;
  }
}

void data_blynk(){
  digitalWrite(Trig, HIGH);
  delay(10);
  digitalWrite(Trig, LOW);

  int duration = pulseIn(Echo, HIGH);
  distance = duration/58;

  Blynk.virtualWrite(V0, distance);
  if (distance < 200){

    //buzz
    if (switchBuzz == 1){
      myTone(buzzerPin);
    }else{
      myNoTone(buzzerPin);
    }

    //LED
    if (switchLed == 1){
      Blynk.virtualWrite(V1, 1);
      digitalWrite(merah, HIGH);
    }else{
      Blynk.virtualWrite(V1, 0);
      digitalWrite(merah, LOW);
    }
  }else{
    Blynk.virtualWrite(V1, 0);
    digitalWrite(merah, LOW);
    myNoTone(buzzerPin);
  }

  Serial.print("Jarak Serangga : ");
  Serial.print(distance);
  Serial.println(" cm");
}

void setup() {
  Serial.begin(115200);
  Serial.println("Copyright © Auvarmf");
  Blynk.begin(auth, ssid, pass);
  pinMode(merah, OUTPUT);
  pinMode(Trig, OUTPUT);
  pinMode(Echo, INPUT);
  pinMode(buzzerPin, OUTPUT);
  timer.setInterval(2500L, data_blynk);
}

void loop() {
  delay(100);
  data_blynk();
  Blynk.run();
  timer.run();
}
