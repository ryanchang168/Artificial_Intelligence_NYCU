#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <fstream>
using namespace std;

class AI{
private:
    int row = 10;
    int col = 5;
    bool gameover = false;
    vector< vector<int> > board;

    bool stable = true;
    int step = 0;
    int turn = 0;
    int mypts = 0;
    int oppopts = 0;
    int limit=-1;
public:
    AI(){
        
        board = vector< vector<int> >(row, vector<int>(col, 0));
        ifstream input_file;
        input_file.open("board2.txt");

        /* ********* testing part ***********
        int number;
        for(auto &r:board)
            for(auto &c:r)
                c = rand()%4 + 1;

        make_stable();
        /// ********* testing part ***********
*/
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

    void make_stable(){
        for (int i=0 ; i<row;i++)
            for(int j=0 ; j<(col-2) ; j++)
                if (board[i][j] == board[i][j+1] == board[i][j+2] != 0){
                    board[i][j] = (board[i][j]+1)%4 + 1;
                }
    }

    int ratio(){
        int count = 0;
        for(int j=0; j<col; j++){
            for(int i=row-1; i>=0; i--){
                if(board[i][j])
                    count++;
                else
                    break;
            }
        }
        return 50/count-1;
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
    
    pair<int, int> minimax(int depth, int score, int alpha, int beta) {
        if(depth > limit)
            return {depth, score};
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

                        for(int l=0; l<col; l++){
                            for(int k=row-1; k>=0; k--){
                                if(board[k][l]){
                                    if(i==k&&j==l)
                                        continue;
                                    int temp2 = make_move(k, l);
                                    if(gameover){
                                        gameover = false;
                                        return {depth, score+temp+temp2};
                                    }

                                    pair<int,int> curr = minimax(depth+1, score+temp+temp2, alpha, beta);
                                    board = tmp;

                                    if(curr.second > best.second)
                                        best = curr;
                                    
                                    else if(curr.second == best.second){
                                        if(curr.first < best.first)
                                            best = curr;
                                    }

                                    // alpha beta pruning
                                    alpha = max(best.second, alpha);
                                    if(beta <= alpha)
                                        return best; 
                                }
                                else
                                    break;
                            }
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
                            return {depth, score-temp};
                        }

                        for(int l=0; l<col; l++){
                            for(int k=row-1; k>=0; k--){
                                if(board[k][l]){
                                    if(i==k&&j==l)
                                        continue;
                                    int temp2 = make_move(k, l);
                                    if(gameover){
                                        gameover = false;
                                        return {depth, score-temp-temp2};
                                    }

                                    pair<int,int> curr = minimax(depth+1, score-temp-temp2, alpha, beta);
                                    board = tmp;

                                    if(curr.second < best.second)
                                        best = curr;
                                    
                                    else if(curr.second == best.second){
                                        if(curr.first > best.first)
                                            best = curr;
                                    }

                                    // alpha beta pruning
                                    beta = min(best.second, beta);
                                    if(beta <= alpha)
                                        return best;
                                }
                                else
                                    break;
                            }
                        }
                    }
                    else    
                        break;
                }
            }
            return best;
        }

    }
    
    vector<pair<int, int>> make_decision(){
        /*
        #######################################################
        ##### This is the main part you need to implement #####
        #######################################################   */

        pair<int, int> best{1000, -1000};
        int x1=0, x2=0, y1=0, y2=0;
        int alpha = -1000, beta = 1000;

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
                    //cout<<temp<<' ';
                    if(gameover){
                        gameover = false;
                        board = tmp;
                        //cout<<"chance:"<<mypts-oppopts+temp<<i<<' '<<j<<endl;
                        if(mypts-oppopts+temp > 0){
                            //cout<<"Success"<<endl;
                            return {{i, j}, {-1, -1}};
                        }
                        else
                            continue;
                    }

                    for(int l=0; l<col; l++){
                        for(int k=row-1; k>=0; k--){
                            if(board[k][l]){
                                if(i==k&&j==l)
                                    continue;
                                int temp2 = make_move(k, l);
                                //cout<<temp2<<' ';
                                if(gameover){
                                    gameover = false;
                                    board = tmp;
                                    //cout<<"chance2:"<<mypts-oppopts+temp+temp2<<' '<<k<<' '<<l<<endl;
                                    if(mypts-oppopts+temp+temp2 > 0){
                                        return {{i, j}, {k, l}};
                                    }
                                    else
                                        continue;
                                }
                               

                                pair<int, int> curr = minimax(0, mypts-oppopts+temp+temp2, alpha, beta);
                                //cout<<curr;
                                gameover = false;
                                board = tmp;

                                if(curr.second > best.second){
                                    x1 = i;
                                    y1 = j;
                                    x2 = k;
                                    y2 = l; 
                                    best = curr;
                                }
                                
                                else if(curr.second == best.second){
                                    if(curr.first < best.first){
                                        x1 = i;
                                        y1 = j;
                                        x2 = k;
                                        y2 = l;
                                        best = curr;
                                    }
                                }

                                alpha = max(best.second, alpha);
                                if(beta <= alpha)
                                    return {{x1, y1}, {x2, y2}};   
                            }
                            else
                                break;
                        }
                    }
                }
                else
                    break;
            }
        }
        return {{x1, y1}, {x2, y2}};
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
		vector<pair<int, int>> xy;
        pair<int, int> xy2;
        int pts;
        show_board();
        cout<<"Set the player's order(0:first, 1:second): ";
        cin>>turn;
        if (turn!=0 && turn!=1){
            cout<<"wrong order";
            exit(0);
        }
            

		while(!gameover){
            if(ratio())
                limit = ratio()*2-1;
            cout<<"-------------------------"<< endl;
			cout<<"Turn"<<step<<endl;
            if(((step%4)/2) == turn){
                cout<<"It's your turn"<<endl;
                /*if(step < 16){
                    xy2 = rand_select();
                    cout << "your move is :"<<xy2.first<<","<<xy2.second << endl;
                    if(board[xy2.first][xy2.second]==0){
                        cout<<"illegal move";
                        exit(0);
                    }
                    pts = make_move(xy2.first,xy2.second);
                    mypts += pts;
                    cout<<"You get "<<pts<<" points"<< endl;
                    show_board();
                }*/
                if(step%2==0){
                    //cout<<"Limit: "<<limit;
                    xy = make_decision();

                    if(board[xy[0].first][xy[0].second]==0){
                        cout<<"Here"<<endl;
                        xy[0] = rand_select();
                    }
                    cout << "your move is :"<<xy[0].first<<","<<xy[0].second << endl;
                    if(board[xy[0].first][xy[0].second]==0){
                        cout<<"illegal move";
                        exit(0);
                    }
                    pts = make_move(xy[0].first,xy[0].second);
                    mypts += pts;
                    cout<<"You get "<<pts<<" points"<< endl;
                    show_board();
                }
                else{
                    cout << "your move is :"<<xy[1].first<<","<<xy[1].second << endl;
                    if(board[xy[1].first][xy[1].second]==0)
                        xy[1] = rand_select();
                    if(board[xy[1].first][xy[1].second]==0){
                        cout<<"illegal move";
                        exit(0);
                    }
                    pts = make_move(xy[1].first,xy[1].second);
                    mypts += pts;
                    cout<<"You get "<<pts<<" points"<< endl;
                    show_board();
                }
            } 
			else{
                cout<<"It\'s opponent\'s turn"<<endl;
                cout<<"Your opponent move is ";
                cin>>xy2.first>>xy2.second; //open this line when you submit

                //xy2 = rand_select(); //can use this while testing ,close this line when you submit

                cout <<"oppo move is:"<<xy2.first<<","<<xy2.second << endl;
                if(board[xy2.first][xy2.second]==0){
                    cout<<"illegal move";
                    exit(0);
                }
			    pts = make_move(xy2.first,xy2.second);
                oppopts += pts;
                cout<<"Your opponent get "<<pts<<" points"<< endl;
                show_board();
            }
        step++;
		}
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

    AI game;
    game.start();
	
    // system("pause");
    return 0;
}
