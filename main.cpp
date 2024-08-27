#include<iostream>
#include"def.hpp"

using namespace std;

#define fen1 "1o1o1o1o/o1o1o1o1/1o1o1o1o/8/8/O1O1O1O1/1O1O1O1O/O1O1O1O1 b 0 1"
#define fen3 "1o1o1o1o/o1o1o1o1/5o1o/2o1o3/3O1O2/O5O1/1O1O1O1O/O1O1O1O1 b 0 2"
//fen for checking normal caps
#define fen4 "1o5o/O3#3/8/4o1o1/5O2/8/5O2/6*1 b 0 2"

//fen for checking continuous captured
#define fen5 "1o5o/O3#3/8/4o3/5*2/8/5O2/6*1 w 0 2"

int main(){
    all_init();

     // makemove testing

   s_board board[1];
   s_movelist list[1];

   parse_fen(start_fen,board);
   generate_all_moves(board,list);

   int move=0,move_num=0;

   print_board(board);
   getchar();

   for(move_num=0;move_num<list->count;move_num++){// loop through moves
        move=list->moves[move_num].move;
        cout<<endl<<" move "<<move<<endl;

        make_move(board,move);

        cout<<endl<<" made: "<<print_move(move)<<endl;// print the move to the screen
        print_board(board);

     //    take_move(board);
     //    cout<<endl<<" taken: "<<print_move(move)<<endl;
     //    print_board(board);

        getchar();

   }

}
