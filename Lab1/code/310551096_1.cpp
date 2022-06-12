#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <fstream>
using namespace std;

vector<int> win;
class AI{
private:
    int row = 6;
    int col = 3;
    bool gameover = false;
    vector< vector<int> > board;

    bool stable = true;
    int step = 0;
    int turn = 0;
    int mypts = 0;
    int oppopts = 0;
    //int limit = 3;

public:
    AI(){
        
        board = vector< vector<int> >(row, vector<int>(col, 0));
        ifstream input_file;
        input_file.open("board.txt");
        int number;
        for(auto &r:board)
            for(auto &c:r){
                input_file>>number;
                c=(number);
            }
    }

    ~AI(){
        for (auto v : board) v.clear();
        board.clear();
    }

    bool checkstable(){
        for (int i=0 ; i<row;i++)
            for(int j=0 ; j<(col-2) ; j++)
                if (board[i][j] == board[i][j+1] == board[i][j+2] != 0){
                    stable = false;
                    return false;                    
                }
        stable = true;
        return true;
    }

    void drop(){
        for(int j=0 ; j<col ; j++){
            int wp = row-1;
            for(int i= row-1; i>=0; i--){
                if (board[i][j]>0){
                    board[wp][j] = board[i][j];
                    wp-=1;
                }
            }
            for(int k=wp; k>=0; k--)
                board[k][j] = 0;
        }
    }

    int last(){
        for(int i=0 ; i<col ; i++){
            for(int j=row-1 ; j>=0 ; j--){
                if(!board[j][i] && row-j==2)
                    return i;
            }
        }
        return -1;
    }


    bool checkgameover(){ //any of last row == 0
        for(auto v :board[row-1])
            if (v == 0){
                gameover = true;
                break;
            }
        return gameover;
    }

    int clean(){
        bool unstable = true;
        int pts = 0;
        while(unstable){
            unstable = false;
            for(int i=0; i<row; i++){
                for(int j=0; j<col-2; j++){
                    if (abs(board[i][j]) == abs(board[i][j+1]) && abs(board[i][j+1])==abs(board[i][j+2]) && abs(board[i][j+2]) != 0){
                        board[i][j] = board[i][j+1] = board[i][j+2] = -abs(board[i][j]);
                        unstable = true;
                    }
                }
            }
            for(int i = 0 ; i <row; i++){
                for(int j = 0; j<col; j++){
                    int p = board[i][j];
                    if (p<0){
                        pts-=p;
                        board[i][j] = 0;
                    }                        
                }
            }
            drop();     
        }
        checkgameover();
        return pts;
    }

    /*int minimax2(int depth, int score){
        vector<vector<int>> tmp = board;
        if(gameover)
            return score;

        if(!(depth%2)){
            int best = -1000;
            for(int i=0; i<row; i++){
                for(int j=0; j<col; j++){
                    if(board[i][j]){
                        vector<vector<int>> tmp = board;
                        best = max(best, minimax2(depth+1, score+make_move(i, j)));
                        board = tmp;
                    }
                }
            }
            return best;
        }

        else{
            int best = 1000;
            for(int i=0; i<row; i++){
                for(int j=0; j<col; j++){
                    if(board[i][j]){
                        vector<vector<int>> tmp = board;
                        best = min(best, minimax2(depth+1, score+make_move(i, j)));
                        board = tmp;
                    }
                }
            }
            return best;
        }

    }
    
    pair<int, int> make_decision2(){
        
        #######################################################
        ##### This is the main part you need to implement #####
        #######################################################   

        int best = -1000, x, y;
        
        for(int i=0; i<row; i++){
            for(int j=0; j<col; j++){
                if(board[i][j]){
                        vector<vector<int>> tmp = board;
                        int curr = minimax2(0, make_move(i, j));
                        //cout<<curr;
                        gameover = false;
                        board = tmp;

                        if(curr > best){
                            x = i;
                            y = j;
                            best = curr;
                        }
                }
            }
        }
        return { x , y };
        // return format : {x,y}
        // Use AI to make decision !
        // random is only for testing !
    }*/

    pair<int, int> minimax(int depth, int score, int alpha, int beta) {
        //if(depth > limit)
            //return {depth, score};
        if(depth%2){
            pair<int, int> best{1000, -1000};
            for(int j=0; j<col; j++){
                for(int i=row-1; i>=0; i--){
                    if(board[i][j]){
                        vector<vector<int>> tmp = board;
                        int temp = make_move(i, j);
                        if(gameover){
                            gameover = false;
                            return {depth, score+temp};
                        }
                        pair<int,int> curr = minimax(depth+1, score+temp, alpha, beta);

                        board = tmp;
                        if(curr.second > best.second)
                            best = curr;
                        
                        else if(curr.second == best.second){
                            if(curr.first < best.first)
                                best = curr;
                        }

                        alpha = max(best.second, alpha);
                        if(beta <= alpha){
                            //cout<<"Pruned..."<<endl;
                            return best;
                        }
                    }
                    else    
                        break;
                }
            }
            return best;
        }

        else{
            pair<int, int> best{1000, 1000};
            for(int j=0; j<col; j++){
                for(int i=row-1; i>=0; i--){
                    if(board[i][j]){
                        vector<vector<int>> tmp = board;
                        int temp = make_move(i, j);
                        if(gameover){
                            gameover = false;
                            return {depth, score+temp};
                        }
                        pair<int,int> curr = minimax(depth+1, score-temp, alpha, beta);
                        board = tmp;

                        if(curr.second < best.second)
                            best = curr;
                        
                        else if(curr.second == best.second){
                            if(curr.first > best.first)
                                best = curr;
                        }

                        beta = min(best.second, beta);
                        if(beta <= alpha){
                            //cout<<"Pruned..."<<endl;
                            return best;
                        }
                    }
                    else    
                        break;
                }
            }
            return best;
        }

    }
    
