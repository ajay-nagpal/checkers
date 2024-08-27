#include<iostream>
#include"def.hpp"
#include<vector>

#include <iomanip>// to convert %3d  in c++  by using  set width

using namespace std;

int parse_fen( const char* fen ,s_board * pos){

    ASSERT(fen!=NULL);
    ASSERT(pos!=NULL);

    int rank=r8;
    int file =fa;
    int piece=0;
    int count=0;
    int i=0;

    int sq64=0;
    int sq100=0;

    reset_board(pos);

    while((rank>=r1)  && *fen){
        count=1;
        switch(*fen){
            case 'o': piece=wm;break;
            case 'O': piece=bm;break;
            case '*': piece=wk;break;
            case '#': piece=bk;break;

            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
                    piece=emptyy;
                    count= (*fen)-'0';// count the number of empty squares
                    break;

            case '/':
            case ' ':
                    rank--;
                    file=fa;
                    fen++;
                    continue;

            default:
                    cout<<"fen error "<<endl;
                    return -1;
        }
        
        for(i=0;i<count;i++){
            sq64=(rank * 8)+ file;
            sq100=SQ100(sq64);

            if(piece!=emptyy){
                pos->pieces[sq100]=piece;
            }
            file++;
        }
        fen++;
    }

    ASSERT(*fen=='w' || *fen =='b');

    pos->side=((*fen=='w')?white:black);

    pos->poskey=generate_pos_key(pos);
    update_list_material(pos);

    return 0;
}

void reset_board(s_board *pos){
    int index=0;

    for(index=0;index<board_sq_num;index++)
        pos->pieces[index]=off_board;

    for(index=0;index<64;index++)
        pos->pieces[SQ100(index)]=emptyy;

    for(index=0;index<2;index++){
        pos->material[index]=0;
    }

    for(index=0;index<3;index++){
        pos->men[index]=0ULL;
        pos->king[index]=0ULL;
    }
        
    for(index=0;index<5;index++)
        pos->piece_num[index]=0;
    
    pos->ply=0;// number of half move played in current search
    pos->hisply=0;// half movess played in whole game
    
    pos->side=both;
    pos->poskey=0ULL;
}

void print_board(const s_board *pos){
    int sq=0,file=0,rank=0,piece=0;

    cout<<endl<<"game board:"<<endl<<endl;

    for(rank=r8;rank>=r1;rank--){
        cout<<rank+1<<" ";
        for(file=fa;file<=fh;file++){
            sq=fr_to_sq(file,rank);
            piece=pos->pieces[sq];
            cout<<setw(3)<<piece_char[piece];
        }
        cout<<endl;
    }
    cout<<endl<<"  ";

    for(file=fa;file<=fh;file++){
        cout<<setw(3)<<char('a'+file);
    }
    cout<<endl;

    cout<<"side:"<<side_char[pos->side]<<endl;
    cout<<dec;
    
    cout<<"poskey:"<<setw(11)<<hex<<uppercase<<pos->poskey<<endl;
    cout<<dec;
}

void update_list_material(s_board *pos){
    int piece=0,sq=0,index=0,color=0;

    for(index=0;index<board_sq_num;index++){
        sq=index;
        piece=pos->pieces[index];

        if(piece!=off_board && piece!=emptyy){
            color=piece_color[piece];

            pos->material[color]+=piece_value[piece];
            // let piecelist[wm][0]=a1;  lkike this
            pos->piece_list[piece][pos->piece_num[piece]]=sq;
            pos->piece_num[piece]++;// 0 se 1 ho gya ab

            // to set bits for men

            if(piece==wm){
                setbit(pos->men[white],SQ64(sq));// set the bit on the bitbmard on the position for white men
                // setting a bit in a a64 index format  and sq is in 100 indexing format
                setbit(pos->men[both],SQ64(sq));
            }
            else if(piece==bm){
                setbit(pos->men[black],SQ64(sq));
                setbit(pos->men[both],SQ64(sq));
            }
            else if(piece==wk){
                setbit(pos->king[black],SQ64(sq));
                setbit(pos->king[both],SQ64(sq));
            }
            else if(piece==bk){
                setbit(pos->king[black],SQ64(sq));
                setbit(pos->king[both],SQ64(sq));
            }
        }
    }
}

