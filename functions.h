#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <allegro.h>
#include <ctime>
#include <coldet.h>
#include "objects.h"
#include "configfileeditor.h"
#include "physics.h"

#define ROWS 14
#define COLS 10

using namespace std;

BITMAP *game_bm, *blocks_bm, *paddle_bm, *block_bm[24], *buffer, *ball_bm, *fireball_bm, *fireball_temp_bm, *modifiers_bm;

int lives;
unsigned long int score = 0;
bool has_time_stop = false;
volatile bool has_time_stop_rest_done; 

BITMAP *grabframe(BITMAP *source, int width, int height, int startx, int starty, int columns, int frame);
int bitmap_setup(int option);
void setup();
int makeboard();
bool checkflags(int col, int row);
int drawboard();
int ballhitblock(bool applydamage);
int updatedir();
void timedloop();
void updatestatsdisp();
int damage(int col1, int row1);
void reposball(bool takelives);
int playagain();
int blocksbroke();
inline void fixangle();
void draw();
bool draw_splash(const char *s1, int time, int x = 0, int y = 0);
int menu();
void calcframerate();
int difficulty(int level);
void showscores();
void pause();


/* Grabs a single frame from a source bitmap and returns a pointer to a temp bitmap containing that frame.  Takes 
the width and height of the desired frame, where grabing should start, number of columns and the frame wanted 
NOTE must destroy the temp bitmap*/
BITMAP *grabframe(BITMAP *source, int width, int height, int startx, int starty, int columns, int frame)
{
 BITMAP *temp = create_bitmap(width,height);
 
 int x = startx + (frame % columns) * width;
 int y = starty + (frame / columns) * height;
 
 blit(source, temp, x, y, 0, 0, width, height);
 
 return temp;
}
/*Either sets up or destroys the various bitmaps used in the program Accepts either 0 - setup bitmaps or 1 destroy bitmaps*/

int bitmap_setup(int option)
{
 switch (option)
 {
        
   case 0:
   game_bm = load_bitmap("game_bm.bmp", NULL);   
   blocks_bm = load_bitmap("blocks_bm.bmp", NULL);
   for (int a = 0; a<24; a++){
       block_bm[a] = grabframe(blocks_bm,50,15,0,0,6,a);
   }
   destroy_bitmap(blocks_bm);
   paddle_bm = load_bitmap("paddle_bm.bmp", NULL);
   buffer = create_bitmap(SCREEN_W,SCREEN_H);
   clear_bitmap(buffer);
   ball_bm = load_bitmap("ball_bm.bmp", NULL);
   modifiers_bm = load_bitmap("modifiers.bmp", NULL);
   fireball_bm = load_bitmap("fireball.bmp", NULL);
   fireball_temp_bm = create_bitmap(14,14);
   break;
   
   case 1:
   
   destroy_bitmap(game_bm);
   destroy_bitmap(paddle_bm);
   destroy_bitmap(buffer);
   destroy_bitmap(ball_bm);
   destroy_bitmap(modifiers_bm);
   destroy_bitmap(fireball_bm);
   
   for (int a = 0; a<24; a++){
       destroy_bitmap(block_bm[a]);
   }
   break;
   default:
   return 1;
 }  
 
 return 0;
 
}

