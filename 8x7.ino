//   ___     ____
//  ( _ )_ _/_  /
// / _  \ \ // / 
// \___/_\_\/_/  
//               
// 8x7.ino
// 
// A simple, interrupt driven routine that will drive an 8 segment, 8 digit
// display attached to the Teensy, with as little muss and fuss as I can 
// think.

#include <TimerOne.h>

#define NDIGITS         8
#define NDIGITS_MASK    7

const int latchpin = 12 ;       // connect to RCK
const int clockpin = 11 ;       // connect to SCK
const int datapin = 10 ;        // connect to DIO

byte col[NDIGITS] = {
    0b10000000,
    0b01000000,
    0b00100000,
    0b00010000,
    0b00001000,
    0b00000100,
    0b00000010,
    0b00000001
} ;

byte seg[] = {
    0b11000000,  // 0
    0b11111001,  // 1
    0b10100100,  // 2
    0b10110000,  // 3
    0b10011001,  // 4
    0b10010010,  // 5
    0b10000010,  // 6
    0b11111000,  // 7
    0b10000000,  // 8
    0b10010000,  // 9  
} ;     

byte segbuf[NDIGITS] ;
int segcnt = 0;

void
LED_irq(void)
{
    digitalWrite(latchpin, LOW);

    // select the digit...
    shiftOut(datapin, clockpin, MSBFIRST, col[segcnt]) ;
    // and select the segments
    shiftOut(datapin, clockpin, MSBFIRST, segbuf[segcnt]) ;

    digitalWrite(latchpin, HIGH) ;
    segcnt ++ ;
    segcnt &= NDIGITS_MASK ;
}

void
setup()
{
    // setup all the data pins...

    pinMode(latchpin, OUTPUT) ;
    pinMode(clockpin, OUTPUT) ;
    pinMode(datapin, OUTPUT) ;

    // Initialize the segment buffers...

    for (int i=0; i<8; i++)
        segbuf[i] = seg[i] ; 

    Serial.begin(9600) ;
    Serial.println("8x7 TEST") ;

    Timer1.initialize(4000);      // 4 ms
    Timer1.attachInterrupt(LED_irq) ;
    Timer1.start() ;

    // Should auto-run at this point... 
}


void
loop()
{
    Serial.println("Looping...") ;
    delay(1000) ;
}
