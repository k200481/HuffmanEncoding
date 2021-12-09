#pragma once
#include <fstream>
#include "HuffmanTree.h"

class HuffmanEncoder
{
public:
	HuffmanEncoder() = default;
	HuffmanEncoder(const std::string& treefilename)
	{
		t.Load(treefilename);
	}

	void CompressFile(const std::string& in_filename, const std::string& out_filename)
	{
		std::ifstream in(in_filename);
		std::ofstream out(out_filename, std::ios::binary);

		std::string text;
		while (in.peek() != EOF)
			text += in.get();

		if (t.IsEmpty())
		{
			t.Generate(text);
		}

		size_t num = 0;
		std::string enc = t.GetEncodedCompressedString(text, num);
		out << num << ' ' << enc;
	}

	void DecompressFile(const std::string& in_filename, const std::string& out_filename)
	{
		std::ifstream in(in_filename, std::ios::binary);
		std::ofstream out(out_filename);

		size_t num = 0;
		in >> num;
		in.ignore();

		size_t num_bytes = num / 8 + num % 8;
		std::string enc;
		for (size_t i = 0; i < num_bytes; i++)
		{
			enc += in.get();
		}
		out << t.GetDecodedCompressedString(enc, num);
	}

private:
	HuffmanTree t;
};

