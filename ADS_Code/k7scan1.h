#pragma once
#include "stdafx.h"
#include <iostream>
#pragma warning(disable:4786)
#include <string>
#include <map>

#include <fstream>

using namespace std;

#define	Getc(s)			getc(s)
#define	Ungetc(c)		{ungetc(c,IP_Input); ugetflag=1;}


class CParser
{
public:
	CParser();	//Constructor

	void InitParse(FILE* inp, FILE* err, FILE* lst);
	int	yyparse();						//parser

	void pr_tokentable();				//test output for tokens
	void Load_tokenentry(string str, int index);//load one token



private:
	string yytext;								//input buffer

	struct tyylval {							//value return
		string s;								//structure
		int i;
	}yylval;

	FILE* IP_Input;								//Input File
	FILE* IP_Error;								//Error Output
	FILE* IP_List;								//List Output

	int  IP_LineNumber;							//Line counter
	int ugetflag;								//checks ungets
	int prflag;									//controls printing

	map<string, int> IP_Token_table;			//Tokendefinitions
	map<int, string> IP_revToken_table;			//reverse Tokendefinitions


	int yylex();						//lexial analyser
	void yyerror(char* ers);			//error reporter
	int IP_MatchToken(string& tok);		//checks the token
	void IP_init_token_table();			//loads the tokens
	void PushString(char c);			//Used for dtring assembly


	const int STRING = 3;
	const int IDENTIFIER = 4;
	const int INTEGER1 = 5;
	const int FLOAT1 = 6;
	const int TOKENSTART = 300;

	enum lexstate {
		L_START, L_INT, L_IDENT, L_STRING, L_FLOAT
	};
};