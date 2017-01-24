#include <LiquidCrystal.h>

/* HC-SR04 Sensor
 
     
   The circuit:
  * VCC connection of the sensor attached to +5V
  * GND connection of the sensor attached to ground
  * TRIG connection of the sensor attached to digital pin 2
  * ECHO connection of the sensor attached to digital pin 4
   Original code for Ping))) example was created by David A. Mellis
   Adapted for HC-SR04 by Tautvidas Sipavicius
   This example code is in the public domain.
 */


const int trigPin = 2;
const int echoPin = 3;
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

// define some values used by the panel and buttons
int lcd_key     = 0;
int adc_key_in  = 0;

#define btnRIGHT  0
#define btnUP     1
#define btnDOWN   2
#define btnLEFT   3
#define btnSELECT 4
#define btnNONE   5

int mode = 0; // Mode=0 Status, Mode = 1 Capaciade



void setup() {
  // initialize serial communication:
  Serial.begin(9600);
  lcd.begin(16,2);

}

void loop()
{
  // establish variables for duration of the ping, 
  // and the distance result in inches and centimeters:
  long duration, inches, cm;

  // The sensor is triggered by a HIGH pulse of 10 or more microseconds.
  // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
  pinMode(trigPin, OUTPUT);
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Read the signal from the sensor: a HIGH pulse whose
  // duration is the time (in microseconds) from the sending
  // of the ping to the reception of its echo off of an object.
  pinMode(echoPin, INPUT);
  duration = pulseIn(echoPin, HIGH);


  lcd_key = read_LCD_buttons();   // read the buttons

   switch (lcd_key){               // depending on which button was pushed, we perform an action
       case btnSELECT:{             //  push button "RIGHT" and show the word on the screen
            Serial.println("SELECT");
            mode = mode+1;
            if (mode > 1){
              mode = 0;
            }
            
       }
   }

  // convert the time into a distance
  cm = microsecondsToCentimeters(duration);

  if(cm > 100) {
    cm = 5;
  }

  lcd.setCursor(0, 0);
  lcd.clear();
  lcd.print("Distancia->");
  lcd.print(cm);
  lcd.print(" cm");
  lcd.setCursor(0,1);
  
  if(cm >= 70) {
    if (mode == 0) {
      lcd.setCursor(0,1);
      lcd.print("Status:VAZIO ");

    } else {
      lcd.setCursor(0,1);
      lcd.print("Capac.-> 0 lts");
    }
  }
  if(cm <= 50 and cm > 15) {
    if (mode == 0){
      lcd.setCursor(0,1);
      lcd.print("Status:QSE CHEIO ");

    }
    else {
      lcd.setCursor(0,1);
      lcd.print("Capac.-> 700 lts");
    }
  }
  if(cm >= 40 and cm <=50) {
    if (mode==0){
      lcd.setCursor(0,1);
      lcd.print("Status:METADE ");

    }
    else {
      lcd.setCursor(0,1);
      lcd.print("Capac.-> 500 lts");
    }
  }
  if(cm < 15) {
    if (mode == 0){
      lcd.setCursor(0,1);
      lcd.print("Status:CHEIO");

    } else {
      lcd.setCursor(0,1);
      
      lcd.print("Capac.-> 1000 lts");
    }
  }
  if(cm < 10) {
    if (mode == 0){
      lcd.setCursor(0,1);
      lcd.print("Status:NA TAMPA");

    }
    else {
      lcd.setCursor(0,1);
      lcd.print("Capac.-> 1000 lts");
    }
  }
  /*
  if(cm < 5) {
     if(mode ==0){
      lcd.setCursor(0,1);
      lcd.print("Status:VAZANDO ");
     }
     else {
      lcd.setCursor(0,1);
      lcd.print("Capac.-> VANZANDO");
     }
  }
  */
  
  delay(5000);
}

long microsecondsToInches(long microseconds)
{
  // According to Parallax's datasheet for the PING))), there are
  // 73.746 microseconds per inch (i.e. sound travels at 1130 feet per
  // second).  This gives the distance travelled by the ping, outbound
  // and return, so we divide by 2 to get the distance of the obstacle.
  // See: http://www.parallax.com/dl/docs/prod/acc/28015-PING-v1.3.pdf
  return microseconds / 74 / 2;
}

long microsecondsToCentimeters(long microseconds)
{
  // The speed of sound is 340 m/s or 29 microseconds per centimeter.
  // The ping travels out and back, so to find the distance of the
  // object we take half of the distance travelled.
  return microseconds / 29 / 2;
}

int read_LCD_buttons(){               // read the buttons
    adc_key_in = analogRead(A0);       // read the value from the sensor 

 

    // my buttons when read are centered at these valies: 0, 144, 329, 504, 741
    // we add approx 50 to those values and check to see if we are close
    // We make this the 1st option for speed reasons since it will be the most likely result

    if (adc_key_in > 1000) return btnNONE; 

    if (adc_key_in < 850)  return btnSELECT;  


    return btnNONE;    
}
