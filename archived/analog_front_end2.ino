#include <ADC.h>
#include <ADC_Module.h>
#include <ADC_util.h>
#include <stdint.h>

#define channel0 A0
#define channel1 A1

//All the register adreesses. Type casting to a pointer to a volatile unsigner 32-bit int type
volatile uint32_t* HC0_REG_ADDR = reinterpret_cast<volatile uint32_t*>(0x400C4000);
volatile uint32_t* HC1_REG_ADDR = reinterpret_cast<volatile uint32_t*>(0x400C4004);
volatile uint32_t* HC2_REG_ADDR = reinterpret_cast<volatile uint32_t*>(0x400C4008);
volatile uint32_t* HC3_REG_ADDR = reinterpret_cast<volatile uint32_t*>(0x400C400C);
volatile uint32_t* HC4_REG_ADDR = reinterpret_cast<volatile uint32_t*>(0x400C4010);
volatile uint32_t* HC5_REG_ADDR = reinterpret_cast<volatile uint32_t*>(0x400C4014);
volatile uint32_t* HC6_REG_ADDR = reinterpret_cast<volatile uint32_t*>(0x400C4018);
volatile uint32_t* HC7_REG_ADDR = reinterpret_cast<volatile uint32_t*>(0x400C401C);
volatile uint32_t* HS_REG_ADDR = reinterpret_cast<volatile uint32_t*>(0x400C4020);
volatile uint32_t* R0_REG_ADDR = reinterpret_cast<volatile uint32_t*>(0x400C4024);
volatile uint32_t* R1_REG_ADDR = reinterpret_cast<volatile uint32_t*>(0x400C4028);
volatile uint32_t* R2_REG_ADDR = reinterpret_cast<volatile uint32_t*>(0x400C402C);
volatile uint32_t* R3_REG_ADDR = reinterpret_cast<volatile uint32_t*>(0x400C4030);
volatile uint32_t* R4_REG_ADDR = reinterpret_cast<volatile uint32_t*>(0x400C4034);
volatile uint32_t* R5_REG_ADDR = reinterpret_cast<volatile uint32_t*>(0x400C4038);
volatile uint32_t* R6_REG_ADDR = reinterpret_cast<volatile uint32_t*>(0x400C403C);
volatile uint32_t* R7_REG_ADDR = reinterpret_cast<volatile uint32_t*>(0x400C4040);
volatile uint32_t* CFG_REG_ADDR = reinterpret_cast<volatile uint32_t*>(0x400C4044); //Configuration
volatile uint32_t* GC_REG_ADDR = reinterpret_cast<volatile uint32_t*>(0x400C4048);  //General Control
volatile uint32_t* GS_REG_ADDR = reinterpret_cast<volatile uint32_t*>(0x400C404C);
volatile uint32_t* CV_REG_ADDR = reinterpret_cast<volatile uint32_t*>(0x400C4050);
volatile uint32_t* OFS_REG_ADDR = reinterpret_cast<volatile uint32_t*>(0x400C4054);
volatile uint32_t* CAL_REG_ADDR = reinterpret_cast<volatile uint32_t*>(0x400C4058);

enum ADC_speed {
  Low,
  Med,
  High,
  ASYNCLK_10,
  ASYNCLK_20
};

ADC_speed speed = ASYNCLK_20;

const uint8_t average = 1;
const uint8_t resolution = 12;
//const uint8_t convSpeed =


volatile uint16_t value = 0;
volatile uint32_t num_samples_taken = 0;
//Volatile: values might be changed outside of this program during runtime.
//Prevents compiler's optimization



ADC *adc = new ADC();

void setup() {
  Serial.begin(2000000);

  pinMode(channel0, INPUT);
  pinMode(channel1, INPUT);

  adc->adc0->enableInterrupts(isr0, 0);
  // Parameter 1: Interrupt Service Routine Function. Has to have no arguments and void return type
  // Parameter 2: Interrupt Priority (0 = highest, 255 lowest)

  setAverage();
  setResolution();


}

