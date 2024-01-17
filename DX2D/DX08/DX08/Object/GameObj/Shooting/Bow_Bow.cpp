#include "framework.h"
#include "Bow_Bow.h"

Bow_Bow::Bow_Bow()
{
	_quad = make_shared<Quad>(L"Resource/Texture/Bow.png");
	_quad->GetTransform()->GetScale() *= 2.0f;

	_quad->GetTransform()->SetPosition({ 100,100 });

	for (int i = 0; i < 30; i++)
	{
		shared_ptr<Bow_Bullet> bullet = make_shared<Bow_Bullet>();
		bullet->isActive = false;
		_bullets.push_back(bullet);
	}
}

Bow_Bow::~Bow_Bow()
{
}

void Bow_Bow::Update()
{
	Fire();

	_quad->Update();

	for (auto bullet : _bullets)
		bullet->Update();
}

void Bow_Bow::Render()
{
	_quad->Render();

	for (auto bullet : _bullets)
		bullet->Render();
}

void Bow_Bow::Fire()
{
	if (KEY_DOWN(VK_LBUTTON))
	{
		for (auto bullet : _bullets)
		{
			if (bullet->isActive == false)
			{
				bullet->EnAble();

				Vector2 direction = MOUSE_POS - _quad->GetTransform()->GetPos();
				bullet->GetTransform()->SetPosition(_quad->GetTransform()->GetPos());
				bullet->SetDirection(direction);
				bullet->GetTransform()->GetAngle() = direction.Angle();

				break;
			}
		}
	}
}

void Bow_Bow::SetTarget(shared_ptr<Collider> collider)
{
	for (auto bullet : _bullets)
		bullet->SetTarget(collider);
}
