#pragma once
class Player
{
public:
	Player();
	~Player();

	String name;
	GameSessionRef ownerSession; // Cycle
};

