#include <iostream>
#include <deque>
#include <vector>
#include <unordered_map>
#include <stack>
#include <queue>
#include <algorithm>

using namespace std;

// 최대힙
template <typename T, typename Container = vector<T>, typename Pred = less<T>>
class Priority_Queue
{
public:
	void push(const T& value)
	{
		v.push_back(value);

		// 규칙 확인

		int now = static_cast<int>(v.size() - 1); // 맨 마지막

		while (true)
		{
			if (now <= 0)
				break;

			int parent = (now - 1) / 2;

			if (Pred()(v[now],v[parent]))
			{
				break;
			}

			std::swap(v[now], v[parent]);
			now = parent;
		}
	}

	void pop()
	{
		v[0] = v.back();
		v.pop_back();

		// 규칙 확인
		int now = 0;

		while (true)
		{
			int leftChild = 2 * now + 1;
			int rightChild = 2 * now + 2;

			if (leftChild >= (int)v.size())
				break;

			int next = now;

			if (Pred()(v[next],v[leftChild]))
				next = leftChild;

			if (rightChild < (int)v.size() && Pred()(v[next],v[rightChild]))
				next = rightChild;

			if (next == now)
				break;

			std::swap(v[now], v[next]);
			now = next;
		}
	}

	const T& top()
	{
		return v[0];
	}

	bool empty()
	{
		return v.empty();
	}

private:
	Container v;
};

int main()
{
	priority_queue<int, vector<int>, greater<int>> stdPq;
	Priority_Queue<int, vector<int>, greater<int>> pq;

	pq.push(10);
	pq.push(7);
	pq.push(6);
	pq.push(3);
	pq.push(15);

	cout << pq.top() << endl;
	pq.pop();
	cout << pq.top() << endl;
	pq.pop();
	cout << pq.top() << endl;

	return 0;
}