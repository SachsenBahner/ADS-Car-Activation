// ADS_Code.cpp : Diese Datei enthält die Funktion "main". Hier beginnt und endet die Ausführung des Programms.

#include "ADS_Code.h"



// Anfage:
// Kundenname; Kundennummer; Start-Datum;Start-Zeit;End-Datum;End-Zeit;Startpunkt;Endpunkt;Kategorie;Autocodierung




int main()
{

	// Huffmann ausgeben

	CHufftree hufft;
	unordered_map<char, string>Codierung;		//unordered_map zur Speicherung der jeweiligen Codierung für die auftretenden Symbole nach Wahrscheinlichkeit
	vector<int> numbers = { 0,1,0,1,2,6,0,7, 0,1,0,2,4,3,5,3, 0,2,0,1,8,8,7,7, 0,2,0,2,9,5,0,4, 0,3,0,1,1,8,7,3, 0,3,0,2,2,6,0,7 };
	hufft.probabilities(numbers);

	CHuffNode* tree;
	tree = hufft.Huffman();
	hufft.pr_hufftree(tree, "", Codierung);

	//Ausgabe der unordered_map
	for (const auto& pair : Codierung) {
		cout << pair.first << " = " << pair.second << endl;
	}

	// Ende Huffmann
	
	//encode
	vector<int> CarKey = {0,3,0,2,2,6,0,7};
	cout << encode(CarKey, Codierung) << endl;

	// TODO: Später für UI ausglieder
	vector<Car> cars = readCarPool(filenameCarPool);
	vector<Customer> customers = readCustomerPool(filenameCustomerPool);
	vector<Station> stations = readStationPool(filenameStationPool);
	// Ende Ausgliedern


	// buchungsanfrage#

	while (1) {
		// frage, ob neue Anfrage

		cout << "Bitte Buchungsdateiname eingeben: ";

		string filename;

		cin >> filename;

		Buchungsanfrage neueAnfrage = buchungsParser.ParseBuchungsanfrage(filename);

		cout << neueAnfrage.customerName << neueAnfrage.customerId;


		if (neueAnfrage.error != FormatError::NoError) {
			
			cout << "Die Anfrage enthält einen Fehler. ";

			if (neueAnfrage.error == FormatError::NotEnoughInputs) {
				cerr << "Nicht genug Inputs!";
			}
			else if (neueAnfrage.error == FormatError::InvalidFormat) {
				cerr << "InvalidFormat!";
			}
			else if (neueAnfrage.error == FormatError::StringMismatch) {
				cerr << "StringMismatch!";
			}
			else if (neueAnfrage.error == FormatError::IntegerMismatch) {
				cerr << "IntegerMismatch!";
			}
			else if (neueAnfrage.error == FormatError::FileNotFound) {
				cerr << "File not Found!";
			}

			break;
		}


		AnfrageError err = pruefeAnfrage(neueAnfrage, bestehendeAnfragen, customers, cars, stations); // überprüfe, ob es Fehler gibt

		if (err  == AnfrageError::NoError) { // Buchung kann angenommen werden
			cout << "Buchung angenommen";
			bestehendeAnfragen.push_back(neueAnfrage);
			break;
		}
			
		
		cout << "Buchung muss abgelehnt werden! Grund: ";

		switch (err)
		{
		case AnfrageError::NoCustomerFound:
			cerr << "Der Kunde wurde nicht gefunden!";
			break;
		case AnfrageError::NoCarFound:
			cerr << "Das Auto wurde nicht gefunden!";
			break;
		case AnfrageError::NoStationFound:
			cerr << "Die Station wurde nicht gefunden!";
			break;
		case AnfrageError::AlreadyBooked:
			cerr << "Leider ist das Auto zu dem gewählten Zeitpunkt schon ausgebucht!";
			break;
		}

		cout << endl;

	}

	


	// K7scan1

	//FILE* inf;
	//cout << system("cd");
		
	//char fistr[200] = "buchung.txt";

	//printf("Enter filename:\n");
	//fgets(fistr, sizeof(fistr), stdin);
	//fistr[strlen(fistr) - 1] = '\0';

	/*inf = fopen(fistr, "r");
	if (inf == NULL) {
		perror("Error");
		printf("Cannot open input file %s\n", fistr);
		return 0;
	}


	BuchungsParser obj;

	obj.InitParse(inf, stderr, stdout);
	//obj.Load_tokenentry("SEMICOLON", 9);
	//obj.pr_tokentable();
	obj.yyparse();*/
	

	//Buchungsanfrage anfrage = obj.ParseBuchungsanfrage()


	return 0;
}



vector<Customer> readCustomerPool(const string& filename) {
	vector<Customer> customerList;

	ifstream inputFile(filename);
	if (!inputFile) {
		cout << "Error opening the file." << endl;
		return customerList;
	}

	string line;

	while (getline(inputFile, line)) {
		istringstream iss(line);
		string name;
		int id;
		if (iss >> name >> id) {
			customerList.push_back({ name, id });
		}
	}

	inputFile.close();

	return customerList;
}

