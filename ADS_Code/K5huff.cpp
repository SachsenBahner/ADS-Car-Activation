// K5huff.cpp 
//
#include "k5huff.h"



CHuffNode::CHuffNode(double key, char symbol,	CHuffNode* left, CHuffNode* right) : key(key), symbol(symbol), left(left), right(right) {
	// Konstruktor
};



/*CHufftree::CHufftree() {
	// Konstruktor
};*/


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
void CHufftree::pr_hufftree(CHuffNode *n)
{
	if(n){
		printf("symbol: %c, key: %f\n",n->symbol,n->key);
		pr_hufftree(n->left);
		pr_hufftree(n->right);
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
		CHuffNode rp(r1->key + r2->key,' ',r1,r2);							
		pq.push (rp);		// insert new CHuffNode to queue
	}
}
//---------------------------------------------------------------

/*void main(int argc, char* argv[])
{
	printf("Huffman Test!\n");
	CHufftree hufft;
	hufft.BuildHeap ( ); 	// Heap erzeugen
//	hufft.pr_pq();return;
	CHuffNode *tree;
	tree=hufft.Huffman();
	hufft.pr_hufftree(tree);
	char c; 
	cin >> c;

}*/

