#include "framework.h"
#include "CollisionScene.h"

CollisionScene::CollisionScene()
{
	_circle1 = make_shared<CircleCollider>(50);
	_circle1->GetTransform()->SetPosition(Vector2(CENTER_X + 200, CENTER_Y + 100));

	_circle2 = make_shared<CircleCollider>(50);
	_circle2->GetTransform()->SetPosition(Vector2(CENTER_X - 20, CENTER_Y + 100));

	_rect1 = make_shared<RectCollider>(Vector2(5, 100));
	_rect1->GetTransform()->SetPosition(Vector2(CENTER_X - 200, CENTER_Y + 100));

	_rect2 = make_shared<RectCollider>(Vector2(100, 100));
	_rect2->GetTransform()->SetPosition(Vector2(CENTER_X - 200, CENTER_Y - 100));
}

CollisionScene::~CollisionScene()
{
}

void CollisionScene::Update()
{
	_rect1->GetTransform()->SetPosition(MOUSE_POS);

	_circle1->Block(_circle2);
	_rect1->Block(_circle1);
	_rect1->Block(_circle2);
	_rect1->Block(_rect2);

	_circle1->Update();
	_circle2->Update();
	_rect1->Update();
	_rect2->Update();
}

void CollisionScene::Render()
{
	_circle1->Render();
	_circle2->Render();
	_rect1->Render();
	_rect2->Render();
}

void CollisionScene::PostRender()
{
	//ImGui::SliderFloat2("Rect Pos", (float*)&_rect->GetTransform()->GetPos(),0, WIN_WIDTH);
	ImGui::SliderFloat2("Circle1 Pos", (float*)&_circle1->GetTransform()->GetPos(),0, WIN_WIDTH);
	ImGui::SliderFloat2("Rect1 Pos", (float*)&_rect1->GetTransform()->GetPos(),0, WIN_WIDTH);
	//ImGui::SliderFloat2("Circle2 Pos", (float*)&_rect2->GetTransform()->GetPos(),0, WIN_WIDTH);
	//ImGui::SliderFloat("RectAngle", &_rect->GetTransform()->GetAngle(), 0, 2 * PI);

	//float circle2_scale = _rect2->GetTransform()->GetScale().x;
	//ImGui::SliderFloat("Circle2 Scale", (float*)&circle2_scale,0, 3);
	//_rect2->GetTransform()->GetScale() = Vector2(circle2_scale, circle2_scale);

	//float rect_Scale = _rect->GetTransform()->GetScale().x;
	//ImGui::SliderFloat("Rect Scale", (float*)&rect_Scale,0, 3);
	//_rect->GetTransform()->GetScale() = Vector2(rect_Scale, rect_Scale);
}