/* Sets up the bricks initialized damage to 0 and randomly selects type of brick*/
int makeboard()
{
 srand(options.seed);
 for(int row = 0; row<ROWS; row++){
   for(int col = 0; col<COLS; col++){     
     block[col][row].x = (SCREEN_W - (COLS * (block_bm[1]->w)))/2 + (col * block_bm[1]->w);
     block[col][row].y = row * (block_bm[1]->h) + 50;
     block[col][row].damage = 0;
     block[col][row].type = rand()%4;
     
     block[col][row].is_drawn = false;
     block[col][row].has_mod = false;
     
     block[col][row].is_nuke = false;
     block[col][row].is_laser = false;
     block[col][row].is_carpet = false;
     block[col][row].is_heal = false;
     block[col][row].is_ballfast = false;
     block[col][row].is_ballslow = false;
     block[col][row].is_attached = false;
     block[col][row].is_addball = false;
     block[col][row].is_removeball = false;
     block[col][row].is_easter = false;
     block[col][row].is_timestop = false;

     if(rand()%options.chance_is_ballfast == 1) block[col][row].is_ballfast = true;
     if(rand()%options.chance_is_ballslow == 1 && !block[col][row].is_ballfast) block[col][row].is_ballslow = true;

     if(rand()%options.chance_is_nuke == 1 && block[col][row].type == 0 && checkflags(col, row)) block[col][row].is_nuke = true; 
     if(rand()%options.chance_is_laser == 1 && block[col][row].type == 1 && checkflags(col, row)) block[col][row].is_laser = true;
     if(rand()%options.chance_is_carpet == 1 && block[col][row].type == 2 && checkflags(col, row)) block[col][row].is_carpet = true;
     if(rand()%options.chance_is_heal == 1 && block[col][row].type == 3 && checkflags(col, row)) block[col][row].is_heal = true;
     if(rand()%options.chance_is_attached == 1 && block[col][row].type == 1 && checkflags(col, row)) block[col][row].is_attached = true;
     if(rand()%options.chance_is_addball == 1 && block[col][row].type == 2 && checkflags(col, row)) block[col][row].is_addball = true;
     if(rand()%options.chance_is_removeball == 1 && block[col][row].type == 3 && checkflags(col, row)) block[col][row].is_removeball = true;
     if(rand()%options.chance_is_timestop == 1 && block[col][row].type == 0 && checkflags(col, row)) block[col][row].is_timestop = true;
     if(rand()%options.chance_is_easter == 1 && checkflags(col, row)) block[col][row].is_easter = true;
   }
 }
 return 0;
}

bool checkflags(int col, int row)
{
 if(block[col][row].is_nuke ||
   block[col][row].is_laser ||
   block[col][row].is_carpet ||
   block[col][row].is_heal ||
   block[col][row].is_ballfast ||
   block[col][row].is_ballslow ||
   block[col][row].is_attached ||
   block[col][row].is_addball ||
   block[col][row].is_removeball) return false;
}