void setAverage() {
  //Setting Average
  //@0x400C4044, bit 15 & 14 determins the averaging mode
  //00: average of 4 samples
  //01: average of 8 samples
  //10: average of 16 samples
  //11: average of 32 samples
  if(average <= 1){
    __disable_irq();                //Modifying a register should never be interuppted. So, turn off interrupt temporarily
    *(GC_REG_ADDR) &= ~ADC_GC_AVGE; //Clearing the Average Enable flag (ADC_GC_AVGE = 0x00000020)
    __enable_irq();                 //Turn it back on
  }
  else{
    __disable_irq();                //Modifying a register should never be interuppted. So, turn off interrupt temporarily
    *(GC_REG_ADDR) |= ADC_GC_AVGE;  //Setting the Avergae Enable flag (ADC_GC_AVGE = 0x00000020)
    __enable_irq();                 //Turn it back on

    if(average <= 4){
      __disable_irq();                //Modifying a register should never be interuppted. So, turn off interrupt temporarily
      *(CFG_REG_ADDR) &= ~ADC_CFG_AVGS(3);  //Setting the 15th & 14th bits to be 0s (ADC_CFG_AVGS(3) = 0x0000C000)
      __enable_irq();                 //Turn it back on
    }
    else if(average <= 8){
      __disable_irq();                //Modifying a register should never be interuppted. So, turn off interrupt temporarily
      *(CFG_REG_ADDR) &= ~(0x01 << 15);  //Setting the 15th bit to be 0
      *(CFG_REG_ADDR) |= (0x01 << 14);   //Setting the 14th bit to be 1
      __enable_irq();                 //Turn it back on
    }
    else if(average <= 16){
      __disable_irq();                //Modifying a register should never be interuppted. So, turn off interrupt temporarily
      *(CFG_REG_ADDR) |= (0x01 << 15);   //Setting the 15th bit to be 1
      *(CFG_REG_ADDR) &= ~(0x01 << 14);  //Setting the 14th bit to be 0
      __enable_irq();                 //Turn it back on
    }
    else{
      __disable_irq();                //Modifying a register should never be interuppted. So, turn off interrupt temporarily
      *(CFG_REG_ADDR) |= ADC_CFG_AVGS(3);  //Setting the 15th & 14th bits to be 1s (ADC_CFG_AVGS(3) = 0x0000C000)
      __enable_irq();                 //Turn it back on
    }
  }
}

void setResolution() {
  //Setting Resolution
  //@0x400C4044, bit 3 & 2 determins the number of bits to be used in the conversion
  //00: 8-bit conversion
  //01: 10-bit conversion
  //10: 12-bit conversion
  if(resolution <= 8){
    __disable_irq();                //Modifying a register should never be interuppted. So, turn off interrupt temporarily
    *(CFG_REG_ADDR) &= ~ADC_CFG_MODE(3); //Setting the MODE flag to be 00 (ADC_CFG_MODE = 0x0000000C)
    __enable_irq();                 //Turn it back on
  }
  else if((8 < resolution) && (resolution <= 10)){
    __disable_irq();                //Modifying a register should never be interuppted. So, turn off interrupt temporarily
    *(CFG_REG_ADDR) &= ~(0x01 << 3);  //Setting the 3rd bit to be 0
    *(CFG_REG_ADDR) |= (0x01 << 2);   //Setting the 2nd bit to be 1
    __enable_irq();                 //Turn it back on
  }
  else{
    __disable_irq();                //Modifying a register should never be interuppted. So, turn off interrupt temporarily
    *(CFG_REG_ADDR) |= (0x01 << 3);   //Setting the 3rd bit to be 1
    *(CFG_REG_ADDR) &= ~(0x01 << 2);  //Setting the 2nd bit to be 0
    __enable_irq();                 //Turn it back on
  }
}

