#include <iostream>
#include <deque>
#include <vector>
#include <unordered_map>
#include <stack>
#include <queue>
#include <algorithm>

using namespace std;

// stack
// queue
template <typename T, typename Contanier = deque<T>>
class Stack
{
public:
	Stack() { v.clear(); }
	~Stack() {}

	void push(const T& value)
	{
		v.push_back(value);
	}

	void pop()
	{
		v.pop_back();
	}

	const T& top()
	{
		return v.back();
	}

	bool empty()
	{
		return v.size() == 0;
	}

	size_t size()
	{
		return v.size();
	}

private:
	Contanier v;
};

class Queue
{
public:
	void push(const int& value)
	{
		l.push_front(value);
	}

	void pop()
	{
		l.pop_front();
	}

	const int& front()
	{
		return l.front();
	}

private:
	deque<int> l;
};

int main()
{
	stack<int> s;
	s.push(1);
	s.push(2);
	s.push(3);
	s.push(4);
	s.push(5);
	s.push(6);

	while (true)
	{
		if (s.empty() == true)
			break;

		// stack 데이터를 활용
		cout << s.top() << endl;
		
		s.pop();
	}

	cout << "----- queue ------" << endl;
	queue<int> q;
	q.push(1);
	q.push(2);
	q.push(3);
	q.push(4);
	q.push(5);
	q.push(6);

	while (true)
	{
		if (q.empty() == true)
			break;

		// queue 의 데이터의 활용
		cout << q.front() << endl;

		q.pop();
	}

	return 0;
}