vector<Station> readStationPool(const string& filename) {
	// liest nur den Namen ein, Station1: wird weggelassen

	vector<Station> stations;

	ifstream inputFile(filename);
	if (!inputFile) {
		cout << "Error opening the file." << endl;
		return stations;
	}

	string line;
	int stationNum = 1;

	while (getline(inputFile, line)) {
		istringstream iss(line);
		string prefix, name;
		if (iss >> prefix >> name) {
			name.erase(remove(name.begin(), name.end(), ':'), name.end());
			stations.push_back({ stationNum, name });
			stationNum++;
		}
	}

	inputFile.close();

	return stations;
}

vector<Car> readCarPool(const string& filename) {
	vector<Car> cars; // ka-ching ;-)

	cout << "vector<Car> readCarPool(const string& filename)" << endl;

	ifstream inputFile(filename);
	if (!inputFile) {
		cout << "Error opening the file." << endl;
		return cars;
	}

	string line;

	while (getline(inputFile, line)) {
		istringstream iss(line);

		string kategorie, name;
		string pricePerHourString, pricePerKmString, carCodierungString; // zunächst string, dann € wegkappen
		float pricePerHour, pricePerKm;
		int carCodierung;

		if (getline(iss, kategorie, ';') &&
			getline(iss, name, ';') &&
			getline(iss, pricePerHourString, ';') &&
			getline(iss, pricePerKmString, ';') &&
			getline(iss, carCodierungString))
		{
			// jetzt € cutten

			pricePerHourString.resize(pricePerHourString.length() - 5); // entfernt €/h
			pricePerKmString.resize(pricePerKmString.length() - 6); // entfernt €/km

			pricePerHour = stof(pricePerHourString);
			pricePerKm = stof(pricePerKmString);
			carCodierung = stoi(carCodierungString);

			cars.push_back({ kategorie, name, pricePerKm, pricePerHour, carCodierung });
		}
	}

	inputFile.close();

	return cars;
}

bool isOverlap(const Buchungsanfrage& neueAnfrage, const vector<Buchungsanfrage>& bestehendeAnfragen) {
	for (const Buchungsanfrage& bestehendeAnfrage : bestehendeAnfragen) {

		// Überprüfe, ob die Startzeit der neuen Anfrage vor der Endzeit der bestehenden Anfrage liegt UND die Endzeit der neuen Anfrage nach der Startzeit der bestehenden Anfrage liegt

		if (neueAnfrage.startZeit < bestehendeAnfrage.endZeit && neueAnfrage.endZeit > bestehendeAnfrage.startZeit) {
			// Es gibt eine Überschneidung
			return true;
		}
	}
	// Keine Überschneidung gefunden
	return false;
}


bool isCustomerLegit(const Buchungsanfrage& neueAnfrage, const vector <Customer>& customers) {
	for (const Customer& customer : customers) {
		if (neueAnfrage.customerName == customer.name && neueAnfrage.customerId == customer.id) {
			// Der Kunde existiert und ID stimmt auch überein
			return true;
		}
	}
	// Der Kunde wurde nicht gefunden
	return false;
}

bool isCarLegit(const Buchungsanfrage& neueAnfrage, const vector <Car>& cars) {
	for (const Car& car : cars) {
		if (neueAnfrage.Autocodierung == car.carCodierung && neueAnfrage.Kategorie == car.kategorie) {
			// Das Auto existiertund ID stimmt auch überein
			return true;
		}
	}
	// Das Auto wurde nicht gefunden
	return false;
}

bool isStationLegit(const string& stationName, const vector <Station>& stations) {
	for (const Station& station : stations) {
		if (stationName == station.name ) {
			// Die Station existiert
			return true;
		}
	}
	// Das Auto wurde nicht gefunden
	return false;
}


AnfrageError pruefeAnfrage(const Buchungsanfrage& neueAnfrage, const vector<Buchungsanfrage>& bestehendeAnfragen, const vector <Customer>& customers, const vector <Car>& cars, const vector <Station>& stations) {
	// prüfe, ob Customer existiert

	if (isCustomerLegit(neueAnfrage, customers) == false)
		//NoCustomerFound
		return AnfrageError::NoCustomerFound;


	// prüfe, ob Start und Zielpunkt existieren

	if (isStationLegit(neueAnfrage.startpunkt, stations) == false || isStationLegit(neueAnfrage.endpunkt, stations) == false)
		//NoStationFound
		return AnfrageError::NoStationFound;



	// prüfe, ob Autocodierung existiert

	if (isCarLegit(neueAnfrage, cars) == false)
		//NoCarFound
		return AnfrageError::NoCarFound;


	// prüfe, ob das Auto ausgebucht ist.

	if(isOverlap(neueAnfrage, bestehendeAnfragen))
		// Es liegt schon eine Buchung vor
		return AnfrageError::AlreadyBooked;

	return AnfrageError::NoError;
}

string encode (vector<int>& CarKey, unordered_map<char, string>& Codierung) {
	string output = "";
	for (int i = 0; i < CarKey.size(); i++)
	{
		output += Codierung['0'+CarKey[i]];
	}
	return output;
}