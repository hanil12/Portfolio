#include <iostream>

using namespace std;


void Print()
{
	cout << "HelloWorld" << endl;
}

class Player;

class Dog
{
public:
	void Move() { cout << "Move" << endl; }
	void Bark(Player* player)
	{
		if (_event != nullptr)
		{
			(player->*_event)();
			cout << "주인을 보며 짖습니다." << endl;
		}
	}

	typedef void(Player::* PlayerFunc)(void);
	PlayerFunc _event;
};

class Player
{
public:
	Player()
	{
		_dog = new Dog();
		_dog->_event = &Player::Move;
	}
	~Player()
	{
		delete _dog;
	}
	void Move() { cout << "Move" << endl; }
	
public:
	Dog* _dog;
};

typedef void(*FN)(void);
typedef void(Player::* PFN)(void);

int main()
{
	// 메인함수에서 전역함수를 담는 함수포인터 변수 만들기
	FN fn;
	fn = &Print;

	fn();

	// 메인함수에서 Player클래스의 멤버함수를 담는 함수포인터 변수 만들기
	// -> 클래스 안의 멤버함수를 호출할려면 호출규약이 일반함수랑 다르기 때문에
	// -> 즉 객체가 필요하다.
	PFN playerFn;
	playerFn = &Player::Move; // & 생략 불가능

	Player p;
	(p.*playerFn)();
	p._dog->Bark(&p);

	// 대신에 이렇게 했을 경우 Dog의 Move는 못받는다. 왜냐
	// -> 소속이 다르기 때문에

	// 우리가 

	return 0;
}