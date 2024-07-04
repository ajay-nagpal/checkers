#include"def.hpp"
#include<iostream>
#include<vector>

using namespace std;

vector<int>sq100to64(board_sq_num);
vector<int>sq64to100(64);

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

}
    
