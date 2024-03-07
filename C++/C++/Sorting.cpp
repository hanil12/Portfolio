// 순차정렬 n^2
// 선택, 버블, 삽입 n^2
// Quick Sort nlogn 
// Merge Sort nlogn
// Heap Sort  nlogn 최대 최소부터 몇개까지만 정렬해주세요

#include <iostream>

using namespace std;

void Swap(int& a, int& b)
{
	int temp = a;
	a = b;
	b = temp;
}

// 순차정렬
// [5] [4] [3] [2] [1] ... 0과 1 자리 비교
// [4] [5] [3] [2] [1] ... 스왑, 1과 2자리 비교
// [3] [5] [4] [2] [1]
// [2] [5] [4] [3] [1]
// [1] [5] [4] [3] [2]

// [1] [4] [5] [3] [2]
// ...
// [1] [2] [5] [4] [3]
void SequenceSort(int* arr, int arrSize)
{
	for (int i = 0; i < arrSize; i++)
	{
		for (int j = i + 1; j < arrSize; j++)
		{
			if (arr[i] > arr[j])
				Swap(arr[i], arr[j]);
		}
	}
}

// 선택정렬
// [5] [4] [3] [2] [1] ... [0],[1] 비교 bestIndex 1
// [5] [4] [3] [2] [1] ... [1],[2] 비교 bestIndex 2
// [5] [4] [3] [2] [1] ... [2],[3]
// [5] [4] [3] [2] "[1]" ... ...제일 작은 bestIndex은 4
// [1] [4] [3] [2] [5] ... 스왑

void SelectSort(int arr[], int arrSize)
{
	for (int i = 0; i < arrSize; i++)
	{
		int bestIndex = i;
		for (int j = i + 1; j < arrSize; j++)
		{
			if (arr[bestIndex] > arr[j])
				bestIndex =j;
		}
		Swap(arr[i], arr[bestIndex]);
	}
}

// 버블 : 두개씩 비교해서 서로 바꿔치기하면서 쭉 올라가는 얘.
// [5][3][2][1][4]  
// [3][5][2][1][4]
// [3][2][5][1][4]
// [3][2][1][5][4]
// [3][2][1][4][5]

// [2][3][1][4][5]
// [2][1][3][4][5]
void BubbleSort(int* arr, int arrSize)
{
	for (int i = 0; i < arrSize; i++)
	{
		for (int j = 0; j < arrSize - i - 1; j++)
		{
			if (arr[j] > arr[j + 1])
			{
				Swap(arr[j], arr[j + 1]);
			}
		}
	}
}

void PrintArr(int* arr, int arrSize)
{
	for (int i = 0; i < arrSize; i++)
	{
		cout << arr[i] << endl;
	}
}

bool findNum(int* arr, int arrSize, int findNum)
{
	for (int i = 0; i < arrSize; i++)
	{
		if (arr[i] == findNum)
		{
			return true;
		}
	}
	return false;
}


// 로또번호 만들기
void MakeLotto()
{
	srand((unsigned int)time(nullptr));

	int arr[7] = {};

	int count = 0;

	while (true)
	{
		int pushNum = rand() % 45 + 1;
		
		if (findNum(arr, 7, pushNum))
			continue;

		arr[count] = pushNum;

		count++;
		if (count == 7)
			break;
	}

	BubbleSort(arr, 7);
	PrintArr(arr, 7);
}

int main()
{
	MakeLotto();

	return 0;
}