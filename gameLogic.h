#ifndef GAMELOGIC_H
#define GAMELOGIC_H
#include<vector>
using namespace std;
int evaluate(vector <vector<char>> board);
bool isNext(vector <vector<char>> board);
vector<int> findNext(vector <vector<char>> board, bool isMaxer, int alpha, int beta);
#endif 