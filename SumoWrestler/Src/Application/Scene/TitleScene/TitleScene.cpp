#include "TitleScene.h"

// 地形
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
	std::shared_ptr<TitleText> spTitle;
	spTitle = std::make_shared<TitleText>();
	m_objList.push_back(spTitle);

	// 地形
	std::shared_ptr<SkySphere> spSkySp;
	spSkySp = std::make_shared<SkySphere>();
	m_objList.push_back(spSkySp);

	// カメラの初期化
	std::shared_ptr<Tracking> spTrac;
	spTrac = std::make_shared<Tracking>();
	spTrac->SetTarget(spTitle);
	m_objList.push_back(spTrac);
}
