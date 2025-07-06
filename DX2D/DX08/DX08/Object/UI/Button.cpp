#include "framework.h"
#include "Button.h"

Button::Button(wstring path)
{
	_quad = make_shared<Quad>(path);
	_quad->SetPS(ADD_PS(L"Shader/UI/ButtonPixelShader.hlsl"));
	_collider = make_shared<RectCollider>(_quad->GetImageSize());
	_collider->GetTransform()->SetParent(_quad->GetTransform());

	_buttonBuffer = make_shared<ButtonBuffer>();
	_buttonBuffer->_data.hover = 0.3f;
	_buttonBuffer->_data.click = 0.6f;
}

Button::Button(Vector2 size)
{
	_quad = nullptr;
	_collider = make_shared<RectCollider>(size);

	_buttonBuffer = make_shared<ButtonBuffer>();
	_buttonBuffer->_data.hover = 0.3f;
	_buttonBuffer->_data.click = 0.6f;
}

Button::~Button()
{
}

void Button::Update()
{
	if (_quad != nullptr)
		_quad->Update();
	_collider->Update();
	_buttonBuffer->Update();

	if (_collider->IsCollision(MOUSE_POS))
	{
		_state = Button::State::HOVER;

		if (KEY_PRESS(VK_LBUTTON))
		{
			_collider->SetRed();
			_state = Button::State::CLICK;
		}

		if (KEY_UP(VK_LBUTTON))
		{
			if (_event != nullptr)
				_event();
			if (_stringEvent != nullptr)
				_stringEvent("NONE");
			if (_intEvent != nullptr)
				_intEvent(-1);
		}
	}
	else
	{
		_collider->SetGreen();
		_state = State::NONE;
	}
}

void Button::PostRender()
{
	_collider->Render();

	_buttonBuffer->_data.state = static_cast<int>(_state);
	_buttonBuffer->SetPSBuffer(1);
	if(_quad != nullptr)
		_quad->Render();
}

void Button::SetPostion(Vector2 pos)
{
	GetTransform()->SetPosition(pos);
}

void Button::SetScale(Vector2 scale)
{
	GetTransform()->SetScale(scale);
}

shared_ptr<Transform> Button::GetTransform()
{
	if (_quad != nullptr)
		return _quad->GetTransform();
	return _collider->GetTransform();
}
