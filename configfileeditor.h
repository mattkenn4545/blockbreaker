#ifndef CONFIGUREFILEEDITOR_H
#define CONFIGUREFILEEDITOR_H

#include <cstdlib>
#include <iostream>
#include <fstream>
#include "objects.h"

#define CONFIG 1
#define HIGHSCORE 2
#define OPENFILE 1
#define NEWFILE 2
using namespace std;

int configgame(int file, int access, const char *filename);
int open(int file, const char *filename);
int save(int file, const char *filename);
int start(int file);
int disp(int file);
int change(int file);
void header();

int configgame(int file, int access, const char *filename)
{    
    while(file != 1 && file != 2){
      for(int a = 0; a<50; a++) cout << "\n";
      header();          
      cout << "What configuration file would you like to use?\n 1. options\n 2. highscores\n\n\n";
      cin >> file;
     
    }
    while(access != 1 && access != 2){
      for(int a = 0; a<50; a++) cout << "\n";
      header();          
      cout << "What would you like to do with the file?\n 1. Open existing file\n 2. Start new file\n\n\n";
      cin >> access;
      
    }
    
    switch(access){
      case 1:
        open(file, filename);
        break;
      case 2:
        start(file);
        break;
    }
    
    access = 0;
    
    while(access != 1 && access != 2 && access != 3){
        for(int a = 0; a<50; a++) cout << "\n";
        header();         
        cout << "Make a selection? \n 1. Show current settings\n 2. Change settings\n 3. Apply settings\n 4. Go back to menu\n";
        cin >> access;   
      
    switch(access){
      case 1:
      disp(file);
      break;
      case 2:
      change(file);
      break;
      case 3:
      save(file, filename);
      break;
    }
    if(access == 4) return 0;
    access = 0;
    }
    
    system("PAUSE");
    return EXIT_SUCCESS;
}

int open(int file, const char *filename)
{
 switch(file){
   case 1:
   {
   ifstream incfg(filename, ios::in | ios::binary);
   
   if(!incfg) {
     cout << "Cannot load the " << filename << " file call Matt for a fix\n";
     system("PAUSE");
     return 1;
   }
   
   incfg.read((char *) &options, sizeof(struct options));
   incfg.close();
   }
   break;
   case 2:
   {
   ifstream inscr(filename, ios::out | ios::binary);
   
   if(!inscr) {
     cout << "Cannot load the " << filename << " file call Matt for a fix\n";
     system("PAUSE");
     return 1;
   }
   
   inscr.read((char *) &highscores, sizeof(struct highscores));
   inscr.close();
   }        
   break;
 }
 return 0;
}

int save(int file, const char *filename)
{
 switch(file){
   case 1:
   {
   ofstream outcfg(filename, ios::out | ios::binary);
   
   if(!outcfg) {
     cout << "Cannot save the " << filename << " file call Matt for a fix\n";
     system("PAUSE");
     return 1;
   }
   
   outcfg.write((char *) &options, sizeof(struct options));
   outcfg.close();
   }
   break;
   case 2:
   {
   ofstream outscr(filename, ios::out | ios::binary);
   
   if(!outscr) {
     cout << "Cannot save the " << filename << " file call Matt for a fix\n";
     system("PAUSE");
     return 1;
   }
   
   outscr.write((char *) &highscores, sizeof(struct highscores));
   outscr.close();
   }        
   break;
 }
 return 0;
}

int start(int file)
{
 switch(file){
   case 1:
     options.seed = 1;
     options.overallspeed = 13;
     
     options.start_lives = 0;
     options.max_ball_speed = 1;
     options.min_ball_speed = 1;
     options.speed_steps = 1;

     options.chance_is_nuke = 1;
     options.chance_is_laser = 1;
     options.chance_is_carpet = 1;
     options.chance_is_heal = 1;
     options.chance_is_ballfast = 1;
     options.chance_is_ballslow = 1;
     options.chance_is_attached = 1;
     options.chance_is_addball = 1;
     options.chance_is_removeball = 1;
     options.chance_is_timestop = 1;
     options.chance_is_easter = 4000;
     break;
   case 2:
     for(int a = 0; a < 10; a++) highscores.highscores[a] = a * 1000; 
     break;
 }
 return 0;
}

