#ifndef MT
#define MT

#include "Arduino.h"

enum rotation_status {CCW, CW, STOP};
typedef  long (*calc_function) (long);

class BaseMotor{
protected:
  uint8_t pin1,pin2,pin3,pin4;
  int time_between_pulses;
  const int lookup[8] = {B01000, B01100, B00100, B00110, B00010, B00011, B00001, B01001};

  calc_function conv_function;

  void out(int);
public:
  BaseMotor(uint8_t, uint8_t, uint8_t, uint8_t);
  virtual void init();

  void set_time_between_pulses(int);
  void set_conversion_function(calc_function);

  void cw_step();
  void ccw_step();
  void stop();


  const int steps_per_rev = 512; // 4096 steps / 8 pulses
  rotation_status status;

};


class BlockingMotor:public BaseMotor{
private:
  void step(int);

public:
  BlockingMotor(uint8_t, uint8_t, uint8_t, uint8_t);
  void rotate_by(long val);
};

typedef void (*on_reach_cback)(long);

class NonBlockingMotor:public BaseMotor{
private:
  long steps;
  long passed_steps;
  bool dest_set;
  on_reach_cback on_dest_reached;

public:
    NonBlockingMotor(uint8_t, uint8_t, uint8_t,uint8_t);
    void init();

    void set_destination(long);
    void update();

    void set_on_destination_reached(on_reach_cback);
};

#endif
