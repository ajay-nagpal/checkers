#include"def.hpp"
#include<iostream>
#include<vector>

using namespace std;

const vector<int> king_dir{-9,-11,11,9};

int square_attacked(const int sq,const int side, const s_board *pos){// sq is attacked , which side is attacking

    ASSERT(sq_on_board(sq));
    ASSERT(side_valid(side));
    ASSERT(check_board(pos));

    int piece=0,index=0,temp_sq=0,dir=0;

    if(side==black){//attacking side is black
        if(piece_color[pos->pieces[sq]]==white &&((pos->pieces[sq-11]==bm  && pos->pieces[sq+11]==emptyy) || (pos->pieces[sq-9]==bm && pos->pieces[sq+9]==emptyy))){
            return true;
        }
    }
    else{// attacking side is white
        if(piece_color[pos->pieces[sq]]==black &&((pos->pieces[sq+9]==wm && pos->pieces[sq-9]==emptyy) || (pos->pieces[sq+11]==wm && pos->pieces[sq-11]==emptyy))){
            return true;
        }
    }
    // kings //attacked by king
    for(index=0;index<king_dir.size();index++){
        piece=pos->pieces[sq+king_dir[index]];
        if(piece!=off_board && is_ki(piece) && piece_color[piece]==side){
            int a=king_dir[index];
            if(a==-9){//-9 pr king
                if(pos->pieces[sq+9]==emptyy){// +9 emptyy ho
                    return true;
                }
            }
            else if(a==-11){
                if(pos->pieces[sq+11]==emptyy){
                    return true;
                }
            }
            else if(a==9){
                if(pos->pieces[sq-9]==emptyy){
                    return true;
                }
            }
            else if(a==11){
                if(pos->pieces[sq-11]==emptyy){
                    return true;
                }
            }
        }
    }
    return false;
}
