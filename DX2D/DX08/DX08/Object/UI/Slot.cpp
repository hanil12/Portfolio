#include "framework.h"
#include "Slot.h"

Slot::Slot()
{
	_quad = make_shared<Quad>(Vector2(70, 70));
	_quad->SetPS(ADD_PS(L"Shader/UI/SlotPixelShader.hlsl"));

	_buttonBuffer = make_shared<ButtonBuffer>();
	_buttonBuffer->_data.state = 1;
	_buttonBuffer->_data.hover = 0.3f;
	_buttonBuffer->_data.click = 0.6f;
}

Slot::~Slot()
{
}

void Slot::Update()
{
	_quad->Update();
	_buttonBuffer->Update();
}

void Slot::Render()
{
	_buttonBuffer->SetPSBuffer(0);
	_quad->Render();
}
