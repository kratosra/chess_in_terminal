#include "../include/square.h"

#include <iostream>

#include <string>

square::square(string s)
{
    colonne = (int)s[0] - 97;
    ligne = (int)s[1] - 49;
}
square::square(int x, int y)
{
    colonne = y;
    ligne = x;
}

string square::toString()
{
    char S[3];
    S[0] = colonne + 97;
    S[1] = ligne + 49;
    S[2] = '\0';
    return S;
}
