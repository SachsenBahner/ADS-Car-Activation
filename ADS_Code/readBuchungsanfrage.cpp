#include "readBuchungsanfrage.h"


BuchungsParser::BuchungsParser() {
	


};	//Constructor



// Anfage:
// Kundenname; Kundennummer; Start-Datum;Start-Zeit;End-Datum;End-Zeit;Startpunkt;Endpunkt;Kategorie;Autocodierung
// Bsp: Zuckerburg; 0000007; 20.06.2023; 18:00; 20.06.2023; 20:30; H-KA; Bahnhof; Cat1; 01012607


FormatError BuchungsParser::checkLineFormat(const string &line, int &errorIndex) {
	istringstream iss(line);
	string kundenname, startpunkt, endpunkt, kategorie, kundennummerStr, startJahrStr, startMonatStr, startTagStr, startStundeStr, startMinuteStr;
	string endJahrStr, endMonatStr, endTagStr, endStundeStr, endMinuteStr, autoCodierungStr;

	int kundennummer, startJahr, startMonat, startTag, startStunde, startMinute;
	int endJahr, endMonat, endTag, endStunde, endMinute, autocodierung;

	
	if (getline(iss, kundenname, ';') &&
		getline(iss, kundennummerStr, ';') &&
		getline(iss, startTagStr, '.') &&
		getline(iss, startMonatStr, '.') &&
		getline(iss, startJahrStr, ';') &&
		getline(iss, startStundeStr, ':') &&
		getline(iss, startMinuteStr, ';') &&
		getline(iss, endTagStr, '.') &&
		getline(iss, endMonatStr, '.') &&
		getline(iss, endJahrStr, ';') &&
		getline(iss, endStundeStr, ':') &&
		getline(iss, endMinuteStr, ';') &&
		getline(iss, startpunkt, ';') &&
		getline(iss, endpunkt, ':') &&
		getline(iss, autoCodierungStr, ';'))
	{
		// jetzt Integer

		/*

		kundennummer = stoi(kundennummerStr);
		startJahr = stoi(startJahrStr);
		startMonat = stoi(startMonatStr);
		startTag = stoi(startTagStr);
		startStunde = stoi(startStundeStr);
		startMinute = stoi(startMinuteStr);

		endJahr = stoi(endJahrStr);
		endMonat = stoi(endMonatStr);
		endTag = stoi(endTagStr);
		endStunde = stoi(endStundeStr);
		endMinute = stoi(endMinuteStr);

		autocodierung = stoi(autoCodierungStr);

		//cars.push_back({ kategorie, name, pricePerKm, pricePerHour, carCodierung });
		*/

		// Überprüfung der eingelesenen Werte
		if (kundenname.empty() || kundennummerStr.empty() || startTagStr.empty() ||
			startMonatStr.empty() || startJahrStr.empty() || startStundeStr.empty() ||
			startMinuteStr.empty() || endTagStr.empty() || endMonatStr.empty() ||
			endJahrStr.empty() || endStundeStr.empty() || endMinuteStr.empty() ||
			startpunkt.empty() || endpunkt.empty() || autoCodierungStr.empty())
		{
			// Fehler: Leerer Wert
			errorIndex = -1;
			return FormatError::InvalidFormat;
		}

		try {
			kundennummer = std::stoi(kundennummerStr);
			startJahr = std::stoi(startJahrStr);
			startMonat = std::stoi(startMonatStr);
			startTag = std::stoi(startTagStr);
			startStunde = std::stoi(startStundeStr);
			startMinute = std::stoi(startMinuteStr);

			endJahr = std::stoi(endJahrStr);
			endMonat = std::stoi(endMonatStr);
			endTag = std::stoi(endTagStr);
			endStunde = std::stoi(endStundeStr);
			endMinute = std::stoi(endMinuteStr);

			autocodierung = std::stoi(autoCodierungStr);
		}
		catch (const std::exception& e) {
			// Fehler: Ungültiger numerischer Wert
			errorIndex = -1;
			return FormatError::IntegerMismatch;
		}

		// Es gab keine Fehler

		// wandle in Time um

		time_t startZeit = getEpochTime(startJahr, startMonat, startTag, startStunde, startMinute);
		time_t endZeit = getEpochTime(endJahr, endMonat, endTag, endStunde, endMinute);


		buchungsanfrage.customerName = kundenname;
		buchungsanfrage.customerId = kundennummer;
		buchungsanfrage.startZeit = startZeit; 
		buchungsanfrage.endZeit = endZeit;
		buchungsanfrage.startpunkt = startpunkt;
		buchungsanfrage.endpunkt = endpunkt;
		buchungsanfrage.Autocodierung = autocodierung;

		return FormatError::NoError;
	}
	else {
		// Fehler: Nicht genügend Werte in der Zeile
		errorIndex = -1;
		return FormatError::NotEnoughInputs;
	}
}






Buchungsanfrage BuchungsParser::ParseBuchungsanfrage(const string& file) {

	clearBuchungsanfrage();



	ifstream inputFile(file);
	if (!inputFile) {
		cout << "Error opening the file." << endl;
		return buchungsanfrage;
	}

	string line;
	getline(inputFile, line);

	inputFile.close();


	// jetzt prüfen, ob Format passt

	int errorIndex = -1;

	FormatError formatError = checkLineFormat(line, errorIndex);



	// NoError,	NotEnoughInputs, InvalidFormat, StringMismatch, IntegerMismatch

	if (formatError == FormatError::NotEnoughInputs) {
		cerr << "Nicht genug Inputs!";
	}
	else if (formatError == FormatError::InvalidFormat) {
		cerr << "InvalidFormat!";
	}
	else if (formatError == FormatError::StringMismatch) {
		cerr << "StringMismatch!";
	}
	else if (formatError == FormatError::IntegerMismatch) {
		cerr << "IntegerMismatch!";
	}


	

	return buchungsanfrage;
}



void BuchungsParser::clearBuchungsanfrage() {
	//memset(&buchungsanfrage, 0, sizeof(buchungsanfrage));
	Buchungsanfrage buchungsanfrage{};
}


// Converts UTC time string to a time_t value.
time_t BuchungsParser::getEpochTime(const int startJahr, const int startMonat, const int startTag, const int startStunde, const int startMinute)
{
	struct tm tm;
	time_t zeit;

	tm.tm_year = startJahr - 1900;  // Jahr abziehen, da tm_year die Anzahl der Jahre seit 1900 erwartet
	tm.tm_mon = startMonat - 1;         // Monat abziehen, da tm_mon die Anzahl der Monate seit Januar erwartet
	tm.tm_mday = startTag;
	tm.tm_hour = startStunde;
	tm.tm_min = 40;
	tm.tm_sec = startMinute;
	tm.tm_isdst = -1;          // Zeitumstellung lokal erkennen

	zeit = mktime(&tm);

	return zeit;
}