#pragma once
class MatrixBuffer;

class Transform
{
public:
	Transform();
	~Transform();

	void UpdateSRT();
	void Update();
	void SetBuffer(UINT slot);

	void SetPosition(Vector2 pos) { _pos = pos; UpdateSRT(); }
	void SetScale(Vector2 scale) { _scale = scale; UpdateSRT(); }

	Vector2& GetScale() { return _scale; }
	float& GetAngle() { return _angle; }
	const Vector2& GetPos() { return _pos; }

	Vector2 GetWorldPos();
	Vector2 GetWorldScale();

	void SetParent(shared_ptr<Transform> parent) { _parent = parent; }

	const XMMATRIX& GetMatrix() { return _srtMatrix; }

private:
	shared_ptr<MatrixBuffer> _world;

	Vector2		_scale = { 1.0f ,1.0f };
	float		_angle = 0.0f;
	Vector2		_pos = { 0.0f, 0.0f };

	XMMATRIX _scaleM	 = XMMatrixIdentity();
	XMMATRIX _rotationM	 = XMMatrixIdentity();
	XMMATRIX _tranlateM	 = XMMatrixIdentity();

	XMMATRIX _srtMatrix = XMMatrixIdentity();

	weak_ptr<Transform> _parent;
};

