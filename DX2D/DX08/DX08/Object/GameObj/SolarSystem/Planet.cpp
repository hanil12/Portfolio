#include "framework.h"
#include "Planet.h"

Planet::Planet(wstring path)
{
	_quad = make_shared<Quad>(path);
	_quad->SetPS(ADD_PS(L"Shader/FilterPixelShader.hlsl"));
	_reverseBuffer = make_shared<ReverseBuffer>();
	_filterBuffer = make_shared<FilterBuffer>();
	_filterBuffer->_data.selected = 6;

	_virtualTrans = make_shared<Transform>();
}

Planet::~Planet()
{
}

void Planet::Update()
{
	_quad->GetTransform()->GetAngle() += _speed;

	_virtualTrans->GetAngle() += _speed * 2.0f;

	_filterBuffer->Update();
	_reverseBuffer->Update();

	_quad->Update();
	_virtualTrans->Update();
}

void Planet::Render()
{
	_reverseBuffer->SetPSBuffer(0);
	_filterBuffer->SetPSBuffer(1);
	_quad->Render();
}

void Planet::SetPos(Vector2 pos)
{
	_quad->GetTransform()->SetPosition(pos);
	_virtualTrans->SetPosition(pos);
}

void Planet::SetScale(Vector2 scale)
{
	_quad->GetTransform()->GetScale() = scale;
	_virtualTrans->GetScale() = scale;
}
