
#include <SPI.h>
#include <RF24.h>
#include <Wire.h>
#include <DIYables_LCD_I2C.h>

#define CE_PIN 5
#define CSN_PIN 4
#define throttle A0
#define yaw A1
#define pitch A2
#define roll A3
#define Arm_Switch 2

RF24 radio(CE_PIN, CSN_PIN);
const uint64_t Address="0xF0F0F0F0E1LL";
bool Ack;

DIYables_LCD_I2C lcd(0x27, 16, 2);  // I2C address 0x27, 16 column and 2 rows

bool Arm_state;
uint16_t desired_rates[4];
uint16_t Dis_rates[4]={1000,1500,1500,1500};

void setup() {
  Serial.begin(115200);
  lcd.init();       // initialize the lcd
  lcd.backlight();
  lcd.clear();          // clear display
  lcd.setCursor(0, 0);  // move cursor to   (0, 0)
  lcd.print("Drone Tx");
  delay(2000);  // display the above for two seconds

  //RF24 check
  lcd.clear();  
  lcd.setCursor(0, 0);  //  Move the cursor to column 0, row 1 (second row)
  lcd.print("Radio Check...");
  delay(1000);

if(!radio.begin()){
  Serial.println("RF not ok");
  lcd.setCursor(0, 1);  //  Move the cursor to column 0, row 1 (second row)
  lcd.print("Radio not ok");
  delay(1000);
  while(1){}
}else {
  Serial.println("RF ok");
  lcd.setCursor(0, 1);  //  Move the cursor to column 0, row 1 (second row)
  lcd.print("Radio ok!");
  delay(1000);
  }
   //RF24 settings
  radio.setPALevel(RF24_PA_LOW);
  radio.setDataRate(RF24_1MBPS);
  radio.setChannel(76);
  radio.openWritingPipe(Address);
  radio.stopListening();

  pinMode(throttle,INPUT);
  pinMode(yaw,INPUT);
  pinMode(pitch,INPUT);
  pinMode(roll,INPUT);
  pinMode(Arm_Switch,INPUT_PULLUP);

  Arm_state = digitalRead(Arm_Switch);
if(Arm_state){
  lcd.clear();          // clear display
  lcd.setCursor(0, 0);  // move cursor to   (0, 0)
  lcd.print("switch to Disarm");
  lcd.setCursor(6, 1);  
  lcd.print("Mode");
  while(1){}
}else{
  lcd.clear();          // clear display
  lcd.setCursor(0, 0);  // move cursor to   (0, 0)
  lcd.print("Disarm Mode ok");
  Ack=radio.write(&Dis_rates,sizeof(Dis_rates));
    if(!Ack){
      lcd.clear();          // clear display
      lcd.setCursor(4, 0);  
      lcd.print("Receiver");
      lcd.setCursor(1, 1);
      lcd.print("not responding");
    }else{
      lcd.clear();          
      lcd.setCursor(2, 0);  
      lcd.print("Receiver Ack");
      delay(1000);
    }

}

  desired_rates[0]=analogRead(throttle);
  desired_rates[1]=analogRead(yaw);
  desired_rates[2]=analogRead(pitch);
  desired_rates[3]=analogRead(roll);
  lcd.clear();          
  lcd.setCursor(0, 0);  lcd.print("Th:");  lcd.print(desired_rates[0]); 
  lcd.setCursor(9, 0);  lcd.print("Ya:");  lcd.print(desired_rates[1]); 
  lcd.setCursor(0, 1);  lcd.print("Pi:");  lcd.print(desired_rates[2]); 
  lcd.setCursor(9, 1);  lcd.print("Ro:");  lcd.print(desired_rates[3]); 
  delay(4000);
  //Calibration

}

void loop() {

  Arm_state = digitalRead(Arm_Switch);
  desired_rates[0]=analogRead(throttle);
  desired_rates[1]=analogRead(yaw);
  desired_rates[2]=analogRead(pitch);
  desired_rates[3]=analogRead(roll);

if(Arm_state){
  for(int i=0; i<4; i++) Ack=radio.write(&desired_rates[i], sizeof(desired_rates));
  if(!Ack){
      lcd.clear();          
      lcd.setCursor(2, 0);  
      lcd.print("Mode: Arming");
      lcd.setCursor(1, 1);
      lcd.print("connection: ok");
    }else{
      lcd.clear();          
      lcd.setCursor(2, 0);  
      lcd.print("Mode: Arming");
      lcd.setCursor(1, 1);
      lcd.print("connection: Nok");
    }
}else{
  Ack=radio.write(&Dis_rates,sizeof(Dis_rates));
    if(!Ack){
      lcd.clear();          
      lcd.setCursor(2, 0);  
      lcd.print("Mode: Arming");
      lcd.setCursor(1, 1);
      lcd.print("connection: ok");
    }else{
      lcd.clear();          
      lcd.setCursor(2, 0);  
      lcd.print("Mode: Arming");
      lcd.setCursor(1, 1);
      lcd.print("connection: Nok");
      }
    }

}
