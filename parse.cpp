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


// helper: skip tokens until ';', 'end', or end-of-input, then consume ';' if found
static void SyncToSemicolon()
{
    int token = PeekToken();
    while (token != 0 && token != ';' && token != END)
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
    FindSTMTS();

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
    while (true)
    {
        int token = PeekToken();
        if (token == 0 || token == END)
            return true; // empty statement list is valid

        if (!FindSTMT())
        {
            SyncToSemicolon();
            continue;
        }
    }
}

//*******************************************
// Find a STMT non-terminal
// NOTE: this code is not complete
bool FindSTMT()
{
    int token = PeekToken();
    
    // STMT -> identifier = EXPR ;
    if (token == IDENTIFIER)
    {
        AdvanceToken();

        if (PeekToken() != '=')
        {
            Error("'='");
            return false;
        }
        AdvanceToken();

        if (!FindEXPR())
        {
            return false;
        }

        if (PeekToken() != ';')
        {
            Error("';'");
            return false;
        }
        AdvanceToken();

        std::cout << "Found a statement\n";
        return true;
    }

    // STMT -> EXPR ;
    {
        if (token == '(' || token == '[' || token == '{' || token == INT_VAL)
        {
            if (!FindEXPR())
            {
                return false;
            }

            if (PeekToken() != ';')
            {
                Error("';'");
                return false;
            }
            AdvanceToken();

            std::cout << "Found a statement\n";
            return true;
        }

        Error("Statement");
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
    return FindTERM();
}

bool FindEXPR_ALPHA()
{
    // EXPR' -> PLUSOP (EXPR) EXPR' | 
    while (true)
    {
        if (!FindPLUSOP())
            return true;

        if (PeekToken() != '(')
        {
            Error("'('");
            return false;
        }
        AdvanceToken();

        if (!FindEXPR())
            return false;

        if (PeekToken() != ')')
        {
            Error("')'");
            return false;
        }
        AdvanceToken();
    }
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

    // TERM -> { identifier }
    if (token == '{')
    {
        AdvanceToken(); // consume '{'

        if (PeekToken() != IDENTIFIER)
        {
            Error("identifier");
            return false;
        }
        AdvanceToken(); // consume identifier

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
    // TERM' -> TIMESOP [EXPR] TERM' | 
    while (true)
    {
        if (!FindTIMESOP())
            return true;

        if (PeekToken() != '[')
        {
            Error("'['");
            return false;
        }
        AdvanceToken(); // consume '['

        if (!FindEXPR())
            return false;

        if (PeekToken() != ']')
        {
            Error("']'");
            return false;
        }
        AdvanceToken(); // consume ']'
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