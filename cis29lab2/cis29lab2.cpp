/*
Cis29
Lab2 - Containers and Regular Expressions
Name: Jiayan Dong
*/

#include<iostream>
#include<fstream>
#include<vector>
#include<regex>
#include<bitset>
#include<memory>

using namespace std;

//MorseChar to store character and its morse code.
class BarcodeChar
{
protected:
	char character;	//Charater
	string barcode;	//Morse Code
public:
	//Default constuctor that initalize the data
	BarcodeChar()
	{
		character = 0;
		barcode = "";
	}
	//overloaded constuctor that initalize the data
	BarcodeChar(char c, string b)
	{
		character = c;
		barcode = b;
	}
	//Setter
	void setCharacter(char c)
	{
		character = c;
	}

	void setBarcode(string b)
	{
		barcode = b;
	}
	//Getter
	char getCharacter()
	{
		return character;
	}

	string getBarcode()
	{
		return barcode;
	}
};

// BarcodeCharBitset inherit from BarcodeChar Class, contain Morse Code as a 9-bit bitset
class BarcodeCharBitset : public BarcodeChar
{
private:
	bitset<9> bset;		//Morse Code as 10-bit bitset
	void convertBitset()	//Private function to convert Morse Code to bitset Morse code
	{
		regex rNarrow("n");	//Using regualr expression to convert
		regex rWide("w");
		string sNarrow("0");
		string sWide("1");
		string s = regex_replace(barcode, rNarrow, sNarrow);
		s = regex_replace(s, rWide, sWide);
		bitset<9> temp(s);
		bset = temp;
	}
public:
	//Default constuctor that initalize the data
	BarcodeCharBitset()
	{
		BarcodeChar();
	}

	//overloaded constuctor that initalize the data
	BarcodeCharBitset(char c, string b)
	{
		character = c;
		barcode = b;
		convertBitset();
	}

	//Setter
	void setBarcodeChar(char c, string b)
	{
		character = c;
		barcode = b;
		convertBitset();
	}

	//Getter
	bitset<9> getBset()
	{
		return bset;
	}
};

//MorseTable to store a Morse Table as shown in the QueueTree.pdf
class BarcodeTable
{
private:
	vector<BarcodeCharBitset> bTable;	//vector contains all BarcodeCharBitsets
	unsigned int size;	//the size of BarcodeCharBitset table
public:
	//Default constuctor that initalize the data
	BarcodeTable()
	{
		size = 44;	//Morse Table size
		bTable.resize(size);
		//Initalize Morse Table
		bTable[0].setBarcodeChar(' ', "nwwnnnwnn");
		bTable[1].setBarcodeChar('-', "nwnnnnwnw");
		bTable[2].setBarcodeChar('+', "nwnnnwnwn");
		bTable[3].setBarcodeChar('$', "nwnwnwnnn");
		bTable[4].setBarcodeChar('%', "nnnwnwnwn");
		bTable[5].setBarcodeChar('*', "nwnnwnwnn");
		bTable[6].setBarcodeChar('.', "wwnnnnwnn");
		bTable[7].setBarcodeChar('/', "nwnwnnnwn");
		bTable[8].setBarcodeChar('0', "nnnwwnwnn");
		bTable[9].setBarcodeChar('1', "wnnwnnnnw");
		bTable[10].setBarcodeChar('2', "nnwwnnnnw");
		bTable[11].setBarcodeChar('3', "wnwwnnnnn");
		bTable[12].setBarcodeChar('4', "nnnwwnnnw");
		bTable[13].setBarcodeChar('5', "wnnwwnnnn");
		bTable[14].setBarcodeChar('6', "nnwwwnnnn");
		bTable[15].setBarcodeChar('7', "nnnwnnwnw");
		bTable[16].setBarcodeChar('8', "wnnwnnwnn");
		bTable[17].setBarcodeChar('9', "nnwwnnwnn");
		bTable[18].setBarcodeChar('A', "wnnnnwnnw");
		bTable[19].setBarcodeChar('B', "nnwnnwnnw");
		bTable[20].setBarcodeChar('C', "wnwnnwnnn");
		bTable[21].setBarcodeChar('D', "nnnnwwnnw");
		bTable[22].setBarcodeChar('E', "wnnnwwnnn");
		bTable[23].setBarcodeChar('F', "nnwnwwnnn");
		bTable[24].setBarcodeChar('G', "nnnnnwwnw");
		bTable[25].setBarcodeChar('H', "wnnnnwwnn");
		bTable[26].setBarcodeChar('I', "nnwnnwwnn");
		bTable[27].setBarcodeChar('J', "nnnnwwwnn");
		bTable[28].setBarcodeChar('K', "wnnnnnnww");
		bTable[29].setBarcodeChar('L', "nnwnnnnww");
		bTable[30].setBarcodeChar('M', "wnwnnnnwn");
		bTable[31].setBarcodeChar('N', "nnnnwnnww");
		bTable[32].setBarcodeChar('O', "wnnnwnnwn");
		bTable[33].setBarcodeChar('P', "nnwnwnnwn");
		bTable[34].setBarcodeChar('Q', "nnnnnnwww");
		bTable[35].setBarcodeChar('R', "wnnnnnwwn");
		bTable[36].setBarcodeChar('S', "nnwnnnwwn");
		bTable[37].setBarcodeChar('T', "nnnnwnwwn");
		bTable[38].setBarcodeChar('U', "wwnnnnnnw");
		bTable[39].setBarcodeChar('V', "nwwnnnnnw");
		bTable[40].setBarcodeChar('W', "wwwnnnnnn");
		bTable[41].setBarcodeChar('X', "nwnnwnnnw");
		bTable[42].setBarcodeChar('Y', "wwnnwnnnn");
		bTable[43].setBarcodeChar('Z', "nwwnwnnnn");
	}

