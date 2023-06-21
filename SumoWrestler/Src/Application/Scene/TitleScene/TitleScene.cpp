#include "TitleScene.h"

// キャラクター
#include "../../Object/Player/Player.h"

// 地形
#include "../../Object/Ground/Ground.h"
#include "../../Object/SkySphere/SkySphere.h"

// シーン
#include "../../Object/SceneText/WinScene/WinScene.h"
#include "../../Object/SceneText/LoseScene/LoseScene.h"

// カメラ
#include "../../Object/Camera/FPS/FPS.h"

void TitleScene::Event()
{
	// シーン切替(Title→Game)
	if (GetAsyncKeyState(VK_RETURN) & 0x8000)
	{
		SceneManager::Instance().SetNextScene(SceneManager::SceneType::Game);
	}
}

void TitleScene::Init()
{
}
