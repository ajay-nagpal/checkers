#include<iostream>
#include"def.hpp"
#include<vector>

using namespace std;

#define move(f,t,ca,pro,flag) ((f)|((t)<<7)|(ca<<14)|(pro<<19)|(flag))
#define square_offboard(sq) (file_board[(sq)]==off_board)

static void add_quite_move(const s_board * pos,int move,s_movelist * list){
    ASSERT(sq_on_board(from_sq(move)));
    ASSERT(sq_on_board(to_sq(move)));
    
    list->moves[list->count].move=move;
    list->moves[list->count].score=0;
    list->count++;
}

static void add_capture_move(const s_board * pos,int move,s_movelist * list){
    ASSERT(sq_on_board(from_sq(move)));
    ASSERT(sq_on_board(to_sq(move)));
    ASSERT(piece_valid_empty(captured(move)));
    
    list->moves[list->count].move=move;
    list->moves[list->count].score=0;
    list->count++;
}

static void add_white_men_cap_move(const s_board * pos , const int from, const int to,
                const int cap, s_movelist * list){

    ASSERT(piece_valid_empty(cap));
    ASSERT(sq_on_board(from));
    ASSERT(sq_on_board(to));

    if(rank_board[from]==r2){
        add_capture_move(pos,move(from,to,cap,wk,0),list);
    }    
    else   
        add_capture_move(pos,move(from,to,cap,emptyy,0),list);          
}

static void add_white_men_move(const s_board * pos , const int from, const int to,s_movelist * list){
    
    ASSERT(sq_on_board(from));
    ASSERT(sq_on_board(to));

    if(rank_board[from]==r2){
        add_capture_move(pos,move(from,to,emptyy,wk,0),list);
    }    
    else{
        add_capture_move(pos,move(from,to,emptyy,emptyy,0),list);
    }         
}

static void add_black_men_cap_move(const s_board * pos , const int from, const int to,
                const int cap, s_movelist * list){

    ASSERT(piece_valid_empty(cap));
    ASSERT(sq_on_board(from));
    ASSERT(sq_on_board(to));

    if(rank_board[from]==r7){
        add_capture_move(pos,move(from,to,cap,bk,0),list);
    }    
    else   
        add_capture_move(pos,move(from,to,cap,emptyy,0),list);          
}

static void add_black_men_move(const s_board * pos , const int from, const int to,s_movelist * list){

    ASSERT(sq_on_board(from));
    ASSERT(sq_on_board(to));

    if(rank_board[from]==r7){
        add_capture_move(pos,move(from,to,emptyy,bk,0),list);
    }    
    else   
        add_capture_move(pos,move(from,to,emptyy,emptyy,0),list);          
}

void generate_all_moves(const s_board * pos,s_movelist * list){
    ASSERT(check_board(pos));
    list->count=0;

    int piece=emptyy;
    int side=pos->side;
    int sq=0,target_sq=0,pce_num=0;

    int dir=0,index=0,piece_index=0;

    if(side==white){ // if the side to move is white
        for(pce_num=0;pce_num<pos->piece_num[wm];pce_num++){// loop through all white mens on the board
            sq=pos->piece_list[wm][pce_num];// which sq it is on
            ASSERT(sq_on_board(sq));
            if(pos->pieces[sq-9]==emptyy){
                add_white_men_move(pos,sq,sq-9,list);
            }
            if(pos->pieces[sq-11]==emptyy){
                add_white_men_move(pos,sq,sq-11,list);
            }
            if((!square_offboard(sq-9) && !square_offboard(sq-18))  && (piece_color[pos->pieces[sq-9]]==black && pos->pieces[sq-18]==emptyy)){// white bllack ko acptured krega
                add_white_men_cap_move(pos,sq,sq-18,pos->pieces[sq-9],list);
            }

            if((!square_offboard(sq-11) && !square_offboard(sq-22))  && (piece_color[pos->pieces[sq-11]]==black && pos->pieces[sq-22]==emptyy)){
                add_white_men_cap_move(pos,sq,sq-22,pos->pieces[sq-11],list);
            }
        }
    }
    else{
        for(pce_num=0;pce_num<pos->piece_num[bm];pce_num++){// loop through all white mens on the board
            sq=pos->piece_list[bm][pce_num];// which sq it is on
            ASSERT(sq_on_board(sq));
            if(pos->pieces[sq+9]==emptyy){
                add_black_men_move(pos,sq,sq+9,list);
            }
            if(pos->pieces[sq+11]==emptyy){
                add_black_men_move(pos,sq,sq+11,list);
            }
            if((!square_offboard(sq+9) && !square_offboard(sq+18))  && (piece_color[pos->pieces[sq+9]]==white && pos->pieces[sq+18]==emptyy)){
                add_black_men_cap_move(pos,sq,sq+18,pos->pieces[sq+9],list);
            }
            if((!square_offboard(sq+11) && !square_offboard(sq+22))  && (piece_color[pos->pieces[sq+11]]==white && pos->pieces[sq+22]==emptyy)){
                add_black_men_cap_move(pos,sq,sq+22,pos->pieces[sq+11],list);
            }
        }
    }
}