	//Getter
	vector<BarcodeCharBitset> getbTable()
	{
		return bTable;
	}
};

//Template HashTable to create BarcodeCharBitset hashtable take bitset<9> as key
template<typename T>
class HashTable
{
private:
	vector<shared_ptr<T>> table;
	unsigned int hash(bitset<9> key) const
	{
		return key.to_ulong();
	}
	unsigned int size;  // hash table size
	unsigned int count; // number of data stored in the hash table
public:
	//Constructor
	HashTable()
	{
		size = 1023;
		count = 0;
		table.resize(size);
	}
	//Overload Constructor
	HashTable(int s)
	{
		size = s;
		count = 0;
		table.resize(size);
	}

	// Clear the data in hash table and then reset hash table size
	void setSize(unsigned int s)
	{
		size = s;
	}

	//Getters
	unsigned int getSize()
	{
		return size;
	}

	unsigned int getCount()
	{
		return count;
	}

	// insert a node at the correct location
	bool insert(T value, bitset<9> get(T&))//insert new value
	{
		if (table[hash(get(value))] == NULL)
		{
			table[hash(get(value))].reset(new T(value));
			count++;
			return true;
		}
		return false;
	}

	// find and then remove
	bool remove(T value, bitset<9> get(T&))//remove a node from hash table
	{
		if (table[hash(get(value))] != nullptr)
		{
			table[hash(get(value))] = nullptr;
			count--;
			return true;
		}
		return false;
	}

	// find a target
	bool search(T value, T& returnvalue, bitset<9> get(T&))//return true if found
	{
		if (table[hash(get(value))] != nullptr)
		{
			returnvalue = *table[hash(get(value))];
			return true;
		}
		return false;
	}

	//insert all value in the table
	void insertTable(vector<T> vTable, bitset<9> get(T&))
	{
		for_each(vTable.begin(), vTable.end(), [&](auto i) {insert(i, get); });
	}

	// quick search directly takes bitset and return value
	shared_ptr<T> quickSearch(bitset<9> bSet)
	{
		return table[hash(bSet)];
	}
};

class Convertor
{
private:
	shared_ptr<HashTable<BarcodeCharBitset>> pHTable;
	string convert(string b)
	{
		vector<bitset<9>> vec;
		string name("");
		int i = 0;
		while (i + 9 < b.size())
		{
			vec.push_back(bitset<9>(b.substr(i, i + 9)));
			i += 9;
		}
		for_each(vec.begin(), vec.end(), [&](auto i) {
			shared_ptr<BarcodeCharBitset> ptemp = pHTable->quickSearch(i);
			if(ptemp)
				name += ptemp->getCharacter();
			});
		return name;
	}
public:
	Convertor(HashTable<BarcodeCharBitset> &p)
	{
		pHTable = make_shared<HashTable<BarcodeCharBitset>>(p);
	}

	string decodeHex(string b)
	{
		bitset<48> bSet(stoll(b, nullptr, 16));
		return convert(bSet.to_string());
	}

	string decodeBin(string b)
	{
		return convert(b);
	}
};

class Node
{
private:
	string name;
	string data;
	vector<shared_ptr<Node>> subNodes;
public:
	Node()
	{
		name = "";
		data = "";
	}

	Node(string n, string d)
	{
		name = n;
		data = d;
	}

	void setName(string n)
	{
		name = n;
	}

	void setData(string d)
	{
		data = d;
	}

	string getData()
	{
		return data;
	}

	string getName()
	{
		return name;
	}

	void pushSubnode(string n, string d)
	{
		shared_ptr<Node> uPtr(new Node(n, d));
		subNodes.push_back(uPtr);
	}

	vector<shared_ptr<Node>> getSubNodes()
	{
		return subNodes;
	}
};

//Product class to store product name and price
class Product
{
protected:
	string name;	//product name
	double price;	//product price
public:
	//Default constuctor that initalize the data
	Product()
	{
		name = "";
		price = 0.0;
	}
	//overloaded constuctor that initalize the data
	Product(string n, double p)
	{
		name = n;
		price = p;
	}
	//Setter
	void setName(string n)
	{
		name = n;
	}

