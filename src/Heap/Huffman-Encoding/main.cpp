#include"Huffman.h"
#include<iostream>
#include<fstream>
#include<math.h>
#include<assert.h>


void binaryToChar(std::string filename) {
	std::ofstream fout("abc.txt");
	std::fstream fin;
	fin.open(filename, std::ios::in);

	assert(!fin.fail());
	
	char ch;
	while (!fin.eof()) {
		bool isCopy = false;
		int num = 0;
		for (int i = 7; i >= 0 && fin >> ch; --i) {
			num += (int(ch) - 48) * std::pow(2, i);
			isCopy = true;
		}
		if (isCopy) {
			std::cout <<"'"<< char(num) << "' ";
			fout << (char)num;
		}
	}
	std::cout << std::endl;
	fout.close();
	fin.close();
}


std::string readFile(const std::string& fileName)
{
	std::ifstream ifs(fileName.c_str(), std::ios::in | std::ios::binary | std::ios::ate);
	std::ifstream::pos_type fileSize = ifs.tellg();
	ifs.seekg(0, std::ios::beg);

	std::vector<char> bytes(fileSize);
	ifs.read(bytes.data(), fileSize);

	return std::string(bytes.data(), fileSize);
}


int main() {
	Huffman huff;
	binaryToChar("sample4.8b");
	std::string ret = readFile("abc.txt");
	huff.createHuffmanStr(ret);
	huff.printHuffman();
	huff.encoding(ret);
	std::cout << ret;

	return 0;
}