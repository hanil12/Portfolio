#include "framework.h"
#include "../../../Scene/BagicScene/BowScene.h"
#include "Bow_Bullet.h"

Bow_Bullet::Bow_Bullet()
{
	_quad = make_shared<Quad>(L"Resource/Texture/Bullet.png");
	_quad->GetTransform()->GetScale() *= 0.1f;

	_col = make_shared<CircleCollider>(70);
	_col->GetTransform()->SetParent(_quad->GetTransform());
	Vector2 temp = _col->GetTransform()->GetPos();
	temp.x += 300.0f;
	_col->GetTransform()->SetPosition(temp);
}

Bow_Bullet::~Bow_Bullet()
{
}

void Bow_Bullet::Update()
{
	if (isActive == false)
		return;

	_check += DELTA_TIME;

	if (_check > _delay)
	{
		isActive = false;
		_check = 0.0f;
	}

	Vector2 temp = _quad->GetTransform()->GetPos();
	temp += _dir * _speed * DELTA_TIME;
	_quad->GetTransform()->SetPosition(temp);

	_quad->Update();
	_col->Update();

	if (_target.expired() == false)
	{
		if (_col->IsCollision(_target.lock()))
		{
			--BowScene::_hp;
			DisAble();
		}
	}
}

void Bow_Bullet::Render()
{
	if (isActive == false)
		return;

	_quad->Render();
	_col->Render();
}

void Bow_Bullet::SetDirection(Vector2 dir)
{
	dir.Normalize();
	_dir = dir;
}

void Bow_Bullet::EnAble()
{
	isActive = true;
	_col->isActive = true;
	_check = 0.0f;
}

void Bow_Bullet::DisAble()
{
	isActive = false;
	_col->isActive = false;
	_check = 0.0f;
}
