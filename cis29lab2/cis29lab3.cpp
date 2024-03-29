/*
Cis29
Lab3 - Threading
This Lab is base on Lab2
Name: Jiayan Dong
Last Modified: 11/17/2019
Description:
Model a supermarket (like Costco as in lab2) where lanes are used to process items in customer carts.
When the customer cart reaches the front of the lane, each item��s barcode in the cart is scanned to
get the name and the price of the product. When the items in the cart have been processed, a receipt
is of the items with the total price is given to the customer. Then the next cart in the queue is 
processed. There is more than one queue processing items.

When a cart reaches the front of the queue, Process the Cart on a thread. The items in the cart are
processed by decrypting the barcode into characters, and then looking up the product first five 
characters in the Product data base. The Product data-base can only be accessed by one item at a 
time, using thread blocking and unblocking techniques.

Purpose: Use Regular expressions, and the STL containers: Vector, Stack, Queue, List (and NO Map).
This assignment simulates Code3of9 Symbology, and is encoded as an binary bar code, and using threads to 
create Model a supermarket (like Costco as in lab2) where lanes are used to process items in customer carts.

Data Files: Carts.csv, ProductPrice.xml
*/

#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <regex>
#include <bitset>
#include <memory>
#include <thread>
#include <mutex>
#include <condition_variable>

using namespace std;
mutex outputmtx;				// Mutex for get price from database output
mutex inputmtx;					// Mutex for input product name into database
mutex getlineMtx;				// Mutex for get carts from Carts.csv
mutex outputFileMtx;			// Mutex for output the Bills.csv
mutex requestMtx;				// Mutex for push or pop the request queue
condition_variable inputCond;	// Condition_variable for input product name into database
condition_variable outputCond;	// Condition_variable for get price from database output

queue<int> requests;			// requests queue to put lane ids and used by database 
vector<bool> ready;				// Flags for input product name ready for each counter
vector<bool> finished;			// Flags for output product price finished for each counter

int joinedNum = 0;				// The number of the joined threads, it's used as a flag to join the database thread

//BarcodeChar to store character and its barcode code.
class BarcodeChar
{
protected:
	char character;	//Charater
	string barcode;	//barcode
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

// BarcodeCharBitset inherit from BarcodeChar Class, contain barcode as a 9-bit bitset
class BarcodeCharBitset : public BarcodeChar
{
private:
	bitset<9> bset;		//barcode as 9-bit bitset
	void convertBitset()	//Private function to convert barcode to bitset barcode
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

//BarcodeTable to store a Barcode Table as shown in the Lab2
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

//Convertor converts a hex/bin barcode to 5 characters string - name
class Convertor
{
private:
	shared_ptr<HashTable<BarcodeCharBitset>> pHTable;	//Barcode char hashtable
	string convert(string b)	//private function convert bin barcode to 5 characters string
	{
		vector<bitset<9>> vec;
		string name("");
		unsigned int i = 0;
		while (i + 9 < b.size())
		{
			vec.push_back(bitset<9>(b.substr(i, i + 9)));
			i += 9;
		}
		for_each(vec.begin(), vec.end(), [&](auto i) {
			shared_ptr<BarcodeCharBitset> ptemp = pHTable->quickSearch(i);
			if (ptemp)
				name += ptemp->getCharacter();
			});
		return name;
	}
public:
	//Overload Constructor to ininitalize convertor with hashtable
	Convertor(HashTable<BarcodeCharBitset>& p)
	{
		pHTable = make_shared<HashTable<BarcodeCharBitset>>(p);
	}

	//public function to decode hex barcode
	string decodeHex(string b)
	{
		bitset<48> bSet(stoll(b, nullptr, 16));
		return convert(bSet.to_string());
	}

	//public function to decode bin barcode
	string decodeBin(string b)
	{
		return convert(b);
	}
};

//Node to store xml data
class Node
{
private:
	string name;	//name of the data
	string data;	//data
	vector<shared_ptr<Node>> subNodes;	//pointers point to subnodes
public:
	//Default constuctor that initalize the data
	Node()
	{
		name = "";
		data = "";
	}

	//overloaded constuctor that initalize the data
	Node(string n, string d)
	{
		name = n;
		data = d;
	}

	//Setter
	void setName(string n)
	{
		name = n;
	}

	void setData(string d)
	{
		data = d;
	}

	//Getter
	string getData()
	{
		return data;
	}

	string getName()
	{
		return name;
	}

	//Push a node as the target node's subnode
	void pushSubnode(string n, string d)
	{
		shared_ptr<Node> uPtr(new Node(n, d));
		subNodes.push_back(uPtr);
	}

