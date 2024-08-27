#include<iostream>
#include"def.hpp"
#include<vector>

using namespace std;

#define move(f,t,ca,prom) ((f)|((t)<<7)|(ca<<14)|(prom<<18))

vector<pair<int,int>>kcap_dir={{-9,-18},{-11,-22},{9,18},{11,22}};// white king capture dir

static void add_capture_move(const s_board * pos,int move,s_movelist * list){
    ASSERT(sq_onboard(from_sq(move)));
    ASSERT(sq_onboard(to_sq(move)));
    ASSERT(piece_valid_empty(captured(move)));
    
    list->moves[list->count].move=move;
    list->moves[list->count].score=0;
    list->count++;
}

static void add_white_cap_move(const s_board * pos , const int from, const int to,
                const int cap, s_movelist * list){

    ASSERT(piece_valid_empty(cap));
    ASSERT(sq_onboard(from));
    ASSERT(sq_onboard(to));

    if(rank_board[from]==r3){
        add_capture_move(pos,move(from,to,cap,wk),list);
        cout<<"rank 3 white cap move "<<endl;
    }  
    else{
        add_capture_move(pos,move(from,to,cap,emptyy),list);   
        cout<<"other rank  white cap move"<<endl;
    }                 
}

static void add_white_move(const s_board * pos , const int from, const int to,s_movelist * list){
    
    ASSERT(sq_onboard(from));
    ASSERT(sq_onboard(to));

    if(rank_board[from]==r2){// for prom
        add_capture_move(pos,move(from,to,emptyy,wk),list);
    }    
    else{
        add_capture_move(pos,move(from,to,emptyy,emptyy),list);
    }         
}

static void add_black_cap_move(const s_board * pos , const int from, const int to,
                const int cap, s_movelist * list){

    ASSERT(piece_valid_empty(cap));
    ASSERT(sq_onboard(from));
    ASSERT(sq_onboard(to));

    if(rank_board[from]==r6){
        add_capture_move(pos,move(from,to,cap,bk),list);
        cout<<"rank 6 black cap move "<<endl;

    } 
    else if(rank_board[from]==r7){
        add_capture_move(pos,move(from,to,emptyy,bk),list);
        cout<<"rank 7 black emptyy cap move "<<endl;
    }   
    else{
        add_capture_move(pos,move(from,to,cap,emptyy),list); 
        cout<<"other rank black cap move "<<endl;
    }   
}

static void add_black_move(const s_board * pos , const int from, const int to,s_movelist * list){

    ASSERT(sq_onboard(from));
    ASSERT(sq_onboard(to));

    if(rank_board[from]==r7){
        add_capture_move(pos,move(from,to,emptyy,bk),list);
    }    
    else   
        add_capture_move(pos,move(from,to,emptyy,emptyy),list);          
}

