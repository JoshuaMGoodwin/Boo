#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>
#include <Servo.h>

AudioPlaySdWav           playSdWav1;
AudioOutputMQS           i2s1;
AudioConnection          patchCord1(playSdWav1, 0, i2s1, 0);
AudioConnection          patchCord2(playSdWav1, 1, i2s1, 1);
AudioControlSGTL5000     sgtl5000_1;

#define SDCARD_CS_PIN    BUILTIN_SDCARD
#define SDCARD_MOSI_PIN  11  // not actually use
#define SDCARD_SCK_PIN   13  // not actually used

// setup variables
Servo myServo;
int servoHome = 90;
int servoPos = 90;
int servoPin = 25;
int ledPin = 28;
int buttonPin = 37;
// audio goes to pin 12

void setup() {
  // setup LED pin
  pinMode(ledPin, OUTPUT);
  
  // setup button pin
  pinMode(buttonPin, INPUT_PULLUP);

  // setup servo and set it to home
  myServo.attach(servoPin);
  myServo.write(servoHome);

  // audio setup
  Serial.begin(9600);
  AudioMemory(8);
  sgtl5000_1.enable();
  sgtl5000_1.volume(0.1);
  SPI.setMOSI(SDCARD_MOSI_PIN);
  SPI.setSCK(SDCARD_SCK_PIN);
  if (!(SD.begin(SDCARD_CS_PIN))) {
    while (1) {
      Serial.println("Unable to access the SD card");
      delay(500);
    }
  }
  delay(1000);
}

void loop() {
  
  // the else side triggers if a button is pushed
  if (digitalRead(buttonPin)){ 
    } else {
      // if sound isn't playing, start it
      if (playSdWav1.isPlaying() == false) {
        playSdWav1.play("BOOLAUGH.WAV");
        delay(10); // wait for library to parse WAV info
      }  
    }
  
  // if music is playing, move the servo and light the LED
  if (playSdWav1.isPlaying()) {
    digitalWrite(ledPin, HIGH);
    myServo.write(68);
    delay(150);
    myServo.write(112);
    delay(150);
    servoPos=112;
  } else {
      // if music isn't playing, set the servo back to home and turn light off 
      if (servoPos != servoHome) {
        myServo.write(servoHome);
        servoPos = servoHome;
        digitalWrite(ledPin, LOW);
      }
    }
}
