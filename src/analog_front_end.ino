const int channel_1 = A0;

void setup() {
  //Serial Baud Rate
  SerialUSB.begin(115200);

  //pin I/O declaration
  pinMode(channel_1, INPUT);
  pinMode(13, OUTPUT);

  //Setting the internal ADCs to free running mode
  ADC->ADC_MR |= 0x80;  //free running mode
  ADC->ADC_CR = 2;      //Start converter
  ADC->ADC_CHER = 0x80; // Enable channel 7 (A0)
}

void loop() {
  int t = micros();
  short a0;
  for(int i = 0; i < 1000000; i++){
    while((ADC->ADC_ISR & 0x80) == 0){
      ; //Do nothing while the conversion is not complete
        //Once it is complete the 8th bit will turn to 1
    }
    a0 = (short)ADC->ADC_CDR[7];   //Reading data
    SerialUSB.println(a0);
  }
  t = micros() - t;
}
