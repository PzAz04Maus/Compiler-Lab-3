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
#include "tokens.h"


// helper: skip tokens until ';' or end-of-input, then consume ';' if found
static void SyncToSemicolon()
{
    int token = PeekToken();
    while (token != 0 && token != ';')
    {
        AdvanceToken();
        token = PeekToken();
    }
    if (token == ';')
        AdvanceToken(); // consume sync token
}

//*******************************************
// Find a PROG non-terminal
// NOTE: this code is not complete
bool FindPROG()
{
    // PROG -> STMTS end
    while (FindSTMT())
    {
        // Keep parsing statements while they exist
    }

    if (PeekToken() != END)
    {
        Error("'end'");
        return false;
    }

    AdvanceToken();
    return true;
}

//*******************************************
// Find a STMTS non-terminal
bool FindSTMTS()
{
    int token = PeekToken();
    if (token == 0)
        return true; // empty statement list is valid

    if (!FindSTMT())
        return false;

    return FindSTMTS();
}

//*******************************************
// Find a STMT non-terminal
// NOTE: this code is not complete
bool FindSTMT()
{
    int token = PeekToken();
    
    if(token == IDENTIFIER )
    {
        AdvanceToken();

        if(PeekToken() != '=')
        {
            Error("=");
            SyncToSemicolon();
            return false;
        }
        AdvanceToken();

        if(!FindEXPR())
        {
            SyncToSemicolon();
            return false;
        }

        if(PeekToken() != ';')
        {
            Error(";");
            SyncToSemicolon();
            return false;
        }
        AdvanceToken();

        std::cout << "Found a statement\n";
        return true;
    }


    {
        if(token=='('|| token == '[' || token == '{' || token == INT_VAL)
        {
            if(!FindEXPR())
            {
                SyncToSemicolon();
                return false;
            }

            if(PeekToken() != ';')
            {
                Error(";");
                SyncToSemicolon();
                return false;
            }
            AdvanceToken();

            std::cout << "Found a statement\n";
            return true;
        }

        Error("Statement");
        SyncToSemicolon();
        return false;
    }

}

bool FindEXPR()
{
    int token = PeekToken();

    // EXPR -> ( EXPR ) EXPR'
    if (token == '(')
    {
        AdvanceToken(); // consume '('

        if (!FindEXPR())
            return false;

        if (PeekToken() != ')')
        {
            Error("')'");
            return false;
        }
        AdvanceToken(); // consume ')'

        // EXPR'
        return FindEXPR_ALPHA();
    }

    // EXPR -> TERM
    if (!FindTERM())
        return false;

    return true;
}

bool FindEXPR_ALPHA()
{
    int token = PeekToken();

    if(token == 0)
    {
        return true; // empty statement is valid
    }

    // EXPR' -> + EXPR
    if (FindPLUSOP())
    {
        if (!FindEXPR())
            return false;
    }

    return true;
}

bool FindTERM()
{
    int token = PeekToken();

    // TERM -> [ EXPR ] TERM'
    if (token == '[')
    {
        AdvanceToken(); // consume '['

        if (!FindEXPR())
            return false;

        if (PeekToken() != ']')
        {
            Error("']'");
            return false;
        }
        AdvanceToken(); // consume ']'

        // TERM'
        return FindTERM_ALPHA();
    }

    // TERM -> INT_VAL
    if (token == INT_VAL)
    {
        AdvanceToken(); // consume INT_VAL
        return true;
    }

    if(token == '{')
    {
        AdvanceToken(); // consume '{'

        if (token != IDENTIFIER)
            return false;

        if (PeekToken() != '}')
        {
            Error("'}'");
            return false;
        }
        AdvanceToken(); // consume '}'

        return true;
    }

    Error("Term");
    return false;
}

bool FindTERM_ALPHA()
{
    int token = PeekToken();

    // empty statement is valid
    if (token == 0)
        return true; 

    if(FindTIMESOP())
    {
        AdvanceToken(); // consume '*'

        if(PeekToken() != '[')
        {
            Error("'['");
            return false;
        }
        AdvanceToken(); // consume '['

        if (!FindTERM())
            return false;

        if(PeekToken() != ']')
        {
            Error("']'");
            return false;
        }
        AdvanceToken(); // consume ']'

        return true;
    }
}

bool FindTIMESOP()
{
    int token = PeekToken();

    if (token == '*')
    {
        AdvanceToken(); // consume '*'
        return true;
    }

    if (token == '/')
    {
        AdvanceToken(); // consume '/'
        return true;
    }

    return false; // not a times operator
}

bool FindPLUSOP()
{
    int token = PeekToken();

    if (token == '+')
    {
        AdvanceToken(); // consume '+'
        return true;
    }

    if (token == '-')
    {
        AdvanceToken(); // consume '-'
        return true;
    }

    return false; // not a plus operator
}