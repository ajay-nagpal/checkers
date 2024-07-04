#ifndef def_hpp
#define def_hpp

#include<iostream>
#include<vector>
using namespace std;

#define debug

#ifndef debug
#define ASSERT(n)
#else
#define ASSERT(n) \
if(!(n)){\
    cout<<#n<<" - failed ";\
    cout<<" on "<<__DATE__;\
    cout<<" at "<<__TIME__;\
    cout<<" in file "<<__FILE__;\
    cout<<" at line "<<__LINE__;\
    exit(1);\
}
#endif

typedef  unsigned long long u64;

#define NAME "vice 1.0" 

#define board_sq_num 100
#define maxmoves 2048// half moves

#define fr_to_sq(f,r) ((11+(f)) + ((r)*10))


enum piece{emptyy,wp,wk,bp,bk};
enum file{fa,fb,fc,fd,fe,ff,fg,fh,fn};
enum rank{r1,r2,r3,r4,r5,r6,r7,r8,rn};
enum color{white,black,both};

enum board120{
    a1=11,b1,c1,d1,e1,f1,g1,h1,
    a2=21,b2,c2,d2,e2,f2,g2,h2,
    a3=31,b3,c3,d3,e3,f3,g3,h3,
    a4=41,b4,c4,d4,e4,f4,g4,h4,
    a5=51,b5,c5,d5,e5,f5,g5,h5,
    a6=61,b6,c6,d6,e6,f6,g6,h6,
    a7=71,b7,c7,d7,e7,f7,g7,h7,
    a8=81,b8,c8,d8,e8,f8,g8,h8,no_sq,off_board
};

class s_undo{
    public:

    int moves,fiftymove;
    u64 poskey;
};

class s_board {
    public:

    int side,ply,hisply,fiftymove;
    u64 poskey;
    
    vector<int>pieces=vector<int>(board_sq_num);
    vector<u64>pawns=vector<u64>(3);
    vector<int>king_sq=vector<int>(2);

    vector<int>piece_num=vector<int>(5);// num of pieces we have on board

    vector<int>major_pieces=vector<int>(2);
 
    vector<int>material=vector<int>(2);

    vector<s_undo>history=vector<s_undo>(maxmoves);
    vector<vector<int>>piece_list=vector<vector<int>>(5,vector<int>(12));

};

extern vector<int> sq100to64;
extern vector<int> sq64to100;

//init.cpp
extern void all_init();


#endif