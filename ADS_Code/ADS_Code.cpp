// ADS_Code.cpp : Diese Datei enthält die Funktion "main". Hier beginnt und endet die Ausführung des Programms.

#include "ADS_Code.h"



// Anfage:
// Kundenname; Kundennummer; Start-Datum;Start-Zeit;End-Datum;End-Zeit;Startpunkt;Endpunkt;Kategorie;Autocodierung




int main()
{
	


	// TODO: Später für UI ausglieder
	vector<Car> cars = readCarPool(filenameCarPool);
	vector<Customer> customers = readCustomerPool(filenameCustomerPool);
	vector<Station> stations = readStationPool(filenameStationPool);
	// Ende Ausgliedern


	// buchungsanfrage
		
	char fistr[200] = "buchung.txt";

	Buchungsanfrage neueAnfrage = buchungsParser.ParseBuchungsanfrage(fistr);

	if (isOverlap(neueAnfrage, bestehendeAnfragen) == false ) { // Buchung kann angenommen werden
		cout << "Buchung angenommen";
		bestehendeAnfragen.push_back(neueAnfrage);
	}
	else {
		cout << "Buchung muss abgelehnt werden";
	}

	cout << endl << "#####" << endl;

	AnfrageError err = pruefeAnfrage(neueAnfrage, bestehendeAnfragen, customers, cars, stations);
	
	cout << (int) err;


	// K7scan1

	//FILE* inf;
	//cout << system("cd");

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


	char c; cin >> c; // nur für pause


	cout << "\n\n\n" << "####################\n\n" << endl;


	// k5huff

/*
	printf("Huffman Test!\n");
	CHufftree hufft;
	hufft.BuildHeap(); 	// Heap erzeugen
	//	hufft.pr_pq();return;
	CHuffNode* tree;
	tree = hufft.Huffman();
	hufft.pr_hufftree(tree);
	char v;
	cin >> v;

*/

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