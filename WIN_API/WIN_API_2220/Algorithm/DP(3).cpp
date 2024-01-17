#include <iostream>
#include <vector>
#include <windows.h>
#include <algorithm>

using namespace std;

// ENCHANT

// +0 �����

// ��ȭ�� �ϴµ�
// + 1
// + 2
// + 3

// N ���� �ϰ� �ʹ�... �� �� ��ȭ�ϴ� ��� ������ ��� ����� ��
// 3���� �ϰ� �ʹ� -> 
// 1 1 1
// 1 + 2
// 2 + 1
// 3

int NUM = 3;
vector<int> cache = vector<int>(100, -1);

int Enchant(int num)
{
	// ����
	if (num > NUM)
		return 0;
	if (num == NUM)
		return 1;

	// ĳ��
	int& ref = cache[num];
	if (ref != -1)
		return ref;

	// ���ϱ�
	ref = Enchant(num + 1) + Enchant(num + 2) + Enchant(num + 3);
	return ref;
}

int main()
{
	int result = Enchant(0);

	cout << result << endl;

	return 0;
}