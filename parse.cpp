//**************************************
// parse.cpp
//
// Starting point for top-down recursive-descent parser
// Used in CST 320 Lab2
//
// Author: Phil Howard 
// phil.howard@oit.edu
//

#include <iostream>
#include "lex.h"
#include "parse.h"
#include "utils.h"

//*******************************************
// Find a PROG non-terminal
// NOTE: this code is not complete
bool FindPROG()
{
    // if (didn't find program)
    {
        Error("Program");
        return false;
    }
    // else
    {
        return true;
    }

}
//*******************************************
// Find a STMT non-terminal
// NOTE: this code is not complete
bool FindSTMT()
{
    // if (didn't find STMT)
    {
        return false;
    }
    // else
    {
        std::cout << "Found a statement\n";
        return true;
    }

}
/*******************************************
bool FindExample()
{
    if (!FindPART1()) return false;
    
    int token = PeekToken();
    if (token != '+') return false;
    AdvanceToken();         // past '+'

    if (!FindPART2()) return false;

    return true;
}
*/

