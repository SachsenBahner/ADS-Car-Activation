#include "k7scan1.h"


CParser::CParser() {
	IP_LineNumber = 1;
	ugetflag = 0;
	prflag = 0;
};	//Constructor


//------------------------------------------------------------------------

// Adds a character to the string value
void CParser::PushString(char c)
{
	yylval.s += c;
	//cout << yylval.s << endl;
}
//------------------------------------------------------------------------
void CParser::Load_tokenentry(string str, int index)
{
	IP_Token_table[str] = index;
	IP_revToken_table[index] = str;
}
void CParser::IP_init_token_table()
{
	Load_tokenentry("STRING", 3);
	Load_tokenentry("IDENTIFIER", 4);
	Load_tokenentry("INTEGER1", 5);
	Load_tokenentry("FLOAT", 6);

	int ii = TOKENSTART;
	Load_tokenentry("AND", ii++);
	Load_tokenentry("OR", ii++);
	Load_tokenentry("Begin", ii++);
	Load_tokenentry("End", ii++);
}
//------------------------------------------------------------------------

void CParser::pr_tokentable()
{

	typedef map<string, int>::const_iterator CI;
	const char* buf;

	printf("Symbol Table ---------------------------------------------\n");

	for (CI p = IP_Token_table.begin(); p != IP_Token_table.end(); ++p) {
		buf = p->first.c_str();
		printf(" key:%s", buf);
		printf(" val:%d\n", p->second);;
	}
}
//------------------------------------------------------------------------

int	CParser::yyparse()
{
	int tok;
	if (prflag)fprintf(IP_List, "%5d ", (int)IP_LineNumber);
	/*
	*	Go parse things!
	*/
	while ((tok = yylex()) != 0) {
		printf("%d ", tok);

		if (tok == STRING)
			printf("%s %s ", IP_revToken_table[tok].c_str(), yylval.s.c_str());
		else if (tok == INTEGER1)
			printf("%s %d ", IP_revToken_table[tok].c_str(), yylval.i);
		else if (tok == FLOAT1)
			printf("%s %f ", IP_revToken_table[tok].c_str(), yylval.kommazahl);
		else if (tok == IDENTIFIER)
			printf("%s %s ", IP_revToken_table[tok].c_str(), yylval.s.c_str());
		else if (tok >= TOKENSTART)
			printf("%s ", IP_revToken_table[tok].c_str());
		else
			printf("%c ", tok);
		if (!prflag)printf("\n");
	}
	return 0;

}
//------------------------------------------------------------------------

/*
 *	Parse Initfile:
 *
 *	  This builds the context tree and then calls the real parser.
 *	It is passed two file streams, the first is where the input comes
 *	from; the second is where error messages get printed.
 */
void CParser::InitParse(FILE* inp, FILE* err, FILE* lst)

{

	/*
	*	Set up the file state to something useful.
	*/
	IP_Input = inp;
	IP_Error = err;
	IP_List = lst;

	IP_LineNumber = 1;
	ugetflag = 0;
	/*
	*	Define both the enabled token and keyword strings.
	*/
	IP_init_token_table();
}
//------------------------------------------------------------------------

/*
 *	yyerror:
 *
 *	  Standard error reporter, it prints out the passed string
 *	preceeded by the current filename and line number.
 */
void CParser::yyerror(char* ers)

{
	fprintf(IP_Error, "line %d: %s\n", IP_LineNumber, ers);
}
//------------------------------------------------------------------------

int CParser::IP_MatchToken(string& tok)
{
	int retval;
	if (IP_Token_table.find(tok) != IP_Token_table.end()) {
		retval = (IP_Token_table[tok]);
	}
	else {
		retval = (0);
	}
	return retval;
}

//------------------------------------------------------------------------

/*
 *	yylex:
 *
 */
int CParser::yylex()
{
	//Locals
	int c;
	lexstate s;
	/*
	*	Keep on sucking up characters until we find something which
	*	explicitly forces us out of this function.
	*/
	for (s = L_START, yytext = ""; 1;) {
		c = Getc(IP_Input);
		yytext = yytext + (char)c;
		if (!ugetflag) {
			if (c != EOF)if (prflag)fprintf(IP_List, "%c", c);
		}
		else ugetflag = 0;
		switch (s) {
			//Starting state, look for something resembling a token.
		case L_START:
			if (isdigit(c)) {
				s = L_INT;
				PushString((char)c);
			}
			else if (c == ';') { // next Value, Seperator
				s = L_IDENT;
			}
			else if (isspace(c) || c == '/') {
				if (c == '\n') {
					IP_LineNumber += 1;
					if (prflag)
						fprintf(IP_List, "%5d ", (int)IP_LineNumber);
				}
				yytext = "";
			}
			else if (isalpha(c) != 0) {   //(c == '"')
				s = L_STRING;
				yylval.s = c;
			}

			else if (c == EOF) {
				return ('\0');
			}
			else {
				return (c);
			}
			break;

			/*
			 *	Suck up the integer digits.
			 */
		case L_INT:
			if (isdigit(c)) {
				PushString((char)c);
				break;
			}
			else if (c == ',' || c == '.') {
				s = L_FLOAT;
				PushString((char)c);
			}
			else {
				Ungetc(c);
				yylval.s = yytext.substr(0, yytext.size() - 1);
				yylval.i = atoi(yylval.s.c_str());
				return (INTEGER1);
			}
			break;

		case L_FLOAT:
			if (isdigit(c)) {
				PushString((char)c);
			}
			else if (c == ',' || c == '.') {
				PushString('.');
			}
			else {
				Ungetc(c);
				yylval.s = yytext.substr(0, yytext.size() - 1);
				yylval.kommazahl = strtof(yylval.s.c_str(), NULL);
				return (FLOAT1);
			}
			break;

			/*
			 *	Grab an identifier, see if the current context enables
			 *	it with a specific token value.
			 */

		case L_IDENT:
			//if (isalpha(c) || isdigit(c) || c == '_')
			if (c == ';' || c == '€') // geändert
				break;
			Ungetc(c);
			yytext = yytext.substr(0, yytext.size() - 1);
			yylval.s = yytext;
			if (c = IP_MatchToken(yytext)) {
				return (c);
			}
			else {
				return (IDENTIFIER);
			}


			/*
			*	Suck up string characters but once resolved they should
			*	be deposited in the string bucket because they can be
			*	arbitrarily long.
			*/

		case L_STRING:
			if (c == '\n')
				IP_LineNumber += 1;
			else if (c == '\r')
				;
			else	if (c == ';' || c == EOF) { // next Value, String ends
				return (STRING);
			}
			else
				PushString((char)c);
			break;
		default: printf("***Fatal Error*** Wrong case label in yylex\n");
		}
	}
}
//------------------------------------------------------------------------

