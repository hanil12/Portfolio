#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

// 함수포인터
// 함수객체
// 람다함수 []() -> {}
// [] : Capture 람다지역 밖에 있는 변수를 캡쳐
// () : 원소
// -> : 람다함수의 반환형식
// {} : 정의부분

// 1. 10이 있는 iter를 찾아주세요. std::find(v.begin(), v.end(), 10);
// 2. 10보다 작은 값이 있는 첫번 째 iter를 찾아주세요.  std::find_if
// 3. 모든 원소가 짝수인지 확인해주세요. std::all_of
// 4. 홀수인 얘들의 개수를 확인해주세요  count_if
// 5. 하나라도 홀수가 있는지 확인해주세요 any of
// 6. 10인 첫번째로 나온 얘를 지워주세요 : remove
// 7. 10보다 작은 원소를 지워주세요. : remove_if

// remove는 지우는게 아니고 move를 한다.
// 

int main()
{
	vector<int> v;

	for (int i = 0; i < 15; i++)
	{
		v.push_back(i);
	}

	// 1. 10이 있는 iter를 찾아주세요.
	//auto find10Iter = std::find(v.begin(), v.end(), 10);

	// 2. 10보다 작은 값이 있는 첫번 째 iter를 찾아주세요

	// 함수포인터
	// 함수객체
	// 람다함수 []() -> {}
	// [] : Capture 람다지역 밖에 있는 변수를 캡쳐
	// () : 원소
	// -> : 람다함수의 반환형식
	// {} : 정의부분

	//int capture = 20;
	//auto find10Iter = std::find_if(v.begin(), v.end(), [capture](int element) -> bool
	//	{
	//		if (element < -10)
	//		{
	//			cout << element << endl;
	//			return true;
	//		}
	//		else
	//			return false;
	//	});
	//cout << *find10Iter << endl;

	// 3. 모든 원소가 짝수인지 확인해주세요
	bool even = std::all_of(v.begin(), v.end(), [](int element) { return element % 2 == 0; });

	// 4. 홀수인 얘들의 개수를 확인해주세요  count
	int a = std::count_if(v.begin(), v.end(), [](int element) { return element % 2 == 1; });

	// 5. 하나라도 홀수가 있는지 확인해주세요 any of
	bool anyOdd = std::any_of(v.begin(), v.end(), [](int element) { return element % 2 == 1; });

	// 7. 10보다 작은 원소를 지워주세요. : remove_if
	//auto iter = std::remove_if(v.begin(), v.end(), [](int element) { return element < 10; });
	// 진짜 지우기
	v.erase(std::remove_if(v.begin(), v.end(), [](int element) { return element < 10; }), v.end());

	return 0;
}