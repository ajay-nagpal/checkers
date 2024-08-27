#include"def.hpp"
#include <fstream>
#include<iostream>
using namespace std;

int is_sq_100(const int sq) {
	return (sq>=0 && sq<100);
}

int piece_valid_empty_off(const int pce) {
	return (piece_valid_empty(pce) || pce == off_board);
}

int  sq_onboard(const int sq){
    return ((file_board[sq]!=off_board) || ((sq>10 && sq<89) && (sq>0)));// boundry offboard h
}

int side_valid(const int side){
    return (side==white|| side ==black)?1:0;
}

int file_rank_valid(const int fr){
    return (fr>=0 && fr<=7)?1:0;// fr>=0 means file fa >=0 and rank >=1  and so on
}

int piece_valid_empty(const int piece){
    return (piece>=emptyy && piece<=bk)?1:0;
}

int piece_valid(const int piece){
    return (piece>=wm && piece<=bk)?1:0;
}
