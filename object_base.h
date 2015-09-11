#ifndef OBJECT_BASE_H
#define OBJECT_BASE_H

#include <allegro.h>
#include <cmath>
#include <new>

#define NOCOL 0
#define BOTTOM 1
#define RIGHT 2
#define LEFT 3
#define TOP 4
#define CORNERBOTRIGHT 5
#define CORNERBOTLEFT 6
#define CORNERTOPRIGHT 7
#define CORNERTOPLEFT 8
#define INSIDE 20
#define COLERROR -1

#define YELLOW makecol(255,255,0)
#define BLUE makecol(0,0,255)
#define RED makecol(255,0,0)

using namespace std;

class object_base {
   public:    
      //Location of the object (usually the top left)
      float x, y;
      // The width and hight of the object
      int width,height;
      //clipping for the object (mesured in pixels from relevent side)
      int clipleft, clipright, cliptop, clipbottom;
      
      //basically takes in to accout cliping and width/height and determines the bounds of an object.  Make sure to run setbounds() for the object to set.
      float left, right, top, bottom;
      int real_height, real_width;
      
      bool allowcorners;

      object_base(int widthobj = -1, int heightobj = -1, float xobj = -1,  float yobj = -1, bool objallowcorners = false, int clipleftobj = 0, int cliprightobj = 0, int cliptopobj = 0, int clipbottomobj = 0);
    
      int collides_with(object_base &obj);
    
      void draw(BITMAP *bitmap, BITMAP *Dest);
      void postcolreposition(object_base &obj, int collision, bool inverse = false);
      
      void draw_bounds(BITMAP *bitmap, bool full_lines = false, int r1 = 0 , int g1 = 255, int b1 = 0, int r2 = 0, int g2 = 0, int b2 = 255);
      void setbounds();

};

class object_base_move : public object_base {
   public:
      int dir;
      float speed;
      bool has_moved;
      
      object_base *Zones;
      
      object_base_move(int dirobj = 0, float speedobj = 0);
      
      void move();
      void change_dir_after_col(int collision);
      void fixangle();
      void draw_move_bounds(BITMAP *bitmap, bool label_zones = false, int r1 = 0 , int g1 = 255, int b1 = 0, int r2 = 0, int g2 = 0, int b2 = 255);
      void determine_zones();
      
      int will_collide_with(object_base_move &obj);
      int will_collide_with(object_base &obj);
};    

//constructor that requires that all variables for object be explicitly stated (or just call with no parms and get a default object)
object_base::object_base(int widthobj, int heightobj, float xobj,  float yobj, bool objallowcorners, int clipleftobj, int cliprightobj, int cliptopobj, int clipbottomobj)
{
   x = xobj;
   y = yobj;
   
   width = widthobj;
   height = heightobj;
    
   clipleft = clipleftobj;
   clipright = cliprightobj;
   cliptop = cliptopobj;
   clipbottom = clipbottomobj;
    
   allowcorners = objallowcorners;
   
   setbounds();
}

