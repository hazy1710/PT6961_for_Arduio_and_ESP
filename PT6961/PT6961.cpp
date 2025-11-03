/*
  PT6961.cpp - Library for communicating with PT6961 LED driver.
  Created by Garrett Blanton January, 16, 2013.
  Released into the public domain.
*/

//0 - A
//1 - B
//2 - C
//3 - D
//4 - E
//5 - F
//6 - G
//7 - Colon

#include "Arduino.h"
#include "PT6961.h"

PT6961::PT6961(int DIN, int CLK, int CS)
{
  pinMode(DIN, OUTPUT);
  pinMode(CLK, OUTPUT);
  pinMode(CS, OUTPUT);
  _DIN = DIN;
  _CLK = CLK;
  _CS = CS;
}

void PT6961::initDisplay()
{
  sendCmd(_DISPLAY_6X12);
  sendCmd(_AUTO_INCREMENT);
  initRAM();
  sendCmd(_DISPLAY_14_16);
}

// Initializes RAM to all zeros
void PT6961::initRAM()
{
  //first clear 8 bytes of the display RAM
  digitalWrite(_CS,LOW);
  shiftOut(_DIN, _CLK, LSBFIRST, 0xC0);
  for(int i=0; i<8; i++){
    shiftOut(_DIN, _CLK, LSBFIRST, 0x00);
  }
  digitalWrite(_CS,HIGH); 
}

// Use to send command based on enumeration
void PT6961::sendCmd(char cmd)
{
  digitalWrite(_CS,LOW);
  shiftOut(_DIN, _CLK, LSBFIRST, cmd);
  digitalWrite(_CS,HIGH);  
}

void PT6961::sendDigit(char digit, char val)
{
  digitalWrite(_CS,LOW);
  shiftOut(_DIN, _CLK, LSBFIRST, digit);
  shiftOut(_DIN, _CLK, LSBFIRST, val);
  digitalWrite(_CS,HIGH);    
}

void PT6961::sendNum(int num, char colon)
{
  int digit1 = num / 1000;
  int digit2 = (num % 1000) / 100;
  int digit3 = (num % 100) / 10;
  int digit4 = (num % 10);
  
  sendDigits(digit1,digit2,digit3,digit4,colon);
}

void PT6961::sendDigits(char digit1, char digit2, char digit3, char digit4, char colon)
{
//-----------------------------0--------1------2-------3-------4-------5-------6------7---------8------9--------A------B--------C------D-------E-------F--------H-------t---
  const uint16_t DISP[18] = {0x064b, 0x0201, 0x0453, 0x0613, 0x0219, 0x061a, 0x065a, 0x0203, 0x065b, 0x061b, 0x025b, 0x065b, 0x044a, 0x064b, 0x045a, 0x005a, 0x0259, 0x0458};
  //двоеточие
  uint8_t DP=0; 
  if(colon == 1)
  {DP=0x04;}
  
  digitalWrite(_CS,LOW);
  
  shiftOut(_DIN, _CLK, LSBFIRST, 0xC0);
  shiftOut(_DIN, _CLK, LSBFIRST, DISP[digit1] | DP);
  shiftOut(_DIN, _CLK, LSBFIRST, (DISP[digit1] >> 8));

  shiftOut(_DIN, _CLK, LSBFIRST, DISP[digit2] | DP);
  shiftOut(_DIN, _CLK, LSBFIRST, (DISP[digit2] >> 8));

  shiftOut(_DIN, _CLK, LSBFIRST, DISP[digit3] | DP);
  shiftOut(_DIN, _CLK, LSBFIRST, (DISP[digit3] >> 8));

  shiftOut(_DIN, _CLK, LSBFIRST, DISP[digit4] | DP);
  shiftOut(_DIN, _CLK, LSBFIRST, (DISP[digit4] >> 8));
  
  digitalWrite(_CS,HIGH);    
}

byte PT6961::readKey()
{

  digitalWrite(_CS,LOW);      
  shiftOut(_DIN, _CLK, LSBFIRST, 0x42);  // чтение данных
  pinMode(_DIN, INPUT);
  delayMicroseconds(1);
  byte dataByte = shiftIn(_DIN, _CLK, LSBFIRST);
  pinMode(_DIN, OUTPUT);
  digitalWrite(_CS,HIGH);

  return   dataByte;
}
