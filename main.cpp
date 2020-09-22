#include<SDL2/SDL.h>
#include<iostream>
#include<string>
#include<vector>
using namespace std;

const int WIDTH = 300;
const int HEIGHT = 300;
SDL_Window* win = NULL;
SDL_Surface* surf = NULL;
SDL_Surface* bg= NULL;

int evaluate(vector <vector<char>> board){
    for(int i=0; i<3; i++){
        if(board[i][0]==board[i][1] and board[i][1]==board[i][2]){
            if(board[i][2]=='X') return 10;
            else if(board[i][2]=='O') return -10;
        }
        if(board[0][i]==board[1][i] and board[1][i]==board[2][i]){
            if(board[2][i]=='X') return 10;
            else if(board[2][i]=='O') return -10;
        }
    }
    if((board[0][0]==board[1][1] and board[1][1]==board[2][2])or(board[0][2]==board[1][1] and board[1][1]==board[2][0])){
        if(board[1][1]=='X') return 10;
        else if(board[1][1]=='O') return -10;
    }
    return 0;
}
bool isNext(vector <vector<char>> board){
    for(int i=0;i<3;i++){
        for(int j=0;j<3;j++){
            if(board[i][j]==' ') return true;
        }
    }
    return false;
}

/* 
    funtion findnext => for finding next best move
    parameters:
    board => current state of board
    isMaxer => true if it is maximizer's move  
*/
vector<int> findNext(vector <vector<char>> board, bool isMaxer){
    //  win, lose, draw base cases
    int eval = evaluate(board);
    if(eval) return {-1, -1, eval};
    if(!isNext(board)) {
        return {-1, -1, 0};    
    }

    //  minimax logic
    if(isMaxer){
        vector<int> score = {-1, -1, -20};
        for(int i=0; i<3; i++){
            for(int j=0; j<3; j++){
                if(board[i][j]==' '){
                    board[i][j]='X';
                    vector<int> tem = findNext(board, false);
                    if(score[2]<tem[2]){
                        score[0]=i;
                        score[1]=j;
                        score[2]=tem[2];
                    }
                    board[i][j]=' ';
                }
            }
        }
        return score;
    }else{
        vector<int> score = {-1, -1, 20};
        for(int i=0; i<3; i++){
            for(int j=0; j<3; j++){
                if(board[i][j]==' '){
                    board[i][j]='O';
                    vector<int> tem = findNext(board, true);
                    if(score[2]>tem[2]){
                        score[0]=i;
                        score[1]=j;
                        score[2]=tem[2];
                    }
                    board[i][j]=' ';
                }
            }
        }
        return score;
    }
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

int main(int argc, char* args[]){
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
                if(board[y/100][x/100]!=' ') {cout<<"Already filled! Try Again!!";}
                else {
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
    SDL_Delay(2000);
    close();
    return 0;
}

