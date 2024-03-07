#include <iostream>

using namespace std;

class Player
{
public:
	Player()
	: _hp(0)
	{
		cout << "Player 생성자 호출" << endl;
	}

	~Player()
	{
		cout << "Player 파괴자 호출" << endl;
	}

	Player(const Player& other)
	{
		cout << "복사생성자 호출" << endl;
		_hp = other._hp;
	}

protected:
	int _hp;
};

class Warrior :public Player
{
public:
	Warrior()
	{
		cout << "Warrior 생성자 호출" << endl;
	}

	~Warrior()
	{
		cout << "Warrior 파괴자 호출" << endl;
	}

private:

};

class Mage : public Player
{
public:

};

int main()
{
	// Player* playerPtr
	// playerPtr를 타고 넘어가면 Player의 객체가 있을 것이다. 

	// new Warrior()
	// 1층은 Player() 구성되어있고
	// 2층 Warrior()로 구성되어있는 건물에서
	// 1층의 주소를 넘기겠다.
	Player* playerPtr = new Warrior();
	Player* playerPtr2 = new Mage();

	Player* playerArr[5];

	for (int i = 0; i < 5; i++)
	{
		if ((i % 2) == 0)
			playerArr[i] = new Warrior();
		else
			playerArr[i] = new Mage();
	}
	// playerArr[0] 는 Warrior
	// playerArr[1] 는 Mage
	// playerArr[2] 는 Warrior
	// playerArr[3] 는 Mage
	// playerArr[4] 는 Warrior
	
	// => Player* 배열로 Mage랑 Warrior를 동시에 관리할 수 있게됬습니다.

	// 파괴자
	

	return 0;
}