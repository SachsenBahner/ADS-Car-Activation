// ADS_Code.h : Diese Datei enthält die Funktion "main". Hier beginnt und endet die Ausführung des Programms.
//

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <time.h>
#include <codecvt>


#include "k7scan1.h"
#include "k5huff.h"
#include "common.h"
#include "readBuchungsanfrage.h"

#pragma warning(disable:4786)
#pragma warning(disable : 4996) //_CRT_SECURE_NO_WARNINGS


using namespace std;


// Define some Variables

string filenameCarPool = "car.pool";
string filenameCustomerPool = "customer.pool";
string filenameStationPool = "Routes.pool";

BuchungsParser buchungsParser;

vector<Buchungsanfrage> bestehendeAnfragen;


// Define some Structs

struct Customer
{
	string name;
	int id;
};

struct Station {
	int number;
	string name;
};

struct Car
{
	string kategorie;
	string name;
	float pricePerHour;
	float pricePerKm;
	int carCodierung;
};


enum class AnfrageError {
	NoError,
	NoCustomerFound,
	NoCarFound,
	NoStationFound,
	AlreadyBooked
};






// Anfage:
// Kundenname; Kundennummer; Start-Datum;Start-Zeit;End-Datum;End-Zeit;Startpunkt;Endpunkt;Kategorie;Autocodierung


// Define some Functions
vector<Car> readCarPool(const string& filename);
vector<Customer> readCustomerPool(const string& filename);
vector<Station> readStationPool(const string& filename);

bool isOverlap(const Buchungsanfrage& neueAnfrage, const vector<Buchungsanfrage>& bestehendeAnfragen);

AnfrageError pruefeAnfrage(const Buchungsanfrage& neueAnfrage, const vector<Buchungsanfrage>& bestehendeAnfragen, const vector <Customer>& customers, const vector <Car>& cars, const vector <Station>& stations);
bool isCustomerLegit(const Buchungsanfrage& neueAnfrage, const vector <Customer>& customers);
bool isCarLegit(const Buchungsanfrage& neueAnfrage, const vector <Car>& cars);
bool isStationLegit(const string& stationName, const vector <Station>& stations);

int main();