//Calls the collision detection function
int object_base::collides_with(object_base &obj)
{
   float righta, lefta, topa, bottoma;    //the bounderies of the first bitmap
   float rightb, leftb, topb, bottomb;    //The next two lines are helper variables that define the x,y coords of the second bitmap'
   bool collisions[4];                    //This array stores if a collision has occured
   int collision;                         //This stores if a collision has occured (used for single type collisions)

   for (int A = 0; A <= 4;  A++) collisions[A] = false;  //Initializes the collision array to false
   
   //set values to all the helper variables
   //These define the bounderies of the objects being tested.
   rightb = right;
   leftb = left;
   topb = top;
   bottomb = bottom;

   righta = obj.right;
   lefta = obj.left;
   topa = obj.top; 
   bottoma = obj.bottom;
       
   //Detects if there is no collision at all.
   if(leftb   > righta  || 
      rightb  < lefta   || 
      topb    > bottoma || 
      bottomb < topa) return NOCOL;
      
      
   //Detects if the second object is fully within the first object
   if (leftb  > lefta    && 
      rightb  < righta   && 
      topb    > topa     && 
      bottomb < bottoma) return INSIDE;
      
   //Detects if the first object is fully within the second object   
   if (leftb  <= lefta   && 
      rightb  >= righta  && 
      topb    <= topa    && 
      bottomb >= bottoma) return INSIDE;      
      
   
   //The main collision tests (kinda funny that these are the main tests that everything else depends and they take up only 4 very short lines)
   if (leftb <= righta && rightb >= righta)    {collisions[RIGHT]  = true;  collision = RIGHT;}
   if (rightb >= lefta && leftb <= lefta)      {collisions[LEFT]   = true;  collision = LEFT;}
   if (topb <= bottoma && bottomb >= bottoma)  {collisions[BOTTOM] = true;  collision = BOTTOM;}
   if (bottomb >= topa && topb <= topa)        {collisions[TOP]    = true;  collision = TOP;}
      
   //The following tests check for situations in which the second bitmap is colliding with one of the corners of the first.
   //This results in two of the above collision tests to be evaluated as true
   //To solve this these tests first looks to see if corner collisions are turned on if they are it returns the correct corrner as the collision type.
   //Otherwise it finds how many pixels deep the second bitmap is into the first for both the width and the height.
   //By conpairing the who values we can determine which collision is 'more' correct.
   //The function then returns the appropriot collison type and all is good in the world
   
   //Test collision with both Right and Top (ie a top right collision)
   if (collisions[RIGHT] && collisions[TOP]){
      if (allowcorners || obj.allowcorners) return CORNERTOPRIGHT;
     
      float DistW, DistH;
     
      DistW = righta  - leftb;
      DistH = bottomb - topa;
      
      if (DistW < DistH) return RIGHT;
      else return TOP;
   }
   //Test collision with both Right and Bottom (ie a bottom right collision)
   if (collisions[RIGHT] && collisions[BOTTOM]){
      if (allowcorners || obj.allowcorners) return CORNERBOTRIGHT;
                        
      float DistW, DistH;
     
      DistW = righta  - leftb;
      DistH = bottoma - topb;
     
      if (DistW < DistH) return RIGHT;
      else return BOTTOM;
   }
   //Test collision with both Left and Top (ie a top left collision)
   if (collisions[LEFT] && collisions[TOP]){
      if (allowcorners || obj.allowcorners) return CORNERTOPLEFT;
                       
      float DistW, DistH;
     
      DistW = rightb  - lefta;
      DistH = bottomb - topa;
     
      if (DistW < DistH) return LEFT;
      else return TOP;
   }
   //Test collision with both Left and Bottom (ie a bottom left collision)
   if (collisions[LEFT] && collisions[BOTTOM]){
      if (allowcorners || obj.allowcorners) return CORNERBOTLEFT;
     
      float DistW, DistH;
     
      DistW = rightb  - lefta;
      DistH = bottoma - topb;
     
      if (DistW < DistH) return LEFT;
      else return BOTTOM;
   } 

   //Just as a side note.  I can't belive how easy the problem with the single type collisions were to solve
   //Before this I had some crazy nested for's that iterated through the collision array seeing if there were two collision types detected if not if returned the single collision type (which really didn't really work right)
   //I solved this by during the collions tests setting collision[0] to the type of collison 
   //This works without issue because the function will only get to this point when all other collision types are excluded
   //What I mean by this is that during the collision detection checks collision[0] will be set twice to two different values when a corner collision is encountered. (is could be set to RIGHT and TOP)
   //The reason why this won't cause a confict is that the corner collision types are dealt with during the above checks making collision[0] only applicable for the following check. (And consequently only set once during the collision tests which makes it a valid test)
   //also note that because of the circumstances need to make collision[0] valid this must be at the end of the function.  (todo in the future would be to somehow get this check right after the collsion detection (for efficency sake) 
   //Also note that it makes sure that a collision is made (ie not NOCOL or 0) this is just to allow the 'return COLERROR' the possibility to occur if the collision detection fails somehow.
   
   if (collision) return collision;
  
return COLERROR; //Error there was no collision type returned (THIS SHOULD NOT HAPPEN)
}

//Draw the bitmap to another bitmap
void object_base::draw(BITMAP *bitmap, BITMAP *Dest)
{
   draw_sprite(Dest,bitmap,x,y);
}

//Call this function with inverse = true if you are using another object to provide a collision boundery
//ie if you have an object that other objects are 'inside'
//What this basically does is instead of placeing obj just outside this object it places it just inside this object

