#ifndef PHYSICS_H
#define PHISICS_H

#include <cmath>

using namespace std;

int updatepos(volatile float &x, volatile float &y, volatile int &dir, volatile float &speed);
int gravity(volatile float &x, volatile float &y, volatile int &dir, volatile float &speed, float gravity);

int updatepos(volatile float &x, volatile float &y, volatile int &dir, volatile float &speed)
{

 double o;
 // If ball is headed at a downward negative slope (x decreasing and y increasing) 
 if (dir > 180 && dir < 270){
    o = 270 - dir;
    o = o *  3.14 / 180;
    x = x - sin(o) * speed;
    y = y + cos(o) * speed;
 }
 else
 // If ball is headed at a downward positive slope (x increasing and y increasing) 
 if (dir > 270 && dir < 360){
    o = 360 - dir;
    o = o *  3.14 / 180;
    x = x + cos(o) * speed;
    y = y + sin(o) * speed;
 }
 else
 // If ball is headed at a upward positive slope (x increasing and y decreasing)
 if (dir > 0 && dir < 90){
    o = dir;
    o = o *  3.14 / 180;
    x = x + cos(o) * speed;
    y = y - sin(o) * speed;
 }
 else
 // If ball is headed at a upward positive slope (x increasing and y increasing)
 if (dir > 90 && dir < 180){
    o = 180 - dir;
    o = o *  3.14 / 180;
    x = x - cos(o) * speed;
    y = y - sin(o) * speed;
 }
 else
 //If ball moving at an 0 or 360 degree angle
 if (dir == 0 || dir == 360){
    x = x + speed;
 }
 else
 //If ball moving at a 90 degree angle
 if (dir == 90){
    y = y - speed;
 }
 else
 //If ball moving at a 180 degree angle
 if (dir == 180){
    x = x - speed;
 }
 else
 //If ball moving at a 270 degree angle
 if (dir == 270){
    y = y + speed;
 }
 return 0;
}

int gravity(volatile float &x, volatile float &y, volatile int &dir, volatile float &speed, float gravity)
{
 /*
  if(dir > 0 && dir < 180) speed = speed*(1-gravity/50);
  if(dir > 180 && dir < 360) speed = speed*(1+gravity/50);
  if(speed < .5) dir = 360 - dir;
  
  if(dir > 90 && dir < 270) dir++;
  if(dir > 0 && dir < 90 || dir > 270 && dir <= 360) dir--;
  if(dir == 0) dir = 360;
  */
}
#endif
