#include "framework.h"
#include "CircleCollider.h"

CircleCollider::CircleCollider(Vector2 center, float radius)
: radius(radius)
, Collider()
{
    this->center = center;
    _type = Collider::Type::CIRCLE;
}

CircleCollider::~CircleCollider()
{

}

void CircleCollider::Update()
{
    if (_isActive == false)
        return;
}

void CircleCollider::Render(HDC hdc)
{
    if (_isActive == false)
        return;
    SelectObject(hdc, _curPen);

	float left = center.x - radius;
	float top = center.y - radius;
	float right = center.x + radius;
	float bottom = center.y + radius;
	Ellipse(hdc, left, top, right, bottom);
}

bool CircleCollider::IsCollision(const Vector2& pos)
{
    if (_isActive == false)
        return false;

    Vector2 centerToPos = pos - center;
    float length = centerToPos.Length();

    return length <= radius;
}

bool CircleCollider::IsCollision(shared_ptr<CircleCollider> circle)
{
    Vector2 centerToCenter = circle->center - center;
    float length = centerToCenter.Length();

    return length <= (radius + circle->radius);
}

bool CircleCollider::IsCollision(const shared_ptr<class RectCollider> rect)
{
    return rect->IsCollision(shared_from_this());
}
