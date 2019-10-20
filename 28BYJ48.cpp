#include "28BYJ48.h"

BaseMotor::BaseMotor(uint8_t p1, uint8_t p2,
             uint8_t p3, uint8_t p4):pin1(p1),pin2(p2),pin3(p3),pin4(p4){}

void BaseMotor::init(){
  pinMode(pin1, OUTPUT);
  pinMode(pin2, OUTPUT);
  pinMode(pin3, OUTPUT);
  pinMode(pin4, OUTPUT);
  status = STOP;
}

void BaseMotor::set_time_between_pulses(int t){
  if (t < 0){
    t*=-1;
  }
  time_between_pulses = t;
}


void BaseMotor::out(int digit){
  digitalWrite(pin1, bitRead(lookup[digit], 0));
  digitalWrite(pin2, bitRead(lookup[digit], 1));
  digitalWrite(pin3, bitRead(lookup[digit], 2));
  digitalWrite(pin4, bitRead(lookup[digit], 3));
}


void BaseMotor::set_conversion_function(calc_function cf){
  conv_function = cf;
}

void BaseMotor::cw_step(){
  for (int i=7; i>=0; i--){
    out(i);
    delayMicroseconds(time_between_pulses);
  }
}
void BaseMotor::ccw_step(){
  for (int i=0; i<8; i++){
    out(i);
    delayMicroseconds(time_between_pulses);
    }
}

void BaseMotor::stop(){
  status=STOP;
}
/////////////////////////////////////////////////
// BLOCKING MOTOR MODE
////////////////////////////////////////////////
BlockingMotor::BlockingMotor(uint8_t p1, uint8_t p2,
                             uint8_t p3,uint8_t p4):
                             BaseMotor(p1,p2,p3,p4){
                             }


void BlockingMotor::rotate_by(long val){
  if (status == STOP){
    return;
  }

  int steps;
  // if conversion functioin provieded convert accordingly
  // else just assign the number of steps
  conv_function != NULL ? steps=conv_function(val): steps=val;
  // rotate
  step(steps);
}

// //blocking
void BlockingMotor::step(int num_of_steps){
  int steps = 0;
  // check provided direction
  if(status == CW){
    while(true){
      cw_step();
      steps++;
      if (steps >= num_of_steps){
        status = STOP;
        break;
      }
    }
  }else{
    while(true){
      ccw_step();
      steps++;
      if (steps >= num_of_steps){
        status = STOP;
        break;
        }
    }
  }
}


//////////////////////////////////////////////
// NON BLOCKING
/////////////////////////////////////////////
NonBlockingMotor::NonBlockingMotor(uint8_t p1, uint8_t p2,
                             uint8_t p3, uint8_t p4):
                             BaseMotor(p1,p2,p3,p4){}

void NonBlockingMotor::init(){
  BaseMotor::init();
  dest_set = false;
}

void NonBlockingMotor::set_destination(long dest){
  if (!dest_set){
    conv_function != NULL ? steps=conv_function(dest):steps=dest;
    dest_set = true;
  }
}

void NonBlockingMotor::update(){
  if (status==STOP){
    return;
  }

  status == CW? cw_step():ccw_step();
  steps--;
  passed_steps++;

  if(steps <= 0){
    status=STOP;
    dest_set = false;
    on_dest_reached(passed_steps);
  }

}

void NonBlockingMotor::set_on_destination_reached(on_reach_cback cb){
  on_dest_reached = cb;
}
