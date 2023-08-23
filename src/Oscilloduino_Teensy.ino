#include <ADC.h>
#include <ADC_util.h>
//Library by

const int channel0 = A0;
const int channel1 = A1;

const uint8_t avg = 1;
const uint8_t resl = 12;
const ADC_CONVERSION_SPEED convSpeed = ADC_CONVERSION_SPEED::ADACK_20;
const ADC_SAMPLING_SPEED smplSpeed = ADC_SAMPLING_SPEED::VERY_HIGH_SPEED;
const uint32_t NUM_SAMPLES = (uint32_t)1000000;

uint32_t num_samples = NUM_SAMPLES / avg;
elapsedMicros timeElapsed0;
elapsedMicros timeElapsed1;
volatile uint32_t num_iter0 = 0;
volatile uint32_t num_iter1 = 0;
volatile uint32_t value0 = 0;
volatile uint32_t value1 = 0;

ADC *adc = new ADC();

void setup() {
  pinMode(channel0, INPUT_PULLUP);
  pinMode(channel1, INPUT_PULLUP);

  Serial.begin(9600);
  delay(100);

  adc->adc0->enableInterrupts(adc0_isr);
  adc->adc1->enableInterrupts(adc1_isr);

  adc->adc0->setAveraging(avg);
  adc->adc1->setAveraging(avg);

  adc->adc0->setResolution(resl);
  adc->adc1->setResolution(resl);

  adc->adc0->setConversionSpeed(convSpeed);
  adc->adc1->setConversionSpeed(convSpeed);

  adc->adc0->setSamplingSpeed(smplSpeed);
  adc->adc1->setSamplingSpeed(smplSpeed);

  adc->adc0->wait_for_cal();
  adc->adc1->wait_for_cal();

  value0 = 0;
  num_iter0 = 0;
  value1 = 0;
  num_iter1 = 0;
  timeElapsed0 = 0;
  timeElapsed1 = 0;

  adc->adc0->startContinuous(channel0);
  adc->adc1->startContinuous(channel1);
}

void loop() {
  if(adc->adc0->fail_flag != ADC_ERROR::CLEAR) {
    Serial.print("ADC0: ");
    Serial.println(getStringADCError(adc->adc0->fail_flag));
  }
  if(adc->adc1->fail_flag != ADC_ERROR::CLEAR) {
    Serial.print("ADC1: ");
    Serial.println(getStringADCError(adc->adc1->fail_flag));
  }

  delay(100);
}

void adc0_isr() {
  if(num_iter0 < num_samples){
    value0 = (uint16_t)adc->adc0->analogReadContinuous();
    num_iter0++;
  }
  else{
    Serial.print("\n\n");
    Serial.println("Channel0 Done");
    Serial.print("Channel0 Elapsed Time: ");
    Serial.print(timeElapsed0);
    Serial.println(" microsec");

    timeElapsed0 = 0;
    value0 = (uint16_t)adc->adc0->analogReadContinuous();
    num_iter0 = 0;
  }
}

void adc1_isr() {
  if(num_iter1 < num_samples){
    value1 = (uint16_t)adc->adc1->analogReadContinuous();
    num_iter1++;
  }
  else{
    Serial.print("\n\n");
    Serial.println("Channel1 Done");
    Serial.print("Channel1 Elapsed Time: ");
    Serial.print(timeElapsed1);
    Serial.println(" microsec");

    timeElapsed1 = 0;
    value1 = (uint16_t)adc->adc1->analogReadContinuous();
    num_iter1 = 0;
  }
}
