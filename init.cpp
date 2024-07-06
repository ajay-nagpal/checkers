#include"def.hpp"
#include<iostream>
#include<vector>

using namespace std;

vector<int>sq100to64(board_sq_num);
vector<int>sq64to100(64);

vector<u64>setmask(64);
vector<u64>clearmask(64);

vector<vector<u64> >piece_keys(5,vector<u64>(100));
u64 side_key;

#define rand64 (    ((u64) rand() )    |\
                    ((u64) rand() << 15) |\
                    ((u64) rand()<<30) |\
                    ((u64) rand()<<45) |\
                    (((u64) rand() & 0xf)<<60)    )


void inithashkey(){
    int index=0;
    int index2=0;

    for(index=0;index<5;index++){
        for(index2=0;index2<100;index2++){

            piece_keys[index][index2]=rand64;
        }
    }
    side_key=rand64;
}

void init_bitmask(){
    int index=0;

    for(index=0;index<64;index++){
        setmask[index]=0ULL;
        clearmask[index]=0ULL;
    }
    for(index=0;index<64;index++){
        setmask[index] |= (1ULL<<index);
        
        clearmask[index]= ~setmask[index];
    }
}

void initsq120to64(){
    int index=0;
    int file=fa;
    int rank=r1;

    int sq=a1;
    int sq64=0;

    for(index=0;index<board_sq_num;index++){
        sq100to64[index]=65;
    }

    for(index=0;index<64;index++){
        sq64to100[index]=101;
    }

    for(rank=r1;rank<=r8;rank++){
        for(file=fa;file<=fh;file++){

            sq=fr_to_sq(file,rank);// curr square
            sq64to100[sq64]=sq;// 64 to 100 at our current square of s64 means 64 based will store that sq
            sq100to64[sq]=sq64;
            sq64++;
        }
    }
}

void all_init(){

    initsq120to64();

    init_bitmask();

    inithashkey();
}
    
