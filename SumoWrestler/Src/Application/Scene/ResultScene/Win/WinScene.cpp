#include "WinScene.h"

// �L�����N�^�[
#include "../../../Object/Player/Player.h"

// �n�`
#include "../../../Object/Ground/Ground.h"
#include "../../../Object/SkySphere/SkySphere.h"

// �V�[��
#include "../../../Object/SceneText/WinScene/WinText.h"

// �J����
#include "../../../Object/Camera/FPS/FPS.h"

void WinScene::Event()
{
	// �V�[���ؑ�(Result��Title)
	if (GetAsyncKeyState(VK_RETURN) & 0x8000)
	{
		SceneManager::Instance().SetNextScene(SceneManager::SceneType::Title);
	}
}

void WinScene::Init()
{
	/* �I�u�W�F�N�g�̏����� */
	// �V�[��
	std::shared_ptr<WinText> win;
	win = std::make_shared<WinText>();
	m_objList.push_back(win);

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