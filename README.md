# 28bYJ48
Library for 28bYJ48 motor
 # Hi, this is a copy of this https://github.com/Mohamad1994HD/28BYJ48 just modified to work with buttton stop and reset all the parametrs, 
 if you will need the code without button stop than download the original one https://github.com/Mohamad1994HD/28BYJ48
 
 ![alt text](https://raw.githubusercontent.com/username/projectname/branch/path/to/img.png)
 
 How to use example:
 
 
 ```
#define ARRAYSIZET 3 
#define switchCW A2 // connect the button to ground and arduino pin A2
#include <28BYJ48.h>
#include <RemoteReceiver.h>

NonBlockingMotor motor(4, 5, 6, 7); // Conect the driver to arduino digital pin 4 -> inp 1, 5 -> inp 2, 6 -> inp 3, 7 -> inp 4 ( inp is a stepper driver input)

int degreRotate = 900; 
int degreRotateBack = 330; 
String keyOpenClose[ ARRAYSIZET ] = {"your Code here from 433mhz or rfid", "else here", "else here"  }; 
String ButtonCode = "00";

boolean buttonFlag = false;
String directionRotate ;

long f1(long x){
   return x*motor.steps_per_rev/360;
}
 
unsigned long waitMotor;

void on_reached(long steps){ } // to get number of steps use println

boolean dataBtn ;

void setup() {
  // put your setup code here, to run once:
  // initialise step motor 
  motor.init();
  motor.set_time_between_pulses(800); // recommended
  motor.set_conversion_function(f1);
  motor.set_on_destination_reached(on_reached);
  pinMode(switchCW,INPUT_PULLUP);// CW push button pin as input
  Serial.begin(9600);
  RemoteReceiver::init(0, 3, showCode); // initialise 433MH it should be connected to pin 2 (digital pin 2)
}

void loop() {
   dataBtn = !digitalRead(switchCW); // get the value from btn
  
          if(dataBtn == 1){
             buttonFlag = true;
           }else{
              buttonFlag = false;
             }
  
  
 if(isInArray(ButtonCode, keyOpenClose)){ // check if key is in array 
  if( dataBtn == 1)  { // that's mean the button is presed so the only way is close the dor
         motor.set_destination(degreRotate);
         motor.status = CCW;
         directionRotate = "CV";
     }else{
         motor.set_destination(degreRotate);
         motor.status = CW;
         directionRotate = "CCV";
    }
     waitMotor = millis(); 
     ButtonCode = "00";
  }
  
  if(buttonFlag && directionRotate == "CCV"){
        motor.status = STOP;
     } 
  
      motor.update(); // commit rotation 
    
     if((millis() - waitMotor) > 15000){// this one need if motor get hot or driver get hot
          waitModeMotor();
      }
}


// set the ButtonCode to 433mh ID
void showCode(unsigned long receivedCode, unsigned int period) {
    //Serial.print("Code: ");
    //Serial.println(receivedCode);
    ButtonCode = receivedCode;
 }


// motor get hot and for this we will need to make all the pins to LOW to avoid HOT
void waitModeMotor(){
  digitalWrite(4, LOW);
  digitalWrite(5, LOW);
  digitalWrite(6, LOW);
  digitalWrite(7, LOW);
  }
  
  
  boolean isInArray(String val, String arrayGet[]){
      boolean isIn=false;
  
   for (int i = 0; i < ARRAYSIZET; i++) {
    if(arrayGet[i]==val){
         isIn=true;
         break;
      }
    }
    
    return isIn ;
}

```


  