/* You guessed it this function draws the board. More specifically it draws sprites of the blocks to the buffer bitmap. Also it checks to see if damage is greater than 5 if it is then it grabs from 
the backgroud the area of the destroyed block*/  
int drawboard()
{
 acquire_bitmap(buffer);
 for(register int row = 0; row<ROWS; row++){
   for(register int col = 0; col<COLS; col++){
       for(register int typ = 0; typ<4; typ++){
           block[col][row].is_drawn = true;
           
           int dam = block[col][row].damage;                           
           if(block[col][row].type == typ && dam < 5) draw_sprite(buffer,block_bm[typ*6+dam], block[col][row].x, block[col][row].y);
           if(dam > 5){ 
             blit(game_bm,buffer,block[col][row].x, block[col][row].y,block[col][row].x, block[col][row].y,block_bm[1]->w,block_bm[1]->h);
           }
             
           if(block[col][row].is_nuke && block[col][row].damage < 5 
             && block[col][row].damage >= 0) 
               masked_blit(modifiers_bm,buffer,0,0,block[col][row].x,block[col][row].y,block_bm[1]->w,block_bm[1]->h);
           else    
           if(block[col][row].is_heal && block[col][row].damage < 5 
             && block[col][row].damage >= 0) 
               masked_blit(modifiers_bm,buffer,50,0,block[col][row].x,block[col][row].y,block_bm[1]->w,block_bm[1]->h);
           else    
           if(block[col][row].is_carpet && block[col][row].damage < 5 
             && block[col][row].damage >= 0) 
               masked_blit(modifiers_bm,buffer,100,0,block[col][row].x,block[col][row].y,block_bm[1]->w,block_bm[1]->h);
           else    
           if(block[col][row].is_laser && block[col][row].damage < 5 
             && block[col][row].damage >= 0) 
               masked_blit(modifiers_bm,buffer,150,0,block[col][row].x,block[col][row].y,block_bm[1]->w,block_bm[1]->h);
           else    
           if(block[col][row].is_attached && block[col][row].damage < 5 
             && block[col][row].damage >= 0) 
               masked_blit(modifiers_bm,buffer,300,0,block[col][row].x,block[col][row].y,block_bm[1]->w,block_bm[1]->h);
           else    
           if(block[col][row].is_addball && block[col][row].damage < 5 
             && block[col][row].damage >= 0) 
               masked_blit(modifiers_bm,buffer,350,0,block[col][row].x,block[col][row].y,block_bm[1]->w,block_bm[1]->h);
           else    
           if(block[col][row].is_removeball && block[col][row].damage < 5 
             && block[col][row].damage >= 0) 
               masked_blit(modifiers_bm,buffer,400,0,block[col][row].x,block[col][row].y,block_bm[1]->w,block_bm[1]->h);               
           else    
           if(block[col][row].is_ballfast && block[col][row].damage < 5) 
             masked_blit(modifiers_bm,buffer,200,0,block[col][row].x,block[col][row].y,block_bm[1]->w,block_bm[1]->h);
           else  
           if(block[col][row].is_ballslow && block[col][row].damage < 5) 
             masked_blit(modifiers_bm,buffer,250,0,block[col][row].x,block[col][row].y,block_bm[1]->w,block_bm[1]->h);
           else  
           if(block[col][row].is_easter && block[col][row].damage < 5) 
             masked_blit(modifiers_bm,buffer,450,0,block[col][row].x,block[col][row].y,block_bm[1]->w,block_bm[1]->h);    
           else  
           if(block[col][row].is_timestop && block[col][row].damage < 5) 
             masked_blit(modifiers_bm,buffer,500,0,block[col][row].x,block[col][row].y,block_bm[1]->w,block_bm[1]->h);         
       }
   }
 }
 release_bitmap(buffer);
 return 0;
}

/* The main function of this function is to call the collision detection function, apply damage to blocks and update hitblock with the location of the 
hit block if there is one.  Also it detects if a collision has occured with the paddle and updates paddle.is_hit with the return value of that test */
int ballhitblock(bool applydamage)
{ 
 int result;    

 for(register int row = 0; row<ROWS; row++){
   for(register int col = 0; col<COLS; col++){
     if(block[col][row].damage < 6){
       result = coldet(block_bm[1], ball_bm, block[col][row].x, block[col][row].y, ball.x, ball.y);
       if(result){
         movebitmap(block_bm[1], ball_bm, block[col][row].x, block[col][row].y, ball.x, ball.y,result);
         if(applydamage) damage(col,row);
         return result;
       }
     }
   }
 }
 return 0;
}

void timedloop()
{
 if(ball.is_attached){
   if(ball.x < paddle.x - ball_bm->w)
      ball.x = paddle.x - ball_bm->w + 1;
   if(ball.x > paddle.x + paddle_bm->w)
      ball.x = paddle.x + paddle_bm->w - 1;
 }
      
 updatepos(ball.x, ball.y, ball.dir, ball.speed);
 
 updatedir();
 
 if(ball.is_attached){
    ball.dir = 270;
    if(mouse_b & 1){ 
      ball.speed = ball.speed2;
      ball.is_attached = false;
    }    
 }     
}

void updatestatsdisp()
{
 blit(game_bm,buffer,300,468,300,468,160,10);    
 textprintf(buffer,font,300,468,YELLOW,"%i",score + (blocksbroke()-1)*100);
 blit(game_bm,buffer,10,1,10,1,SCREEN_W,10); 
 textprintf(buffer,font,1,1,YELLOW,"Ball speed %f Ball direction %i", ball.speed, ball.dir);
 blit(game_bm,buffer,10,10,10,10,SCREEN_W,10); 
 textprintf(buffer,font,1,10,YELLOW,"Seed %i FPS %i Counter %i ",options.seed, framerate.fps, framerate.counter);
}

