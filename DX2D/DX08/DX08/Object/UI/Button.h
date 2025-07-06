#pragma once
class Button
{
public:
	enum State
	{
		NONE,
		HOVER,
		CLICK
	};

	Button(wstring path);
	Button(Vector2 size);
	~Button();

	void Update();
	void PostRender();

	void SetPostion(Vector2 pos);
	void SetScale(Vector2 scale);

	void SetEvent(CallBack callBack) { _event = callBack; }
	void SetStringEvent(CallBack_String callBack) { _stringEvent = callBack; }
	void SetIntEvent(CallBack_Int callback) { _intEvent = callback; }

	void SetParent(shared_ptr<Transform> transform) { GetTransform()->SetParent(transform); }
private:
	shared_ptr<Transform> GetTransform();

	State _state;
	shared_ptr<ButtonBuffer> _buttonBuffer;

	shared_ptr<Quad> _quad;
	shared_ptr<Collider> _collider;

	CallBack _event;
	CallBack_String _stringEvent;
	CallBack_Int _intEvent;
};