//Note this function DOES take in to account any cliping on BOTH objects
void object_base::postcolreposition(object_base &obj, int collision, bool inverse)
{
   if (inverse){  //--------------This is what will be called with inverse = false ---------------
                  //-----Used when another object is used to define a zone------------------------
                  //In other words when one object fully surrounds another so that a boundery is established
      if(collision == TOP)    y = obj.top - cliptop;
      else
      if(collision == BOTTOM) y = obj.bottom - real_height - cliptop;
      else
      if(collision == RIGHT)  x = obj.right - real_width - clipleft;     
      else
      if(collision == LEFT)   x = obj.left - clipleft;
      else
      if(collision == CORNERTOPRIGHT) {y = obj.top - cliptop; x = obj.right - real_width - clipleft;}
      else
      if(collision == CORNERTOPLEFT)  {y = obj.top - cliptop; x = obj.left - clipleft;}
      else
      if(collision == CORNERBOTRIGHT) {y = obj.bottom - real_height - cliptop; x = obj.right - real_width - clipleft;}
      else 
      if(collision == CORNERBOTLEFT)  {y = obj.bottom - real_height - cliptop; x = obj.left - clipleft;} 
   }
   else{  //------------This is what will be called normally (ie with inverse = false)---------------
      if(collision == TOP)    y = obj.top - real_height - clipbottom;
      else
      if(collision == BOTTOM) y = obj.bottom - cliptop;
      else
      if(collision == RIGHT)  x = obj.right - clipleft;     
      else
      if(collision == LEFT)   x = obj.left - real_width - clipright;
      else
      if(collision == CORNERTOPRIGHT) {y = obj.top - real_height - clipbottom; x = obj.right - clipleft;}
      else
      if(collision == CORNERTOPLEFT)  {y = obj.top - real_height - clipbottom; x = obj.left - real_width - clipright;}
      else
      if(collision == CORNERBOTRIGHT) {y = obj.bottom - cliptop; x = obj.right - clipleft;}
      else 
      if(collision == CORNERBOTLEFT)  {y = obj.bottom - cliptop; x = obj.left - real_width - clipright;}    
   }
   
   setbounds();
}

//Draw lines of the BITMAP bitmap showing where clipping will occur
void object_base::draw_bounds(BITMAP *bitmap, bool full_lines, int r1, int g1, int b1, int r2, int g2, int b2)
{
   //Draw a lines where clipping will occur
   if (full_lines){      
      //left
      vline(bitmap, x + clipleft,            y,                         y + height,                makecol(r1,g1,b1));
      //right
      vline(bitmap, x + width - clipright,   y,                         y + height,                makecol(r1,g1,b1));
      //Top
      hline(bitmap, x,                       y + cliptop,               x + width,                 makecol(r2,g2,b2));
      //Bottom
      hline(bitmap, x,                       y + height - clipbottom,   x + width,                 makecol(r2,g2,b2));
   }
   else{
      //left
      vline(bitmap, x + clipleft,            y + cliptop,               y + height - clipbottom,   makecol(r1,g1,b1));
      //right
      vline(bitmap, x + width - clipright,   y + cliptop,               y + height - clipbottom,   makecol(r1,g1,b1));
      //Top
      hline(bitmap, x + clipleft,            y + cliptop,               x + width - clipright,     makecol(r2,g2,b2));
      //Bottom
      hline(bitmap, x + clipleft,            y + height - clipbottom,   x + width - clipright,     makecol(r2,g2,b2));        
   }
   
   //Draw the absolute bounds for the object
      //left
      vline(bitmap, x,                       y,                         y + height,                makecol(r1,g1,b1));
      //textprintf(bitmap, font, x, y + height/2, YELLOW, "L");
      //right
      vline(bitmap, x + width,               y,                         y + height,                makecol(r1,g1,b1));
      //textprintf(bitmap, font, x + width - 8, y + height/2, YELLOW, "R");
      //Top
      hline(bitmap, x,                       y,                         x + width,                 makecol(r2,g2,b2));
      //textprintf(bitmap, font, x + width/2 , y + 2, YELLOW, "T");
      //Bottom
      hline(bitmap, x,                       y + height,                x + width,                 makecol(r2,g2,b2));
      //textprintf(bitmap, font, x + width/2 , y + height - 8, YELLOW, "B");
}

void object_base::setbounds()
{
   right  = (x + width) - clipright;
   left   =  x + clipleft;   
   top    =  y + cliptop;
   bottom = (y + height) - clipbottom;
   
   real_width  = width - clipleft - clipright;
   real_height = height - cliptop - clipbottom;
}
     