	void setPrice(double p)
	{
		price = p;
	}
	//Getter
	string getName()
	{
		return name;
	}

	double getPrice()
	{
		return price;
	}

	bool operator == (const Product& p)
	{
		return this->name == p.name;
	}
};

//ProductTable class to store all product node
class ProductTable
{
private:
	vector<Product> table;
	int size;
public:
	ProductTable()
	{
		size = 0;
	}

	int getSize()
	{
		return size;
	}

	void insert(Product p)
	{
		table.push_back(p);
		size++;
	}

	double searchPrice(string name)
	{
		Product temp;
		table.erase(remove(table.begin(), table.end(), temp), table.end());
		auto ite = find_if(table.begin(), table.end(), [&](auto i) {return i.getName() == name; });
		return ite->getPrice();
	}
};

class XMLProcessor
{
private:
	string infilename;
	vector<Node> xmlData;
	regex beginPattern;
	regex endPattern;
	regex leafPattern;
	void _process(ifstream &infile, Node &data)
	{
		string line;
		smatch match;
		getline(infile, line);
		if (regex_search(line, match, leafPattern))
		{
			data.pushSubnode(match[1], match[2]);
			_process(infile, data);
		}
		else if (regex_match(line, match, endPattern))
		{
			return;
		}
		else if (regex_match(line, match, beginPattern))
		{
			data.setName(match[1]);
			_process(infile, data);
		}
	}
public:
	XMLProcessor(string i)
	{
		infilename = i;
		beginPattern.assign(R"(<(.*)>)");
		endPattern.assign(R"(</(.*)>)");
		leafPattern.assign(R"(<(.*)>(.*)<(/\1)>)");

	}

	void process()
	{
		smatch match;
		string line;
		ifstream infile;
		infile.open(infilename);
		if (!infile)
		{
			cout << "Error happened to open the input file!" << endl;
			system("pause");
			exit(EXIT_FAILURE);
		}
		while (!infile.eof() && infile.good())
		{
			Node temp;
			_process(infile, temp);
			xmlData.push_back(temp);
		}
	}

	void insertAll(ProductTable &pT, Convertor &convertor)
	{
		for_each(xmlData.begin(), xmlData.end(), [&](Node i) {
			if (!i.getSubNodes().empty())
			{
				string name = convertor.decodeBin(i.getSubNodes()[0]->getData());
				double price = stod(i.getSubNodes()[1]->getData());
				pT.insert(Product(name, price));
			}
			});	
	}
};

class BillOutput
{
private:
	string cartFilename;
	string outFilename;
public:
	BillOutput(string c, string o)
	{
		cartFilename = c;
		outFilename = o;
	}

	void output(ProductTable& pT, Convertor& convertor)
	{
		string line1;
		string line2;
		regex rBarcode("\\b\\w+\\b");
		smatch result;
		string name;
		double price;
		ifstream infile;
		infile.open(cartFilename);
		if (!infile)
		{
			cout << "Error happened to open the input file!" << endl;
			system("pause");
			exit(EXIT_FAILURE);
		}
		ofstream outfile;
		outfile.open(outFilename);


		while (!infile.eof() && infile.good())
		{
			double total = 0;
			getline(infile, line1);
			getline(infile, line2);
			outfile << line1 << '\n';
			while (regex_search(line2, result, rBarcode))
			{
				name = convertor.decodeHex(result.str());
				price = pT.searchPrice(name);
				total += price;
				outfile << convertor.decodeHex(result.str()) << ',' << price << '\n';
				line2 = result.suffix();
			}
			outfile << "Total" << ',' << total << '\n' << '\n';
		}

		infile.close();
		outfile.close();
	}

};
int main()
{
	string xmlFilename;
	string cartFilename;
	string billFilename;
	cout << "Welcome to Barcode Code 39 Decryption Program" << endl;
	cout << "Please enter the ProductPrice.xml to decrypt(with filename extension): ";
	getline(cin, xmlFilename);
	cout << "Please enter the Carts.csv(shopping carts) filename(with filename extension): ";
	getline(cin, cartFilename);
	cout << "Please enter the output filename(with filename extension .csv): ";
	getline(cin, billFilename);
	cout << "Please wait..." << endl;

	BarcodeTable bTable;	//Barcode Table in the Lab2
	HashTable<BarcodeCharBitset> hTable;
	hTable.insertTable(bTable.getbTable(), [](BarcodeCharBitset& b) {return b.getBset(); });
	Convertor convertor(hTable);
	ProductTable pTable;
	XMLProcessor processor(xmlFilename);
	processor.process();
	processor.insertAll(pTable, convertor);
	BillOutput billOutput(cartFilename, billFilename);
	billOutput.output(pTable, convertor);
	cout << "The file has been decrypted and The bill is saved. Thank you!" << endl;
	system("pause");
	return 0;
}