#pragma once
class BowScene : public Scene
{
public:
	BowScene();
	virtual ~BowScene();

	// Scene��(��) ���� ��ӵ�
	virtual void Update() override;
	virtual void Render() override;
	virtual void PostRender() override;

	static int _hp;
private:
	shared_ptr<Bow_Bow> _bow;

	// Boss
	shared_ptr<Quad> _quad;
	shared_ptr<Collider> _col;
};

