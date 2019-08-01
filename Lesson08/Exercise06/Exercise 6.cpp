#include <iostream>
#include <time.h>
#include <iomanip>
#include <algorithm>
#include <utility>
#include <vector>

#define DEBUG 0

#if DEBUG
#define PRINT(x) cerr << x
#else
#define PRINT(x)
#endif

using namespace std;

vector<vector<pair<int, int>>> found;

int LCS_BruteForce(string A, string B, int i, int j, vector<pair<int, int>> subsequence)
{
	if(i >= A.size() || j >= B.size())
	{
		found.push_back(subsequence);

		//Terminate recursion and return length of subsequence
		return subsequence.size();
	}

	// if A[i] = B[j]:
	if(A[i] == B[j])
	{
		// Increase length of subsequence by 1
		subsequence.push_back({ i, j });

		// Increment both i and j by 1
		return LCS_BruteForce(A, B, i + 1, j + 1, subsequence);
	}
	/*
	 Option 1) Explore further possibilities with i + 1, and j
	 Option 2) Explore further possibilities with i, and j + 1

	 LCS from this state is equal to maximum value of Option 1 and Option 2
	 */

	return max(LCS_BruteForce(A, B, i + 1, j, subsequence),
			   LCS_BruteForce(A, B, i, j + 1, subsequence));
}

void PrintSubsequences(string A, string B)
{
	// Lambda function for custom sorting logic
	sort(found.begin(), found.end(), [](auto a, auto b)
	 {
		 // Sort subsequences by length
		 if(a.size() != b.size())
		 {
			 return a.size() < b.size();
		 }

		 // Sort subsequences of same size by lexicographical order of index
		 return a < b;
	 });

	found.erase(unique(found.begin(), found.end()), found.end());

	int previousSize = 0;

	for(auto subsequence : found)
	{
		if(subsequence.size() != previousSize)
		{
			previousSize = subsequence.size();
			PRINT("SIZE = " << previousSize << endl);
		}
		// Fill with underscores as placeholder characters
		string a_seq(A.size(), '_');
		string b_seq(B.size(), '_');

		for(auto pair : subsequence)
		{
			// Fill in the blanks with the characters of each string that are part of the subsequence

			a_seq[pair.first] = A[pair.first];
			b_seq[pair.second] = B[pair.second];
		}
		PRINT("\t" << a_seq << " " << b_seq << endl);
	}
}

const int UNKNOWN = -1;

int LCS_Memoization(string A, string B, int i, int j, vector<vector<int>> &memo)
{
	// Base case -- LCS is always zero for empty strings
	if(i == 0 || j == 0)
	{
		return 0;
	}

	// Have we found a result for the prefixes of the two strings?
	if(memo[i - 1][j - 1] != UNKNOWN)
	{
		// If so, return it
		return memo[i - 1][j - 1];
	}

	// Are the last characters of A's prefix and B's prefix equal?
	if(A[i-1] == B[j-1])
	{
		// LCS for this state is equal to 1 plus the LCS of the prefixes of A and B, both reduced by one character
		memo[i - 1][j - 1] = 1 + LCS_Memoization(A, B, i - 1, j - 1, memo);

		// Return the cached result
		return memo[i - 1][j - 1];
	}
	// If the last characters are not equal, LCS for this state is equal to the maximum LCS of A’s prefix reduced by one character and B’s prefix, and B’s prefix reduced by one character and A’s prefix
	memo[i - 1][j - 1] = max(LCS_Memoization(A, B, i - 1, j, memo),
							 LCS_Memoization(A, B, i, j - 1, memo));

	return memo[i - 1][j - 1];
}

void GetTime(clock_t &timer)
{
	timer = clock() - timer;

	cout << "TIME TAKEN: " << fixed << setprecision(5) << (float)timer / CLOCKS_PER_SEC << " SECONDS" << endl;

	timer = clock();
}

int main()
{
	string A, B;
	cin >> A >> B;

	int tests = 2;

	clock_t timer = clock();

	for(int i = 0; i < tests; i++)
	{
		int LCS;

		switch(i)
		{
			case 0:
			{
				LCS = LCS_BruteForce(A, B, 0, 0, {});

			#if DEBUG
				PrintSubsequences(A, B);
			#endif

				break;
			}
			case 1:
			{
				vector<vector<int>> memo(A.size(), vector<int>(B.size(), UNKNOWN));
				LCS = LCS_Memoization(A, B, A.size(), B.size(), memo);
				break;
			}
		}
		cout << "Length of the longest common subsequence of " << A << " and " << B << " is: " << LCS << endl;

		GetTime(timer);
	}
	return 0;
}
