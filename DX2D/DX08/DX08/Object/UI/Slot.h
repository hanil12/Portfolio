#pragma once
class Slot
{
public:
	Slot();
	~Slot();

	void Update();
	void Render();

	void SetPosition(Vector2 pos) { _quad->GetTransform()->SetPosition(pos); }
	void SetParent(shared_ptr<Transform> transform) { _quad->GetTransform()->SetParent(transform); }

	void SetChoice() { _buttonBuffer->_data.state = 2; }
	void SetRelease() { _buttonBuffer->_data.state = 1; }

	shared_ptr<Transform> GetTransform() { return _quad->GetTransform(); }

private:
	shared_ptr<Quad> _quad;
	shared_ptr<ButtonBuffer> _buttonBuffer;
};

