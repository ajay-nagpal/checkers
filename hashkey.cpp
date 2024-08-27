#include"def.hpp"
#include<iostream>

using namespace std;

u64 generate_pos_key(const s_board *pos){
    int sq=0;
    u64 finalkey=0;
    int piece=emptyy;

    for(sq=0;sq<board_sq_num;sq++){
        piece=pos->pieces[sq];

        if(piece!=no_sq && piece!=emptyy && piece!=off_board){
            ASSERT(piece>=wm && piece<=bk);
            finalkey ^= piece_keys[piece][sq];
        }
    }

    if(pos->side==black)//cz it makes first move in ch white
        finalkey^=side_key;// dono board m alag alag ki chance h to same nhi hogi

    return finalkey;
}
