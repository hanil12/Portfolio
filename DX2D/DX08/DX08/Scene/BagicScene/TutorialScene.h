#pragma once
class TutorialScene : public Scene
{
public:
	TutorialScene();
	virtual ~TutorialScene();

	virtual void Init() override;
	virtual void Update() override;
	virtual void Render() override;
	virtual void PostRender() override;

private:
	shared_ptr<Quad> _quad;

	// 상수버퍼를 이용해서
	// -> 1. Shader를 이용해서 좌우반전 시켜야됌
	// -> 2. Shader에 넘길 정보(버퍼)를 만들어야됌
	// 좌우 반전 시키기
	shared_ptr<ReverseBuffer> _reverseBuffer;
	shared_ptr<FilterBuffer> _filterBuffer;
};

