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

	Buchungsanfrage anfrage = buchungsParser.ParseBuchungsanfrage(fistr);

	cout << anfrage.customerName << endl;


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


	//char c; cin >> c; // nur für pause


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