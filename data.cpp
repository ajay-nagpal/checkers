#include<iostream>
#include"def.hpp"
#include<vector>

using namespace std;

string  piece_char=".o*O#";// dot for empty
string  side_char="wb-";
string  rank_char="12345678";
string  file_char="abcdefgh";

vector<int>piece_value{0,10,30,10,30};
vector<int>piece_color{both,white,white,black,black};
vector<bool>piece_men {false, true, false, true, false};
vector<bool>piece_king{false, false, true, false, true};
