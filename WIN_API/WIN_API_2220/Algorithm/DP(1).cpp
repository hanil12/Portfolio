#include <iostream>
#include <vector>
#include <windows.h>
#include <algorithm>
#include <algorithm>

using namespace std;

// ����
// f(n) = f(n-1) + f(n-2) // -> ���� �κ� ����
// nCr = n-1Cr + n-1Cr-1 // -> ���� �κ� ����

vector<vector<int>> cache = vector<vector<int>>(100, vector<int>(100, -1));

int Combination(int n, int r)
{
	// ���� ���
	if (r == 0 || n == r)
		return 1;

	// ĳ��
	if (cache[n][r] != -1)
		return cache[n][r];

	cache[n][r] = Combination(n - 1, r) + Combination(n - 1, r - 1);

	// ���ϱ�
	return cache[n][r];
}

int main()
{
	vector<int> v = { 1,2,3,4,5,6,7 };

	int count = 0;
	while (true)
	{
		// 4 x 3 x 2 x 1
		if (std::next_permutation(v.begin(), v.begin() + 4))
		{
			cout << "���� ��° v" << endl;
			count++;
		}
		else
		{
			cout << count << endl;
			break;
		}
	}

	return 0;
}