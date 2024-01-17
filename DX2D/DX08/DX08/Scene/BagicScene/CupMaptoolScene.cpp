#include "framework.h"
#include "CupMaptoolScene.h"

CupMaptoolScene::CupMaptoolScene()
{
	_track = make_shared<Quad>(L"Resource/Texture/CupHead/clown_bg_track.png");
	_track->GetTransform()->GetScale() *= 0.5f;
	_track->GetTransform()->SetPosition(CENTER);

	_test.push_back(Vector2(1, 1));
	_test.push_back(Vector2(5, 5));
	_test.push_back(Vector2(7, 7));
}

CupMaptoolScene::~CupMaptoolScene()
{
}

void CupMaptoolScene::Update()
{
	if (KEY_DOWN(VK_RBUTTON))
		_track->GetTransform()->SetPosition(MOUSE_POS);

	_track->Update();
}

void CupMaptoolScene::Render()
{
	_track->Render();
}

void CupMaptoolScene::PostRender()
{
	if (ImGui::Button("SAVE TrackPos", { 100,30 }))
	{
		Save();
	}
}

void CupMaptoolScene::Save()
{
	_trackPos = _track->GetTransform()->GetWorldPos();
	BinaryWriter writer = BinaryWriter(L"Save/CupHeadTrackPos.track");

	writer.Byte((void*)&_trackPos, sizeof(Vector2));

	writer.UInt(_test.size());
	writer.Byte((void*)_test.data(), sizeof(Vector2) * _test.size());
}

void CupMaptoolScene::Load()
{
}
