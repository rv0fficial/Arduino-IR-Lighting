#include <avr/io.h>
#include <IRremote.h> //Library

//variables
int x= 16;
int y= 1;


//initial duty cycle  
int CTRL_brightness = 90;
int main()
{
  DDRD=0xF0;//initialize the equipment
  DDRB=0x27;//initialize the equipment
  
  OCR1A =19999; //control frequency of PWM signal(10ms period)
  OCR1B =17999; //value to control duty cycle
    
  EICRA = 0x02; //Make INT0 falling edge triggered
  EIMSK = 0x01; //1<<INT0; enable external interrupt 0
  
  TCCR1A =0x00 ; //WGM01-01 --> 0 timer/counter operation mode make as CTC 
  TCCR1B =0x0A ;//WGM02 --> 1 to ctc mode, CS00-02--> clk/8(make the prescaling value to 8)
      
  sei (); // enable global interrupts 

  
    
    IRrecv irrecv(11);
    decode_results rem_results;
    // that the Arduino can send out commands through the USB connection.
    Serial.begin(9600);
    irrecv.enableIRIn(); 
 
  while(1) {
  
    if (irrecv.decode(&rem_results)) 
    {
      unsigned int rvalue = rem_results.value; //assign remote results to int rvalue varidable.

   //switch satatement for control the leds sequence
        switch (rvalue) 
        {
              case 36975: //on button num
              
        if (x<=128)
        {
        PORTD ^=x;
          
          x=x*2;
          break;
        }
        
        else if(y<=4)
        {
          PORTB ^=y;
           
            y=y*2;
            break; 
        }
        
        else if(x==128 && y==4)
        {
            PORTD=0xF0;
            PORTB=0x07; 
         }
         
               case 43095://off button num 
        if(y>1)
        {
        y=y/2;
        PORTB ^= y;
          break;
        }
        
        else if(y==1 && x>=16)
        {
        x=x/2;
        PORTD ^= x; 
          break;
        }
        
        else if(y==0 && x==16)
        {
          x=16;
          y=1;
          PORTB=0X00;
          PORTD=0X00;
          break;
        }

        
        case 12495://CTRL_brightness button num
        TIMSK1 = (1 << OCIE1B)|(1 << OCIE1A); //enable Timer1 compare match A int and compare match B.
    if((PINB&0x10)== 0x10)
    {
        PORTB=0x00;
         break;    
    }
    else
    {
      PORTB=0x20;
       break;
    }
    
    }
       
    irrecv.resume(); //receive or transmit Infrared Remote Control codes
  }
}
}
ISR (TIMER1_COMPA_vect) {//ISR for Timer1 compare match A
    PORTD=0xF0;//set 
    PORTB=0x07;
}

ISR (TIMER1_COMPB_vect) { //ISR for Timer1 compare match B
  PORTD=0x00;//clear
  PORTB=0x00;
}
ISR(INT0_vect) //ISR for external interrupt 0 
{
 //contrlling the brightness reducing the dutycycles.
  int j=(((CTRL_brightness/10)*1000)/(0.5))-1;
  
  OCR1B=j;
  
  if(CTRL_brightness > 0)
  { 
    CTRL_brightness = CTRL_brightness - 20;
  }
  else if(CTRL_brightness<0)
  {
    CTRL_brightness=90;
    OCR1B=35999;
    PORTD=0x00;
    PORTB=0x00;
    TIMSK1 = (0 << OCIE1B)|(0 << OCIE1A);   
  }
}