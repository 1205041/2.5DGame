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
	if (GetAsyncKeyState('P') & 0x8000)
	{
		KdAudioManager::Instance().Play("Asset/Sounds/SE/PushButton.wav");
		SceneManager::Instance().SetNextScene(SceneManager::SceneType::Title);
		KdAudioManager::Instance().StopAllSound();
	}
}

void LoseScene::Init()
{
	KdAudioManager::Instance().Play("Asset/Sounds/SE/GameSet.wav");
	KdAudioManager::Instance().Play("Asset/Sounds/BGM/Result.wav", true);

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
	player->SetPos({ 0,0,0 });
	player->RegistHitObj(ground);
	m_objList.push_back(player);

	// �J�����̏�����
	std::shared_ptr<Tracking> trac;
	trac = std::make_shared<Tracking>();
	trac->SetTarget(lose);
	m_objList.push_back(trac);
}