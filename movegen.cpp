#include<iostream>
#include"def.hpp"
#include<vector>

using namespace std;

#define move(f,t,ca,pro,flag) ((f)|((t)<<7)|(ca<<14)|(pro<<19)|(flag))
#define square_offboard(sq) (file_board[(sq)]==off_board)

vector<pair<int,int>>kcap_dir={{-9,-18},{-11,-22},{9,18},{11,22}};// white king capture dir

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

static void add_white_cap_move(const s_board * pos , const int from, const int to,
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

static void add_white_move(const s_board * pos , const int from, const int to,s_movelist * list){
    
    ASSERT(sq_on_board(from));
    ASSERT(sq_on_board(to));

    if(rank_board[from]==r2){
        add_capture_move(pos,move(from,to,emptyy,wk,0),list);
    }    
    else{
        add_capture_move(pos,move(from,to,emptyy,emptyy,0),list);
    }         
}

static void add_black_cap_move(const s_board * pos , const int from, const int to,
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

static void add_black_move(const s_board * pos , const int from, const int to,s_movelist * list){

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
        for(pce_num=0;pce_num<pos->piece_num[wm];pce_num++){// loop through all white men on the board
            sq=pos->piece_list[wm][pce_num];// which sq it is on
            target_sq=sq;

            ASSERT(sq_on_board(sq));
            if(pos->pieces[sq-9]==emptyy){
                add_white_move(pos,sq,sq-9,list);
            }
            if(pos->pieces[sq-11]==emptyy){
                add_white_move(pos,sq,sq-11,list);
            }

            while((!square_offboard(target_sq-9) && !square_offboard(target_sq-18))  && (piece_color[pos->pieces[target_sq-9]]==black && pos->pieces[target_sq-18]==emptyy)){// white bllack ko acptured krega
                add_white_cap_move(pos,target_sq,target_sq-18,pos->pieces[target_sq-9],list);
                target_sq-=18;
            }
            while((!square_offboard(target_sq-11) && !square_offboard(target_sq-22))  && (piece_color[pos->pieces[target_sq-11]]==black && pos->pieces[target_sq-22]==emptyy)){
                add_white_cap_move(pos,target_sq,target_sq-22,pos->pieces[target_sq-11],list);
                target_sq-=22;
            }
        }

        for(pce_num=0;pce_num<pos->piece_num[wk];pce_num++){// loop through all white kings on the board
            sq=pos->piece_list[wk][pce_num];// which sq it is on
            ASSERT(sq_on_board(sq));

            for(int i=0;i<4;i++){
                if( pos->pieces[sq+kcap_dir[i].first]==emptyy){
                    add_white_move(pos,sq,sq+kcap_dir[i].first,list);
                }
            }


            // code using pair {sq,emp_sq} with single while loop
            // if((!square_offboard(sq-9) && !square_offboard(sq-18))  && (piece_color[pos->pieces[sq-9]]==black && pos->pieces[sq-18]==emptyy)){// white bllack ko acptured krega
            //     add_white_cap_move(pos,sq,sq-18,pos->pieces[sq-9],list);
            // }
            // if((!square_offboard(sq-11) && !square_offboard(sq-22))  && (piece_color[pos->pieces[sq-11]]==black && pos->pieces[sq-22]==emptyy)){
            //     add_white_cap_move(pos,sq,sq-22,pos->pieces[sq-11],list);
            // }
            // if((!square_offboard(sq+9) && !square_offboard(sq+18))  && (piece_color[pos->pieces[sq+9]]==black && pos->pieces[sq+18]==emptyy)){// white bllack ko acptured krega
            //     add_white_cap_move(pos,sq,sq+18,pos->pieces[sq+9],list);
            // }
            // if((!square_offboard(sq+11) && !square_offboard(sq+22))  && (piece_color[pos->pieces[sq+11]]==black && pos->pieces[sq+22]==emptyy)){
            //     add_white_cap_move(pos,sq,sq+22,pos->pieces[sq+11],list);
            // }

            //create an array of pairs for storing dir and emp_sq 

            // bn gya ye bhi single captured tk work krega ise aage ke capture bhi krne
            //chiye to use while instead of  if
            // for(int i=0;i<4;i++){
            //     while((!square_offboard(target_sq+kcap_dir[i].first) && !square_offboard(target_sq+kcap_dir[i].second))  && (piece_color[pos->pieces[target_sq+kcap_dir[i].first]]==black && pos->pieces[target_sq+kcap_dir[i].second]==emptyy)){// white bllack ko acptured krega
            //         add_white_cap_move(pos,target_sq,target_sq+kcap_dir[i].second,pos->pieces[target_sq+kcap_dir[i].first],list);
            // isse to shi nhi bnega agla cal baki ke 4 dir pr me se kisi pr bhi ho skta h        
            //target_sq+=kcap_dir[i].second;
            //     }
            //     target_sq=sq;
            // }
        }
    }
    else{
        for(pce_num=0;pce_num<pos->piece_num[bm];pce_num++){// loop through all black men on the board
            sq=pos->piece_list[bm][pce_num];// which sq it is on
            ASSERT(sq_on_board(sq));
            if(pos->pieces[sq+9]==emptyy){
                add_black_move(pos,sq,sq+9,list);
            }
            if(pos->pieces[sq+11]==emptyy){
                add_black_move(pos,sq,sq+11,list);
            }
            if((!square_offboard(sq+9) && !square_offboard(sq+18))  && (piece_color[pos->pieces[sq+9]]==white && pos->pieces[sq+18]==emptyy)){
                add_black_cap_move(pos,sq,sq+18,pos->pieces[sq+9],list);
            }
            if((!square_offboard(sq+11) && !square_offboard(sq+22))  && (piece_color[pos->pieces[sq+11]]==white && pos->pieces[sq+22]==emptyy)){
                add_black_cap_move(pos,sq,sq+22,pos->pieces[sq+11],list);
            }
        }

        for(pce_num=0;pce_num<pos->piece_num[bk];pce_num++){// loop through all black kings on the board
            sq=pos->piece_list[bk][pce_num];// which sq it is on
            ASSERT(sq_on_board(sq));
            
            for(int i=0;i<4;i++){
                if( pos->pieces[sq+kcap_dir[i].first]==emptyy){// white bllack ko acptured krega
                    add_black_move(pos,sq,sq+kcap_dir[i].first,list);
                }
            }

            // if((!square_offboard(sq+9) && !square_offboard(sq+18))  && (piece_color[pos->pieces[sq+9]]==white && pos->pieces[sq+18]==emptyy)){
            //     add_black_cap_move(pos,sq,sq+18,pos->pieces[sq+9],list);
            // }
            // if((!square_offboard(sq+11) && !square_offboard(sq+22))  && (piece_color[pos->pieces[sq+11]]==white && pos->pieces[sq+22]==emptyy)){
            //     add_black_cap_move(pos,sq,sq+22,pos->pieces[sq+11],list);
            // }
            // if((!square_offboard(sq-9) && !square_offboard(sq-18))  && (piece_color[pos->pieces[sq-9]]==white && pos->pieces[sq-18]==emptyy)){
            //     add_black_cap_move(pos,sq,sq-18,pos->pieces[sq-9],list);
            // }
            // if((!square_offboard(sq-11) && !square_offboard(sq-22))  && (piece_color[pos->pieces[sq-11]]==white && pos->pieces[sq-22]==emptyy)){
            //     add_black_cap_move(pos,sq,sq-22,pos->pieces[sq-11],list);
            // }

            //same for black attacking
            for(int i=0;i<4;i++){
                if((!square_offboard(sq+kcap_dir[i].first) && !square_offboard(sq+kcap_dir[i].second))  && (piece_color[pos->pieces[sq+kcap_dir[i].first]]==white && pos->pieces[sq+kcap_dir[i].second]==emptyy)){// white bllack ko acptured krega
                    add_white_cap_move(pos,sq,sq+kcap_dir[i].second,pos->pieces[sq+kcap_dir[i].first],list);
                }
            }
        }
    }
}
