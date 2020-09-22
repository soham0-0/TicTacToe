#include<SDL2/SDL.h>
#include<iostream>
#include<string>
using namespace std;

const int WIDTH = 300;
const int HEIGHT = 300;
SDL_Window* win = NULL;
SDL_Surface* surf = NULL;
SDL_Surface* img = NULL;

bool init(){
    bool success = true;
    if(SDL_Init(SDL_INIT_VIDEO)<0){
        success = false;
    }else{
        win = SDL_CreateWindow("TicTacToe", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
        if(win==NULL){
            success=false;
        }else{
            surf = SDL_GetWindowSurface(win);
        }
    }
    return success;
}

bool load(char* str){
    img = SDL_LoadBMP(str);
    if(!img) return false;
    return true;
}

void close(){
    SDL_FreeSurface(img);
    img=NULL;
    SDL_DestroyWindow(win);
    win=NULL;

    SDL_Quit();
}

int main(int argc, char* args[]){

    if(!init()){
        cout<<"Initialization Failed!\n";
    }else{
        if(!load("img/TIC.bmp")){
            cout<<"Failed to load!\n";
        }else{
            SDL_BlitSurface(img, NULL, surf, NULL);
            SDL_UpdateWindowSurface(win);
        }
    }
    bool quit=false;
    SDL_Event e;
    while(!quit){
        while(SDL_PollEvent(&e)!=0){
            int x,y;
            if(e.type==SDL_QUIT) quit=true;
            else if(SDL_GetMouseState(&x,&y)&SDL_BUTTON(1)){
                cout<<x/100<<" "<<y/100<<endl;
            }
        }
    }
    close();
    return 0;
}

