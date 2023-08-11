#include <ADC.h>
#include <ADC_util.h>
#include <stdint.h>
//Thank you Pedro Villanueva !!!

#define channel0 A0
#define channel1 A1

bool avg_done;
int avg;
bool res_done;
int res;

volatile uint16_t value = 0; 
volatile uint32_t num_samples_taken = 0;
//Volatile: values might be changed outside of this program during runtime. 
//Prevents compiler's optimization

ADC *adc = new ADC();

void setup() {
  Serial.begin(2000000);

  avg_done = false;
  
  pinMode(channel0, INPUT);
  pinMode(channel1, INPUT);

  adc->adc0->enableInterrupts(isr0, 0);
  // Parameter 1: Interrupt Service Routine Function. Has to have no arguments and void return type
  // Parameter 2: Interrupt Priority (0 = highest, 255 lowest) 

  while(!avg_done){
    Serial.print("Take an average per how many samples? (1, 4, 8, 16, 32):");
    while(Serial.available() == 0){
      ;
    }
    avg = Serial.parseInt();
    if(avg == 1 || avg == 4 || avg == 8 || avg == 16 || avg == 32){
      adc->adc0->setAveraging((uint8_t) avg);
      avg_done = true;
    }
    else{
      Serial.print("Invalid Input");
    }
  }
  
}

void loop() {
  // put your main code here, to run repeatedly:

}

void isr0(void) {

}

