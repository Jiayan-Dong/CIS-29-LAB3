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
#include <functional>

using namespace std;

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
	function<bitset<9>(T)> get;
public:
	//Constructor
	HashTable(bitset<9> getBSet(T&))
	{ 
		get.assign(getBSet);
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

	// Destructor
	~HashTable()
	{ 
		
	}

	// Clear the data in hash table and then reset hash table size
	void setSize(int s)
	{
		size = s;
	}

	// insert a node at the correct location
	void insert(T value)//insert new value
	{
		table[hash(get(value))] = &value;
	}

	// find and then remove a node
	bool remove(T value)//remove a node from hash table
	{
		if (table[hash(get(value))] != NULL)
		{
			table[hash(get(value))] = NULL;
			return true;
		}
		return false;
	}

	// find a target node
	bool search(T value, T& returnvalue)//return true if found
	{
		if (table[hash(get(value))] != NULL)
		{
			returnvalue = *table[hash(get(value))];
			return true;
		}
		return false;
	}

	T quickSearch(bitset<9> bSet)
	{
		return *table[hash(bSet)];
	}
};


class BarcodeTable
{
private:
	vector<pair<char, string>> table;
	unsigned int size;
	shared_ptr<HashTable<pair<char, string>>> pHTable;
	void initialHashTable(HashTable<pair<char, string>>& hTable)
	{
		regex rNarrow("n");
		regex rWide("w");
		string sNarrow("0");
		string sWide("1");

		auto get = [&](pair<char, string>& p) {
			string s = regex_replace(p.second, rNarrow, sNarrow);
			s = regex_replace(s, rWide, sWide);
			bitset<9> bset(s);
			return bset;
		};

		pHTable.reset(new HashTable<pair<char, string>>(get);
		for_each(table.begin(), table.end(), [&](auto i) {hTable.insert(i); });
	}
public:
	BarcodeTable()
	{
		size = 44;
		table.resize(size);
		table[0] = make_pair(' ', "nwwnnnwnn");
		table[1] = make_pair('-', "nwnnnnwnw");
		table[2] = make_pair('+', "nwnnnwnwn");
		table[3] = make_pair('$', "nwnwnwnnn");
		table[4] = make_pair('%', "nnnwnwnwn");
		table[5] = make_pair('*', "nwnnwnwnn");
		table[6] = make_pair('.', "wwnnnnwnn");
		table[7] = make_pair('/', "nwnwnnnwn");
		table[8] = make_pair('0', "nnnwwnwnn");
		table[9] = make_pair('1', "wnnwnnnnw");
		table[10] = make_pair('2', "nnwwnnnnw");
		table[11] = make_pair('3', "wnwwnnnnn");
		table[12] = make_pair('4', "nnnwwnnnw");
		table[13] = make_pair('5', "wnnwwnnnn");
		table[14] = make_pair('6', "nnwwwnnnn");
		table[15] = make_pair('7', "nnnwnnwnw");
		table[16] = make_pair('8', "wnnwnnwnn");
		table[17] = make_pair('9', "nnwwnnwnn");
		table[18] = make_pair('A', "wnnnnwnnw");
		table[19] = make_pair('B', "nnwnnwnnw");
		table[20] = make_pair('C', "wnwnnwnnn");
		table[21] = make_pair('D', "nnnnwwnnw");
		table[22] = make_pair('E', "wnnnwwnnn");
		table[23] = make_pair('F', "nnwnwwnnn");
		table[24] = make_pair('G', "nnnnnwwnw");
		table[25] = make_pair('H', "wnnnnwwnn");
		table[26] = make_pair('I', "nnwnnwwnn");
		table[27] = make_pair('J', "nnnnwwwnn");
		table[28] = make_pair('K', "wnnnnnnww");
		table[29] = make_pair('L', "nnwnnnnww");
		table[30] = make_pair('M', "wnwnnnnwn");
		table[31] = make_pair('N', "nnnnwnnww");
		table[32] = make_pair('O', "wnnnwnnwn");
		table[33] = make_pair('P', "nnwnwnnwn");
		table[34] = make_pair('Q', "nnnnnnwww");
		table[35] = make_pair('R', "wnnnnnwwn");
		table[36] = make_pair('S', "nnwnnnwwn");
		table[37] = make_pair('T', "nnnnwnwwn");
		table[38] = make_pair('U', "wwnnnnnnw");
		table[39] = make_pair('V', "nwwnnnnnw");
		table[40] = make_pair('W', "wwwnnnnnn");
		table[41] = make_pair('X', "nwnnwnnnw");
		table[42] = make_pair('Y', "wwnnwnnnn");
		table[43] = make_pair('Z', "nwwnwnnnn");
	}
	
	~BarcodeTable()
	{

	}

	vector<pair<char, string>> getTable()
	{
		return table;
	}

	unsigned int getSize()
	{
		return size;
	}


};

int main()
{

	system("pause");
	return 0;
}