#include"def.hpp"
#include<iostream>
#include<vector>

using namespace std;

#define hash_piece(piece,sq) (pos->poskey ^= (piece_keys[(piece)][(sq)]))
#define hash_side            (pos->poskey ^= (side_key))

static void clear_piece(const int sq, s_board * pos){//clear piece from board

    ASSERT(sq_onboard(sq));
    int piece=pos->pieces[sq];
    ASSERT(piece_valid(piece));

    int color=piece_color[piece];
    int index=0,t_piece_num=-1;

    hash_piece(piece,sq);// hash out old piece

    pos->pieces[sq]=emptyy;
    pos->material[color]-=piece_value[piece];
   
    if(piece_men[pos->pieces[sq]]){
        clearbit(pos->men[color],SQ64(sq));
        clearbit(pos->men[both],SQ64(sq));
    }
    else if(piece_king[pos->pieces[sq]]){
        clearbit(pos->king[color],SQ64(sq));
        clearbit(pos->king[both],SQ64(sq));
    }
    
    for(index=0;index<pos->piece_num[piece];index++){
        if(pos->piece_list[piece][index]==sq){
            t_piece_num=index;
            break;
        }
    }
    ASSERT(t_piece_num!=-1);

    pos->piece_num[piece]--;
    pos->piece_list[piece][t_piece_num]=pos->piece_list[piece][(pos->piece_num[piece])];
}

static void add_piece(const int sq,s_board * pos, const int piece){
    ASSERT(piece_valid(piece));
    ASSERT(sq_onboard(sq));

    int color=piece_color[piece];//color of the piece that we are adding 
    hash_piece(piece,sq);//hash in

    pos->pieces[sq]=piece;//add that piece in piece array on that square

    if(piece_men[piece]){
        setbit(pos->men[color],SQ64(sq));
        setbit(pos->men[both],SQ64(sq));
    }
    else if(piece_king[piece]){
        setbit(pos->king[color],SQ64(sq));
        setbit(pos->king[both],SQ64(sq));
    }
    
    pos->material[color]+=piece_value[piece];//add material value of that piece us material m
    pos->piece_list[piece][pos->piece_num[piece]++]=sq;// tehn add this piece to a piece list and increment the number of pieces
}

// move piece function
static void move_piece(const int from, const int to, s_board * pos){
    ASSERT(sq_onboard(from));
    ASSERT(sq_onboard(to));

    int piece=pos->pieces[from];// from ke piece konsa h and color konsa h

    clear_piece(from,pos);
    add_piece(to,pos,piece);
}

// make move function
void make_move(s_board * pos,int move){
    
    ASSERT(check_board(pos));

    int from=from_sq(move);
    int to=to_sq(move);
    int side=pos->side;

    ASSERT(sq_onboard(from));
    ASSERT(sq_onboard(to));

    ASSERT(side_valid(side));
    ASSERT(piece_valid(pos->pieces[from]));
    
    pos->history[pos->hisply].poskey=pos->poskey;// store the hash key in history array
    // then we store information history array
    pos->history[pos->hisply].moves=move;// store the move that was made

    int cap=captured(move);
    int cap_sq;
    if(cap!=emptyy){// cap piece bta rha
        ASSERT(piece_valid(cap));
        // cap ka sq  bhi clear krna h
        if(piece_color[cap]==black){
            if(to==from-18){
                cap_sq=from-9;
            }
            else    
                cap_sq=from-11;
        }
        else{
            if(to==from+18){
                cap_sq=from+9;
            }
            else    
                cap_sq=from+11;
        }
        clear_piece(cap_sq,pos);
    }
    pos->hisply++;// used for indexing our history array
    pos->ply++;//used for search

    // now we have cleared all our pieces that haas been captured off.
    // clear piece aur add piece function bhi shi h 
    // so now we can move our piece on the board
    move_piece(from,to,pos);

    // now check weather we promoted something
    int prom_piece=promoted(move);
    if(prom_piece!=emptyy){
       
        // ASSERT that it is a piece and  also not a men cz we cant be promoting to a men
        ASSERT(piece_valid(prom_piece)  &&  !piece_men[prom_piece]);
        
        clear_piece(to,pos);// clear the piece first and on the to square
        add_piece(to,pos,prom_piece);// add promoted piece
    }

    pos->side^=1;// now change the side
    hash_side;// hash in the side

    // we are done making all our move and changes on baord so ASSERT checkboard again
    ASSERT(check_board(pos));
}

// take move fun
// make move ka opp
void take_move(s_board *pos){

    ASSERT(check_board(pos));
    // decrement hisply and ply cz they were incremented during make move fun
    pos->hisply--;
    pos->ply--;

    // from history array we can get the move that was made  cz we stored it during makemove
    int move=pos->history[pos->hisply].moves;
    //from or to bhi reversed ho gye honge
    // we are going from, to to  from cz we are taking the move back
    int from=from_sq(move);
    int to=to_sq(move);

    ASSERT(sq_onboard(from));
    ASSERT(sq_onboard(to));

    // flip the side and hash the side
    (pos->side)^=1;
    hash_side;

    move_piece(to,from,pos);// now move the piece back means from to sq to from sq pr
    // obviously capture se phle hi likha jaega
    // because we want to move our piece away before we add the captured piece on the to square

    int cap=captured(move);
    if(cap!=emptyy){
        ASSERT(piece_valid(cap));
        add_piece(to,pos,cap);//add the captued piece on the to square
    }
    if(promoted(move)!=emptyy){//if promoted
        ASSERT(piece_valid(promoted(move)) && !piece_men[promoted(move)]);

        clear_piece(from,pos);// clear piece from  from positon
        add_piece(from,pos,((piece_color[promoted(move)]==white) ?wm:bm));// add a pawn 
        // kuki make mpove me promoted piece add kiya tha to yha use clkear krek hm pawn add krnge
    }
    ASSERT(check_board(pos));
}
