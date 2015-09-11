#include "functions.h"

using namespace std;
 
int main() 
{
 //config();
 bool is_fullscreen = false;
 
 options.seed = time(0);

 allegro_init();
 set_display_switch_mode(SWITCH_BACKAMNESIA);
 set_color_depth(32);
 text_mode(-1);   
 //set_gfx_mode(GFX_AUTODETECT_FULLSCREEN, 640, 480, 0, 0);
 set_gfx_mode(GFX_AUTODETECT_WINDOWED, 640, 480, 0, 0);
 
 install_keyboard();
 install_mouse();

 draw_splash("splash.bmp",2000);
 draw_splash("jumbledmess.bmp",2000);
 bool quit = menu();
 setup();
 while(!key[KEY_ESC] && !quit && !key[KEY_Q]){
                     
 draw();
 
 if(key[KEY_F] && is_fullscreen){
   set_gfx_mode(GFX_AUTODETECT_WINDOWED, 640, 480, 0, 0);
   is_fullscreen = false;
 }
 else
 if(key[KEY_F] && !is_fullscreen){ 
   set_gfx_mode(GFX_AUTODETECT_FULLSCREEN, 640, 480, 0, 0);
   is_fullscreen = true;
 }
 
 if(lives < 0){
   draw_splash("splash_youlose.bmp",1000);
   draw_splash("splash_sorry.bmp",1000);
   if(playagain()) quit = true;
   setup();
   score = 0;
 }
 else
 if(lives >= 0 && blocksbroke() > (COLS*ROWS)){
   draw_splash("splash_youwin.bmp",1000);
   draw_splash("splash_congrats.bmp",1000); 
   if(playagain()) quit = true;
   setup();
 }

 
 if(key[KEY_R]){          
   setup();
   score = 0;
 }
 
 if(key[KEY_M]){
   remove_int(timedloop);
   remove_int(updatestatsdisp);
   remove_int(calcframerate);
   menu();
   setup();
   score = 0;
 }
 }
 remove_int(timedloop);
 remove_int(updatestatsdisp);
 remove_int(calcframerate);
 draw_splash("splash_comebacksoon.bmp", 1000); 
 bitmap_setup(1);
 allegro_exit();
 return 0;     
}  
END_OF_MAIN();