int check_board(const s_board *pos){
    vector<int> temp_piece_number{0,0,0,0,0};
    vector<int> temp_material{0,0};

    int sq64=0,temp_piece=0,temp_piece_num=0,SQ100=0,color=0,mcount=0;

    vector<u64> temp_men={0ULL,0ULL,0ULL};
    vector<u64> temp_king={0ULL,0ULL,0ULL};

    temp_men[white]=pos->men[white];
    temp_men[black]=pos->men[black];
    temp_men[both]=pos->men[both];

    temp_king[white]=pos->king[white];
    temp_king[black]=pos->king[black];
    temp_king[both]=pos->king[both];

    // check piece list
    // loop on piece type from wm to black king bk
    for(temp_piece=wm;temp_piece<=bk;temp_piece++){
        for(temp_piece_num=0;temp_piece_num<pos->piece_num[temp_piece];temp_piece_num++){
            
            SQ100=pos->piece_list[temp_piece][temp_piece_num];// from our piece list get the square for our piece
            ASSERT(pos->pieces[SQ100]==temp_piece);
        }
    }

    // check piece count and other counterts
    for(sq64=0;sq64<64;sq64++){
        SQ100=SQ100(sq64);

        temp_piece=pos->pieces[SQ100];
        temp_piece_number[temp_piece]++;
        
        color=piece_color[temp_piece];

        temp_material[color]+=piece_value[temp_piece];
    }

    for(temp_piece=wm;temp_piece<=bk;temp_piece++){
        ASSERT(temp_piece_number[temp_piece]==pos->piece_num[temp_piece]);
    }

    mcount=cnt(temp_men[white]);
    ASSERT(mcount==pos->piece_num[wm]);

    mcount=cnt(temp_men[black]);
    ASSERT(mcount==pos->piece_num[bm]);

    mcount=cnt(temp_men[both]);
    ASSERT(mcount==(pos->piece_num[bm]+pos->piece_num[wm]));

    // mcount=cnt(temp_king[white]);
    // ASSERT(mcount==pos->piece_num[wk]);

    // mcount=cnt(temp_king[black]);
    // ASSERT(mcount==pos->piece_num[bk]);

    // mcount=cnt(temp_king[both]);
    // ASSERT(mcount==(pos->piece_num[bk]+pos->piece_num[wk]));

    // check bitboard squares
    while(temp_men[white]){
        sq64=pop(temp_men[white]);//pop removes a bit from a bitboard and returns a
        // 64 based square index we are popping from temp board and 
        ASSERT(pos->pieces[SQ100(sq64)]==wm);// getting 100 based square from that 64 based square
        // and checking from aour pieces array that there is wm bm on that square or not
    }

    while(temp_men[black]){
        sq64=pop(temp_men[black]);
        ASSERT(pos->pieces[SQ100(sq64)]==bm);
    }

    while(temp_men[both]){
        sq64=pop(temp_men[both]);
        ASSERT((pos->pieces[SQ100(sq64)]==bm) || (pos->pieces[SQ100(sq64)]==wm));
    }

    // while(temp_king[white]){
    //     sq64=pop(temp_king[white]);//pop removes a bit from a bitboard and returns a
    //     // 64 based square index we are popping from temp board and 
    //     ASSERT(pos->pieces[SQ100(sq64)]==wk);// getting 100 based square from that 64 based square
    //     // and checking from aour pieces array that there is wk on that square or not
    // }

    // while(temp_king[black]){
    //     sq64=pop(temp_king[black]);
    //     ASSERT(pos->pieces[SQ100(sq64)]==bk);
    // }

    // while(temp_king[both]){
    //     sq64=pop(temp_king[both]);
    //     ASSERT((pos->pieces[SQ100(sq64)]==bk) || (pos->pieces[SQ100(sq64)]==wk));
    // }
    // material,minor,major,big peieces should be same 
    ASSERT(temp_material[white]==pos->material[white]
            && temp_material[black]==pos->material[black]);

    ASSERT(pos->side==white || pos->side==black);
    // position key should be same sas freshly generated key
    ASSERT(generate_pos_key(pos)==pos->poskey);//vid 20 9:00 se

    return true;
}
