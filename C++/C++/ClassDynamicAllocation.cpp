#include <iostream>

using namespace std;

// 얕은 복사
// 클래스에서 동적할당이 되어있지 않는 얘들을 그저 '복사만' 하는 용도
// 
// 깊은 복사
// 클래스에서 동적할당이 되어있는 포인터들을 '복사만 하는게 아니라' 복사생성을 해서 새로운 얘를 가르키게끔 하는 용도

class Pet
{

};

class Player
{
public:
	Player()
	: _hp(0)
	{
		_pet = new Pet();
	}

	~Player()
	{
		cout << "파괴자 호출" << endl;
		delete _pet;
		cout << "pet 파괴" << endl;
	}

	// 깊은 복사
	// 복사생성자
	Player(const Player& other)
	{
		cout << "복사생성자 호출" << endl;
		_hp = other._hp;
		_pet = new Pet(*(other._pet));
	}

private:
	int _hp;
	Pet* _pet;
};

int main()
{
	Player player1;
	Player player2 = Player(player1);

	// 리턴하면서 터진다?
	// 살펴봤더니 player1은 파괴자가 잘 호출되서 잘 삭제가 되었는데
	// player2가 삭제될 때 delete _pet 하니까 터짐...?

	return 0;
}