int disp(int file)
{
 switch(file){
   case 1:
   for(int a = 0; a<50; a++) cout << "\n";
   cout << "Current values for the Option file\n\n";
   cout << "--------------------------------------------------------------------------------\n";
   cout << "Current Seed (What blocks are generated) " << options.seed << "\n";
   cout << "--------------------------------------------------------------------------------\n";
   cout << "Overall speed of game (how often timed loop is called) " << options.overallspeed << " milliseconds\n"; 
   cout << "--------------------------------------------------------------------------------\n";
   cout << "Starting lives " << options.start_lives << "\n";
   cout << "--------------------------------------------------------------------------------\n";
   cout << "Max ball speed " << options.max_ball_speed << "\n";
   cout << "--------------------------------------------------------------------------------\n";
   cout << "Min ball speed " << options.min_ball_speed << "\n";
   cout << "--------------------------------------------------------------------------------\n";
   cout << "Steps it takes to go from min ball speed to max ball speed " << options.speed_steps << "\n";
   cout << "--------------------------------------------------------------------------------\n";
   cout << "Chance of a nuclear block being generated 1 in " << options.chance_is_nuke << "\n" ;
   cout << "--------------------------------------------------------------------------------\n";
   cout << "Chance of a laser block being generated 1 in " << options.chance_is_laser << "\n";
   cout << "--------------------------------------------------------------------------------\n";
   cout << "Chance of a carpet bomb block being generated 1 in " << options.chance_is_carpet << "\n";
   cout << "--------------------------------------------------------------------------------\n";
   cout << "Chance of a heal block being generated 1 in " << options.chance_is_heal << "\n";
   cout << "--------------------------------------------------------------------------------\n";
   cout << "Chance of a ball speed up block being generated 1 in " << options.chance_is_ballfast << "\n";
   cout << "--------------------------------------------------------------------------------\n";
   cout << "Chance of a ball slow down block being generated 1 in " << options.chance_is_ballslow << "\n";
   cout << "--------------------------------------------------------------------------------\n";
   cout << "Chance of a attach-ball-to-paddle block being generated 1 in " << options.chance_is_attached << "\n";
   cout << "--------------------------------------------------------------------------------\n";
   cout << "Chance of a add ball block being generated 1 in " << options.chance_is_addball << "\n";
   cout << "--------------------------------------------------------------------------------\n";
   cout << "Chance of a remove ball block being generated 1 in " << options.chance_is_removeball << "\n";
   cout << "--------------------------------------------------------------------------------\n";
   cout << "Chance of a time stop block being generated 1 in " << options.chance_is_timestop << "\n\n";
   cout << "--------------------------------------------------------------------------------\n";
   cout << "Note that there is a special type of block but \nI'm keeping that one a surprise and not allowing editing of it \n\n";
   system("PAUSE");   
   break;
   case 2:
   cout << "Current values for the highscores file\n";
   for(int a = 0; a < 10; a++) cout << "Score: " << a+1 << " " << highscores.highscores[a] << "\n";
   system("PAUSE");
   break;
 }
 return 0;
}

