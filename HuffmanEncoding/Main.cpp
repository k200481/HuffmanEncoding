#include <iostream>
#include <fstream>
#include "HuffmanEncoder.h"

void GenerateRandomFileText(const std::string& filename, size_t num_chars)
{
	srand(time(NULL));
	std::ofstream out(filename);
	for (size_t i = 0; i < 10000; i++)
	{
		out << char('A' + rand() % 26);
	}
}

bool CompareFiles(const std::string& filename1, const std::string& filename2)
{
	std::ifstream in1(filename1);
	std::ifstream in2(filename2);

	while(in1.good() && in2.good())
	{
		if (in1.get() != in2.get())
			return false;
	}
	return true;
}

int main(void)
{
	constexpr const char* InputFile = "Input.txt";
	constexpr const char* EncodedFile = "Encoded.txt";
	constexpr const char* DecodedFile = "Decoded.txt";

	GenerateRandomFileText(InputFile, 100000);
	
	HuffmanEncoder he;

	he.CompressFile(InputFile, EncodedFile);
	he.DecompressFile(EncodedFile, DecodedFile);

	std::cout << "Comparing Decompressed file to original:\n";
	std::cout << (CompareFiles(InputFile, DecodedFile) ? "Files Match!" : "Files Don't Match") << std::endl;


	return 0;
}