void setConvSpeed(){
  //Setting Conversion Speed
  //@0x400C4044, bit 10 determins whether high speed configuration is on or not
  //0: Normal Conversion
  //1: High Speed Conversion
  //bit 7 controls the low-power coonfiguration. reduces the comparator enable period
  //0: Normal mode
  //1: Low power mode
  bool asynch = false;
  uint32_t ADC_speed_cfg;

  switch(speed){
    case ADC_speed::Low:
      __disable_irq();                //Modifying a register should never be interuppted. So, turn off interrupt temporarily
      *(CFG_REG_ADDR) &= ~ADC_CFG_ADHSC;  //Clearing the 10th bit
      *(CFG_REG_ADDR) |= ADC_CFG_ADLPC;   //Setting the 7th bit to be 1
      *(CFG_REG_ADDR) |= ADC_CFG_ADLPC;
      __enable_irq();                 //Turn it back on
      ADC_speed_cfg = get_CFG_LOW_SPEED(F_BUS_ACTUAL); //F_BUS_ACTUAL = the raw timer counts
      break;
    case ADC_speed::Med:
      __disable_irq();                //Modifying a register should never be interuppted. So, turn off interrupt temporarily
      *(CFG_REG_ADDR) &= ~ADC_CFG_ADHSC;    //Clearing the 10th bit
      *(CFG_REG_ADDR) &= ~ADC_CFG_ADLPC;    //Clearing the 7th bit
      __enable_irq();                 //Turn it back on
      ADC_speed_cfg = get_CFG_MEDIUM_SPEED(F_BUS_ACTUAL);
      break;
    case ADC_speed::High:
      __disable_irq();                //Modifying a register should never be interuppted. So, turn off interrupt temporarily
      *(CFG_REG_ADDR) |= ADC_CFG_ADHSC;   //Setting the 10th bit to be 1
      *(CFG_REG_ADDR) &= ~ADC_CFG_ADLPC;  //Clearing the 7th bit
      __enable_irq();                 //Turn it back on
      ADC_speed_cfg = get_CFG_HIGH_SPEED(F_BUS_ACTUAL);
      break;
    case ADC_speed::ASYNCLK_10:
      __disable_irq();                //Modifying a register should never be interuppted. So, turn off interrupt temporarily
      *(CFG_REG_ADDR) &= ~ADC_CFG_ADHSC;  //Clearing the 10th bit
      __enable_irq();                 //Turn it back on
      asynch = true;
      break;
    case ADC_speed::ASYNCLK_20:
      __disable_irq();                //Modifying a register should never be interuppted. So, turn off interrupt temporarily
      *(CFG_REG_ADDR) |= ADC_CFG_ADHSC;  //Setting the 10th bit
      __enable_irq();                 //Turn it back on
      asynch = true;
      break;
  }

  //@0x400C4048,
  //bit 0 determins whether the asynchronous clock is enabled
  //@0x400C4044,
  //bit 0 and 1 determins which clock is used
  //bit 6 and 5 determins the clock division ratio
  if(asynch){
    __disable_irq();  //Modifying a register should never be interuppted. So, turn off interrupt temporarily
    *(GC_REG_ADDR) |= ADC_GC_ADACKEN;  //Setting the 10th bit
    *(CFG_REG_ADDR) |= ADC_CFG_ADICLK(3); //Selecting ADACK (Asynchronous clock)
    *(CFG_REG_ADDR) &= ~ADC_CFG_ADIV(3);  //Clearing CLock Divide Select (No clock division)
    __enable_irq();   //Turn it back on
  }
  else{
    uint32_t InClkBits = ADC_speed_cfg & ADC_CFG_ADICLK(3);
    uint32_t DivBits = ADC_speed_cfg & ADC_CFG_ADIV(3);
    __disable_irq();  //Modifying a register should never be interuppted. So, turn off interrupt temporarily
    *(GC_REG_ADDR) &= ~ADC_GC_ADACKEN;  //Clearing the 10th bit. Asynchronous clock is disabled
    if(InClkBits & 0x01 == 1){
      *(CFG_REG_ADDR) |= 0x01;
    }
    else{
      *(CFG_REG_ADDR) &= ~0x01;
    }
    if(InClkBits & 0x02 == 1){
      *(CFG_REG_ADDR) |= 0x02;
    }
    else{
      *(CFG_REG_ADDR) &= ~0x02;
    }

    if(DivBits & (0x01 << 5) == 1){
      *(CFG_REG_ADDR) |= (0x01 << 5);
    }
    else{
      *(CFG_REG_ADDR) &= ~(0x01 << 5);
    }
    if(DivBits & (0x02 << 5) == 1){
      *(CFG_REG_ADDR) |= (0x02 << 5);
    }
    else{
      *(CFG_REG_ADDR) &= ~(0x02 << 5);
    }
    __enable_irq();   //Turn it back on
  }
}

void setSmplSpeed(){

}

void calibrate() {
  __disable_irq();
  *(GC_REG_ADDR) &= ~ADC_GC_CAL; //Making sure there is no conversion going on
  *(GS_REG_ADDR) |= ADC_GS_CALF; //Clearing calibration failure flag by writing 1 to GS REG
  *(GC_REG_ADDR) |= ADC_GC_CAL;  //Starting calibration
  __enable_irq();
}

void waitForCalib() {
  while((*(GC_REG_ADDR) & ADC_GC_CAL) == 1){
    yield();  //Yields control back to the Arduino scheduler so that it can deal with other tasks
  }
  if(*(GS_REG_ADDR) & ADC_GS_CALF == 1){
    Serial.println("Error: Calibration Failed.");
    return;
  }
}

void initialization(){

  *(CFG_REG_ADDR) |= ADC_CFG_OVWREN;//Enabling overwriting of old data
  *(CFG_REG_ADDR) &= ~ADC_CFG_REFSEL(3);  //Selecting VREFH?VREFL as reference voltage
  setResolution(10);
  setAverage(32);
  setConvSpeed(ADC_speed::Low);
  setSamplingSpeed(); //Havent written yet

  calibrate();
}

void loop() {
  // put your main code here, to run repeatedly:

}

void isr0(void) {

}

