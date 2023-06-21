#pragma once


#ifndef COMMON_H
#define COMMON_H

#include <string>
#include <time.h>

using namespace std;

enum class FormatError {
	NoError,
	NotEnoughInputs,
	InvalidFormat,
	StringMismatch,
	IntegerMismatch,
	FileNotFound
};

struct Buchungsanfrage
{
	string customerName;
	int customerId;
	time_t startZeit;
	time_t endZeit;
	string startpunkt;
	string endpunkt;
	string Kategorie;
	int Autocodierung;
	FormatError error;

};




extern Buchungsanfrage buchungsanfrage;





#endif