	//Get vector of pointers that point to subnode
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

//ProductTable class to store all product objects
class ProductTable
{
private:
	vector<Product> table;	//vector to store all product object
	int size;
public:
	//Default constuctor that initalize the data
	ProductTable()
	{
		size = 0;
	}
	//overloaded constuctor that initalize the data
	int getSize()
	{
		return size;
	}
	//insert a product object into the table
	void insert(Product p)
	{
		table.push_back(p);
		size++;
	}
	//search the price by passing the product name and product price by reference
	void searchPrice(vector<string>& names, vector<double>& prices)
	{
		Product temp;
		table.erase(remove(table.begin(), table.end(), temp), table.end());	// erase all items in database without name
		int counterNum = names.size();		// get the number of counters
		this_thread::sleep_for(std::chrono::milliseconds(500));		// sleep 500ms so counters can prepare their first item
		// loop until all counters thread end
		while (true)
		{
			bool poped = false;
			unique_lock<mutex> lk(inputmtx);
			int i = 0;
			if (joinedNum == counterNum)	// all counters thread end
				return;						// end database thread
			inputCond.wait(lk, [&]()->bool {	// using condion varible to check if there is one counter need to process data, and the get the counter id
				if (!requests.empty())
				{
					{
						lock_guard<mutex> lock(requestMtx);		// Lock requests queue to protect it
						i = requests.front();
						requests.pop();
					}
					poped = true;
					return true;
				}
				else if (joinedNum == counterNum)
					return true;
				return false;
				});		
			if (poped)
			{
				ready[i] = false;		// set input product name ready flag back to false for next request
				string name = names[i];
				auto ite = find_if(table.begin(), table.end(), [&](auto i) {return i.getName() == name; });
				prices[i] = ite->getPrice();		// set up product price
				finished[i] = true;		// set this counter finished accessing database flag to true
				lk.unlock();
				outputCond.notify_all();	// unlock and notify other counter so that other counter can access database 
			}
		}
	}
};

//Generic Class XMLProcessor to using regular expressions and use it to parse the ProductPrice.XML file.
class XMLProcessor
{
private:
	string infilename;	//xml filename
	vector<Node> xmlData;	//vector contain xmlData node
	regex beginPattern;	//regex begin pattern
	regex endPattern;	//regex end pattern
	regex leafPattern;	//regex leaf pattern
	//Generic Private Recursion function to process a Node contains subnodes, and store it in xmlData vector
	void _process(ifstream& infile, Node& data)
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
	//Overload constuctor that initalize the data with xml filename
	XMLProcessor(string i)
	{
		infilename = i;
		beginPattern.assign(R"(<(.*)>)");
		endPattern.assign(R"(</(.*)>)");
		leafPattern.assign(R"(<(.*)>(.*)<(/\1)>)");
	}
	//Generic Public process function to process all xml noded
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
	//insertAll insert Product object of xml node data into Product Table
	void insertAll(ProductTable& pT, Convertor& convertor)
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

//Class Counter(Lanes) to achieve mult-thread
class Counter
{
private:
	regex rBarcode;				// regular epression to spilt line2(items)
	smatch result;				// smatch to store items
	double price, total;		// product price and total price of one cart
	string line1, line2, name;	// line1: Cart Number, line2: Items
	vector<pair<string, double>> items;	// vector to store information of items(name and price) 

	// getCart function to get line1(Cart Number), line2(Items), using mutex to ensure the input is correct
	void getCart(ifstream& infile, string& line1, string& line2)
	{
		lock_guard<mutex> lck(getlineMtx);		
		getline(infile, line1);
		getline(infile, line2);
	}
public:
	//Default constuctor that initalize the data
	Counter()
	{
		rBarcode.assign(R"(\b\w+\b)");
		price, total = 0;
		line1, line2, name = "";
	}

