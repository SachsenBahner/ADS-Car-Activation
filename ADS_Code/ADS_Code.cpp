// ADS_Code.cpp : Diese Datei enthält die Funktion "main". Hier beginnt und endet die Ausführung des Programms.

#include "ADS_Code.h"



// Anfage:
// Kundenname; Kundennummer; Start-Datum;Start-Zeit;End-Datum;End-Zeit;Startpunkt;Endpunkt;Kategorie;Autocodierung




int main()
{
	// Ausgabesprache festlegen
	locale::global(locale("German_germany.UTF-8"));

	cout << "Car-Activation V1.0" << endl;

	cout << "Lese Pools ein..." << endl;
	vector<Car> cars = readCarPool(filenameCarPool);
	vector<Customer> customers = readCustomerPool(filenameCustomerPool);
	vector<Station> stations = readStationPool(filenameStationPool);
	unordered_map<char, string> Codierung;
	Codierung = printHuffmann(cars);

	while (1) {
		char userInput;
		cout << endl;

		cout << "[7mBitte wähle eine Option aus:[0m" << endl;
		cout << "  1 Huffmann-Baum ausgeben" << endl;
		cout << "  2 Pool Daten neu einlesen" << endl;
		cout << "  3 Pooldaten ausgeben" << endl;
		cout << "  4 Alle Buchungen ausgeben" << endl;
		cout << "  5 Neue Buchung einlesen" << endl;
		cout << "  6 Programm verlassen" << endl;

		cout << endl << "Deine Eingabe: ";

		cin >> userInput;
		cin.ignore(); // \n ignorieren
		cout << endl;

		switch (userInput)
		{
		case '1':
			cout << "Huffmann-Baum wird ausgegeben." << endl;
			Codierung = printHuffmann(cars);
			break;
		case '2':
			cout << "Lese Pools ein..." << endl;
			cars = readCarPool(filenameCarPool);
			customers = readCustomerPool(filenameCustomerPool);
			stations = readStationPool(filenameStationPool);
			cout << "Pool Daten wurden neu einlesen." << endl;
			break;
		case '3':
			printPools(customers, cars, stations);
			break;
		case '4':
			cout << "Alle Buchungen werden ausgegeben." << endl;
			printBuchungen(bestehendeAnfragen);
			break;
		case '5':
			cout << "Bitte Buchungsname angeben: " << endl;
			neueBuchung(customers, cars, stations, Codierung);
			break;
		case '6':
			return 0;
			break;
		default:
			cout << "Unbekannte Eingabe. Bitte erneut probieren." << endl;
			break;
		}
	}

	return 0;
}



vector<Customer> readCustomerPool(const string& filename) {
	//cout << "vector<Customer> readCustomerPool(const string & filename)" << endl;
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
		if (getline(iss, name, ',') && (iss >> id)) {
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
		if (getline(iss, prefix, ':') && getline(iss, name)) {
			stations.push_back({ stationNum, name });
			stationNum++;
		}
	}

	inputFile.close();

	return stations;
}

vector<Car> readCarPool(const string& filename) {
	vector<Car> cars; // ka-ching ;-)

	//cout << "vector<Car> readCarPool(const string& filename)" << endl;

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

			//cout << "CarPool: " << kategorie << name << pricePerKm << pricePerHour << carCodierung << endl;

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
		// cout << "Customer: " << neueAnfrage.customerName << " =? " << customer.name << endl;
		if (neueAnfrage.customerName == customer.name || neueAnfrage.customerId == customer.id) {
			// Der Kunde existiert und ID stimmt auch überein
			return true;
		}
	}
	// Der Kunde wurde nicht gefunden
	return false;
}

bool isCarLegit(const Buchungsanfrage& neueAnfrage, const vector <Car>& cars) {
	for (const Car& car : cars) {
		//cout << "Checkcar: " << neueAnfrage.Autocodierung << car.carCodierung << " Cat: " << neueAnfrage.Kategorie << car.kategorie << endl;
		if (neueAnfrage.Autocodierung == car.carCodierung){
			//cout << "Codierung stimmt;" << endl;
			//if (neueAnfrage.Kategorie == car.kategorie) {  <-- not needed, ID ist einzigartiger
			//	cout << "Kategorie stimmt" << endl;
				// Das Auto existiertund ID stimmt auch überein
				return true;
			//}
		}
	}
	// Das Auto wurde nicht gefunden
	return false;
}

