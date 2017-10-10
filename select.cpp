#include<iostream>
#include<sstream>
#include<fstream>
#include<vector>
#include<tuple>
#include<climits>

#define BYTE_COUNT 4

using namespace std;
typedef tuple<uint, uint> i2tuple;

uint recurse(vector<ifstream*>, vector<i2tuple>, uint, uint);
uint binarySearch(ifstream *datFile, i2tuple indices, uint midpoint);
uint swapByteOrder(uint num);

int main()
{
	ifstream input;
	vector <ifstream*> datFiles; //???
	vector <i2tuple> idxList;
	ofstream output;
	uint numOfFiles;
	uint numOfCols;
	uint k; //kth smallest index
	uint n;

	//open input.txt and retrieve necessary variables
	input.open("input.txt");
	if (input.is_open())
	{
		string line;
		if (!getline(input, line))
		{
			cout << "Invalid input format" << endl;
			return 1;
		}

		istringstream stream(line);
		string value;

		getline(stream, value, ',');
		numOfFiles = atoi(value.c_str());
		getline(stream, value, ',');
		numOfCols = atoi(value.c_str());
		getline(stream, value, ',');
		k = atoi(value.c_str());
	}
	else
	{
		cout << "No input file(s)" << endl;
		return 1;
	}

	n = numOfFiles * numOfCols;

	//use first number from input.txt to obtain number of .dat files
	for (uint i = 1; i <= numOfFiles; i++)
	{
		//add each file to the vector of ifstreams in the format of "1.dat"
		ifstream *file = new ifstream(to_string(i) + ".dat", ios::binary);
		datFiles.push_back(file);
		idxList.push_back(i2tuple(0, numOfCols - 1));
	}

	cout << "The lowest " << k << "th element in data: " << recurse(datFiles, idxList, k, n) << endl;

	return 0;
}

uint recurse(vector<ifstream*> datFiles, vector<i2tuple> idxList, uint k, uint n)
{

	uint midpoint;

	if (k == 1)
	{
		uint value = UINT_MAX;
		for (uint i = 0; i < datFiles.size(); i++)
		{
			if (get<0>(idxList[i]) <= get<1>(idxList[i]))
			{
				(*datFiles[i]).seekg(get<0>(idxList[i]) * BYTE_COUNT).read((char *)&midpoint, BYTE_COUNT);
				midpoint = swapByteOrder(midpoint);
				value = (midpoint < value ? midpoint : value);
			}
		}
		return value;
	}

	vector<uint> midIndices; //holds the temporary middle idx values

	uint idx;

	//find the midpoint of the topmost vector
	for (idx = 0; idx < datFiles.size(); idx++)
	{
		if (get<0>(idxList[idx]) <= get<1>(idxList[idx]))
		{
			(*(datFiles[0])).seekg(((get<1>(idxList[idx]) - get<0>(idxList[idx])) / 2 + get<0>(idxList[idx])) * BYTE_COUNT).read((char *)&midpoint, BYTE_COUNT);
			midpoint = swapByteOrder(midpoint);
			midIndices.push_back((get<1>(idxList[idx]) - get<0>(idxList[idx])) / 2 + get<0>(idxList[idx]));
			break;
		}
	}

	uint lesser = (get<1>(idxList[idx]) - get<0>(idxList[idx])) / 2;
	uint greater = get<1>(idxList[idx]) - get<0>(idxList[idx]) - lesser + 1;

	//count how many idx's are lesser and greater than the midpoint
	for (idx++; idx < datFiles.size(); idx++)
	{
		if (get<0>(idxList[idx]) <= get<1>(idxList[idx]))
		{
			uint mid = binarySearch(datFiles[idx], idxList[idx], midpoint);
			lesser += mid - get<0>(idxList[idx]);
			greater += get<1>(idxList[idx]) -  mid + 1;
			midIndices.push_back(mid);
		}
	}

	//use above information to determine
	if (k > lesser)
	{
		for (uint i = 0; i < idxList.size(); i++)
		{
			if (get<0>(idxList[i]) <= get<1>(idxList[i]))
			{
				get<0>(idxList[i]) = midIndices[i];
			}
		}
		k -= lesser;
		n -= lesser;
	}
	else
	{
		for (uint i = 0; i < idxList.size(); i++)
		{
			if (get<0>(idxList[i]) <= get<1>(idxList[i]))
			{
				get<1>(idxList[i]) = midIndices[i] - 1;
			}
		}
		n -= greater;
	}

	return recurse(datFiles, idxList, k, n);
}

uint binarySearch(ifstream *datFile, i2tuple indices, uint midpoint)
{
	uint low = get<0>(indices);
	uint high = get<1>(indices);
	uint mid = 0;
	while (low != high) //when low == high, both hold index of smallest value >= midpoint
	{
		uint compare;
		mid = (low + high) / 2;
		(*datFile).seekg(mid * BYTE_COUNT).read((char *)&compare, BYTE_COUNT);
		compare = swapByteOrder(compare);
		if (compare < midpoint)
		{
			low = mid + 1;
		}
		else
		{
			high = mid;
		}
	}
	return low;
}

uint swapByteOrder(uint num)
{
	char *bytes = (char *) &num;
	return (bytes[0] << 24 | bytes[1] << 16 | bytes[2] << 8 | bytes[3]);
}
