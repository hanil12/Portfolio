#include "framework.h"
#include "Cup_Monster.h"

Cup_Monster::Cup_Monster()
{
	CreateAction();
	Vector2 colliderSize = _sprite->GetClipVertexSize();
	colliderSize.x *= 0.65f;
	colliderSize.y *= 0.8f;
	_collider = make_shared<RectCollider>(colliderSize);

	_transform = _collider->GetTransform();
	_sprite->GetTransform()->SetParent(_transform);
	_sprite->SetReverse();
	_action->Play();

	CreateBullet();
}

Cup_Monster::~Cup_Monster()
{
	_transform = nullptr;
}

void Cup_Monster::Update()
{
	_collider->Update();

	_action->Update();
	_sprite->Update();

	for (auto bullet : _bullets)
		bullet->Update();

	_shotCheck += DELTA_TIME;
}

void Cup_Monster::Render()
{
	_sprite->SetActionClip(_action->GetCurClip());
	_sprite->Render();
	_collider->Render();

	for (auto bullet : _bullets)
		bullet->Render();
}

void Cup_Monster::CreateBullet()
{
	for (int i = 0; i < 3; i++)
	{
		shared_ptr<Cup_Bullet> bullet = make_shared<Cup_Bullet>();
		bullet->isActive = false;
		_bullets.push_back(bullet);
	}
}

void Cup_Monster::Shot(Vector2 pos)
{
	if (_shotCheck < _shotDelay)
		return;

	auto iter = std::find_if(_bullets.begin(), _bullets.end(), [](const shared_ptr<Cup_Bullet>& bullet)-> bool
		{
			if (bullet->isActive == false)
				return true;
			return false;
		});

	if (iter == _bullets.end())
		return;

	Vector2 dir = (pos - _transform->GetPos()).NormalVector2();
	(*iter)->Enable();
	(*iter)->SetPostion(_transform->GetWorldPos());
	(*iter)->Fire(dir);

	_shotCheck = 0.0f;
}

void Cup_Monster::Dameged(int amount)
{
	if (_hp <= 0)
	{
		return;
	}

	_hp -= amount;
}

void Cup_Monster::CreateAction()
{
	wstring srvPath = L"Resource/Texture/CupHead/Clown_Page_Last_Spawn_Penguin_Start.png";
	string xmlPath = "Resource/XML/Clown_Page_Last_Spawn_Penguin_Start.xml";
	MyXML xml = MyXML(xmlPath, srvPath);

	_action = make_shared<Action>(xml.GetClips(), "Boss_IDLE");
	Vector2 size = xml.AverageSize() * 0.7f;
	_sprite = make_shared<Sprite>(srvPath, size);
}
