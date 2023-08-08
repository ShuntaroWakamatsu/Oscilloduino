void setup() {
  SerialUSB.begin(115200); // baud rate ignored for usb virtual serial
  pinMode(2, OUTPUT); // measure pin 2 with frequency counter
}

void loop() {
  digitalWrite(2, HIGH);
  SerialUSB.println("Output is HIGH");
  digitalWrite(2, LOW);
  SerialUSB.println("Output is Low");
}