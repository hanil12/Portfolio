#pragma once
class SpriteScene : public Scene
{
public:
	struct InstanceData
	{
		XMMATRIX matrix;
		Vector2 maxFrame;
		Vector2 curFrame;
	};

	SpriteScene();
	virtual ~SpriteScene();

	virtual void Update() override;
	virtual void Render() override;
	virtual void PostRender() override;

private:
	shared_ptr<Quad> _quad;
	vector<InstanceData> _instanceDates;
	shared_ptr<VertexBuffer> _instanceBuffer;

	shared_ptr<Transform> _transform;
};