int damage(int col1, int row1)
{
    
 if(block[col1][row1].type == 0){
   block[col1][row1].damage += 1;
   score += 100 + ball.speed * 100;
 }
 else
 if(block[col1][row1].type == 1){
   block[col1][row1].damage += 2;
   score += 150 + ball.speed * 100;
 }
 else
 if(block[col1][row1].type == 2){
   block[col1][row1].damage += 3;
   score += 170 + ball.speed * 100;
 }
 else
 if(block[col1][row1].type == 3){
   block[col1][row1].damage += 4;
   score += 160 + ball.speed * 100;
 }

 
 if (block[col1][row1].is_nuke && block[col1][row1].damage > 5){
   for(register int row = 0; row<ROWS; row++){
      for(register int col = 0; col<COLS; col++){
        block[col][row].damage += 3;
        score = score + (COLS*ROWS-blocksbroke())*165;
      }
   }
 }
 else
 if (block[col1][row1].is_heal && block[col1][row1].damage > 5){
   for(register int row = 0; row<ROWS; row++){
      for(register int col = 0; col<COLS; col++){
        if(block[col][row].damage < 5){
          block[col][row].damage -= 2;
          if(block[col][row].damage < 0) block[col][row].damage = 0;
          score = score + (COLS*ROWS-blocksbroke())*20;          
        }
      }
   }
 }
 else
 if (block[col1][row1].is_carpet && block[col1][row1].damage > 5){
   for(register int row = 0; row<ROWS; row++){
      for(register int col = 0; col<COLS; col++){
        if(row % 2 == 1 && col % 2 == 1) block[col][row].damage += 2;
        if(row % 2 == 0 && col % 2 == 0) block[col][row].damage += 2;
        score = score + (COLS*ROWS-blocksbroke())*83;
      }
   }
 }
 else
 if (block[col1][row1].is_laser && block[col1][row1].damage > 5){
   for(register int row = 0; row<ROWS; row++){
      for(register int col = 0; col<COLS; col++){
        if(row == row1 || col == col1) block[col][row].damage += rand()%3+3;
        score = score + (COLS+ROWS)*175;
      }
   }
 }
 else
 if (block[col1][row1].is_ballfast && block[col1][row1].damage > 5){
   ball.speed += (options.max_ball_speed-options.min_ball_speed)/options.speed_steps;
   if(ball.speed > options.max_ball_speed) ball.speed = options.max_ball_speed;
   score = score + 100;
 }
 else
 if (block[col1][row1].is_ballslow && block[col1][row1].damage > 5){
   ball.speed -= (options.max_ball_speed-options.min_ball_speed)/options.speed_steps;
   if(ball.speed < options.min_ball_speed) ball.speed = options.min_ball_speed;
   score = score + 100;
 }
 /* TODO (#1#): blit over balls after the ball has been put on paddle */
 else
 if (block[col1][row1].is_attached){
   reposball(false);
   blit(game_bm,buffer,ball.x-ball.speed,ball.y-ball.speed,ball.x-ball.speed,ball.y-ball.speed,ball_bm->w+ball.speed*2+1,ball_bm->h+ball.speed*2+1);
   score = score + 100;
 } 
 else
 if (block[col1][row1].is_addball && block[col1][row1].damage > 5){
   lives++;
   if(lives > 10) lives = 10;
   blit(game_bm,buffer,512,466,512,466,512 + (lives+1*ball_bm->w) + (lives+1*2),470);
   score = score + 100;
   for (int a = 0; a<lives; a++){
     draw_sprite(buffer,ball_bm,512 + a*ball_bm->w + a*2,466);
   }
 }
 else
 if (block[col1][row1].is_removeball && block[col1][row1].damage > 5){
   lives--;
   if(lives < 0) lives = 0;
   blit(game_bm,buffer,512,466,512,466,512 + (lives+1*ball_bm->w) + (lives+1*2),470);
   score = score + 100;
   for (int a = 0; a<lives; a++){
     draw_sprite(buffer,ball_bm,512 + a*ball_bm->w + a*2,466);
   }
 }
 else
 if (block[col1][row1].is_timestop && block[col1][row1].damage > 5){
   has_time_stop = true;
 }
 return 0;
}

