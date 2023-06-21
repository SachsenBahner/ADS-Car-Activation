#pragma once

#include <iostream>
#include <sstream>
#include <vector>
#include <fstream>
#include <time.h>
#include <string>

#include "common.h"

#pragma warning(disable:4786)


class BuchungsParser
{
public:
	BuchungsParser();	// Konstruktor

	Buchungsanfrage ParseBuchungsanfrage(const string& file);



private:
	Buchungsanfrage buchungsanfrage;


	FormatError checkLineFormat(const string& line, int& errorIndex);
	void clearBuchungsanfrage();
	time_t getEpochTime(const int startJahr, const int startMonat, const int startTag, const int startStunde, const int startMinute);

	
};