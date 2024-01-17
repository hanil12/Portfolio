#include "framework.h"
#include "Program.h"

Program::Program()
{
	Timer::GetInstance()->SetLockFPS(60);
	SCENE->Init();
}

Program::~Program()
{
}

void Program::Update()
{
	if (KEY_DOWN(VK_F1))
		Collider::isDebug = !Collider::isDebug;

	InputManager::GetInstance()->Update();
	Timer::GetInstance()->Update();
	Audio::GetInstance()->Update();
	Camera::GetInstance()->Update();

	SceneManager::GetInstance()->Update();
	EFFECT->Update();
}

void Program::Render()
{
	Device::GetInstance()->Clear();

	Camera::GetInstance()->SetProjectBuffer();
	Camera::GetInstance()->SetCameraWorldBuffer();

	SceneManager::GetInstance()->PreRender();

	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	Device::GetInstance()->SetMainRenderTarget();

	ALPHA->SetState();

	SceneManager::GetInstance()->Render();
	EFFECT->Render();

	DirectWrite::GetInstance()->GetDC()->BeginDraw();

	wstring fps = L"FPS : " + to_wstring((int)Timer::GetInstance()->GetFPS());
	RECT rect = { 0,0,100,100 };
	DirectWrite::GetInstance()->RenderText(fps, rect);

	Camera::GetInstance()->PostRender();
	Camera::GetInstance()->UICameraBuffer();
	SceneManager::GetInstance()->PostRender();
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	DirectWrite::GetInstance()->GetDC()->EndDraw();
	Device::GetInstance()->Present();
}
