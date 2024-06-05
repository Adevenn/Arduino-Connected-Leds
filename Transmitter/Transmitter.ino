/* ADEVEN CORP COPYRIGHT */

/* Adruino UNO PINS
IR receiver (5V)
  G -> GND
  R -> 5V
  Y  -> 7
NRF24L01 (3.3V is no module. 5V if module)
  CE    -> 9
  CSN   -> 10
  MOSI  -> 11
  MISO  -> 12
  SCK   -> 13
*/

#include <SPI.h>
#include <RF24.h>
#include <IRremote.hpp>
#include <Adafruit_SSD1306.h>

#define WIRE Wire

//(screenHeight, screenWidth, wire)
Adafruit_SSD1306 screen = Adafruit_SSD1306(128, 32, &WIRE);

//Set radio pins (CE and CSN)
RF24 radio(9, 10);    // Instanciation du NRF24L01
//Set IReceiver pin
const int IR_PIN = 2;

//Pipe name, for radio requests
const byte pipe[6] = "ADV01";

///Effect's names
const char jump[4]  = "Jump";
const char breath[6] = "Breath";

///Color's names
const char yellow[6] = "Yellow";
const char blueMedium[6] = "Blue M";
const char pink[4] = "Pink";
const char grey[4] = "Grey";
const char orangeLight[8] = "Orange L";
const char blueLight2[7] = "Blue L2";
const char violetStarWars[10] = "Violet S-W";
const char lightOff[3] = "Off";
const char white[5] = "White";
const char blue[4] = "Blue";
const char blueMedium2[7] = "Blue M2";
const char violet[6] = "Violet";
const char orange[6] = "Orange";
const char blueLight[6] = "Blue L";
const char orangeDark[8] = "Orange D";
const char greenLight[7] = "Green L";
const char red[3] = "Red";
const char green[5] = "Green";

//Max size : 32 bytes
struct Datas {
  int color = 64;
  int effect = 5;
  int delay = 50;
  int maxBrigthness = 100;
};

Datas _d;

void setup() {
  setupScreen();
  IrReceiver.begin(IR_PIN);
  setupRadio();
  updateScreen();
  writePayload();
}

void setupRadio(){
  radio.begin();
  radio.setChannel(125);
  radio.setDataRate(RF24_250KBPS);
  radio.openWritingPipe(pipe);
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();
}

void setupScreen(){
  screen.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  screen.setTextColor(SSD1306_WHITE);
  screen.setTextSize(1);
}

void loop() {
  if(IrReceiver.decode()){ //Is infrared receiver get a data ?
    Datas d = _d;
    int newCode = IrReceiver.decodedIRData.command;
    switch(newCode){
      case 5 : //effects
      case 7 :
        _d.effect = newCode;
        break;
      case 19 : //Delay Slow
        _d.delay += 10;
        break;
      case 23 : //Delay Quick
        if(_d.delay > 10){
          _d.delay -= 10;
        }
        break;
      case 24 : //Colors
      case 25 :
      case 26 :
      case 27 :
      case 28 :
      case 29 :
      case 30 :
      case 64 :
      case 68 :
      case 69 :
      case 73 :
      case 77 :
      case 80 :
      case 81 :
      case 84 :
      case 85 :
      case 88 :
      case 89 :
        _d.color = newCode;
        break;
      case 92 : //maxBrightness up
        _d.maxBrigthness += 10;
        if(_d.maxBrigthness > 100){
          _d.maxBrigthness = 100;
        }
        break;
      case 93 : //maxBrightness down
        _d.maxBrigthness -= 10;
        if(_d.maxBrigthness < 0){
          _d.maxBrigthness = 0;
        }
      break;
    }
    if(!dataCompare(d)){
      updateScreen();
    }
    writePayload();
    delay(100);
    IrReceiver.resume();
  }
}

///Compare 2 Datas objects
bool dataCompare(Datas d){
  return (d.color == _d.color && d.delay == _d.delay && d.effect == _d.effect && d.maxBrigthness == _d.maxBrigthness);
}

void writePayload(){
  radio.write(&_d, sizeof(_d));
}

void updateScreen(){
  screen.clearDisplay();
  screen.setCursor(0, 0);
  screen.print("Color : ");
  screen.println(codeToColor());
  screen.print("Delay : ");
  screen.print(_d.delay);
  screen.println(" ms");
  screen.print("Effect : ");
  screen.println(codeToEffect());
  screen.print("Brightness : ");
  screen.println(_d.maxBrigthness);
  screen.display();
}

String codeToColor(){
  switch(_d.color){
    case 24 : //Yellow
      return charToString(6, yellow);
    case 25 : //Blue Medium
      return charToString(6, blueMedium);
    case 26 : //Pink
      return charToString(4, pink);
    case 27 : //Grey
    return charToString(4, grey);
    case 28 : //Orange light
      return charToString(8, orangeLight);
    case 29 : //Blue light 2
      return charToString(7, blueLight2);
    case 30 : //Violet star wars
      return charToString(10, violetStarWars);
    case 64 : //Light off
      return charToString(3, lightOff);
    case 68 : //White
      return charToString(5, white);
    case 69 : //Blue
      return charToString(4, blue);
    case 73 : //Blue Medium 2
      return charToString(7, blueMedium2);
    case 77 : //Violet
      return charToString(6, violet);
    case 80 : //Orange
      return charToString(6, orange);
    case 81 : //Blue Light
      return charToString(6, blueLight);
    case 84 : //Orange Dark
      return charToString(8, orangeDark);
    case 85 : //Green Light
      return charToString(7, greenLight);
    case 88 : //Red
      return charToString(3, red);
    case 89 : //Green
      return charToString(5, green);
    default :
      return "";
  }
}

String charToString(int cLenght, char* c){
  String a;
  for(int i = 0; i < cLenght; i++){
    a += c[i];
  }
  return a;
}

String codeToEffect(){
  switch(_d.effect){
    case 5 :
      return charToString(4, jump);
    case 7 :
      return charToString(6, breath);
  }
}