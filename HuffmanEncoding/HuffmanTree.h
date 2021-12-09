#pragma once
#include <iostream>
#include "DSA.h"
#include "Heap.h"
#include <unordered_map>
#include <fstream>

class HuffmanTree
{
	struct Node
	{
		struct Data
		{
			char ch;
			size_t f;
		} data;
		Node* left = nullptr;
		Node* right = nullptr;

		Node(const Data& d)
			:
			data(d)
		{}
		~Node()
		{
			delete left;
			delete right;
		}

		size_t GetHeight() const
		{
			size_t l = 0, r = 0;
			if (left)
				l = left->GetHeight();
			if (right)
				r = right->GetHeight();

			return 1 + std::max(l, r);
		}
	};

	typedef Node* NodePtr;
public:
	HuffmanTree() = default;
	HuffmanTree(const std::string& str);
	HuffmanTree(const HuffmanTree&) = delete;
	HuffmanTree& operator=(const HuffmanTree&) = delete;
	~HuffmanTree();
	// set up tree and map for encoding/decoding
	void Generate(const std::string& str);
	// loads saved huffman tree from file (interface)
	void Load(const std::string& filename);
	// saves the huffman tree in a given file (interface)
	void Save(const std::string& filename);
	// print character codes
	void PrintCodes() const;
	// takes a string, encodes it, and prints it to given file
	void EncodeFile(std::ofstream& out, const std::string& str) const;
	// takes an input file, decodes it, and stores it in given output file
	void DecodeFile(std::ifstream& in, std::ofstream& out);
	// returns the compressed string as it would be stored into the file
	// also returns the number of bits in the compressed string 
	// num of bits is needed by the decompresseion func
	std::string GetEncodedCompressedString(const std::string& str, size_t& num) const;
	// returns the compressed string as it would be stored into the file
	// overloaded to not need reference to num of bits
	std::string GetEncodedCompressedString(const std::string& str) const;
	// returns the compressed string in an expanded form so that each bit is a character
	std::string GetEncodedExpandedString(const std::string& str);
	// decompresses string where one char contians multiple bits
	// i.e. strings read directly from the compressed file
	std::string GetDecodedCompressedString(const std::string& compressed_enc, int num) const;
	// decompreses string where one char contains one bit of the compressed data
	std::string GetDecodedExpandedString(const std::string& enc) const;
	// get the height of the tree
	size_t GetHeight() const;
	// see if the tree has beeen initialized
	bool IsEmpty() const;

private:
	// generates the huffman tree
	void GenerateTree(DSA<Node::Data> arr);
	// generates the codes map that stores char codes
	void GenerateCodeMap(const Node* n, std::string code = "");
	// saves the huffman tree in a given file (impl)
	void Save(std::ofstream& out, NodePtr n);
	// loads saved huffman tree from file (impl)
	void Load(std::ifstream& in, NodePtr& n);

private:
	NodePtr root = nullptr;
	std::unordered_map<char, std::string> codes;
};