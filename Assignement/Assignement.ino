#include <LM35.h> //LM

#define LED LED_BUILTIN
#define TEMP_SENSOR_PIN A0

LM35 tempProbe(TEMP_SENSOR_PIN);

void setBlinkFast(){
  OCR1A = 15625; // compare match register 62500 Hz / 4 =  15625
}

void setBlinkSlow(){
  OCR1A = 31250; // compare match register 62500 Hz / 2 = 31250
}

void initTimer(){
  cli();                    // disable all interrupts
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1  = 0;
  setBlinkSlow();
  TCCR1B |= (1 << WGM12);   // CTC mode
  TCCR1B |= (1 << CS12);    // Frequency 16Mhz/ 256 prescaler = 62500 Hz
  TIMSK1 = (1 << OCIE1A);   // Local interruption OCIE1A
  sei();                    // enable all interrupts
}

void onTimer(){
  static bool state = HIGH;
  digitalWrite(LED, state);
  state = !state;
}

void setup(){
  pinMode(LED, OUTPUT);
  initTimer();
}

ISR(TIMER1_COMPA_vect){ //ISR of timer compare
  onTimer();
}

void loop(){
  if(tempProbe.cel() > 30){
    setBlinkSlow();
  }
  else{
    setBlinkFast();
  }
}