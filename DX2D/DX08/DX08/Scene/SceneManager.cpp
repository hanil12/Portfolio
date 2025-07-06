#include "framework.h"
#include "SceneManager.h"

#include "../Scene/BagicScene/TutorialScene.h"
#include "../Scene/BagicScene/SolarSystemScene.h"
#include "../Scene/BagicScene/BowScene.h"
#include "../Scene/BagicScene/CollisionScene.h"
#include "../Scene/BagicScene/SpriteScene.h"
#include "../Scene/BagicScene/ActionScene.h"
#include "../Scene/BagicScene/CupHeadScene.h"
#include "../Scene/BagicScene/EffectScene.h"
#include "../Scene/BagicScene/CupMaptoolScene.h"
#include "../Scene/BagicScene/InventoryScene.h"

SceneManager* SceneManager::_instance = nullptr;

SceneManager::SceneManager()
{
	DATA_M->LoadItemInfo();

	_sceneTable["SpriteScene"] = make_shared<SpriteScene>();
	_sceneTable["SolarSystemScene"] = make_shared<SolarSystemScene>();
	_sceneTable["CupHeadScene"] = make_shared<CupHeadScene>();
	_sceneTable["InventoryScene"] = make_shared<InventoryScene>();

	_curScene = _sceneTable["CupHeadScene"];
}

SceneManager::~SceneManager()
{
	_curScene = nullptr;
}

void SceneManager::Update()
{
	if (_curScene == nullptr) return;
	_curScene->Update();
}

void SceneManager::Render()
{
	if (_curScene == nullptr) return;
	_curScene->Render();
}

void SceneManager::PreRender()
{
	if (_curScene == nullptr) return;
	_curScene->PreRender();
}

void SceneManager::PostRender()
{
	if (_curScene == nullptr) return;
	_curScene->PostRender();
}

void SceneManager::Init()
{
	_curScene->Init();
}

void SceneManager::SetScene(string name)
{
	if (_sceneTable.count(name) == 0)
		return;

	if (_curScene != nullptr)
	{
		_curScene->Finalize();
	}
	_curScene = _sceneTable[name];
	_curScene->Init();
}