bool isStationLegit(const string& stationName, const vector <Station>& stations) {
	for (const Station& station : stations) {
		//cout << stationName << station.name << endl; 
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


unordered_map<char, string> printHuffmann(vector<Car>& cars) {
	// Huffmann ausgeben

	CHufftree hufft;
	unordered_map<char, string>Codierung;		//unordered_map zur Speicherung der jeweiligen Codierung für die auftretenden Symbole nach Wahrscheinlichkeit

	vector<int> numbers;
	for (Car car : cars) {
		int number = car.carCodierung;
		numbers.push_back(0);	//führende Null der CarCoddes hinzügen, wird bei int immer entfernt
		while (number > 0) {
			int digit = number % 10;
			numbers.push_back(digit);
			number /= 10;
		}
	}
	hufft.probabilities(numbers);

	CHuffNode* tree;
	tree = hufft.Huffman();
	hufft.pr_hufftree(tree, "", Codierung);

	//Ausgabe der unordered_map
	cout << endl << "binäre 0 im Baum nach links | binäre 1 im Baum nach rechts" << endl;

	cout << endl << setw(16) << left << "[3mSymbol[0m";
	cout << setw(16) << left << "[3mCodierung[0m" << endl;

	for (const auto& pair : Codierung) {
		cout << setw(8) << left << pair.first;
		cout << setw(8) << left << pair.second << endl;
	}

	return Codierung;
	// Ende Huffmann

	//encode
	//vector<int> CarKey = { 0,3,0,2,2,6,0,7 };
	//cout << encode(CarKey, Codierung) << endl;
}


void printBuchungen(const vector<Buchungsanfrage>& anfragen) {

	if (anfragen.empty()) {
		cout << "Es liegen noch keine Anfragen vor." << endl;
		return;
	}

	// Kundenname; Kundennummer; Start-Datum;Start-Zeit;End-Datum;End-Zeit;Startpunkt;Endpunkt;Kategorie;Autocodierung

	//cout << left << setw(20) << "[7mKundenname" << setw(7) << "KundenID" << setw(15) << "Startzeitpunkt" << setw(15) << "Endzeitpunkt" << "Startpunkt" << "Endpunkt" << "Autokategorie" << "Autocodierung[0m" << endl;
	cout << left << "[7m" << setw(20) << "Kundenname" << setw(9) << "KundenID" << setw(16) << "Startzeitpunkt" << setw(15) << "Endzeitpunkt";
	cout << setw(12) << "Startpunkt" << setw(12) << "Endpunkt" << setw(10) << "Kategorie" << setw(20) << "Autocodierung[0m" << endl;

	for (Buchungsanfrage af : anfragen) {
		cout << left << setw(20) << af.customerName << setw(9) << format("{:07}", af.customerId) << setw(16) << formatDate(af.startZeit) << "  " << setw(15) << formatDate(af.endZeit);
		cout << setw(12) << af.startpunkt << setw(12) << af.endpunkt << setw(11) << af.Kategorie << setw(20) << af.Autocodierung << endl;
	}

	

	

}

string formatDate(time_t time)
{
	string str(30, '\0');
	strftime(&str[0], str.size(), "%d.%m.%y %H:%M", localtime(&time));
	return str;
}

void neueBuchung(vector<Customer>& customers, vector<Car>& cars, vector<Station>& stations, unordered_map<char, string>& Codierung) {
	string filename;
	cin >> filename;
	Buchungsanfrage neueAnfrage = buchungsParser.ParseBuchungsanfrage(filename);


	if (neueAnfrage.error != FormatError::NoError) {

		cout << "[41m" << "Die Anfrage enthält einen Fehler. ";

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
		cout << "[0m";
		return;
	}


	AnfrageError err = pruefeAnfrage(neueAnfrage, bestehendeAnfragen, customers, cars, stations); // überprüfe, ob es Fehler gibt

	if (err == AnfrageError::NoError) { // Buchung kann angenommen werden
		cout << "[42m" << "Buchung angenommen!" << "[0m" << endl;
		
		vector<int> CarKey;
		int number = neueAnfrage.Autocodierung;
		CarKey.push_back(0);	//führende Null der CarCodes hinzügen, wird bei int immer entfernt
		while (number > 0) {
			int digit = number % 10;
			CarKey.push_back(digit);
			number /= 10;
		}
		cout << "Codierungsbitstream: " << encode(CarKey, Codierung) << endl;
		bestehendeAnfragen.push_back(neueAnfrage);
		return;
	}



	cout << "[41m" << "Buchung muss abgelehnt werden! Grund: ";

	switch (err)
	{
	case AnfrageError::NoCustomerFound:
		cout << "Der Kunde wurde nicht gefunden!" << "[0m" << endl;
		break;
	case AnfrageError::NoCarFound:
		cout << "Das Auto wurde nicht gefunden!" << "[0m" << endl;
		break;
	case AnfrageError::NoStationFound:
		cout << "Die Station wurde nicht gefunden!" << "[0m" << endl;
		break;
	case AnfrageError::AlreadyBooked:
		cout << "Leider ist das Auto zu dem angegebenen Zeitpunkt schon ausgebucht!" << "[0m" << endl;
		break;
	}

	cout << endl;
}

void printPools(vector<Customer>& customers, vector<Car>& cars, vector<Station>& stations) {
	cout << left << setw(20) << "[7mKundenname" << "Kundennummer[0m" << endl;
	for (Customer cs : customers) {
		cout << left << setw(20) << cs.name << cs.id << endl;;
	}

	cout << endl << endl;

	cout << left << setw(12) << "[7mStationsID" << " Station[0m" << endl;
	for (Station st : stations) {
		cout << left << setw(12) << st.number << st.name << endl;;
	}

	cout << endl << endl;

	cout << left << setw(14) << "[7mAutoname" << setw(12) << "Kategorie" << setw(12) << "Kodierung" << setw(7) << "Preis/h" << setw(7) << "Preis/km[0m" << endl;
	for (Car cr : cars) {
		cout << left << setw(14) << cr.name << setw(12) << cr.kategorie << setw(12) << cr.carCodierung << setw(7) << fixed << setprecision(2) << cr.pricePerHour << setw(7) << cr.pricePerKm << endl;

	}
}