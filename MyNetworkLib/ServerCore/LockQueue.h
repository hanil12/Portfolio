#pragma once

template<typename T>
class LockQueue
{
public:
	void push(T item)
	{
		WRITE_LOCK;
		_items.push(item);
	}

	T pop()
	{
		WRITE_LOCK;
		if (_items.empty())
			return T();

		T ret = _items.front();
		_items.pop();

		return ret;
	}

	void PopAll(OUT Vector<T>& items)
	{
		WRITE_LOCK;
		while(T item = pop())
			items.push_back(item);
	}

	void Clear()
	{
		WRITE_LOCK;
		_items = Queue<T>();
	}

private:
	USE_LOCK;
	Queue<T> _items;
};