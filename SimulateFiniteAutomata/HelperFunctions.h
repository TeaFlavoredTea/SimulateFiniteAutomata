#pragma once
#include <ctype.h>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <tuple>
#include <iomanip>
#include <sstream>

//changes the output of isprint
// '`' char is nolonger accepted
int my_isprint(char c);

//reads a text file. each line in the file is a string in the vector
std::vector<std::string> getStringsFromFile(std::string fileName, bool includeEmptyLines);

//reads a string formatted: { num0 num1 ... numN}
//returns a vector of integers vect[0] = num0, vect[1] = num1 ... vect[N] = numN
std::vector<int> stringToVector(std::string s);

//reads a string formatted:1,a,b
//returns a tuple of the type int, char, int
std::tuple <int, char, int> stringToTuple(std::string s);

std::string intToString(int num);