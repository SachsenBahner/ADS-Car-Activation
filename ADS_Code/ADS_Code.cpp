// ADS_Code.cpp : Diese Datei enthält die Funktion "main". Hier beginnt und endet die Ausführung des Programms.
//

#include <iostream>
#include <fstream>
#include <string>

#include <time.h>

#include <codecvt>



#include "k7scan1.h"
#include "k5huff.h"


#pragma warning(disable:4786)
#pragma warning(disable : 4996) //_CRT_SECURE_NO_WARNINGS

using namespace std;


struct Buchungsanfrage {
	string kundenName;
	int kundenNummer;
	time_t startZeit;
	time_t endZeit;
	string startpunkt;
	string endpunkt;
	string Kategorie;
	int Autocodierung;
};



// Anfage:
// Kundenname; Kundennummer; Start-Datum;Start-Zeit;End-Datum;End-Zeit;Startpunkt;Endpunkt;Kategorie;Autocodierung




int main()
{
	FILE *datei; 
	char line[100];

	if (fopen_s(&datei, "car.pool", "r") == 0) {
		if (fgets(line, 100, datei) == NULL) {
			printf("fgets error");
		}
		else
			printf("%s", line);
		fclose(datei);
	}


	


	// K7scan1

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
	//char c; cin >> c; // nur für pause


	cout << "\n\n\n" << "####################\n\n" << endl;


	// k5huff

	/*printf("Huffman Test!\n");
	CHufftree hufft;
	hufft.BuildHeap(); 	// Heap erzeugen
	//	hufft.pr_pq();return;
	CHuffNode* tree;
	tree = hufft.Huffman();
	hufft.pr_hufftree(tree);
	char v;
	cin >> v;
	*/


	struct tm tm;
	time_t t;

	tm.tm_year = 2023 - 1900;  // Jahr abziehen, da tm_year die Anzahl der Jahre seit 1900 erwartet
	tm.tm_mon = 6 - 1;         // Monat abziehen, da tm_mon die Anzahl der Monate seit Januar erwartet
	tm.tm_mday = 8;
	tm.tm_hour = 15;
	tm.tm_min = 40;
	tm.tm_sec = 0;
	tm.tm_isdst = -1;          // Zeitumstellung lokal erkennen

	t = mktime(&tm);

	std::cout << "Time: " << t << std::endl;



	return 0;
}