void reposball(bool takelives)
{
 if (takelives) lives -= 1;
 
 ball.is_attached = true;
 ball.speed2 = ball.speed;
 /* TODO (#1#): Figure Out why this expression always is true and 
                why it is needed */
 if (ball.speed2 <= options.min_ball_speed) ball.speed2 = options.min_ball_speed;
 ball.speed = 0;
 
 blit(game_bm,buffer,512,466,512,466,512 + (lives+1*ball_bm->w) + (lives+1*2),470);
 for (int a = 0; a<lives; a++){
   draw_sprite(buffer,ball_bm,512 + a*ball_bm->w + a*2,466);
 } 
 
 ball.y = paddle.y - ball_bm->h - 1;
 ball.x = paddle.x + paddle_bm->w/2 - ball_bm->w/2;
}


int updatedir()
{

 //Detect if ball has hit a block
 int collision = ballhitblock(true);
 
 if(collision){
   if(collision == TOP || collision == BOTTOM) ball.dir = 360 - ball.dir;
   if(collision == LEFT || collision == RIGHT) ball.dir = 540 - ball.dir;
   if(collision == INSIDE) reposball(false);
   fixangle();
 }
 
 if(ball.x < 20){ 
   ball.x = 20;
   ball.dir = 540 - ball.dir;
   fixangle();
   return 0;
 }
 
 if(ball.x > SCREEN_W - 20 - ball_bm->w){
   ball.x = SCREEN_W - 20 - ball_bm->w - 1;
   ball.dir = 540 - ball.dir;
   fixangle();
   return 0;
 }
 
 if(ball.y < 20){
   ball.y = 20;        
   ball.dir = 360 - ball.dir;
   fixangle();
   return 0;
 }
 
 if(ball.y > SCREEN_H - 50 - ball_bm->h){
   reposball(true);
   fixangle();
   return 0;
 }

 //Checks for collision with the paddle then moves ball to just outside the paddle to prevent snagging
 collision = coldet(paddle_bm,ball_bm,paddle.x,paddle.y,ball.x,ball.y);
   
 //Mods the direction of the ball when it hits the paddle 
   if (collision){
     int ballpos_x;
     int paddlepos_x;
     ballpos_x = ball.x + (ball_bm->w)/2;
     paddlepos_x = paddle.x + (paddle_bm->w)/2; 
     ball.dir = 90 + (paddlepos_x - ballpos_x);
     ball.y = paddle.y - ball_bm->h - 1;
   }
/*
Disabling this becuse i need more code to get it to work the way I want it to.
  //Speed-up ball in you right click within 10 pixels of the ball hitting the paddle 
  collision = coldet(paddle_bm,ball_bm,paddle.x,paddle.y,ball.x,ball.y, false, false,0,0,0,-10);
  if(collision && mouse_b & 1) ball.speed = ball.speed + .25;
*/

  //Prevents the ball from going side to side a lot with no significant upward movement.
 if(ball.dir >= 0 && ball.dir < 20) ball.dir = 20;
 if(ball.dir <= 180 && ball.dir > 160) ball.dir = 160;
 
return 0;
}

bool draw_splash(const char *s1, int time, int x, int y)
{
 BITMAP *splash;
 bool is_bad = false;
 if(!(splash = load_bitmap(s1, NULL))){ 
   alert("Image file", s1, "was not found. Please locate.", "CONTINUE", NULL, 0, 0);
   is_bad = true;
 }
   else {
     draw_sprite(screen,splash,x,y);
     if(time) rest(time);
   }
 destroy_bitmap(splash);
 return is_bad;
}

int playagain()
{
   bool is_bad = draw_splash("splash_playagain.bmp", NULL);
   
   while (!key[KEY_Y] && !key[KEY_N] && !key[KEY_Q] && !key[KEY_ESC] && !is_bad);
   if(key[KEY_N] || key[KEY_Q] || key[KEY_ESC] || is_bad) return 1;
   if(key[KEY_Y]) return 0;
}

