#include <iostream>
#include <vector>
#include <windows.h>
#include <algorithm>

using namespace std;

// Dynamic Programming
// 컴퓨터 공학 : 동적 -> 실행 시간에 할 수 있는 무언가
// => 이름이 멋있어서

// f(0) = 1
// f(1) = 1
// f(n) = f(n-1) + f(n-2)

// 1. 완전탐색
// 배열 인덱스 탐색
// DFS
// 트리... 재귀호출

// 2. 최적 부분 문제를 찾는다.

// 3. 메모이제이션

vector<int> cache = vector<int>(100,-1);

// f(0) = 1
// f(1) = 1
// f(n) = f(n-1) + f(n-2)

int fibonacci(int n, int& count)
{
	// 기저사항
	if (n == 0 || n == 1)
		return 1;
	if (n == 2)
		count++;

	// 메모이제이션
	// 캐시
	if (cache[n] != -1)
		return cache[n];

	// 구하기
	cache[n] = fibonacci(n - 1, count) + fibonacci(n - 2, count);
	return cache[n];
}

int main()
{
	__int64 start = GetTickCount64();
	int count = 0;

	cout << fibonacci(99, count) << endl;
	cout << count << endl;
	__int64 end = GetTickCount64();

	cout << end - start << "ms" << endl;

	return 0;
}