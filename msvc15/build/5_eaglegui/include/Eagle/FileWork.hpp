


#ifndef FileWork_H
#define FileWork_H


#include <string>
#include <cstdio>
#include <iostream>



// Platform neutral getline - reads up unti EOF, \r (OSX), \r\n (WIN), or \n (NIX)
// May only work in BINARY MODE.
std::istream& GetLinePN(std::istream& is , std::string& s);
// Same as above, but ONLY works in BINARY MODE!
FILE* GetLinePN(FILE* f , std::string& s);

void SkipWhiteSpace(FILE* f);
bool ReadTextInteger(FILE* f , int* store);

// ONLY works in BINARY MODE!
int fpeek(FILE* f);




#endif // FileWork_H




