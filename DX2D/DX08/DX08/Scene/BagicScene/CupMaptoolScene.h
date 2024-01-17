#pragma once
class CupMaptoolScene : public Scene
{
public:
	CupMaptoolScene();
	~CupMaptoolScene();

	virtual void Update() override;
	virtual void Render() override;
	virtual void PostRender() override;

	void Save();
	void Load();

private:
	Vector2 _trackPos;
	shared_ptr<Quad> _track;
	vector<Vector2> _test;
};

