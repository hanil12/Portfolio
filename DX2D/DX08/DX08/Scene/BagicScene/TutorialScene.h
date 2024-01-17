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

	// ������۸� �̿��ؼ�
	// -> 1. Shader�� �̿��ؼ� �¿���� ���Ѿ߉�
	// -> 2. Shader�� �ѱ� ����(����)�� �����߉�
	// �¿� ���� ��Ű��
	shared_ptr<ReverseBuffer> _reverseBuffer;
	shared_ptr<FilterBuffer> _filterBuffer;
};

