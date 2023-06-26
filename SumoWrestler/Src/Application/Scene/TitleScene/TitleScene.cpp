#include "TitleScene.h"

// キャラクター
#include "../../Object/Player/Player.h"

// 地形
#include "../../Object/Ground/Ground.h"
#include "../../Object/SkySphere/SkySphere.h"

// タイトルシーン
#include "../../Object/SceneText/TitleScene/TitleText.h"

// カメラ
#include "../../Object/Camera/Tracking/Tracking.h"

void TitleScene::Event()
{
	// シーン切替(Title→Game)
	if (GetAsyncKeyState(VK_RETURN) & 0x8000)
	{
		KdAudioManager::Instance().Play("Asset/Sounds/SE/PushButton.wav");
		SceneManager::Instance().SetNextScene(SceneManager::SceneType::Game);
		KdAudioManager::Instance().StopAllSound();
	}
}

void TitleScene::Init()
{
	ShowCursor(false);

	KdAudioManager::Instance().Play("Asset/Sounds/BGM/Title.wav", true);
	
	/* オブジェクトの初期化 */
	// シーン
	std::shared_ptr<TitleText> title;
	title = std::make_shared<TitleText>();
	m_objList.push_back(title);

	// 地形
	std::shared_ptr<Ground> ground;
	ground = std::make_shared<Ground>();
	m_objList.push_back(ground);

	std::shared_ptr<SkySphere> skySp;
	skySp = std::make_shared<SkySphere>();
	m_objList.push_back(skySp);

	// キャラ
	std::shared_ptr<Player> player;
	player = std::make_shared<Player>();
	player->SetPos({ 0,0,0 });
	m_objList.push_back(player);

	// カメラの初期化
	std::shared_ptr<Tracking> trac = std::make_shared<Tracking>();;
	trac->SetTarget(title);
	m_objList.push_back(trac);
}
