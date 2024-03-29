// K5huff.cpp 
//
#include "k5huff.h"
//Bestimmung Codierung
//Kategorie-Fahrzeugnummer(in der Kategorie)-4RndNr
//01 02 7734


CHuffNode::CHuffNode(double key, char symbol,	CHuffNode* left, CHuffNode* right) : key(key), symbol(symbol), left(left), right(right) {
	// Konstruktor
};

/*CHufftree::CHufftree() {
	// Konstruktor
};*/

void CHufftree::probabilities(vector<int>& codes) {
	int total_count, count;
	double probability;
	total_count = codes.size();
	cout << "Ausgabe der Wahrscheinlichkeiten: \n";
	for (int i = 0; i <= 9; i++) {
		count = 0;
		for (int num : codes) {
			if (num == i) {
				count++;
			}
		}		
		probability = 1.0 * count / total_count;
		if (count != 0) {pq.push(CHuffNode(probability, i + '0'));};
		cout << "Symbol " << i << "  " << std::fixed << std::setprecision(2) << probability << "\n";
	}
}

//------------------------------------------------------------

void CHufftree::BuildHeap()
{
	printf("Creating Data!\n");
	pq.push(CHuffNode(0.18,'a'));
	pq.push(CHuffNode(0.38,'b'));
	pq.push(CHuffNode(0.12,'c'));
	pq.push(CHuffNode(0.02,'d'));
	pq.push(CHuffNode(0.30,'e'));

}

//---------------------------------------------------------------
void CHufftree::pr_pq()
{
	CHuffNode m;
	while(!pq.empty()){
		m=pq.top();
		pq.pop();
		printf("%c %f\n",m.symbol,m.key);
	}
}
void CHufftree::pr_hufftree(CHuffNode *n, string code, unordered_map<char, string>& Codierung)
{
	if(n){
		//printf("symbol: %c, key: %f\n",n->symbol,n->key);
		if(n->symbol != '#') {
			Codierung[n->symbol] = code;
		}
		pr_hufftree(n->left, code + "0", Codierung);
		pr_hufftree(n->right, code + "1", Codierung);
	}
}
//---------------------------------------------------------------

CHuffNode * CHufftree::Huffman ()
{
	CHuffNode *r1;
	CHuffNode *r2;
	while (1)		  			//loop until tree created
	{
		r1= new CHuffNode;
		r2= new CHuffNode;
		// minimum CHuffNode to r1 and remove from queue
		*r1= pq.top(); pq.pop();
		if (pq.empty( ) ) 
			return r1;			// return the created tree
		// minimum CHuffNode to r2 and remove from queue
		*r2= pq.top(); pq.pop();
		// new CHuffNode with sum of weight
		CHuffNode rp(r1->key + r2->key,'#',r1,r2);							
		pq.push (rp);		// insert new CHuffNode to queue
	}
}
//---------------------------------------------------------------


// Alle Programmfunktionen werden über das Hauptprogramm in der Datei ADS_CODE.cpp angesprochen! 
/* int main(int argc, char* argv[])
{
	CHufftree hufft;
	unordered_map<char, string>Codierung;		//unordered_map zur Speicherung der jeweiligen Codierung für die auftretenden Symbole nach Wahrscheinlichkeit
	//Codierung['9'] = "010101";
	vector<int> numbers = {0,1,0,1,2,6,0,7, 0,1,0,2,4,3,5,3, 0,2,0,1,8,8,7,7, 0,2,0,2,9,5,0,4, 0,3,0,1,1,8,7,3, 0,3,0,2,2,6,0,7};
	hufft.probabilities(numbers);

	CHuffNode *tree;
	tree=hufft.Huffman();
	hufft.pr_hufftree(tree, "", Codierung);

	//Ausgabe der unordered_map
	for (const auto& pair : Codierung){
		cout << pair.first << " = " << pair.second << endl;
	}
}

*/