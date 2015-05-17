#include <Wire.h>

unsigned char frequencyH = 0;
unsigned char frequencyL = 0;

unsigned int frequencyB;
double frequency = 0;
double channels[6]={91.1,93.5,94.3,98.3,101.0,107.4};
unsigned int i=0;
void setup()
{
  Wire.begin();
  Serial.begin(9600);
  pinMode(PUSH2, INPUT_PULLUP);
}

void loop()
{
  if(!(digitalRead(PUSH2))) //If button Pressed
{
  while(!(digitalRead(PUSH2))) //Release the Kraken!!
  {
    delay(100);
  }
  if(digitalRead(PUSH2)) // Now Released .. Whoosh
  {
    if(i==6)
    {
      i=0;
    }
  unsigned char buffer[5];  
  frequency = channels[i]; //starting frequency
  setFrequency();
  Serial.print("Current Channel: ");
  Serial.println(channels[i]);
  Wire.requestFrom(0x60,5); //reading TEA5767
  if (Wire.available()) 
  {
    for (int i=0; i<5; i++) 
    {

      buffer[i]= Wire.read();
    }
  Serial.print("Signal Level: ");  
  Serial.print((buffer[3]>>4));
  Serial.println("/16 ");
  Serial.print("Transmission: ");
  if (buffer[2]&0x80) Serial.println("STEREO");
  else Serial.println("MONO");  
  }
  i++;


  }

delay(2000); // Keep Going but slowly!
}
}

void setFrequency()
{
  frequencyB = 4 * (frequency * 1000000 + 225000) / 32768;
  frequencyH = frequencyB >> 8;
  frequencyL = frequencyB & 0XFF;
  delay(100);
  Wire.beginTransmission(0x60);
  Wire.write(frequencyH);
  Wire.write(frequencyL);
  Wire.write(0xB0);
  Wire.write(0x10);
  Wire.write((byte)0x00);
  Wire.endTransmission();
  delay(100);  
}