int menu()
{
  draw_splash("menu.bmp",NULL);
  bool has_choose = false;
  while (!key[KEY_1] || !key[KEY_1_PAD] || !key[KEY_2] || !key[KEY_2_PAD] || !key[KEY_3] || !key[KEY_3_PAD] || 
         !key[KEY_4] || !key[KEY_4_PAD] || !key[KEY_5] || !key[KEY_5_PAD] || !key[KEY_ESC] || !key[KEY_Q]){
         
  if(key[KEY_1] || key[KEY_1_PAD]) return 0;
  if(key[KEY_2] || key[KEY_2_PAD]) {
    has_choose = true;
      set_gfx_mode(GFX_TEXT, 640, 480, 0, 0);
      configgame(CONFIG,OPENFILE,"configuration");
      set_gfx_mode(GFX_AUTODETECT_WINDOWED, 640, 480, 0, 0);
  }
  if(key[KEY_3] || key[KEY_3_PAD]) {
    draw_splash("menu_howtoplay.bmp",1000); 
    draw_splash("menu_typesofblocks.bmp",1000);
    has_choose = true;
  }
  if(key[KEY_4] || key[KEY_4_PAD]) {
    draw_splash("menu_credits.bmp",1000);
    has_choose = true;
  }
  if(key[KEY_5] || key[KEY_5_PAD] || key[KEY_ESC]) return 1;
  if(has_choose) menu();
  }
  
}
void setup()
{
 difficulty(0);
 options.seed = time(0);
 options.seed++;
 bitmap_setup(0);
 paddle.y = (SCREEN_H - 50) - paddle_bm->h;
 makeboard();
 blit(game_bm,buffer,0,0,0,0,SCREEN_W,SCREEN_H);
 install_int(timedloop, options.overallspeed);
 install_int(updatestatsdisp, 100);
 install_int(calcframerate,1000);
 lives = options.start_lives;
 reposball(false);
}

int blocksbroke()
{
 int number = 1;
 for(register int row = 0; row<ROWS; row++){
   for(register int col = 0; col<COLS; col++){
     if(block[col][row].damage > 5) number++;
   }
 }
 return number;
}

void fixangle()
{
 /* Makes sure the angle is between 0 and 360 degrees if not it corrects the angle.  Allows for negative angles. ie an angle of -45 = 315 ie 360 - 45 */
 for (;ball.dir < 0;) ball.dir = 360 + ball.dir; 
 ball.dir = ball.dir % 360;
}

void draw()
{         
 drawboard();
 draw_sprite(buffer, ball_bm, ball.x, ball.y);
 draw_sprite(buffer,paddle_bm,paddle.x,paddle.y);
 
 draw_sprite(screen,buffer,0,0);
 
 blit(game_bm, buffer, ball.x-ball.speed*2, ball.y-ball.speed*2, ball.x-ball.speed*2, ball.y-ball.speed*2, ball_bm->w+ball.speed*4, ball_bm->h+ball.speed*4);
 blit(game_bm, buffer, 0, paddle.y-ball_bm->h-1, 0, paddle.y-ball_bm->h-1, SCREEN_W, paddle_bm->h+ball_bm->h+1);
 
 if(mouse_x < 20) mouse_x = 20;
 if(mouse_x > SCREEN_W - 20 - paddle_bm->w) mouse_x = SCREEN_W - 20 - paddle_bm->w;
 paddle.x = mouse_x;
 
 framerate.fps_tick++;
}

void calcframerate()
{
 framerate.counter++;
 
 framerate.fps = framerate.fps_tick;
 framerate.fps_tick = 0;
}

int difficulty(int level)
{
 open(CONFIG,"configuration");
   
return 0;
}

void showscores()
{
}

void pause()
{
 if(has_time_stop_rest_done){
   remove_int(pause);
   install_int(timedloop, options.overallspeed);
 }
 remove_int(timedloop);
 has_time_stop_rest_done = true;
}
#endif

