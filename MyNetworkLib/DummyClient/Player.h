#pragma once
class Player
{
public:
	~Player() { _session = nullptr; }

	void Init() { _id = String(); _name = String(); _session = nullptr; }
	void SetId(String id) { _id = id; }
	void SetName(String name) { _name = name; }
	void SetSession(SessionRef session) { _session = session; }

	String GetName() { return _name; }
	SessionRef GetSession() { return _session; }

private:
	String _id;
	String _name;
	SessionRef _session;
};

extern PlayerRef GPlayer;