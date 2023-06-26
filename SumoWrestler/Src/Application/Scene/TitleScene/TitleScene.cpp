#include "TitleScene.h"

// �L�����N�^�[
#include "../../Object/Player/Player.h"

// �n�`
#include "../../Object/Ground/Ground.h"
#include "../../Object/SkySphere/SkySphere.h"

// �^�C�g���V�[��
#include "../../Object/SceneText/TitleScene/TitleText.h"

// �J����
#include "../../Object/Camera/Tracking/Tracking.h"

void TitleScene::Event()
{
	// �V�[���ؑ�(Title��Game)
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
	
	/* �I�u�W�F�N�g�̏����� */
	// �V�[��
	std::shared_ptr<TitleText> title;
	title = std::make_shared<TitleText>();
	m_objList.push_back(title);

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
	m_objList.push_back(player);

	// �J�����̏�����
	std::shared_ptr<Tracking> trac = std::make_shared<Tracking>();;
	trac->SetTarget(title);
	m_objList.push_back(trac);
}
