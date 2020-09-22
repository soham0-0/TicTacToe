#include<SDL2/SDL.h>
#include<iostream>
#include<string>
#include<vector>
#include "gameLogic.h"
using namespace std;

const int WIDTH = 300;
const int HEIGHT = 300;
SDL_Window* win = NULL;
SDL_Surface* surf = NULL;
SDL_Surface* bg= NULL;

bool init();                            //  to initialize window
SDL_Surface* load(char* str);           //  to load image surface
pair <int,int> findCoord(int x, int y); //  to convert indices to cartesian coordinates
SDL_Rect fixMove(int i, int j);         //  to set move (X or Y) at index i,j
void close();

int main(int argc, char* args[]){
    //  initializing empty board
    vector <vector<char>> board = {
            {' ', ' ', ' '},
            {' ', ' ', ' '},
            {' ', ' ', ' '}            
    };
    if(!init()){
        cout<<"Initialization Failed!\n";
    }else{
        bg = load("img/TIC.bmp");
        if(!bg){
            cout<<"Failed to load!\n";
        }else{
            SDL_BlitSurface(bg, NULL, surf, NULL);
            SDL_UpdateWindowSurface(win);
        }
    }
    SDL_Surface *Xsurf=load("img/X.bmp"), *Osurf=load("img/O.bmp");
    bool quit=false;
    SDL_Event e;
    while(!quit){
        while(SDL_PollEvent(&e)!=0){
            int x,y;
            if(e.type==SDL_QUIT) quit=true;
            else if(SDL_GetMouseState(&x,&y)&SDL_BUTTON(1)){
                if(board[y/100][x/100]==' ') {
                    board[y/100][x/100] = 'X';
                    SDL_Rect playerRect = fixMove(y/100, x/100);
                    SDL_BlitScaled(Xsurf, NULL, surf, &playerRect);
                    SDL_UpdateWindowSurface(win); 
                    SDL_Delay(500);
                    vector<int> v = findNext(board, false);
                    if(isNext(board))   board[v[0]][v[1]]='O';
                    SDL_Rect pcRect = fixMove(v[0], v[1]);
                    SDL_BlitScaled(Osurf, NULL, surf, &pcRect);
                    SDL_UpdateWindowSurface(win);
                }
                int res = evaluate(board);
                if(res!=0 or !isNext(board)) {
                    quit = true;
                    if(res>0){
                        cout<<"You Win!!\n";
                    }else if(res<0){
                        cout<<"You Lose!!\n";
                    }else{
                        cout<<"Draww!!\n";
                    }
                } 
            }
        }
    }
    SDL_Delay(1000);
    close();
    return 0;
}

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

SDL_Surface* load(char* str){
    SDL_Surface* img = NULL;
    img = SDL_LoadBMP(str);
    return img;
}

pair <int,int> findCoord(int x, int y){
    return {(y)*100+10, (x)*100+10};
}

SDL_Rect fixMove(int i, int j){
    pair<int, int> coord = findCoord(i,j);
    SDL_Rect stretchRect;
    stretchRect.x = coord.first;
    stretchRect.y = coord.second;
    stretchRect.w = 80;
    stretchRect.h = 80;
    return stretchRect;
}

void close(){
    SDL_FreeSurface(bg);
    bg=NULL;
    SDL_DestroyWindow(win);
    win=NULL;

    SDL_Quit();
}