void generate_all_moves(const s_board * pos,s_movelist * list){
    ASSERT(check_board(pos));
    list->count=0;

    int piece=emptyy;
    int side=pos->side;
    int sq=0,pce_num=0;

    int index=0;

    if(side==white){ // if the side to move is white
        for(pce_num=0;pce_num<pos->piece_num[wm];pce_num++){// loop through all white men on the board
            sq=pos->piece_list[wm][pce_num];// which sq it is on

            ASSERT(sq_onboard(sq));
            if(sq_onboard(sq-9) && pos->pieces[sq-9]==emptyy){
                add_white_move(pos,sq,sq-9,list);
            }
            if(sq_onboard(sq-11) && pos->pieces[sq-11]==emptyy){
                add_white_move(pos,sq,sq-11,list);
            }

            if((sq_onboard(sq-9) && sq_onboard(sq-18))  && (piece_color[pos->pieces[sq-9]]==black && pos->pieces[sq-18]==emptyy)){// white bllack ko acptured krega
                add_white_cap_move(pos,sq,sq-18,pos->pieces[sq-9],list);
            }
            if((sq_onboard(sq-11) && sq_onboard(sq-22))  && (piece_color[pos->pieces[sq-11]]==black && pos->pieces[sq-22]==emptyy)){
                add_white_cap_move(pos,sq,sq-22,pos->pieces[sq-11],list);
            }
        }

        for(pce_num=0;pce_num<pos->piece_num[wk];pce_num++){// loop through all white kings on the board
            sq=pos->piece_list[wk][pce_num];// which sq it is on
            ASSERT(sq_onboard(sq));

            for(int i=0;i<4;i++){
                int to=sq+kcap_dir[i].first;
                if( pos->pieces[to]==emptyy){
                    add_white_move(pos,sq,sq+kcap_dir[i].first,list);
                }
            }


            // code using pair {sq,emp_sq} with single while loop
            // if((sq_onboard(sq-9) && sq_onboard(sq-18))  && (piece_color[pos->pieces[sq-9]]==black && pos->pieces[sq-18]==emptyy)){// white bllack ko acptured krega
            //     add_white_cap_move(pos,sq,sq-18,pos->pieces[sq-9],list);
            // }
            // if((sq_onboard(sq-11) && sq_onboard(sq-22))  && (piece_color[pos->pieces[sq-11]]==black && pos->pieces[sq-22]==emptyy)){
            //     add_white_cap_move(pos,sq,sq-22,pos->pieces[sq-11],list);
            // }
            // if((sq_onboard(sq+9) && sq_onboard(sq+18))  && (piece_color[pos->pieces[sq+9]]==black && pos->pieces[sq+18]==emptyy)){// white bllack ko acptured krega
            //     add_white_cap_move(pos,sq,sq+18,pos->pieces[sq+9],list);
            // }
            // if((sq_onboard(sq+11) && sq_onboard(sq+22))  && (piece_color[pos->pieces[sq+11]]==black && pos->pieces[sq+22]==emptyy)){
            //     add_white_cap_move(pos,sq,sq+22,pos->pieces[sq+11],list);
            // }

            //create an array of pairs for storing dir and emp_sq 

            // bn gya ye bhi single captured tk work krega ise aage ke capture bhi krne
            //chiye to use while instead of  if
            for(int i=0;i<4;i++){
                if((sq_onboard(sq+kcap_dir[i].first) && sq_onboard(sq+kcap_dir[i].second))  && (piece_color[pos->pieces[sq+kcap_dir[i].first]]==black && pos->pieces[sq+kcap_dir[i].second]==emptyy)){// white bllack ko acptured krega
                    add_white_cap_move(pos,sq,sq+kcap_dir[i].second,pos->pieces[sq+kcap_dir[i].first],list);
                }
            }
        }
    }
    else{
        for(pce_num=0;pce_num<pos->piece_num[bm];pce_num++){// loop through all black men on the board
            sq=pos->piece_list[bm][pce_num];// which sq it is on
            ASSERT(sq_onboard(sq));
            if(sq_onboard(sq+9) &&pos->pieces[sq+9]==emptyy){
                add_black_move(pos,sq,sq+9,list);
            }
            if(sq_onboard(sq+11) && pos->pieces[sq+11]==emptyy){
                add_black_move(pos,sq,sq+11,list);
            }
            if((sq_onboard(sq+9) && sq_onboard(sq+18))  && (piece_color[pos->pieces[sq+9]]==white && pos->pieces[sq+18]==emptyy)){
                add_black_cap_move(pos,sq,sq+18,pos->pieces[sq+9],list);
            }
            if((sq_onboard(sq+11) && sq_onboard(sq+22))  && (piece_color[pos->pieces[sq+11]]==white && pos->pieces[sq+22]==emptyy)){
                add_black_cap_move(pos,sq,sq+22,pos->pieces[sq+11],list);
            }
        }

        for(pce_num=0;pce_num<pos->piece_num[bk];pce_num++){// loop through all black kings on the board
            sq=pos->piece_list[bk][pce_num];// which sq it is on
            ASSERT(sq_onboard(sq));
            
            for(int i=0;i<4;i++){
                int a=sq+kcap_dir[i].first;
                if( sq_onboard(a) && pos->pieces[a]==emptyy){// white bllack ko acptured krega
                    add_black_move(pos,sq,a,list);
                }
            }

            // if((sq_onboard(sq+9) && sq_onboard(sq+18))  && (piece_color[pos->pieces[sq+9]]==white && pos->pieces[sq+18]==emptyy)){
            //     add_black_cap_move(pos,sq,sq+18,pos->pieces[sq+9],list);
            // }
            // if((sq_onboard(sq+11) && sq_onboard(sq+22))  && (piece_color[pos->pieces[sq+11]]==white && pos->pieces[sq+22]==emptyy)){
            //     add_black_cap_move(pos,sq,sq+22,pos->pieces[sq+11],list);
            // }
            // if((sq_onboard(sq-9) && sq_onboard(sq-18))  && (piece_color[pos->pieces[sq-9]]==white && pos->pieces[sq-18]==emptyy)){
            //     add_black_cap_move(pos,sq,sq-18,pos->pieces[sq-9],list);
            // }
            // if((sq_onboard(sq-11) && sq_onboard(sq-22))  && (piece_color[pos->pieces[sq-11]]==white && pos->pieces[sq-22]==emptyy)){
            //     add_black_cap_move(pos,sq,sq-22,pos->pieces[sq-11],list);
            // }

            //same for black attacking
            for(int i=0;i<4;i++){
                if((sq_onboard(sq+kcap_dir[i].first) && sq_onboard(sq+kcap_dir[i].second))  && (piece_color[pos->pieces[sq+kcap_dir[i].first]]==white && pos->pieces[sq+kcap_dir[i].second]==emptyy)){// white bllack ko acptured krega
                    add_black_cap_move(pos,sq,sq+kcap_dir[i].second,pos->pieces[sq+kcap_dir[i].first],list);
                }
            }
        }
    }
}