/*
 * These are the inplementation functions for the object_base_move class
*/

object_base_move::object_base_move(int dirobj, float speedobj)
{
   dir = dirobj;
   speed = speedobj;
   has_moved = false;
   
   Zones = new object_base [6];
}

void object_base_move::move()
{
   float o;
   const float pi = 3.141592;
 
   fixangle();
 
   // If ball is headed at a downward negative slope (x decreasing and y increasing) 
   if (dir > 180 && dir < 270){
      o = 270 - dir;
      o = o *  pi / 180;
      x = x - sin(o) * speed;
      y = y + cos(o) * speed;
   }
   else
   // If ball is headed at a downward positive slope (x increasing and y increasing) 
   if (dir > 270 && dir < 360){
      o = 360 - dir;
      o = o *  pi / 180;
      x = x + cos(o) * speed;
      y = y + sin(o) * speed;
   }
   else
   // If ball is headed at a upward positive slope (x increasing and y decreasing)
   if (dir > 0 && dir < 90){
      o = dir;
      o = o *  pi / 180;
      x = x + cos(o) * speed;
      y = y - sin(o) * speed;
   }
   else
   // If ball is headed at a upward positive slope (x increasing and y increasing)
   if (dir > 90 && dir < 180){
      o = 180 - dir;
      o = o *  pi / 180;
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
   
   setbounds();
}

//This changes the direction of the object after a collision
//Obviously this should be called after a collision test

//Takes as an argument the result of a collision test
//Special note this does nothing if there is an inside collision
//This allows for using other objects as bounding boxes for other objects (Pritty neat huh:/)
void object_base_move::change_dir_after_col(int collision)
{
   if(collision == TOP || collision == BOTTOM) dir = 360 - dir;
   else
   if(collision == LEFT || collision == RIGHT) dir = 540 - dir;
   else 
   //the following is just temp will need to make calculations for correct angle later
   if(collision == CORNERBOTRIGHT) dir = 0-45;
   else
   if(collision == CORNERBOTLEFT) dir = 180+45;
   else
   if(collision == CORNERTOPRIGHT) dir = 0+45;
   else
   if(collision == CORNERTOPLEFT) dir = 180-45;
}

inline void object_base_move::fixangle()
{
   /* Makes sure the angle is between 0 and 360 degrees if not it corrects the angle.  Allows for negative angles. ie an angle of -45 = 315 ie 360 - 45 */
   for (;dir < 0;) dir = 360 + dir; 
   dir = dir % 360;
}

void object_base_move::draw_move_bounds(BITMAP *bitmap, bool label_zones, int r1, int g1, int b1, int r2, int g2, int b2)
{
   object_base ZoneA1, ZoneA2, ZoneB1, ZoneB2, ZoneC, Zone;
   
   ZoneA1 = Zones[0];
   ZoneA2 = Zones[1];
   ZoneB1 = Zones[2];
   ZoneB2 = Zones[3];
   ZoneC  = Zones[4];
   Zone   = Zones[5];
   
   //Draw the Bounds
   if ((dir > 0 && dir < 90) || (dir > 180 && dir < 270)){
      ZoneA1.draw_bounds(bitmap);
      line(bitmap, ZoneA1.right, ZoneA1.top, ZoneA1.left, ZoneA1.bottom, makecol(r2,g2,b2)); 

      ZoneA2.draw_bounds(bitmap);
      line(bitmap, ZoneA2.right, ZoneA2.top, ZoneA2.left, ZoneA2.bottom, makecol(r2,g2,b2));

      ZoneC.draw_bounds(bitmap);
      line(bitmap, ZoneC.right, ZoneC.top, ZoneC.left, ZoneC.bottom, makecol(r2,g2,b2));

      ZoneB1.draw_bounds(bitmap);
      ZoneB2.draw_bounds(bitmap);
   }
   else      
   if ((dir > 90 && dir < 180) || (dir > 270 && dir < 360)){
      ZoneA1.draw_bounds(bitmap);
      line(bitmap, ZoneA1.left, ZoneA1.top, ZoneA1.right, ZoneA1.bottom, makecol(r2,g2,b2)); 

      ZoneA2.draw_bounds(bitmap);
      line(bitmap, ZoneA2.left, ZoneA2.top, ZoneA2.right, ZoneA2.bottom, makecol(r2,g2,b2));

      ZoneC.draw_bounds(bitmap);
      line(bitmap, ZoneC.left, ZoneC.top, ZoneC.right, ZoneC.bottom, makecol(r2,g2,b2));

      ZoneB1.draw_bounds(bitmap);
      ZoneB2.draw_bounds(bitmap);
   }
   Zone.draw_bounds(bitmap);
   
   if(label_zones){    
      textprintf(bitmap,font,ZoneA1.x + ZoneA1.width/2,ZoneA1.y + ZoneA1.height/2,YELLOW,"A1");
      textprintf(bitmap,font,ZoneA2.x + ZoneA2.width/2,ZoneA2.y + ZoneA2.height/2,YELLOW,"A2");

      textprintf(bitmap,font,ZoneB1.x + ZoneB1.width/2,ZoneB1.y + ZoneB1.height/2,YELLOW,"B1");
      textprintf(bitmap,font,ZoneB2.x + ZoneB2.width/2,ZoneB2.y + ZoneB2.height/2,YELLOW,"B2");

      textprintf(bitmap,font,ZoneC.x + ZoneC.width/2,ZoneC.y + ZoneC.height/2,YELLOW,"C");

      textprintf(bitmap,font,Zone.x + Zone.width/2,Zone.y + Zone.height/2,YELLOW,"Z");
   }
   
  
}

void object_base_move::determine_zones()
{
   object_base ZoneA1, ZoneA2, ZoneB1, ZoneB2, ZoneC, Zone;
   object_base Blank, Base;
   
   float o;
   const float pi = 3.141592;

   o = dir * pi / 180;

   //Determine Zones
   if (dir == 0 || dir == 360){
      Zone.x = right;
      Zone.y = top;
      
      Zone.width = speed;
      Zone.height = real_height;
   }
   else
   if (dir == 180){
      Zone.x = left - speed;
      Zone.y = top;
      
      Zone.width = speed;
      Zone.height = real_height;   
   }
   else
   if (dir == 90){
      Zone.x = left;
      Zone.y = top - speed;
      
      Zone.width = real_width;
      Zone.height = speed;   
   }
   else
   if (dir == 270){
      Zone.x = left;
      Zone.y = bottom;
      
      Zone.width = real_width;
      Zone.height = speed;   
   }
   else

   //If the dir is from 0 to 90
   if (dir > 0 && dir < 90){
      Base.width  = cos(o) * speed;
      Base.height = sin(o) * speed;      
      
      //Zone A1
      ZoneA1 = Base;
      
      ZoneA1.x = left;
      ZoneA1.y = top - ZoneA1.height;
     
      //Zone A2
      ZoneA2 = Base;
      
      ZoneA2.x = right;
      ZoneA2.y = bottom - ZoneA2.height;
           
      //Zone C
      ZoneC = Base;
      
      ZoneC.x = right;
      ZoneC.y = top - ZoneC.height;
      
      //Zone B1
      ZoneB1 = Base;
      
      ZoneB1.width = real_width - Base.width;

      ZoneB1.x = left + ZoneA1.width;
      ZoneB1.y = top - ZoneB1.height;
      
      if(ZoneB1.width < 0) ZoneB1 = Blank;
      
      //Zone B2
      ZoneB2 = Base;
      
      ZoneB2.height = real_height - Base.height;

      ZoneB2.x = right;
      ZoneB2.y = top; 
      
      if(ZoneB2.height < 0) ZoneB2 = Blank;
   }
   else
   
   //If the dir is from 90 to 180
   if (dir > 90 && dir < 180){
      Base.width  = -cos(o) * speed;
      Base.height = sin(o) * speed;      
      
      //Zone A1
      ZoneA1 = Base;
      
      ZoneA1.x = right - ZoneA1.width;
      ZoneA1.y = top - ZoneA1.height;
    
      //Zone A2
      ZoneA2 = Base;
      
      ZoneA2.x = left - ZoneA2.width;
      ZoneA2.y = bottom - ZoneA2.height;
           
      //Zone C
      ZoneC = Base;
      
      ZoneC.x = left - ZoneC.width;
      ZoneC.y = top - ZoneC.height;
      
      //Zone B1
      ZoneB1 = Base;
      
      ZoneB1.width = real_width - Base.width;

      ZoneB1.x = left;
      ZoneB1.y = top - ZoneB1.height;
      
      if(ZoneB1.width < 0) ZoneB1 = Blank;
      
      //Zone B2
      ZoneB2 = Base;
      
      ZoneB2.height = real_height - Base.height;

      ZoneB2.x = left - ZoneB2.width;
      ZoneB2.y = top; 
      
      if(ZoneB2.height < 0) ZoneB2 = Blank;
   }
   else
   
   //If the dir is from 180 to 270
   if (dir > 180 && dir < 270){
      Base.width  = -cos(o) * speed;
      Base.height = -sin(o) * speed;      
      
      //Zone A1
      ZoneA1 = Base;
      
      ZoneA1.x = right - ZoneA1.width;
      ZoneA1.y = bottom;
     
      //Zone A2
      ZoneA2 = Base;
      
      ZoneA2.x = left - ZoneA2.width;
      ZoneA2.y = top;
           
      //Zone C
      ZoneC = Base;
      
      ZoneC.x = left - ZoneC.width;
      ZoneC.y = bottom;
      
      //Zone B1
      ZoneB1 = Base;
      
      ZoneB1.width = real_width - Base.width;

      ZoneB1.x = left;
      ZoneB1.y = bottom;
      
      if(ZoneB1.width < 0) ZoneB1 = Blank;
      
      //Zone B2
      ZoneB2 = Base;
      
      ZoneB2.height = real_height - Base.height;

      ZoneB2.x = left - ZoneB2.width;
      ZoneB2.y = bottom - ZoneB2.height; 
      
      if(ZoneB2.height < 0) ZoneB2 = Blank;
   }
   else
   
   //If the dir is from 270 to 360
   if (dir > 270 && dir < 360){
      Base.width  = cos(o) * speed;
      Base.height = -sin(o) * speed;      
      
      //Zone A1
      ZoneA1 = Base;
      
      ZoneA1.x = left;
      ZoneA1.y = bottom;
     
      //Zone A2
      ZoneA2 = Base;
      
      ZoneA2.x = right;
      ZoneA2.y = top;
           
      //Zone C
      ZoneC = Base;
      
      ZoneC.x = right;
      ZoneC.y = bottom;
      
      //Zone B1
      ZoneB1 = Base;
      
      ZoneB1.width = real_width - Base.width;

      ZoneB1.x = right - ZoneB1.width;
      ZoneB1.y = bottom;
      
      if(ZoneB1.width < 0) ZoneB1 = Blank;
      
      //Zone B2
      ZoneB2 = Base;
      
      ZoneB2.height = real_height - Base.height;

      ZoneB2.x = right;
      ZoneB2.y = bottom - ZoneB2.height; 
      
      if(ZoneB2.height < 0) ZoneB2 = Blank;
   } 
   
   //setbounds()
   ZoneA1.setbounds();
   ZoneA2.setbounds();
   ZoneB1.setbounds();
   ZoneB2.setbounds();
   ZoneC.setbounds();
   Zone.setbounds();
      
   Zones[0] = ZoneA1;
   Zones[1] = ZoneA2;
   Zones[2] = ZoneB1;
   Zones[3] = ZoneB2;
   Zones[4] = ZoneC;
   Zones[5] = Zone;  
}

int object_base_move::will_collide_with(object_base_move &obj)
{
    determine_zones();
    obj.determine_zones();
}

int object_base_move::will_collide_with(object_base &obj)
{
    int col;
    float o1, o2 ,o;
    
    const float pi = 3.141592;

    o = dir * pi / 180; 
    
    determine_zones();
    col = collides_with(obj);
    if(col) return col;
    
    if (dir > 0 && dir < 90){
       col = Zones[2].collides_with(obj);
       if (col) return BOTTOM;
    
       col = Zones[3].collides_with(obj);
       if (col) return LEFT;
       
       col = Zones[4].collides_with(obj);
       if (col) {
          return col;
       }
    }
    else
    if (dir > 90 && dir < 180){
       col = Zones[2].collides_with(obj);
       if (col) return BOTTOM;
    
       col = Zones[3].collides_with(obj);
       if (col) return RIGHT;
    }
    if (dir > 180 && dir < 270){
       col = Zones[2].collides_with(obj);
       if (col) return TOP;
    
       col = Zones[3].collides_with(obj);
       if (col) return RIGHT;
    }
    else
    if (dir > 270 && dir < 360){
       col = Zones[2].collides_with(obj);
       if (col) return TOP;
    
       col = Zones[3].collides_with(obj);
       if (col) return LEFT;
    }
}
#endif
