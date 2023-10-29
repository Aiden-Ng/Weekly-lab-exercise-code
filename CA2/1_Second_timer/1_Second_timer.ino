//https://www.youtube.com/watch?v=IdL0_ZJ7V2s&ab_channel=GreatScott%21
//https://ww1.microchip.com/downloads/en/DeviceDoc/Atmel-7810-Automotive-Microcontrollers-ATmega328P_Datasheet.pdf
void setup() {
  Serial.begin(9600);
  TCCR1A = 0; //cuz its a 16 bit counter A = 8 bit 
  TCCR1B = 0; //cuz its a 16 bit counter B = 8 bit
  TCNT1 = 3036; //To reduce the offset 
  TCCR1B |= (1<<CS12);  //set prescalar to 256 
  TIMSK1 |= (1<<TOIE1); //enable overflow 
}

ISR(TIMER1_OVF_vect) {
  Serial.println(millis());
  TCNT1 = 3036; //To reduce the offset 
}

void loop() {
  

}
