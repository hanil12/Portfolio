#include <iostream>
#include <vector>
#include <windows.h>
#include <algorithm>

using namespace std;

// LIS (Longest Increasing Sequence) : 수열이 주어졌을 때 제일 긴 부분증가수열의 길이 찾기
// 부분 증가 수열

// 1 4 3 2 6 8

// 부분 수열
// 순서는 지키되, 인접하지 않을 수도 있는 수열
// 1 4 6 (o)
// 1 3 6 8 (o)
// 4 1 8 2 (x)

// 부분 증가 수열
// 부분 수열이면서 원소들이 늘어나는 형태의 수열
// 1 4 6 (O)
// 1 3 6 8 (O)
// 1 4 3 2 (X)

vector<int> v2 = { 5,6,7,8,1,2,3 };
vector<int> cache = vector<int>(100, -1);

// LIS(5) = 1
// LIS(4) = LIS(5) + 1

int LIS(int pos)
{
	// 기저사례
	if(pos == v2.size() - 1)
		return 1;

	// 메모이제이션
	int& ref = cache[pos];

	if(ref != -1)
		return ref;

	// 구하기
	ref = 1;
	for (int i = pos + 1; i < v2.size(); i++)
	{
		if(v2[pos] <= v2[i])
			ref = max(ref, LIS(i) + 1);
	}

	return ref;
}

int main()
{
	int lis = 0;

	for (int i = 0; i < v2.size(); i++)
	{
		lis = max(lis, LIS(i));
	}

	cout << lis << endl;

	return 0;
}