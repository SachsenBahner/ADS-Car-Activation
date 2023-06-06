# Car Activation Programm

Dieses Programm, mit dem Namen "Car Activation", wurde in C++ entwickelt und nutzt die Programme "k5huff" und "k7scan1" von Prof. Schäfer. Es wird Entwickelt von Kjell und Cedric.

Es ermöglicht die Verarbeitung von Buchungsanfragen für Carsharing und umfasst die folgenden Hauptfunktionen:

- Lexikalische Analyse der Buchungsanfrage
- Festlegung der Signal-Codierung für den Huffman-Algorithmus
- Ausgabe des Huffman-Baums
- Ausgabe des Verifizierungsergebnisses des Autos



## Lexikalische Analyse der Buchungsanfrage

Das Car Activation Programm führt eine lexikalische Analyse der Buchungsanfrage durch. Hierbei wird das Format "Kundenname; Kundennummer; Start-Datum; Start-Zeit; End-Datum; End-Zeit; Startpunkt; Endpunkt; Kategorie; Autocodierung" verwendet. Die lexikalische Analyse zerlegt die Buchungsanfrage in einzelne Bestandteile und überprüft deren Korrektheit.

## Festlegung der Signal-Codierung für den Huffman-Algorithmus

Für den Huffman-Algorithmus wurde eine Signal-Codierung basierend auf praxisrelevantem Hintergrund recherchiert. Die Codierung berücksichtigt Merkmale wie Kategorie, Sitzanzahl und weitere relevante Informationen der Fahrzeuge. Die Codierung erfolgt mithilfe des Programms "k5huff" von Prof. Schäfer.

## Ausgabe des Huffman-Baums

Das Car Activation Programm gibt den Huffman-Baum aus, der bei der Signal-Codierung mit dem Huffman-Algorithmus erstellt wurde. Der Huffman-Baum veranschaulicht die Codierung der Signale und ermöglicht eine effiziente Decodierung der Buchungsanfragen.

## Ausgabe des Verifizierungsergebnisses des Autos

Nach der lexikalischen Analyse und der Codierung der Buchungsanfrage wird das Verifizierungsergebnis des Autos ausgegeben. Diese Ausgabe gibt Aufschluss darüber, ob das gewünschte Fahrzeug verfügbar ist und ob die Buchungsanfrage erfolgreich verarbeitet wurde.

## Benutzerfreundliche Konsolenausgaben

Das Car Activation Programm bietet eine benutzerfreundliche Konsolenausgabe als User-Interface. Die Ausgaben sind klar strukturiert und leicht verständlich, um dem Benutzer eine intuitive Interaktion mit dem Programm zu ermöglichen.

## Funktion

Das Car Activation Programm verfügt über einen Fahrzeugpool, einen Kundenstammpool und einen Standortpool. Es ermöglicht die Verwaltung von Buchungsanfragen und die Überprüfung der Fahrzeugverfügbarkeit. Das Programm arbeitet mit dem angegebenen Buchungsanfrage-Format, analysiert und verifiziert die Anfragen, und gibt entsprechende Rückmeldungen aus.
