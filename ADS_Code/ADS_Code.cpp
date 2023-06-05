// ADS_Code.cpp : Diese Datei enthält die Funktion "main". Hier beginnt und endet die Ausführung des Programms.
//

#include <iostream>
#include <fstream>
#include <string>


#include "k7scan1.h"

#pragma warning(disable:4786)
#pragma warning(disable : 4996) //_CRT_SECURE_NO_WARNINGS

using namespace std;



// Anfage:
// Kundenname; Kundennummer; Start-Datum;Start-Zeit;End-Datum;End-Zeit;Startpunkt;Endpunkt;Kategorie;Autocodierung




int main()
{
	FILE* inf;
	char fistr[200] = "buchung.txt";

	//cout << system("cd");

	//printf("Enter filename:\n");
	//fgets(fistr, sizeof(fistr), stdin);
	//fistr[strlen(fistr) - 1] = '\0';

	inf = fopen(fistr, "r");
	if (inf == NULL) {
		perror("Error");
		printf("Cannot open input file %s\n", fistr);
		return 0;
	}


	CParser obj;

	obj.InitParse(inf, stderr, stdout);
	//obj.Load_tokenentry("SEMICOLON", 9);
	//obj.pr_tokentable();
	obj.yyparse();
	char c; cin >> c;

	return 0;
}
