#include <Arduino.h>

int sensores[4] = {1, 2, 3, 4};
int pinBot = 12;
int pinesDisplay[7] = {5, 6, 7, 8, 9, 10, 11};

int display[6][7] = {
  {0,1,1,0,0,0,0},
  {1,1,0,1,1,0,1},
  {1,1,1,1,0,0,1},
  {0,1,1,0,0,1,1},
  {0,0,0,1,1,1,1},
  {0,0,0,0,0,0,1}
};

bool finished = false, estado = true;

int tActual, tPrevio, tDelay = 10000;
int tActual2, tPrevio2, tDelay2 = 500;

volatile bool flag[4] = {false, false, false, false};
volatile bool boton = false;

void IRAM_ATTR ISR0() { flag[0] = true; }
void IRAM_ATTR ISR1() { flag[1] = true; }
void IRAM_ATTR ISR2() { flag[2] = true; }
void IRAM_ATTR ISR3() { flag[3] = true; }


void IRAM_ATTR ISR_Bot() { boton = true; }

void setup() {
  for (int i = 0; i < 4; i++) {
    pinMode(sensores[i], INPUT_PULLUP);
  }
  attachInterrupt(digitalPinToInterrupt(sensores[0]), ISR0, FALLING);
  attachInterrupt(digitalPinToInterrupt(sensores[1]), ISR1, FALLING);
  attachInterrupt(digitalPinToInterrupt(sensores[2]), ISR2, FALLING);
  attachInterrupt(digitalPinToInterrupt(sensores[3]), ISR3, FALLING);

  for (int i = 0; i < 7; i++) {
    pinMode(pinesDisplay[i], OUTPUT);
  }

  pinMode(pinBot, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(pinBot), ISR_Bot, FALLING);
}

void loop() {
  for(int i = 0; i<4; i++){
    if(flag[i]){
      for(int j = 0; j < 7; j++){
        digitalWrite(pinesDisplay[j], display[i][j]);
      }
      if(flag[3]) {
        finished = true;
        tPrevio = millis();
      }
      flag[i] = false;
    }
  }
  tActual = millis();
  if(finished && tActual-tPrevio >= tDelay){
    if(!boton){
      tActual2 = millis();
      if(tActual2-tPrevio2 >= tDelay2){
        if(estado) estado = false;
        else estado = true;
        tPrevio2 = tActual2;
      }
      if(estado){
        for(int i = 0; i < 7; i++){
          digitalWrite(pinesDisplay[i], display[4][i]);
        }
      }else{
        for(int i = 0; i < 7; i++){
          digitalWrite(pinesDisplay[i], LOW);
        }
      }
    }else{
      for(int i = 0; i < 7; i++){
        digitalWrite(pinesDisplay[i], display[5][i]);
      }
      finished = false;
      boton = false;
    }
  }
}