    pair<int, int> make_decision(){
        /*
        #######################################################
        ##### This is the main part you need to implement #####
        #######################################################   */

        pair<int, int> best{1000, -1000};
        int x, y;
        int alpha=-1000, beta=1000;

        /*vector<vector<int>> tmp = board;
        if(last()>=0){
            int temp = oppopts-mypts+make_move(row-1, last());
            cout<<"temp"<<temp<<endl;
            board = tmp;

            if(temp>0){
                cout<<"Success"<<endl;
                return {row-1, last()};
            }
        }*/

        for(int j=0; j<col; j++){
            for(int i=row-1; i>=0; i--){
                if(board[i][j]){
                    vector<vector<int>> tmp = board;
                    int temp = make_move(i, j);
                    if(gameover){
                        gameover = false;
                        board = tmp;
                        //cout<<"chance:"<<mypts-oppopts+temp<<endl;
                        if(mypts-oppopts+temp > 0){
                            //cout<<"Success"<<endl;
                            return { i , j};
                        }
                        else
                            continue;
                    }
                    pair<int, int> curr = minimax(0, mypts-oppopts+temp, alpha, beta);
                    //cout<<curr;
                    gameover = false;
                    board = tmp;

                    if(curr.second > best.second){
                        x = i;
                        y = j; 
                        best = curr;
                    }
                    
                    else if(curr.second == best.second){
                        if(curr.first < best.first){
                            x = i;
                            y = j;
                            best = curr;
                        }
                    }
                    
                    alpha = max(best.second, alpha);
                    if(beta <= alpha)
                        return {x, y};
                }
                else
                    break;
            }
        }
        return { x , y };
        // return format : {x,y}
        // Use AI to make decision !
        // random is only for testing !
    }

    pair<int, int> rand_select(){
        int p = 0;
        int x ;
        int y ;
        while (!p){
            x = rand()%row;
            y = rand()%col;
            p = board[x][y];
        }
        return { x , y };
    }    

    int make_move(int x, int y){
        int pts = board[x][y];
        board[x][y]= 0;
        drop();
        if (checkgameover())
            return pts;
        pts+= clean();
        return pts;
    }

    int start(){
		cout<<"Game start!"<<endl;
		pair<int, int> xy;
        int pts;
        show_board();
        cout<<"Set the player's order(0:first, 1:second): ";
        cin>>turn;
        if (turn!=0 && turn!=1){
            cout<<"wrong order";
            exit(0);
        }
            

		while(!gameover){
            cout<<"-------------------------"<< endl;
			cout<<"Turn"<<step<<endl;
            if((step%2) == turn){
                cout<<"It\'s your turn"<<endl;
                xy = make_decision();
                cout << "your move is :"<<xy.first<<","<<xy.second << endl;
                if(board[xy.first][xy.second]==0){
                    cout<<"illegal move";
                    exit(0);
                }
			    pts = make_move(xy.first,xy.second);
                mypts += pts;
                cout<<"You get "<<pts<<" points"<< endl;
                show_board();
            } 
			else{
                cout<<"It\'s opponent\'s turn"<<endl;
                cout<<"Your opponent move is ";
                cin>>xy.first>>xy.second; //open this line when you submit

                //xy = make_decision2();
                //xy = rand_select(); //can use this while testing ,close this line when you submit

                cout <<"oppo move is:"<<xy.first<<","<<xy.second << endl;
                if(board[xy.first][xy.second]==0){
                    cout<<"illegal move";
                    exit(0);
                }
			    pts = make_move(xy.first,xy.second);
                oppopts += pts;
                cout<<"Your opponent get "<<pts<<" points"<< endl;
                show_board();
            }
        step++;
		}
        //win.push_back(mypts-oppopts);
        //gameover
        if(mypts > oppopts){
            cout<<"You win!"<<endl;
            return 1;
        }
        else if(mypts < oppopts){
            cout<<"You lose!"<<endl;
            return -1;
        }
        else{
            cout<<"Tie!"<<endl;
            return 0;
        }
	}

    void show_board(){
        cout<<"my points:"<< mypts << endl;
        cout<<"opponent\'s points: "<< oppopts <<endl;
        cout<<"The board is : "<<endl;
        for(int i = 0; i< row; i++){
            for(int j = 0;j < col; j++){
                cout<<board[i][j];
            }
            cout << endl;
        }
        cout<<"-------------------------"<< endl;
    }    
};
int main(){
    srand( time(NULL));

    //int n=10;
    AI game;
    game.start();
    
    // system("pause");
    return 0;
}
