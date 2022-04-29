  #include <SPI.h>
  #include <nRF24L01.h>
  #include <RF24.h>
  const uint64_t pipeOut = 0xE9E8F0F0E1LL;   //IMPORTANT: The same as in the receiver 0xE9E8F0F0E1LL | Bu adres alıcı ile aynı olmalı
  RF24 radio(9, 10); // select CE,CSN pin | CE ve CSN pinlerin seçimi
  struct Signal {
  byte throttle;
  
  byte roll;
 
  byte aux1;
  byte aux2;
};
  Signal data;
  void ResetData() 
{
  data.throttle = 4;   // Motor stop | Motor Kapalı (Signal lost position | sinyal kesildiğindeki pozisyon)
  
  data.roll = 441;     // Center | merkez (Signal lost position | sinyal kesildiğindeki pozisyon)
 
  data.aux1 = 0;    // Center | merkez (Signal lost position | sinyal kesildiğindeki pozisyon)
  data.aux2 = 0;    // Center | merkez (Signal lost position | sinyal kesildiğindeki pozisyon)
}
  void setup()
{
  //Start everything up
  radio.begin();
  radio.openWritingPipe(pipeOut);
  radio.setAutoAck(false);
  radio.setDataRate(RF24_250KBPS);
  radio.setPALevel(RF24_PA_HIGH);
  radio.stopListening(); //start the radio comunication for Transmitter | Verici olarak sinyal iletişimi başlatılıyor
  ResetData();
 
}
  // Joystick center and its borders | Joystick merkez ve sınırları
  int mapJoystickValues(int val, int lower, int middle, int upper, bool reverse)
{
  val = constrain(val, lower, upper);
  if ( val < middle )
  val = map(val, lower, middle, 0, 128);
  else
  val = map(val, middle, upper, 128, 255);
  return ( reverse ? 255 - val : val );
}
  void loop()
{
  // Control Stick Calibration | Kumanda Kol Kalibrasyonları
  // Setting may be required for the correct values of the control levers. | :Kontrol kolların doğru değerleri için ayar gerekebilir.
  data.throttle = map(analogRead(A0), 302, 579, 0, 255);  // "true" or "false" for signal direction | "true" veya "false" sinyal yönünü belirler
  data.roll = map(analogRead(A1), 0, 210, 0, 255);     // "true" or "false" for servo direction | "true" veya "false" servo yönünü belirler
  data.aux1 = analogRead(A2);    // "true" or "false" for servo direction | "true" veya "false" servo yönünü belirler
  data.aux2 = analogRead(A3);    // "true" or "false" for servo direction | "true" veya "false" servo yönünü belirler
  radio.write(&data, sizeof(Signal));
  // put your setup code here, to run once:

}
