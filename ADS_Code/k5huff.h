#pragma once
#include "stdafx.h"
#include <iostream>
#include <queue>
#include <string>
#include <vector>
#include <unordered_map>
#include <iomanip>

using namespace std;


class CHuffNode
{
public:
	double key;
	char symbol;
	CHuffNode* left, * right;
	bool operator<(const CHuffNode& x) const
	{
		return key > x.key;
	}
	CHuffNode(double key = 0, char symbol = ' ', CHuffNode* left = NULL, CHuffNode* right = NULL);
};


class CHufftree
{
public:
	priority_queue<CHuffNode> pq;
	void BuildHeap();
	void pr_hufftree(CHuffNode*);
	void pr_pq();
	CHuffNode* Huffman();
	CHufftree() {};
};
