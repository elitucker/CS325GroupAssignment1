#include<iostream>
#include<fstream>
#include<vector>
#include<tuple>

using namespace std;
typedef tuple<int, int> i2tuple;

int recurse(vector<ifstream*>, vector<i2tuple>, int, int);
int binarySearch(ifstream *datFile, i2tuple indices, int midpoint);

int main()
{
	ifstream input;
	vector <ifstream*> datFiles; //???
	vector <i2tuple> idxList;
	ofstream output;
	int numOfFiles;
	int numOfCols;
	int k; //kth smallest index
	int n;

	//open input.txt and retrieve necessary variables
	input.open("input.txt", ios::binary);
	if (input.is_open())
	{
		input >> numOfFiles;
		input >> numOfCols;
		input >> k;
	}
	else
	{
		return -1;
	}

	n = numOfFiles * numOfCols;

	//use first number from input.txt to obtain number of .dat files
	for (int i = 1; i <= numOfFiles; i++)
	{
		//add each file to the vector of ifstreams in the format of "1.dat"
		ifstream *file = new ifstream(to_string(i) + ".dat", ios::binary);
		datFiles.push_back(file);
		idxList.push_back(i2tuple(0, i));
	}

	cout << "The lowest " << k << "th element in data: " << recurse(datFiles, idxList, k, n) << endl;

	return 0;
}

int recurse(vector<ifstream*> datFiles, vector<i2tuple> idxList, int k, int n)
{
	if (n == 1)
	{
		for (int i = 0; i < datFiles.size(); i++)
		{
			if (get<0>(idxList[i]) <= get<1>(idxList[i]))
			{
				return (*datFiles[i]).seekg(get<0>(idxList[i])).get();
			}
			return -1;
		}
	}

	vector<int> midIndices; //holds the temporary middle idx values
	int greater = 0;
	int lesser = 0;
	//find the midpoint of the topmost vector
	int midpoint = (*datFiles[0]).seekg((get<1>(idxList[0]) - get<0>(idxList[0])) / 2).get();

	//count how many idx's are lesser and greater than the midpoint
	for (int i = 0; i < datFiles.size(); i++)
	{
		if (get<0>(idxList[i]) <= get<1>(idxList[i]))
		{
			int mid = binarySearch(datFiles[i], idxList[i], midpoint);
			lesser += mid - get<0>(idxList[i]);
			greater += get<1>(idxList[i]) -  mid + 1;
			midIndices.push_back(mid);
		}
	}

	//use above information to determine
	if (k > lesser)
	{
		for (int i = 0; i < idxList.size(); i++)
		{
			get<0>(idxList[i]) = midIndices[i];
		}
		k -= lesser;
		n -= lesser;
	}
	else
	{
		for (int i = 0; i < idxList.size(); i++)
		{
			get<1>(idxList[i]) = midIndices[i] - 1;
		}
		n -= greater;
	}

	return recurse(datFiles, idxList, k, n);
}

int binarySearch(ifstream *datFile, i2tuple indices, int midpoint)
{
	int low = get<0>(indices);
	int high = get<1>(indices);
	int mid = 0;
	while (low != high) //when low == high, both hold index of smallest value >= midpoint
	{
		mid = (low + high) / 2;
		if ((*datFile).seekg(mid).get() < midpoint)
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

/*

pick an array
middle element of largest array
binary search through other arrays to find that value or value <
find indexes of those elements

k = 10

M = 3

1, 2, 3, 4, 5
2, 4, 6, 8, 10
3, 6, 9, 12, 15

3 + 1 + 1 = 5
2 + 4 + 4 = 10

10 - 5 = 5 = k

M = 4

4, 5
4, 6, 8, 10
6, 9, 12, 15

1 + 1 + 0 = 2
1 + 3 + 4 = 8

5 - 2 = 3 = k

M = 5

5
6, 8, 10
6, 9, 12, 15

1 + 0 + 0 = 1
0 + 3 + 4 = 7

3 - 1 = 2 = k

M = 8

6, 8, 10
6, 9, 12, 15

2 + 1 + 0 = 3
1 + 3 + 0 = 4

if k < L then eliminate G
DO NOT REDUCE K AT THIS POINT

lower idx = 3
upper idx = 4
6, 8
6

M = 6

1 + 1 = 2
1 + 0 = 1

2 - 1 = 1 = k

6
6

*/
