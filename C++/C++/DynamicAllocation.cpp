#include <iostream>
#include <Windows.h>

// 지금까지 사용하던 메모리 영역
// Code
// Data
// Stack
// Heap...???
// -> 사용자 관리 메모리
// -> 프로그래머 메모리
// 우리가 직접 제어하는 메모리 영역
// Data영역, Stack영역에 있던 얘들을 우리가 함부로 삭제하고 생성할 수 있었냐?

// Heap 영역에 데이터를 생성해준거니까 다른 지역에서도 다 사용이 가능한게 맞는거죠 ??
// 답: YES
// 그러나 Heap 영역에 할당된 얘들은 무조건 주소값으로만 알 수 있기 때문에 포인터로 접근, 사용 가능

// Data 영역
// 생성: 프로그램 시작할 때
// 삭제: 프로그램 끝날 때
// 전역변수, 정적변수

// Stack 영역
// 생성: 함수 호출, 지역 입장
// 삭제: 함수 반환, 지역 끝
// [반환주소값][지역변수][매개변수]

// Heap 영역
// 생성: malloc,  new,    new[]
// 삭제: free  , delete, delete []
// [new 변수]

// 커널영역에 나 얼마만큼 할당할거야
// -> 얼마만큼 할당한 다음 그 첫번 째 주소를 넘겨줍니다.'
// 즉 반환값이 할당된 영역의 첫번째 주소

// 정적변수
// - static 변수
// 프로그램에 딱 한개만 있는 변수
// 만약에 static이 앞에 붙었다. 모든 전역에서 쓸 수 있는 변수로 바뀝니다.
// -> 클래스 안에 있더라도 static을 붙히면 전역에 쓸 수 있는 변수로 바뀜.

int a = 10;
static int b = 10;

using namespace std;

void Print()
{
	
}

class Pet
{
private:
	const char* _name;
};

// is - a 관계 : 상속
// has - a 관계 : 소유

// class가 다른 클래스를 갖고있다.
// has - a 관계 일 때
class Player
{
public:
	Player()
	: _hp(0)
	, _pet(nullptr)
	{
		_pet = new Pet();
		cout << "생성자 호출" << endl;
	}
	~Player()
	{
		cout << "파괴자 호출" << endl;
	}

	void Swap(Pet* otherPet)
	{
		delete _pet;

		_pet = otherPet;
	}

	// static 변수를 제어할 때 static 함수를 써야한다.
	static void SetMaxHp(int hp)
	{
		maxHp = hp;
		cout << maxHp << endl;
	}

	static int GetMaxHp() { return maxHp; }

	static int maxHp; // 모든 플레이어가 공유하는 MaxHp

private:
	int _hp;
	// Pet pet; // -> 메모리도 많이 차지하고, 갖고 있지 않은 경우 말할 때도 메모리를 차지한다.
	Pet* _pet; // 펫의 포인터를 갖고 있는 플레이어, 메모리 차지하는 건 4바이트

};

// 초기화 여기서??
int Player::maxHp = 10;

int main()
{
#pragma region 동적할당
	// C
	// 1000바이트만큼 Heap영역에 할당해주세요.
	// (void*) -> 만능 주소
	// 포인터 앞에 데이터형 : 포탈 타고 가면 나오는 데이터형의 힌트
	void* ptr = malloc(1000);
	// 4바이트만큼 int형으로 10 바꿔주세요.
	*(int*)ptr = 10;
	*((int*)ptr + 1) = 1;


	// new delete
	// Heap에 가서 랜덤으로 4바이트만큼 할당자리를 찾아서 거기에 할당
	int* intPtr = new int();
	*intPtr = 10;

	// new[] delete []

	int* arrPtr = new int[250];
	arrPtr[3] = 3;

	// 삭제하는 부분
	// 삭제하는 부분이 누락되면 당장에는 큰 일이 일어나지는 않지만
	// 이게 계속 누적되다보면 프로그램이 뻗는다.

	free(ptr);
	delete intPtr;
	delete[] arrPtr;
#pragma endregion
	
#pragma region Dangling Pointer
	int* receive = nullptr;
	{
		int* intPtr = new int();
		(*intPtr) = 5;
		receive = intPtr;
	}

	//cout << (*receive) << endl; // 댕글링 포인터

	delete receive;
#pragma endregion

	static int b = 5;
	Pet pet;

	Player* player1 = new Player();
	// player1->pet = pet; // 복사 대입 연산자..
	//player1->_pet = &pet; // ?? 아니 원래 player1이 들고 있던 펫이 있었는데 걔(new Pet())는 어떻하죠?

	cout << b << endl;

	cout << player1->maxHp << endl;
	player1->SetMaxHp(50);
	cout << player1->maxHp << endl;

	Player::maxHp = 1000;
	cout << player1->maxHp << endl;

	delete player1;

	return 0;
}
