#include <iostream>

using namespace std;

// this
// this는 자기 자신의 주소를 담고있는 포인터

// 복사생성자
// Player (const Player& player)
// 매개변수를 읽기전용으로 갖고 올 것이기 때문에 const Player& 형식으로 갖고온다.
// 하는 일: 매개변수의 정보를 복사해서 자기한테 입력한다.

// 복사생성자 호출하는 2가지 방법
//Player player2(player);
//Player player2 = player;

// 과제
// + - /               3
// 전위 후위 --        3
// pos3 = pos1 * pos2; // (2,2) * (5,5) = (10,10) 구현 1
// 총 7개 오버로딩

// 굳이 오버로딩 안해도 되는 경우가 있다.

class Position
{
public:
	// 기본생성자
	Position()
	: _x(0)
	, _y(0)
	{
	}
	// 파괴자
	~Position() {}

	// 타입변환생성자
	Position(int x, int y)
	: _x(x)
	, _y(y)
	{
	}
	// 복사생성자 (암시적 생성자)
	Position(const Position& pos)
	{
		_x = pos._x;
		_y = pos._y;
	}
	// 연산자 오버로딩

	Position operator*(int val)
	{
		Position result;
		this->_x = _x * val;
		this->_y = _y * val;
		
		return *this;
	}

	// 전위 ++pos
	// 값을 먼저 더하고 그 뒤에 반환합니다.
	Position& operator++()
	{
		_x++;
		_y++;

		return *this;
	}

	// 후위 pos++
	// 값을 반환하고 그 뒤에 연산
	Position operator++(int)
	{
		Position temp(*this);
		_x++;
		_y++;
		return temp;
	}

	// 복사대입연산자
	Position& operator=(const Position& pos)
	{
		_x = pos._x;
		_y = pos._y;

		return *this;
	}

	Position& operator=(int value)
	{
		_x = value;
		_y = value;

		return *this;
	}

public:
	int _x;
	int _y;
};

// 연산자 오버로딩을 굳이 멤버함수에서 하지 않아도 된다.
Position operator+(Position p1, Position p2)
{
	Position result;
	result._x = p1._x + p2._x;
	result._y = p1._y + p2._y;

	return result;
}


void PrintPosition(Position p)
{
	cout << p._x << "," << p._y << endl;
}

int main()
{
	Position pos1(1 ,1);
	Position pos2(5, 5);
	Position pos3(pos1); // 복사생성자 호출
	Position pos4 = pos2; // 복사생성자 호출

	pos1 = pos1 * 5;
	PrintPosition(pos1);
	pos1++;
	PrintPosition(pos1);
	++pos1;
	PrintPosition(pos1);

	//pos3 = pos1 * pos2; // (2,2) * (5,5) = (10,10)
	pos1 = pos2 + pos3;

	pos2 = pos1;

	int a = 10;
	int b = 5;
	int c = 15;

	// 대입연산자: a에다가 b를 대입하고 a의 원본을 반환한다.
	(c = (a = b)) = 5;
	c = a++;

	pos2 = (pos1 = (pos3 = 5));
	pos2 = pos1++;

	return 0;
}