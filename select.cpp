#include<iostream>
#include<fstream>
#include<vector>
#include<tuple>

using namespace std;
typedef tuple<int, int> i2tuple;

int recurse(vector<ifstream>, vector<i2tuple>, int, int);

int main()
{
	ifstream input;
	vector <ifstream*> datFiles; //???
	vector <i2tuple> idxList;
	ofstream output;
	int numOfFiles;
	int numOfCols;
	//k represents the ith lowest index to be found
	int k;

	//open input.txt and retrieve necessary variables
	input.open("input.txt", binary);
	if (input.is_open())
	{
		input >> numOfFiles;
		input >> numOfCols;
		input >> k;
	}

	//use first number from input.txt to obtain number of .dat files
	for (int i = 1; i <= numOfFiles; i++)
	{
		//add each file to the vector of ifstreams in the format of "1.dat"
		ifstream file = new ifstream(i + ".dat", binary);
		datFiles.push_back(&file);
		idxList.push_back(i2tuple(0, i));
	}

	cout << "The lowest " << k << "th element in data: " << recurse(datFiles, idxList, k) << endl;

	return 0;
}


int recurse(vector<ifstream> datFiles, vector<i2tuple> idxList, int k)
{
	//if top array is "empty" (lower idx > upper idx), remove it from the datFiles vector
	if (get<0>(get<0>(idxList)) > get<0>(get<1>(idxList)))
	{
		datFiles.erase(0);
	}

	vector<int> temp;		//holds the temporary middle idx values
	int greater = 0;
	int lesser = 0;
	//find the midpoint of the topmost vector
	int midPoint = get<0>(datFiles.seekg(get<0>(get<1>(idxList))) / 2);

	//count how many idx's are lesser and greater than the midpoint
	for (int i = 0; i < datFiles.size(); i++)
	{
		int j = get<i>(get<0>(idxList));
		while (get<i>(datFiles.seekg(j) <= midpoint)
		{
			lesser++;
			j++;
		}
		get<i>(temp) = j;
		greater += get<i>(get<1>(idxList)) - get<i>(get<0>(idxList)) - j;

	}

	//use above information to determine 
	if (lesser < greater && k !< lesser)
	{
		for (int i = 0; i < idxList.size(); i++)
		{
			get<i>(get<0>(idxList)) = get<i>(temp);
		}
		if (k <= n/2)
		{
			//TODO: determine n to see if k is on the lower half of idx's
			k = k - lesser;
		}
	}
	else
	{
		for (int i = 0; i < idxList.size(); i++)
		{
			get<i>(get<1>(idxList)) = get<i>(temp);
		}
		if (k > n/2)
		{
			//TODO: determine n to see if k is on the lower half of idx's
			k = k - greater;
		}
	}

	if (greater == 0 && k < lesser)
	{
		int idx = 0;
		for(int i = 0; i < datFiles.size(); i++)
			for (int j = 0; j < get<i>(get<1>(idxList)); j++)
			{
				idx++;
				if (idx == k)
					return get<i>(datFiles.seekg(j));
			}
	}
	
	return recurse(datFiles, idxList, k);
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