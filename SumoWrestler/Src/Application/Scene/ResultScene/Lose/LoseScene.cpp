#include "LoseScene.h"

// �L�����N�^�[
#include "../../../Object/Player/Player.h"

// �n�`
#include "../../../Object/Ground/Ground.h"
#include "../../../Object/SkySphere/SkySphere.h"

// �V�[��
#include "../../../Object/SceneText/LoseScene/LoseText.h"

// �J����
#include "../../../Object/Camera/Tracking/Tracking.h"

void LoseScene::Event()
{
	// �V�[���ؑ�(Result��Title)
	if (GetAsyncKeyState(VK_RETURN) & 0x8000)
	{
		SceneManager::Instance().SetNextScene(SceneManager::SceneType::Title);
	}
}

void LoseScene::Init()
{
	/* �I�u�W�F�N�g�̏����� */
	// �V�[��
	std::shared_ptr<LoseText> lose;
	lose = std::make_shared<LoseText>();
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
	std::shared_ptr<Tracking> trac = std::make_shared<Tracking>();;
	trac->SetTarget(player);
	player->SetCamera(trac);
	m_objList.push_back(trac);
}