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

#define NAME "Jasper 1.0" 

#define board_sq_num 100

#define fr_to_sq(f,r) ((11+(f)) + ((r)*10))// gives 100 based index
#define SQ64(sq100) (sq100to64[(sq100)])
#define SQ100(sq64) (sq64to100[(sq64)])

#define start_fen "1o1o1o1o/o1o1o1o1/1o1o1o1o/8/8/O1O1O1O1/1O1O1O1O/O1O1O1O1 b 0 1"

#define pop(b)  popbit(b)
#define cnt(b)  countbit(b)

#define setbit(bb,sq)   ((bb) |=setmask[(sq)])
#define clearbit(bb,sq) ((bb) &=clearmask[(sq)])

#define is_ki(p) (piece_king[(p)])

#define from_sq(m)     ((m) & 0x7F )
#define to_sq(m)       (((m)>>7) & 0x7F )
#define cap1(m)        (((m)>>14) & 7 )
#define cap2(m)        (((m)>>17) & 0x7F )

#define promoted(m)    (((m)>>18) & 1 )
#define move_flag_cap  0x1C000
#define move_flag_prom 0x20000 

#define maxmoves 1024// half moves
#define max_pos_moves 128// 1 given pos pr max move

enum piece{emptyy,wm,wk,bm,bk};
enum file{fa,fb,fc,fd,fe,ff,fg,fh,fn};
enum rank{r1,r2,r3,r4,r5,r6,r7,r8,rn};
enum color{white,black,both};

enum board100{
    a1=11,b1,c1,d1,e1,f1,g1,h1,
    a2=21,b2,c2,d2,e2,f2,g2,h2,
    a3=31,b3,c3,d3,e3,f3,g3,h3,
    a4=41,b4,c4,d4,e4,f4,g4,h4,
    a5=51,b5,c5,d5,e5,f5,g5,h5,
    a6=61,b6,c6,d6,e6,f6,g6,h6,
    a7=71,b7,c7,d7,e7,f7,g7,h7,
    a8=81,b8,c8,d8,e8,f8,g8,h8,no_sq,off_board
};

class s_move{
    public:
    int move,score;
};

class s_movelist{
    public:
    
    int count;//count of the number of move on the movelist
    vector<s_move>moves=vector<s_move>(max_pos_moves);
};

class s_undo{
    public:

    int moves;
    u64 poskey;
};

class s_board {
    public:
    int side;
    u64 poskey;
    int ply,hisply;
    vector<int>pieces=vector<int>(board_sq_num);//vector<pieces>
    
    vector<u64>men=vector<u64>(3);
    vector<u64>king=vector<u64>(3);

    vector<int>piece_num=vector<int>(5);// num of pieces we have on board

    vector<int>material=vector<int>(2);

    vector<s_undo>history=vector<s_undo>(maxmoves);
    // let piecelist[wm][0]=a1;  lkike this
    vector<vector<int>>piece_list=vector<vector<int>>(5,vector<int>(12));
};

extern vector<int> sq100to64;
extern vector<int> sq64to100;

extern vector<u64> setmask;// 64 size but not used here
extern vector<u64> clearmask;

extern vector<vector<u64>> piece_keys;
extern u64 side_key;

extern string piece_char;
extern string side_char;
extern string rank_char;
extern string file_char;

extern vector<int> piece_value;
extern vector<int> piece_color;
extern vector<bool>piece_men;
extern vector<bool>piece_king;

extern vector<int> file_board;
extern vector<int> rank_board;

//init.cpp
extern void all_init();

//bitboard.cpp
extern void print_bit_board(u64 bb);
extern int popbit(u64 &bb);
extern int countbit(u64 b);

// hashkey.cpp
extern u64 generate_pos_key(const s_board *pos);

//board.cpp
extern void reset_board(s_board *pos);
extern int parse_fen(const char * fen ,s_board * pos);
extern void print_board(const s_board *pos);
extern void update_list_material(s_board *pos);
extern int check_board(const s_board *pos);

// attack.cpp
extern int square_attacked(const int sq,const int side, const s_board *pos);

//validate.cpp
extern int is_sq_100(const int sq);
extern int piece_valid_empty_off(const int pce);
extern int sq_onboard(const int sq);
extern int side_valid(const int side);
extern int file_rank_valid(const int fr);
extern int piece_valid_empty(const int piece);
extern int piece_valid(const int piece);

//io.cpp
extern char * print_move(const int move);
extern char * print_sq(const int sq);
extern void print_move_list(const s_movelist* list);

// movegen.cpp
extern void generate_all_moves(const s_board * pos,s_movelist * list);

//makemove.cpp
extern void make_move(s_board * pos,int move);
extern void take_move(s_board *pos);

#endif