#include <iostream>
#include <vector>
#include <unordered_map>
#include <string>
#include <map>
#include <algorithm>

using namespace std;

// 레이드
// 0. World2 
// 1. Boss 구현 - Monster를 상속받게, Goblin을 상속받아도 OK
// 2. Boss 한마리, 플레이어 10명이서 싸우기
// - Boss는 AggroSystem을 이용하여 어그로 수치가 제일 높은 3 ~ 4명을 공격
// Player*	damage
// 0x1111...700
// 0x1213   500
// 0x1210...300
// 0x1110...1000 ... 죽으면

class Player
{
public:
	friend class Pet; // -> Pet이제 Player의 private 영역의 멤버함수, 멤버변수를 쓸 수 있게 된다.

	Player(int a, int b)
	{
		cout << "타입변환 생성자" << endl;
	}

	virtual void Attack() { cout << "Attack" << endl; };

private:
	int _hp;
};

class Knight : public Player
{
public:
	Knight()
	: Player(10,5)
	{

	}

	virtual void Attack() final override { cout << "Knight Attack!" << endl; };
};

class Pet
{
public:
	void Set(Player* p) { this->p = p; }
	void Act() 
	{ 
		p->_hp = 0;
	}

private:
	Player* p;
};

enum class ClassType
{
	NONE,
	KNIGHT,
	MAGE,
	ARCHER
};

enum class PetType
{
	NONE,
	KNIGHT_PET,
	MAGE_PET
};

int main()
{

}