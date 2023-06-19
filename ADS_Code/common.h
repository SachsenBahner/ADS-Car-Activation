#pragma once


#ifndef COMMON_H
#define COMMON_H

#include <string>
#include <time.h>

using namespace std;

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
};


extern Buchungsanfrage buchungsanfrage;





#endif