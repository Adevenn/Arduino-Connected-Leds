/* ADEVEN CORP COPYRIGHT */

/*  Adruino UNO PINS
IR Receiver
  Data -> 2

NRF24L01 (3.3V is no module. 5V if module)
  CE    -> 9
  CSN   -> 10 (Hardware SPI SS)
  MOSI  -> 11 (Hardware SPI MOSI)
  MISO  -> 12 (Hardware SPI MISO)
  SCK   -> 13 (Hardware SPI SCK)
*/

#include <SPI.h>
#include <RF24.h>

//Radio pins (CE and CSN)
RF24 radio(9, 10);

//Led pins
const int greenPin = 3;
const int redPin = 5;
const int bluePin = 6;

//PWM values
int redValue = 0;
int greenValue = 0;
int blueValue = 0;

float brightness = 0;
unsigned long lastRepetitionTime;

bool isJumpEffectOn;
bool isBreathEffectOn;
bool isBreathUp = false;
bool isFadeEffectOn;

//Pipe name
const byte pipe[6] = "ADV01";

struct Datas {
  int color;
  int effect;
  int delay;
  int maxBrigthness;
};
Datas _d;

void setup() {
  pinMode(redPin,OUTPUT);
  pinMode(greenPin,OUTPUT);
  pinMode(bluePin,OUTPUT);

  radio.begin();
  radio.setChannel(125);
  radio.setDataRate(RF24_250KBPS);
  radio.openReadingPipe(0, pipe);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();
}

void loop() {
  breathEffect();
  
  if (radio.available()) {
    Datas d = _d;
    radio.read(&_d, sizeof(_d));

    if(_d.color != d.color){
      switch(_d.color){
        case 24 : //Yellow
          redValue = 255;
          greenValue = 80;
          blueValue = 0;
          applyColor(redValue, greenValue, blueValue);
          break;
        case 25 : //Medium Blue 1
          redValue = 10;
          greenValue = 150;
          blueValue = 240;
          applyColor(redValue, greenValue, blueValue);
          break;
        case 26 : //Pink
          redValue = 255;
          greenValue = 0;
          blueValue = 125;
          applyColor(redValue, greenValue, blueValue);
          break;
        case 27 : //Grey
          redValue = 148;
          greenValue = 70;
          blueValue = 180;
          applyColor(redValue, greenValue, blueValue);
          break;
        case 28 : //Orange Light
          redValue = 255;
          greenValue = 50;
          blueValue = 0;
          applyColor(redValue, greenValue, blueValue);
          break;
        case 29 : //Light Blue 2
          redValue = 20;
          greenValue = 180;
          blueValue = 213;
          applyColor(redValue, greenValue, blueValue);
          break;
        case 30 : //Violet star wars
          redValue = 150;
          greenValue = 8;
          blueValue = 255;
          applyColor(redValue, greenValue, blueValue);
          break;
        case 64 : //Light off
          redValue = 0;
          greenValue = 0;
          blueValue = 0;
          applyColor(0, 0, 0);
          break;
        case 68 : //White
          redValue = 255;
          greenValue = 255;
          blueValue = 255;
          applyColor(redValue, greenValue, blueValue);
          break;
        case 69 : //Blue
          redValue = 0;
          greenValue = 0;
          blueValue = 255;
          applyColor(redValue, greenValue, blueValue);
          break;
        case 73 : //Blue Medium 1
          redValue = 0;
          greenValue = 50;
          blueValue = 255;
          applyColor(redValue, greenValue, blueValue);
          break;
        case 77 : //Violet
          redValue = 125;
          greenValue = 0;
          blueValue = 250;
          applyColor(redValue, greenValue, blueValue);
          break;
        case 80 : //Orange
          redValue = 255;
          greenValue = 40;
          blueValue = 0;
          applyColor(redValue, greenValue, blueValue);
          break;
        case 81 : //Blue Light
          redValue = 50;
          greenValue = 100;
          blueValue = 255;
          applyColor(redValue, greenValue, blueValue);
          break;
        case 84 : //Orange Dark
          redValue = 230;
          greenValue = 10;
          blueValue = 0;
          applyColor(redValue, greenValue, blueValue);
          break;
        case 85 : //Light Green
          redValue = 8;
          greenValue = 200;
          blueValue = 8;
          applyColor(redValue, greenValue, blueValue);
          break;
        case 88 : //Red
          redValue = 255;
          greenValue = 0;
          blueValue = 0;
          applyColor(redValue, greenValue, blueValue);
          break;
        case 89 : //Green
          redValue = 0;
          greenValue = 255;
          blueValue = 0;
          applyColor(redValue, greenValue, blueValue);
          break;
      }
    }

    if(_d.effect != d.effect){
      switch(_d.effect){
        case 5 : //Jump Effect
          isJumpEffectOn = true;
          isBreathEffectOn = false;
          isFadeEffectOn = false;
          brightness = _d.maxBrigthness;
          applyColor(redValue, greenValue, blueValue);
          break;
        case 7 : //Breath Effect
          isJumpEffectOn = false;
          isBreathEffectOn = true;
          isFadeEffectOn = false;
          break;
      }
    }

    if(_d.maxBrigthness != d.maxBrigthness && isJumpEffectOn){
      brightness = _d.maxBrigthness;
      applyColor(redValue, greenValue, blueValue);
    }
  }
}

void fade(){

}

void breathEffect(){
  if(isBreathEffectOn && millis() > (lastRepetitionTime + _d.delay)){
    if(_d.maxBrigthness <= 0 && brightness != 0){
      brightness = 0;
      applyColor(redValue, greenValue, blueValue);
    }
    else if (_d.maxBrigthness > 0){
      if(brightness >= _d.maxBrigthness){
        isBreathUp = false;
      }
      else if(brightness <= 5){
        isBreathUp = true;
      }
      if(isBreathUp){
        brightness += 2;
      }
      else{
        brightness -= 2;
      }
      applyColor(redValue, greenValue, blueValue);
    }
    lastRepetitionTime = millis();
  }
}

///Apply the new color to leds
void applyColor(int newRed, int newGreen, int newBlue) {
  analogWrite(redPin, newRed*(brightness/100));
  analogWrite(greenPin, newGreen*(brightness/100));
  analogWrite(bluePin, newBlue*(brightness/100));
}