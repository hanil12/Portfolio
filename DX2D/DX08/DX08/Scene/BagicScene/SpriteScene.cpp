#include "framework.h"
#include "SpriteScene.h"

SpriteScene::SpriteScene()
{
	//for (int i = 0; i < 300; i++)
	//{
	//	shared_ptr<Sprite> sprite = make_shared<Sprite>(L"Resource/Texture/zelda.png", Vector2(10.0f, 8.0f), Vector2(50, 50));

	//	Vector2 pos = { MathUtility::RandomFloat(0.0f, WIN_WIDTH), MathUtility::RandomFloat(0.0f, WIN_HEIGHT) };
	//	float scaleX = MathUtility::RandomFloat(0.0f, 2.0f);
	//	Vector2 scale = { scaleX, scaleX };
	//	sprite->GetTransform()->SetPosition(pos);
	//	sprite->GetTransform()->GetScale() = scale;

	//	Vector2 curFrame = { MathUtility::RandomInt(0, 10), MathUtility::RandomInt(0,8) };
	//	sprite->SetCurFrame(curFrame);

	//	_sprites.push_back(sprite);
	//}

	_quad = make_shared<Quad>(L"Resource/Texture/zelda.png");
	_quad->SetVS(ADD_VS(L"Shader/InstancingVertexShader.hlsl"));
	_quad->SetPS(ADD_PS(L"Shader/InstancingPixelShader.hlsl"));

	_instanceDates.resize(300);

	for (auto& data : _instanceDates)
	{
		Transform transform;
		Vector2 pos = { MathUtility::RandomFloat(0.0f, WIN_WIDTH), MathUtility::RandomFloat(0.0f, WIN_HEIGHT) };
		float scaleX = MathUtility::RandomFloat(0.01f, 0.1f);

		transform.SetPosition(pos);
		transform.GetScale() = Vector2(scaleX, scaleX);
		transform.UpdateSRT();

		data.maxFrame = Vector2(10.0f, 8.0f);
		data.curFrame = Vector2(MathUtility::RandomInt(0, 10), MathUtility::RandomInt(0, 8));

		data.matrix = XMMatrixTranspose(transform.GetMatrix());
	}

	_transform = make_shared<Transform>();
	_transform->SetPosition(CENTER);
	_transform->GetScale() = { 0.5f,0.5f };
	_transform->UpdateSRT();

	_instanceDates[0].matrix = XMMatrixTranspose(_transform->GetMatrix());

	_instanceBuffer = make_shared<VertexBuffer>(_instanceDates.data(), sizeof(InstanceData), _instanceDates.size(), 0, true);
}

SpriteScene::~SpriteScene()
{
}

void SpriteScene::Update()
{
	//for (auto sprite : _sprites)
	//	sprite->Update();

	Vector2 temp = _transform->GetPos();
	temp.x += 300.0f * DELTA_TIME;
	_transform->SetPosition(temp);

	_instanceDates[0].matrix = XMMatrixTranspose(_transform->GetMatrix());

	_instanceBuffer->Update();
}

void SpriteScene::Render()
{
	//for (auto sprite : _sprites)
	//	sprite->Render();

	_instanceBuffer->IASet(1);

	_quad->SetRender();

	DC->DrawIndexedInstanced(6, _instanceDates.size(), 0, 0, 0);
}

void SpriteScene::PostRender()
{
}
