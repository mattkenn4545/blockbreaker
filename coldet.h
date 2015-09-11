#ifndef COLDET_H
#define COLDET_H

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


int coldet (float xa, float ya, float xb, float yb, int widtha, int heighta, int widthb, int heightb, bool allowcorners = false, int clipaleft = 0, int cliparight = 0, int clipatop = 0, int clipabottom = 0, int clipbleft = 0, int clipbright = 0, int clipbtop = 0, int clipbbottom = 0);

//Note the returned collision direction with relative to the First bitmap. So if you have two bitmaps one to the right and one to the left and one to the right collides with the one one the left this function would return right because the second bitmap is colliding to the right side of the first.
int coldet (float xa, float ya, float xb, float yb, int widtha, int heighta, int widthb, int heightb, bool allowcorners, int clipaleft, int cliparight, int clipatop, int clipabottom, int clipbleft, int clipbright, int clipbtop, int clipbbottom)
{
   float righta, lefta, topa, bottoma;    //the bounderies of the first bitmap
   float rightb, leftb, topb, bottomb;    //The next two lines are helper variables that define the x,y coords of the second bitmap'
   int collision[4];                      //This array stores the values of the collision detection checks

   for (int A = 0; A <= 4;  A++) collision[A] = NOCOL;  //Initializes the collision array to NOCOL
 
   //set values to all the helper variables
   //These define the bounderies of the objects being tested.
   righta = (xa + widtha)   - cliparight;
   lefta = xa + clipaleft;
   topa = ya + clipatop;
   bottoma = (ya + heighta) - clipabottom;

   rightb = (xb + widthb)   - clipbright;
   leftb = xb + clipbleft;
   topb = yb + clipbtop; 
   bottomb = (yb + heightb) - clipbbottom;
       
   //Detects if there is no collision at all.
   if(leftb   > righta  || 
      rightb  < lefta   || 
      topb    > bottoma || 
      bottomb < topa) return NOCOL;
      
   //Detects if the second bitmap is fully within the first bitmap
   if (leftb  > lefta   && 
      rightb  < righta  && 
      topb    > topa    && 
      bottomb < bottoma) return INSIDE;      
   
   //The main collision tests (kinda funny that these are the main tests that everything else depends and they take up only 4 very short lines)
   if (leftb <= righta && rightb >= righta)   {collision[RIGHT]  = RIGHT;  collision[0] = RIGHT;}
   if (rightb >= lefta && leftb <= lefta)     {collision[LEFT]   = LEFT;   collision[0] = LEFT;}
   if (topb <= bottoma && bottomb >= bottoma) {collision[BOTTOM] = BOTTOM; collision[0] = BOTTOM;}
   if (bottomb >= topa && topb <= topa)       {collision[TOP]    = TOP;    collision[0] = TOP;}
      
   //The following tests check for situations in which the second bitmap is colliding with one of the corners of the first.
   //This results in two of the above collision tests to be evaluated as true
   //To solve this these tests first looks to see if corner collisions are turned on if they are it returns the correct corrner as the collision type.
   //Otherwise it finds how many pixels deep the second bitmap is into the first for both the width and the height.
   //By conpairing the who values we can determine which collision is 'more' correct.
   //The function then returns the appropriot collison type and all is good in the world
   
   //Test collision with both Right and Top (ie a top right collision)
   if (collision[RIGHT] && collision[TOP]){
      if (allowcorners) return CORNERTOPRIGHT;
     
      float DistW, DistH;
     
      DistW = righta  - leftb;
      DistH = bottomb - topa;
     
      if (DistW < DistH) return RIGHT;
      else return TOP;
   }
   //Test collision with both Right and Bottom (ie a bottom right collision)
   if (collision[RIGHT] && collision[BOTTOM]){
      if (allowcorners) return CORNERBOTRIGHT;
                        
      float DistW, DistH;
     
      DistW = righta  - leftb;
      DistH = bottoma - topb;
     
      if (DistW < DistH) return RIGHT;
      else return BOTTOM;
   }
   //Test collision with both Left and Top (ie a top left collision)
   if (collision[LEFT] && collision[TOP]){
      if (allowcorners) return CORNERTOPLEFT;
                       
      float DistW, DistH;
     
      DistW = rightb  - lefta;
      DistH = bottomb - topa;
     
      if (DistW < DistH) return LEFT;
      else return TOP;
   }
   //Test collision with both Left and Bottom (ie a bottom left collision)
   if (collision[LEFT] && collision[BOTTOM]){
      if (allowcorners) return CORNERBOTLEFT;
     
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
   if (collision[0]) return collision[0];
  
return COLERROR; //Error there was no collision type returned (THIS SHOULD NOT HAPPEN)
}
#endif
