#include "ResultScene.h"

// �L�����N�^�[
#include "../../Object/Player/Player.h"

// �n�`
#include "../../Object/Ground/Ground.h"
#include "../../Object/SkySphere/SkySphere.h"

// �V�[��
#include "../../Object/SceneText/WinScene/WinScene.h"
#include "../../Object/SceneText/LoseScene/LoseScene.h"

// �J����
#include "../../Object/Camera/FPS/FPS.h"

void ResultScene::Event()
{
	// �V�[���ؑ�(Result��Title)
	if (GetAsyncKeyState(VK_RETURN) & 0x8000)
	{
		SceneManager::Instance().SetNextScene(SceneManager::SceneType::Title);
	}
}

void ResultScene::Init()
{
	/* �I�u�W�F�N�g�̏����� */
	// �V�[��
	std::shared_ptr<WinScene> win;
	win = std::make_shared<WinScene>();
	win->SetScale({ 1.0f, 1.0f, 1.0f });
	m_objList.push_back(win);

	std::shared_ptr<LoseScene> lose;
	lose = std::make_shared<LoseScene>();
	m_objList.push_back(lose);

	// �n�`
	std::shared_ptr<Ground> ground;
	ground = std::make_shared<Ground>();
	m_objList.push_back(ground);

	std::shared_ptr<SkySphere> skySp;
	skySp = std::make_shared<SkySphere>();
	m_objList.push_back(skySp);

	// �L����
	std::shared_ptr<Player> player;
	player = std::make_shared<Player>();
	player->RegistHitObj(ground);
	m_objList.push_back(player);

	// �J�����̏�����
	std::shared_ptr<FPS> fps = std::make_shared<FPS>();;
	fps->SetTarget(player);
	player->SetCamera(fps);
	m_objList.push_back(fps);
}
