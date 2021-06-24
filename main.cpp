#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include<game_world_classes.h>
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>


int main()
{
    int choice;
    do{
        Menu menu;
        choice=menu.loop();
        if(choice == 1){
            int decision;
            do{
                Difficulty choose;
                int mode = choose.loop();
                Scene scene(mode);
                decision = scene.loop();
            }while(decision!=0);
        }
        if(choice == 2){
            Option instruct;
            instruct.loop();
        }
    }while(choice!=3 && choice!=0);


}
