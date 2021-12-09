#include "HuffmanTree.h"

HuffmanTree::HuffmanTree(const std::string& str)
{
	Generate(str);
}

HuffmanTree::~HuffmanTree()
{
	delete root;
}

void HuffmanTree::Generate(const std::string& str)
{
	// delete any old tree
	delete root;
	DSA<Node::Data> arr;
	for (const auto& ch : str)
	{
		bool exists = false;
		for (size_t i = 0; i < arr.GetSize(); i++)
		{
			if (arr[i].ch == ch)
			{
				arr[i].f++;
				exists = true;
				break;
			}
		}
		if (!exists)
			arr.PushBack({ ch, 1 });
	}

	GenerateTree(arr);
	GenerateCodeMap(root);
}

void HuffmanTree::Load(const std::string& filename)
{
	delete root;
	std::ifstream in(filename);
	Load(in, root);
	GenerateCodeMap(root);
}

void HuffmanTree::Save(const std::string& filename)
{
	std::ofstream out(filename);
	Save(out, root);
}

void HuffmanTree::PrintCodes() const
{
	for (const auto& i : codes)
	{
		std::cout << i.first << ": " << i.second << std::endl;
	}
}

void HuffmanTree::EncodeFile(std::ofstream& out, const std::string& str) const
{
	size_t num = 0;
	std::string compressed_enc = GetEncodedCompressedString(str, num);

	// save total num of bits
	out.write((char*)&num, sizeof(size_t));
	// save bytes
	size_t num_bytes = compressed_enc.length();
	out.write((char*)compressed_enc.data(), num_bytes);
}

void HuffmanTree::DecodeFile(std::ifstream& in, std::ofstream& out)
{
	// get total num of bits
	size_t num = 0;
	in.read((char*)&num, sizeof(size_t));
	// total bytes
	size_t num_bytes = num / 8 + num % 8;
	// get bytes
	std::string enc;
	for (size_t i = 0; i < num_bytes; i++)
	{
		enc += in.get();
	}

	out << GetDecodedCompressedString(enc, num);
}

std::string HuffmanTree::GetEncodedCompressedString(const std::string& str, size_t& num) const
{
	size_t i = 0; // bit num
	std::string compressed_enc;
	char val = 0;
	for (auto c : str)
	{
		const auto& code = codes.at(c);
		for (auto bit : code)
		{
			val |= ((bit - '0') << i);
			i++;
			num++;
			if (i == 8)
			{
				compressed_enc += val;
				i = 0;
				val = 0;
			}
		}
	}
	std::cout << std::endl;
	// add leftover bits
	if (i != 0)
		compressed_enc += val;
	return compressed_enc;
}

std::string HuffmanTree::GetEncodedCompressedString(const std::string& str) const
{
	size_t i = 0; // bit num
	std::string compressed_enc;
	char val = 0;
	for (auto c : str)
	{
		const auto& code = codes.at(c);
		for (auto bit : code)
		{
			val |= ((bit - '0') << i);
			i++;
			if (i == 8)
			{
				compressed_enc += val;
				i = 0;
				val = 0;
			}
		}
	}
	std::cout << std::endl;
	// add leftover bits
	if (i != 0)
		compressed_enc += val;

	return compressed_enc;
}

std::string HuffmanTree::GetEncodedExpandedString(const std::string& str)
{
	std::string enc;
	for (auto c : str)
	{
		auto code = codes[c];
		for (auto& bit : code)
			enc += code;
	}
	return enc;
}

std::string HuffmanTree::GetDecodedCompressedString(const std::string& compressed_enc, int num) const
{
	Node* ptr = root;
	std::string str;
	for (size_t i = 0, j = 0; i < num; i++, j += (i % 8 == 0))
	{
		const int bit = ((compressed_enc[j] >> (i % 8)) & 1);

		if (bit == 0)
			ptr = ptr->left;
		else
			ptr = ptr->right;

		if (!ptr->left && !ptr->right)
		{
			str += ptr->data.ch;
			ptr = root;
		}
	}
	return str;
}

std::string HuffmanTree::GetDecodedExpandedString(const std::string& enc) const
{
	Node* ptr = root;
	std::string decoded;
	for (auto c : enc)
	{
		if (ptr->left && c == '0')
			ptr = ptr->left;
		else if (ptr->right && c == '1')
			ptr = ptr->right;

		if (!ptr->left && !ptr->right)
		{
			decoded += ptr->data.ch;
			ptr = root;
		}
	}
	return decoded;
}

size_t HuffmanTree::GetHeight() const
{
	return root->GetHeight();
}

bool HuffmanTree::IsEmpty() const
{
	return root == nullptr;
}

void HuffmanTree::GenerateTree(DSA<Node::Data> arr)
{
	Heap<NodePtr> h(
		// lambda to compare NodePtrs
		[](const NodePtr& l, const NodePtr& r)
		{
			return l->data.f < r->data.f;
		}
	);

	// push nodes onto heap
	for (size_t i = 0; i < arr.GetSize(); i++)
	{
		h.Push(new Node(arr[i]));
	}

	// generate tree
	while (h.GetSize() > 1)
	{
		// pop least freq nodes
		auto left = h.Peek();
		h.Pop();
		auto right = h.Peek();
		h.Pop();

		// generate partial treee using popped nodes
		NodePtr r = new Node({ '\0', left->data.f + right->data.f });
		r->left = left;
		r->right = right;
		h.Push(r);
	}
	root = h.Peek();
	h.Pop();
}

void HuffmanTree::GenerateCodeMap(const Node* n, std::string code)
{
	// if leaf node, add data to map
	if (!n->left && !n->right)
	{
		codes.insert({ n->data.ch, code });
		return;
	}

	if (n->left)
	{
		GenerateCodeMap(n->left, code + '0');
	}
	if (n->right)
	{
		GenerateCodeMap(n->right, code + '1');
	}
}

void HuffmanTree::Save(std::ofstream& out, NodePtr n)
{
	if (n == nullptr)
	{
		out << 0 << ' ';
		return;
	}

	if (n->left && n->right)
		out << 1 << ' ';
	else
		out << 2 << ' ';

	if (n->data.ch != '\0')
		out << n->data.ch << ' ';
	out << n->data.f << ' ';

	Save(out, n->left);
	Save(out, n->right);
}

void HuffmanTree::Load(std::ifstream& in, NodePtr& n)
{
	size_t i;
	in >> i;
	if (i == 0)
		return;
	else if (i == 1)
	{
		in >> i;
		n = new Node({ '\0', i });
	}
	else if (i == 2)
	{
		char c;
		in >> c;
		in >> i;
		n = new Node({ c, i });
	}

	Load(in, n->left);
	Load(in, n->right);
}
