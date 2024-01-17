#include "framework.h"
#include "Player.h"

Player::Player(shared_ptr<Maze> maze)
: _maze(maze)
{
	Init();
}

Player::~Player()
{
}

void Player::Init()
{
	_pos = _maze->GetStartPos();
	_maze->GetBlock(_pos)->SetType(Block::Type::PLAYER);

	_discorvered = vector<vector<bool>>(_poolCountY, vector<bool>(_poolCountX, false));
	_parent = vector<vector<Vector2>>(_poolCountY, vector<Vector2>(_poolCountX, Vector2(-1, -1)));

	AStar(_pos, _maze->GetEndPos());
}

void Player::Update()
{
	if (_pathIndex >= _path.size())
	{
		int randValue = rand() % 3;

		switch (randValue)
		{
		case 0 : 
			_maze->CreateMaze_Kruskal();
			break;
		case 1:
			_maze->CreateMaze();
			break;
		case 2:
			_maze->CreateMaze_Prim();
			break;
		default:
			break;
		}


		_pathIndex = 0;
		_path.clear();
		Init();
		return;
	}

	_time += 0.4f;

	if (_time > 1.0f)
	{
		_time = 0.0f;
		_pos = _path[_pathIndex];

		if (_pathIndex != 0)
		{
			Vector2 temp = _path[_pathIndex - 1];
			_maze->GetBlock(temp)->SetType(Block::Type::FOOT_PRINT);
		}

		_pathIndex++;
	}
	_maze->GetBlock(_pos)->SetType(Block::Type::PLAYER);
}

void Player::RightHand()
{
	Vector2 pos = _pos; // ���� ��ġ
	_path.push_back(pos);
	Vector2 endPos = _maze->GetEndPos();
	_dir = Dir::DIR_UP;

	Vector2 frontPos[4] =
	{
		Vector2 {0, -1}, // UP 0
		Vector2 {-1, 0}, // LEFT 1
		Vector2 {0, 1}, // DOWN 2
		Vector2 {1, 0} // RIGHT 3
	};

	while (pos != endPos)
	{
		// ���� �ٶ󺸴� ���� �������� ���������� �� �� �ִ��� Ȯ��
		int newDir = (_dir - 1 + DIR_COUNT) % DIR_COUNT;
		Vector2 oldDirVector2 = frontPos[_dir]; // ���� ����
		Vector2 newDirVector2 = frontPos[newDir]; // ���ο� ����

		if (CanGo(pos + newDirVector2))
		{
			_dir = static_cast<Dir>(newDir);
			pos += newDirVector2;
			_path.push_back(pos);
		}
		
		// ���� �ٶ󺸴� ���� �������� ������ �� �ִ��� Ȯ��
		else if (CanGo(pos + oldDirVector2))
		{
			pos += oldDirVector2;
			_path.push_back(pos);
		}

		// ������, ���� �� �����ִٸ� �������� ȸ��
		else
		{
			_dir = static_cast<Dir>((_dir + 1 + DIR_COUNT) % DIR_COUNT);
		}
	}

	// Stack
	stack<Vector2> s;

	for (int i = 0; i < _path.size() - 1; i++)
	{
		if (s.empty() == false && s.top() == _path[i + 1])
		{
			s.pop();
		}
		else
			s.push(_path[i]);
	}

	s.push(_path.back());
	_path.clear();

	while (true)
	{
		if (s.empty() == true)
			break;
		_path.push_back(s.top());
		s.pop();
	}

	std::reverse(_path.begin(), _path.end());
}

void Player::DFS(Vector2 pos, Vector2 end, vector<Vector2>& tempPath)
{
	tempPath.push_back(pos);
	_maze->GetBlock(pos)->SetType(Block::Type::SEARCH_PRINT);
	_visited[pos.y][pos.x] = true;
	if (pos == end)
	{
		return;
	}

	Vector2 frontPos[4] =
	{
		Vector2 {0, 1}, // DOWN 2
		Vector2 {1, 0}, // RIGHT 3
		Vector2 {0, -1}, // UP 0
		Vector2 {-1, 0} // LEFT 1
	};

	for (int i = 0; i < 4; i++)
	{
		Vector2 there = pos + frontPos[i];

		if (CanGo(there) == false)
			continue;
		if (_visited[there.y][there.x] == true)
			continue;
		DFS(there, end, tempPath);
	}
}

void Player::BFS(Vector2 start, Vector2 end)
{
	Vector2 frontPos[4] =
	{
		Vector2 {0, 1}, // DOWN 2
		Vector2 {1, 0}, // RIGHT 3
		Vector2 {0, -1}, // UP 0
		Vector2 {-1, 0} // LEFT 1
	};

	queue<Vector2> q;
	q.push(start);
	_discorvered[start.y][start.x] = true;
	_parent[start.y][start.x] = start;

	while (true)
	{
		if (q.empty() == true)
			break;

		Vector2 here = q.front();
		q.pop();

		if (here == end)
			break;

		for (int i = 0; i < 4; i++)
		{
			Vector2 there = here + frontPos[i];
			if (_discorvered[there.y][there.x] == true)
				continue;
			if (CanGo(there) == false)
				continue;
			if (here == there)
				continue;

			_maze->GetBlock(there)->SetType(Block::Type::SEARCH_PRINT);
			q.push(there);
			_discorvered[there.y][there.x] = true;
			_parent[there.y][there.x] = here;
		}
	}

	Vector2 pos = end;
	_path.push_back(pos);
	while (true)
	{
		_path.push_back(_parent[pos.y][pos.x]);
		pos = _parent[pos.y][pos.x];

		if (pos == start)
			break;
	}

	std::reverse(_path.begin(), _path.end());
}

