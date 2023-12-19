#include <IRremote.h>

int IRpin = 11;
IRrecv irrecv(IRpin);
decode_results results;

void setup()
{
Serial.begin(9600);
irrecv.enableIRIn(); // Start the receiver
}

void loop()
{
if (irrecv.decode(&results))
{
unsigned int rvalue;
rvalue =results.value;
Serial.println(rvalue); // Print the Serial 'results.value'
irrecv.resume(); // Receive the next value
}

}

// testing and capturing signals from an infrared (IR) remote control using an Arduino. 