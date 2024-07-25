#include<iostream>
#include"def.hpp"
#include<vector>

using namespace std;

int popbit(u64 & bb){
    int i=0;
    for(i=0;i<64;i++){
        if(bb & (1ULL<<i)){
           bb&=(bb-1);
            break;
        }
    }
    return i;
}

int countbit(u64 bb){
    int i=0,count=0;
    for(i=0;i<64;i++){
        if((bb & (1ULL<<i))){
            count++;
        }
    }
    return count;
}

void print_bit_board(u64 bb){
    u64 shiftme=1ULL;

    int rank=0;
    int file=0;
    int sq=0;
    int sq64=0;

    cout<<endl;

    for(rank=r8;rank>=r1;rank--){
        for(file=fa;file<=fh;file++){
            sq=fr_to_sq(file,rank);

            sq64=SQ64(sq);

            if((shiftme<<sq64)& bb)
                cout<<"x";
            else    
                cout<<"-";
        }
        cout<<endl;
    }
}



