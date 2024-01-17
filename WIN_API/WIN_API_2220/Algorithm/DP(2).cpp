#include <iostream>
#include <vector>
#include <windows.h>
#include <algorithm>

using namespace std;

// LIS (Longest Increasing Sequence) : ������ �־����� �� ���� �� �κ����������� ���� ã��
// �κ� ���� ����

// 1 4 3 2 6 8

// �κ� ����
// ������ ��Ű��, �������� ���� ���� �ִ� ����
// 1 4 6 (o)
// 1 3 6 8 (o)
// 4 1 8 2 (x)

// �κ� ���� ����
// �κ� �����̸鼭 ���ҵ��� �þ�� ������ ����
// 1 4 6 (O)
// 1 3 6 8 (O)
// 1 4 3 2 (X)

vector<int> v2 = { 5,6,7,8,1,2,3 };
vector<int> cache = vector<int>(100, -1);

// LIS(5) = 1
// LIS(4) = LIS(5) + 1

int LIS(int pos)
{
	// �������
	if(pos == v2.size() - 1)
		return 1;

	// �޸������̼�
	int& ref = cache[pos];

	if(ref != -1)
		return ref;

	// ���ϱ�
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