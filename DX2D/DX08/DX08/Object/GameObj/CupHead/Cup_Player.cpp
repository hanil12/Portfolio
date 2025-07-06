#include "framework.h"
#include "Cup_Player.h"

using namespace tinyxml2;

Cup_Player::Cup_Player()
{
	wstring srvPath = L"Resource/Texture/CupHead/Idle.png";
	CreateAction("Idle", Action::Type::LOOP);
	CreateAction("Run", Action::Type::LOOP);
	CreateAction("AimStraightShot", Action::Type::END);
	CreateAction("Jump", Action::Type::LOOP);
	_actions[State::CUP_SHOT]->SetCallBack(std::bind(&Cup_Player::SetIDLE, this));

	_collider = make_shared<CircleCollider>(73);

	_transform = _collider->GetTransform();

	for (auto sprite : _sprites)
		sprite->GetTransform()->SetParent(_transform);

	_transform->GetScale() *= 0.7f;

	_oldState = State::CUP_IDLE;
	_actions[State::CUP_IDLE]->Play();
}

Cup_Player::~Cup_Player()
{
	_transform = nullptr;
}

void Cup_Player::Update()
{
	_collider->Update();

	Jump();
	Input();

	for (auto sprite : _sprites)
		sprite->Update();

	for (auto action : _actions)
		action->Update();
}

void Cup_Player::Render()
{
	_sprites[_curState]->SetActionClip(_actions[_curState]->GetCurClip());
	_sprites[_curState]->Render();
	_collider->Render();
}

void Cup_Player::SetRight()
{
	_isRight = true;

	for (auto sprite : _sprites)
	{
		sprite->GetLeftRight() = 0;
	}
}

void Cup_Player::SetLeft()
{
	_isRight = false;

	for (auto sprite : _sprites)
	{
		sprite->GetLeftRight() = 1;
	}
}

void Cup_Player::Input()
{
	if (KEY_PRESS('A'))
	{
		Vector2 temp = _transform->GetPos();
		temp.x -= _speed * DELTA_TIME;
		_transform->SetPosition(temp);
		if(_curState != State::JUMP)
			SetAction(State::CUP_RUN);

		SetLeft();
	}

	if (KEY_PRESS('D'))
	{
		Vector2 temp = _transform->GetPos();
		temp.x += _speed * DELTA_TIME;
		_transform->SetPosition(temp);
		if (_curState != State::JUMP)
			SetAction(State::CUP_RUN);

		SetRight();
	}

	if (KEY_UP('A') || KEY_UP('D'))
	{
		if (_curState != State::JUMP)
			SetAction(State::CUP_IDLE);
	}
}

void Cup_Player::Jump()
{
	if (KEY_DOWN(VK_SPACE))
	{
		_jumpPower = 700.0f;
		_curState = State::JUMP;
		Audio::GetInstance()->Play("jump");
	}

	Vector2 temp = _transform->GetPos();
	temp.y += _jumpPower * DELTA_TIME;
	_transform->SetPosition(temp);

	if (_curState == State::CUP_RUN || _curState == State::CUP_SHOT)
		return;

	if (abs(_jumpPower) <= 30.0f)
		SetAction(State::CUP_IDLE);
	else
		SetAction(State::JUMP);
}

void Cup_Player::Ground()
{
	_jumpPower = 0.0f;
}

void Cup_Player::Falling()
{
	_jumpPower -= GRAVITY * GRAVITY * DELTA_TIME;
}

void Cup_Player::SetAction(State state)
{
	_curState = state;

	if (_curState == _oldState)
		return;

	_actions[_curState]->Play();
	_actions[_oldState]->Reset();
	_oldState = _curState;
}

void Cup_Player::SetIDLE()
{
	SetAction(State::CUP_IDLE);
}

void Cup_Player::Damaged(float amount)
{
	if (amount <= 0.0f)
		return;

	_hp -= amount;

	if (_hp < 0.0f)
		_hp = 0.0f;
}

void Cup_Player::CreateAction(string name, Action::Type type)
{
	// idle.png
	string xmlPath = "Resource/XML/" + name + ".xml";
	wstring srvPath(name.begin(), name.end());
	srvPath = L"Resource/Texture/CupHead/" + srvPath + L".png";

	MyXML xml = MyXML(xmlPath, srvPath);

	string actionName = "CUP_" + name;
	_actions.emplace_back(make_shared<Action>(xml.GetClips(), actionName, type));
	_sprites.emplace_back(make_shared<Sprite>(srvPath, xml.AverageSize()));
}