void Player::Djikstra(Vector2 start, Vector2 end)
{
	Vector2 frontPos[8] =
	{
		Vector2 {0, 1}, // DOWN 2
		Vector2 {1, 0}, // RIGHT 3
		Vector2 {0, -1}, // UP 0
		Vector2 {-1, 0}, // LEFT 1

		Vector2 {1, 1}, // RIGHT_DOWN
		Vector2 {-1, 1}, // LEFT_DOWN
		Vector2 {1, -1}, // RIGHT UP
		Vector2 {-1, -1} // LEFT UP
	};

	priority_queue<Vertex_Djikstra, vector<Vertex_Djikstra>, greater<Vertex_Djikstra>> pq;
	vector<vector<float>> best = vector<vector<float>>(25, vector<float>(25, 100000.0f));

	Vertex_Djikstra startV;
	startV.pos = start;
	startV.g = 0;
	pq.push(startV);
	best[start.y][start.x] = 0;
	_discorvered[start.y][start.x] = true;
	_parent[start.y][start.x] = start;

	while (true)
	{
		if (pq.empty() == true)
			break;

		Vector2 here = pq.top().pos;
		float cost = pq.top().g;
		pq.pop();

		if (here == end)
			break;

		if (best[here.y][here.x] < cost)
			continue;

		for (int i = 0; i < 8; i++)
		{
			Vector2 there = here + frontPos[i];
			if (CanGo(there) == false)
				continue;
			if (here == there)
				continue;

			float distance = (there - here).Length();
			float nextCost = distance + best[here.y][here.x];

			if (best[there.y][there.x] < nextCost)
				continue;

			_maze->GetBlock(there)->SetType(Block::Type::SEARCH_PRINT);
			Vertex_Djikstra thereV;
			thereV.pos = there;
			thereV.g = nextCost;
			best[there.y][there.x] = nextCost;
			pq.push(thereV);
			_discorvered[there.y][there.x] = true;
			_parent[there.y][there.x] = here;
		}
	}

	Vector2 pos = end;
	_path.push_back(pos);
	while (true)
	{
		_path.push_back(_parent[pos.y][pos.x]);
		pos = _parent[pos.y][pos.x];

		if (pos == start)
			break;
	}

	std::reverse(_path.begin(), _path.end());
}

void Player::AStar(Vector2 start, Vector2 end)
{
	Vector2 frontPos[8] =
	{
		Vector2 {0, 1}, // DOWN 2
		Vector2 {1, 0}, // RIGHT 3
		Vector2 {0, -1}, // UP 0
		Vector2 {-1, 0}, // LEFT 1

		Vector2 {1, 1}, // RIGHT_DOWN
		Vector2 {-1, 1}, // LEFT_DOWN
		Vector2 {1, -1}, // RIGHT UP
		Vector2 {-1, -1} // LEFT UP
	};

	priority_queue<Vertex, vector<Vertex>, greater<Vertex>> pq;
	vector<vector<float>> best = vector<vector<float>>(_poolCountY, vector<float>(_poolCountX , 100000.0f));

	Vertex startV;
	startV.pos = start;
	startV.g = 0;
	startV.h = start.Mahattan(end);
	startV.f = startV.g + startV.h;
	pq.push(startV);
	best[start.y][start.x] = startV.f;
	_discorvered[start.y][start.x] = true;
	_parent[start.y][start.x] = start;

	while (true)
	{
		if (pq.empty() == true)
			break;

		Vertex here = pq.top();
		float f = here.f;
		pq.pop();

		if (here.pos == end)
			break;

		if (best[here.pos.y][here.pos.x] < f)
			continue;

		for (int i = 0; i < 8; i++)
		{
			Vector2 there = here.pos + frontPos[i];
			if (CanGo(there) == false)
				continue;
			if (here.pos == there)
				continue;

			float distance = (there - here.pos).Length();
			float nextG = distance + here.g;
			float nextH = there.Mahattan(end);
			float nextF = nextG + nextH;

			if (best[there.y][there.x] < nextF)
				continue;

			//_maze->GetBlock(there)->SetType(Block::Type::SEARCH_PRINT);
			Vertex thereV;
			thereV.pos = there;
			thereV.g = nextG;
			thereV.h = nextH;
			thereV.f = nextF;
			best[there.y][there.x] = nextF;
			pq.push(thereV);
			_discorvered[there.y][there.x] = true;
			_parent[there.y][there.x] = here.pos;
		}
	}

	Vector2 pos = end;
	_path.push_back(pos);
	while (true)
	{
		_path.push_back(_parent[pos.y][pos.x]);
		pos = _parent[pos.y][pos.x];

		if (pos == start)
			break;
	}

	std::reverse(_path.begin(), _path.end());
}

bool Player::CanGo(Vector2 pos)
{
	if (_maze->GetBlock(pos)->GetType() == Block::Type::DISABLE)
		return false;

	return true;
}
