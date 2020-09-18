#include <bits/stdc++.h>
using namespace std;
typedef long long int ll;
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
vector<int> findNext(vector <vector<char>> board, bool isX, int depth){
    int eval = evaluate(board);
    if(eval) return {-1, -1, eval};
    if(!isNext(board)) {
        return {-1, -1, 0};    
    }
    if(isX){
        vector<int> score = {-1, -1, -20};
        for(int i=0; i<3; i++){
            for(int j=0; j<3; j++){
                if(board[i][j]==' '){
                    board[i][j]='X';
                    vector<int> tem = findNext(board, false, depth+1);
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
                    vector<int> tem = findNext(board, true, depth+1);
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

int main() {
    vector <vector<char>> board = {
            {' ', ' ', ' '},
            {' ', ' ', ' '},
            {' ', ' ', ' '}            
    };
    cout<<"Do you want to move first? Press 'F' to move first or press any key to continue:\n";
    char c='!',player='O',pc='X';
    cin>>c;
    bool isX=true;
    if(c!='F'){
        isX=false;
        pc='O';
        board[0][0]=pc;
        player='X';
    }
	while(1){
        int res = evaluate(board);
        if(res!=0 or !isNext(board)) {
            for(int i=0; i<3; i++){
                for(int j=0; j<3; j++){
                    cout<<" "<<board[i][j]<<" ";
                    if(j!=2) cout<<"|";
                }
                if(i!=2) cout<<"\n - | - | - \n";
            }
            cout<<"\n";
            if(res<0){
                cout<<"You Win!!\n";
            }else if(res>0){
                cout<<"You Lose!!\n";
            }else{
                cout<<"Draww!!\n";
            }
            break;
        } 
	    for(int i=0; i<3; i++){
            for(int j=0; j<3; j++){
                cout<<" "<<board[i][j]<<" ";
                if(j!=2) cout<<"|";
            }
            if(i!=2) cout<<"\n - | - | - \n";
        }
        cout<<"\nEnter coordinate of your move in (x,y) format {0-index and no space}: ";
        string coord;
        cin>>coord;
        if(board[coord[1]-'0'][coord[3]-'0']==' '){
            board[coord[1]-'0'][coord[3]-'0']=player;
        }else{
            cout<<"Already Filled! RETRY:\n";
            continue;
        }
	    vector<int> v = findNext(board, isX, 0);
	    if(isNext(board))   board[v[0]][v[1]]=pc;
        cout << "\033[2J\033[1;1H";
	}
	return 0;
}