int change(int file)
{
 int choice;
 float value;
 switch(file){
   case 1:
   while(choice != 99 && value != 99){
   for(int a = 0; a<50; a++) cout << "\n";
   cout << "Choose which value you would like to change. 99 returns to menu\n\n";
   cout << "--------------------------------------------------------------------------------\n";
   cout << "--------------------------------------------------------------------------------\n";
   cout << "1.  Current Seed (What blocks are generated) " << options.seed << "\n";
   cout << "--------------------------------------------------------------------------------\n";
   cout << "2.  Starting lives currently " << options.start_lives << "\n";
   cout << "--------------------------------------------------------------------------------\n";
   cout << "3.  Max ball speed currently " << options.max_ball_speed << "\n";
   cout << "--------------------------------------------------------------------------------\n";
   cout << "4.  Min ball speed currently " << options.min_ball_speed << "\n";
   cout << "--------------------------------------------------------------------------------\n";
   cout << "5.  Chance of a nuclear block being generated currently 1 in " << options.chance_is_nuke << "\n";
   cout << "--------------------------------------------------------------------------------\n";
   cout << "6.  Chance of a laser block being generated currently 1 in " << options.chance_is_laser << "\n";
   cout << "--------------------------------------------------------------------------------\n";
   cout << "7.  Chance of a carpet bomb block being generated currently 1 in " << options.chance_is_carpet << "\n";
   cout << "--------------------------------------------------------------------------------\n";
   cout << "8.  Chance of a heal block being generated currently 1 in " << options.chance_is_heal << "\n";
   cout << "--------------------------------------------------------------------------------\n";
   cout << "9.  Chance of a ball speed up block being generated currently 1 in " << options.chance_is_ballfast << "\n";
   cout << "--------------------------------------------------------------------------------\n";
   cout << "10. Chance of a ball slow down block being generated currently 1 in " << options.chance_is_ballslow << "\n";
   cout << "--------------------------------------------------------------------------------\n";
   cout << "11. Chance of a attach-ball-to-paddle block being generated currently 1 in " << options.chance_is_attached << "\n";
   cout << "--------------------------------------------------------------------------------\n";
   cout << "12. Chance of a add ball block being generated currently 1 in " << options.chance_is_addball << "\n";
   cout << "--------------------------------------------------------------------------------\n";
   cout << "13. Chance of a remove ball block being generated currently 1 in " << options.chance_is_removeball << "\n";
   cout << "--------------------------------------------------------------------------------\n";
   cout << "14. Chance of a time stop block being generated currently 1 in " << options.chance_is_timestop << "\n";
   cout << "--------------------------------------------------------------------------------\n";
   cout << "15. Overall speed of game (how often timed loop is called) currently " << options.overallspeed << " milliseconds\n";
   cout << "--------------------------------------------------------------------------------\n";
   cout << "16. Steps it takes to go from min ball speed to max ball speed currently " << options.speed_steps << "\n\n";
   cout << "--------------------------------------------------------------------------------\n";
   cout << "Note that there is a special type of block but \nI'm keeping that one a surprise and not allowing editing of it \n\n";
   cout << "--------------------------------------------------------------------------------\n";
   cin >> choice;
   cout << "What value would you like it to be changed to?(Make sure the value is a number!!!)\n";
   cin >> value;
   
   if(choice == 1) options.seed = value;
   if(choice == 2) options.start_lives = value;
   if(choice == 3) options.max_ball_speed = value;
   if(choice == 4) options.min_ball_speed = value;
   if(choice == 5) options.chance_is_nuke = value;
   if(choice == 6) options.chance_is_laser= value;
   if(choice == 7) options.chance_is_carpet = value;
   if(choice == 8) options.chance_is_heal = value;
   if(choice == 9) options.chance_is_ballfast = value;
   if(choice == 10) options.chance_is_ballslow = value;
   if(choice == 11) options.chance_is_attached = value;
   if(choice == 12) options.chance_is_addball = value;
   if(choice == 13) options.chance_is_removeball = value;
   if(choice == 14) options.chance_is_timestop = value;
   if(choice == 15) options.overallspeed = value;
   if(choice == 16) options.speed_steps = value;
   if(choice == 19) options.chance_is_easter = value;
   } 
   break;
   case 2:
   while(choice != 99 && value != 99){
   for(int a = 0; a<50; a++) cout << "\n";             
   cout << "Choose which value you would like to change. 99 returns to menu\n\n";
   for(int a = 0; a < 10; a++) cout << a+1 << ". Score: " << a+1 << " " << highscores.highscores[a] << "\n";
   cin >> choice;
   cout << "What value would you like it to be changed to?(Make sure the value is a number!!!)\n";
   cin >> value;
   highscores.highscores[choice-1] = value;
   }
   break;
 }
 return 0;
}



void header()
{
    cout << "--------------------------------------------------------------------------------\n";
    cout << "--------------------------------------------------------------------------------\n"; 
    cout << "--------------------------------------------------------------------------------\n";
    cout << "------------------------Super Blockbreaker Settings-----------------------------\n";
    cout << "--------------------------------------------------------------------------------\n";
    cout << "--------------------------------------------------------------------------------\n";
    cout << "--------------------------------------------------------------------------------\n";
    cout << "--------------------------------------------------------------------------------\n\n";
}
#endif