	// outputBill function to simulate lanes(counters) processing items in customer carts, and then output the bill
	void outputBill(ifstream& infile, ofstream& outfile, Convertor& convertor, string& name, double& price, int i)
	{
		while (!infile.eof() && infile.good())
		{
			items.clear();
			total = 0;
			getCart(infile, line1, line2);		// Get line1(Cart Number), line2(Items)
			while (regex_search(line2, result, rBarcode))	// Using regular epression to spilt line2(items)
			{
				name = convertor.decodeHex(result.str());	// Convert Barcode in to product name
				{
					lock_guard<mutex> lock(requestMtx);		// Lock requests queue to protect it
					requests.push(i);
				}
				ready[i] = true;							// set this counter's ready to access database flag to true
				inputCond.notify_all();
				unique_lock<mutex> lk(outputmtx);
				outputCond.wait(lk, [&] {return finished[i]; });	// Wait until database finish its previous work
				total += price;	
				items.push_back(make_pair(name, price));	// Using vectors push back to simulate items in cart, pushing one item into the vector's end
				finished[i] = false;						// set output product price finished flag back to false for next request
				line2 = result.suffix();					// delete the processed item
				lk.unlock();
				inputCond.notify_all();						// unlock and notify database so that other counter can access database
			}
			lock_guard<mutex> lck(outputFileMtx);			// using mutex to ensure the output bill for the cart is correct
			outfile << line1 << '\n';
			for_each(items.begin(), items.end(), [&](auto i) {outfile << i.first << ',' << i.second << '\n'; });
			outfile << "Total," << total << '\n';
			outfile << "Lane Id," << i + 1 << '\n' << '\n';
		}
		joinedNum++;		// Once the counter(lane) ended, joined thread number + 1
		inputCond.notify_one();
	}
};

//Class BillOutput to output the bill in an .csv file
class BillOutput
{
private:
	string cartFilename;	//input cart csv filename
	string outFilename;		//output bill csv filename
	int counterNum;
public:
	//Overload constuctor that initalize the data with filenames
	BillOutput(string cF, string oF, int cN)
	{
		cartFilename = cF;
		outFilename = oF;
		counterNum = cN;
	}

	//output function Process cart csv file and output bill csv file, this function simulate that lanes are used to process items in customer carts by using threads
	void output(ProductTable& pT, Convertor& convertor)
	{
		ifstream infile;
		infile.open(cartFilename);
		if (!infile)	// inputfile vaildation
		{
			cout << "Error happened to open the input file!" << endl;
			system("pause");
			exit(EXIT_FAILURE);
		}
		ofstream outfile;
		outfile.open(outFilename);
		vector<string> names(counterNum, "");	// vector that contain product names to be accessed in counter(lane) thread and database
		vector<double> prices(counterNum, 0);	// vector that contain product prices to be accessed in counter(lane) thread and database
		vector<thread> counters(counterNum);	// vector that contain counter(lane) thread
		ready.resize(counterNum);				// set up ready flags
		finished.resize(counterNum);			// set up finished flags
		for(int i = 0; i < counterNum; i++)		// set up all counter(lane) threads
			counters[i] = thread(&Counter::outputBill, Counter(), ref(infile), ref(outfile), ref(convertor), ref(names[i]), ref(prices[i]), i);
		thread dataCenter = thread(&ProductTable::searchPrice, ref(pT), ref(names), ref(prices));	//set up database thread
		dataCenter.join();
		for (int i = 0; i < counterNum; i++)
		{
			counters[i].join();			
		}
					
		infile.close();
		outfile.close();
	}
};

//main function
int main()
{
	int counterNum = 10;	// Simulate 10 counters(lanes) processing carts
	
	string xmlFilename;
	string cartFilename;
	string billFilename;
	cout << "Welcome to Barcode Code 39 Decryption and supermarket check-out lanes simulator Program" << endl;
	cout << "Please enter the ProductPrice.xml to decrypt(with filename extension): ";
	getline(cin, xmlFilename);
	cout << "Please enter the Carts.csv(shopping carts) filename(with filename extension): ";
	getline(cin, cartFilename);
	cout << "Please enter the output filename(with filename extension .csv): ";
	getline(cin, billFilename);
	cout << "Please wait..." << endl;

	BarcodeTable bTable;	//Barcode Table in the Lab2
	HashTable<BarcodeCharBitset> hTable;	//hashtable for barcode char
	hTable.insertTable(bTable.getbTable(), [](BarcodeCharBitset& b) {return b.getBset(); });	//initailize hashtable
	Convertor convertor(hTable);	//convertor initailized by hashtable
	ProductTable pTable;	//Product table to store product objects
	XMLProcessor processor(xmlFilename);	//XMLProcessor initailized by xml filename
	processor.process();	//Process the xml file
	processor.insertAll(pTable, convertor);	//convert node objects to product objects and then insert them to Product table
	BillOutput billOutput(cartFilename, billFilename, counterNum);	//BillOutput initailized by input and output filenames
	billOutput.output(pTable, convertor);	//output file by using Product table and convertors
	cout << "The file has been decrypted and The bill is saved. Thank you!" << endl;
	system("pause");
	return 0;
}