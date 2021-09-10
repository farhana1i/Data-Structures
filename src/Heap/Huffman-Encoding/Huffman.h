#pragma once
#include<iostream>
#include<fstream>
#include"MinHeap.h"
#include<vector>
#include<unordered_map>

class Huffman {
private:
	struct HNode
	{
		int freq;
		char character;
		HNode* left, * right;

		HNode(int freq = 0, char ch = '#') {
			this->freq = freq;
			this->character = ch;
			this->left = this->right = nullptr;
		}
	}* root;

	// delete all nodes
	void deleteAll(HNode*& node) {
		if (node == nullptr) return;

		deleteAll(node->left);
		deleteAll(node->right);
		delete node;
	}

	// to set frequencies of each character
	void setFreqList(std::string input, int freqList[]) {
		for (int i = 0; i < input.size(); ++i) {
			freqList[int(input[i])]++;
		}
	}

	// return encoded code of given symbol
	void encodedCode(HNode* node, char ch, std::vector<char>& code, bool& isFound) {
		if (node == nullptr || isFound) return;

		if (node->left != nullptr || node->right != nullptr) {
			if (node->left != nullptr) {
				code.push_back('0');
				encodedCode(node->left, ch, code, isFound);
				if (isFound) return;
				code.pop_back();
			}

			if (node->right != nullptr) {
				code.push_back('1');
				encodedCode(node->right, ch, code, isFound);
				if (isFound) return;
				code.pop_back();
			}
		}

		else {
			if (node->character == ch) {
				isFound = true;
			}
		}
	}
	void encodedCode(char ch, std::vector<char>& code) {
		bool isFound = false;
		encodedCode(this->root, ch, code, isFound);
	}

	// return true, if ch is already present in string
	bool isPresent(int ind, std::string str, char ch) {
		for (int i = 0; i < ind && i < str.size(); i++) {
			if (str[i] == ch)
				return true;
		}
		return false;
	}

public:
	// constructor
	Huffman() {
		this->root = nullptr;
	}

	// to create huffman tree
	void createHuffmanTree(MinHeap<int, HNode*>& huffmanHeap) {
		// delete previous data
		deleteAll(this->root);
		this->root = nullptr;

		// build huffman tree
		while (huffmanHeap.getTotalItems() > 1) {
			HNode* min1 = nullptr, * min2 = nullptr;
			huffmanHeap.getMin(min1);
			huffmanHeap.deleteMin();
			huffmanHeap.getMin(min2);
			huffmanHeap.deleteMin();

			HNode* newNode = new HNode(min1->freq + min2->freq);
			newNode->left = min1;
			newNode->right = min2;
			huffmanHeap.insert(min1->freq + min2->freq, newNode);
		}
		huffmanHeap.getMin(this->root);
		huffmanHeap.deleteMin();

	}
	
	// to create huffman tree for file
	void createHuffman(std::string const filename) {
		std::fstream fin(filename);
		if (fin.fail()) {
			std::cout << "Fail To Open File\n";
		}
		else {
			MinHeap<int, HNode*> huffmanHeap;
			// build huffman heap
			while (!fin.eof()) {
				char ch = '\0';
				int freq = 0;

				fin >> ch;
				fin >> freq;
				HNode* newNode = new HNode(freq, ch);
				huffmanHeap.insert(freq, newNode);
			}
			createHuffmanTree(huffmanHeap);
		}
	}
	
	// to create huffman tree for input string
	void createHuffmanStr(std::string input) {
		int freqList[256] = { 0 };
		setFreqList(input, freqList);

		// create huffman minHeap
		MinHeap<int, HNode*> huffmanHeap;
		for (int i = 0; i < input.size(); ++i) {
			if (!isPresent(i, input, input[i])) {
				HNode* newNode = new HNode(freqList[int(input[i])], input[i]);
				huffmanHeap.insert(freqList[int(input[i])], newNode);
			}
		}
		createHuffmanTree(huffmanHeap);
	}

	// to print huffman tree
	void printHuffman(HNode* node, std::vector<char> code) const {
		if (node == nullptr) return;
		if (node->left != nullptr && node->right != nullptr) {			
			if (node->left != nullptr)
				code.push_back('0');
			printHuffman(node->left, code);
			code.pop_back();
			if (node->right != nullptr)
				code.push_back('1');
			printHuffman(node->right, code);
			code.pop_back();
		}
		else {
			std::cout << "Character: " << node->character << "\t\tFreq: " << node->freq << "\t\tCode: ";
			for (char c : code)
				std::cout << c;
			std::cout << "\n";
		}
	}
	void printHuffman() const {
		if (!root->left && !root->right) {
			std::cout << "Character: " << root->character << "\t\tFreq: " << root->freq << "\t\tCode: 0";
			return;
		}

		std::vector<char> code;
		printHuffman(this->root, code);
	}

	// to encode string
	void encoding(std::string& input) {
		if (!root->left && !root->right) {
			input = "0";
			return;
		}

		std::string encodedStr = "";
		for (int i = 0; i < input.size(); i++) {
			std::vector<char> code;
			encodedCode(input[i], code);
			for (char ch : code) {
				encodedStr.push_back(ch);
			}
		}
		input.erase();
		input = encodedStr;
	}

	// to encode string
	std::string decoding(std::string encodedStr) {
		std::string decodedStr = "";
		if (this->root == nullptr) return decodedStr;
		HNode* start = this->root;
		for (int i = 0; i < encodedStr.size(); i++) {
			if (encodedStr[i] == '0') {
				start = start->left;
			}
			else if (encodedStr[i] == '1') {
				start = start->right;
			}
			if (start->left == nullptr && start->right == nullptr) {
				decodedStr.push_back(start->character);
				start = this->root;
			}
		}
		return decodedStr;
	}

	// destructor
	~Huffman() {
		deleteAll(this->root);
	}
};
