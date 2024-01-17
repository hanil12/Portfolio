#pragma once
class CollisionScene : public Scene
{
public:
	CollisionScene();
	~CollisionScene();

	virtual void Update() override;
	virtual void Render() override;
	virtual void PostRender() override;

private:
	shared_ptr<CircleCollider> _circle1;
	shared_ptr<CircleCollider> _circle2;
	shared_ptr<RectCollider> _rect1;
	shared_ptr<RectCollider> _rect2;
};

