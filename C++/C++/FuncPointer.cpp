#include <iostream>
#include <functional>

using namespace std;

void PrintHello()
{
	cout << "Hello World!" << endl;
}

void PrintHello(int a, int b)
{
	cout << "Hello World!2" << endl;
}

class Player
{
public:
	void PrintPlayer()
	{
		cout << "Hello Player" << endl;
	}

private:
};

class Effect;

template<typename T>
using EffectEvent = void(T::*)(void);

using EffectDelegate = std::function<void(int,int)>;

class Action
{
public:
	void EndAction(Effect* effect)
	{
		cout << "Action 끝" << endl;
		// Effect의 함수를 넘겨서
		if (_event != nullptr)
		{
			(effect->*_event)();
			// 어떤 클래스의 멤버함수를 호출할려면 객체가 있어야함 
			// _event();
		}
	}

	void EndAction2()
	{
		cout << "Action 끝" << endl;
		// Effect의 함수를 넘겨서
		if (_event2 != nullptr)
		{
			_event2(5,2);
			// 어떤 클래스의 멤버함수를 호출할려면 객체가 있어야함 
			// _event();
		}
	}

	EffectEvent<Effect> _event;
	EffectDelegate _event2;
};

class Animation
{
public:

	void Print()
	{
		cout << "Animation이 끝났습니다." << endl;
	}
};

class Effect
{
public:
	Effect()
	{
		_action = new Action();
	}
	~Effect()
	{
		delete _action;
	}

	void Play()
	{
		cout << "Play Effect" << endl;
		_action->EndAction(this);
	}

	void Play2() 
	{
		cout << "Play Effect2" << endl;
		_action->EndAction2();
	}

	void EndEffect()
	{
		cout << "Effect 끝" << endl;
		cout << "Effect 출력중지" << endl;
	}

	void EndEffect2(int a, int b)
	{
		cout << a << endl;
		cout << b << endl;
		cout << "Effect~~~~~~~~~~~~~~~~~~~~~~" << endl;
		cout << "Effect 출력중지~~~~~~~~~~" << endl;
	}

	Action* _action;
};

typedef void(*Fn)(void);
using Delegate = void(*)(int, int);

typedef void(Player::* Pfunc)(void);
using PlayerDelegate = void(Player::*)(void);

int main()
{
	//Fn fn;
	//fn = &PrintHello;

	//fn();

	//Delegate fn2;
	//fn2 = &PrintHello;

	//fn2(1, 2);

	//// 멤버함수포인터
	//Player p;
	//Player* p2 = new Player();
	//p.PrintPlayer();

	//PlayerDelegate playerFn;
	//playerFn = &Player::PrintPlayer;
	//(p.*playerFn)();
	//(p2->*playerFn)();
	//*. => ->

	// 통짜로 멤버함수포인터 쓰는 방법
	// -> 멤버함수를 쓰려면 객체가 필요하니까, 
	//void Play()
	//{
	//	cout << "Play Effect" << endl;
	//	_action->EndAction(this);
	//}
	// 이런식으로 자기자신 객체까지 넘겨줘야 호출이 가능하고
	Effect* effect = new Effect();
	effect->_action->_event = &Effect::EndEffect;
	effect->Play();

	// std::function 을 쓰는 방법
	// -> 멤버함수를 쓰려면 객체가 필요한데
	// bind(&Effect::EndEffect2,effect); 
	// bind 의 첫번 째 매개변수로 객체를 묶어서 그 객체의 멤버함수 호출

	// (5,2)
	// 1, std::placeholders::_1  => 1 > (5,2) ... (1,5)
	// 1, std::placeholders::_2  =>  ... (2,1)
	effect->_action->_event2 = bind(&Effect::EndEffect2, effect, std::placeholders::_1, std::placeholders::_2);
	effect->Play2();

	Action* action = new Action();
	Animation* animation = new Animation();
	// action->_event = &Animation::Print;

	return 0;
}