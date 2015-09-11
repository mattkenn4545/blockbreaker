#ifndef OBJECTS_H
#define OBJECTS_H
using namespace std;

struct block{
 volatile float x, y;
 int damage;
 int type;
 bool is_drawn;
 
 bool has_mod;
 
 bool is_nuke;
 bool is_laser;
 bool is_carpet;
 bool is_ballfast;
 bool is_ballslow;
 bool is_heal;
 bool is_attached;
 bool is_addball;
 bool is_removeball;
 bool is_timestop;
 bool is_easter;
}block[15][30];

struct ball{
 volatile float x, y;
 volatile int dir;
 volatile float speed;
 volatile float speed2;
 volatile bool is_attached;
}ball;

struct paddle{
 volatile float x, y;
}paddle;

struct framerate{
 volatile int counter;      
 volatile int fps;
 volatile int fps_tick; 
}framerate;

struct options{
 int seed;
 int overallspeed;

 int start_lives;
 float max_ball_speed;
 float min_ball_speed;
 int speed_steps ;

 int chance_is_nuke;
 int chance_is_laser;
 int chance_is_carpet;
 int chance_is_heal;
 int chance_is_ballfast;
 int chance_is_ballslow;
 int chance_is_attached;
 int chance_is_addball;
 int chance_is_removeball;
 int chance_is_timestop;
 int chance_is_easter;
}options;

struct highscores {unsigned long int highscores[9];}highscores;

#endif
