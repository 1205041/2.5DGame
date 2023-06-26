#include "GameScene.h"

// �L�����N�^
#include "../../Object/Player/Player.h"
#include "../../Object/Enemy/Enemy.h"

// �n�`
#include "../../Object/Ground/Ground.h"
#include "../../Object/SkySphere/SkySphere.h"

// �J����
#include "../../Object/Camera/TPS/TPS.h"

void GameScene::Event()
{
	
	/* �� ���̒i�K�ł͍X�V����܂��� �I�I */
}

void GameScene::Init()
{
	KdAudioManager::Instance().Play("Asset/Sounds/SE/GameStart.wav");
	KdAudioManager::Instance().Play("Asset/Sounds/BGM/Game.wav", true);

	/* �I�u�W�F�N�g�̏����� */
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
	player->SetPos({ -3.0f,-3.0f,0 });
	m_objList.push_back(player);

	std::shared_ptr<Enemy> enemy;
	enemy = std::make_shared<Enemy>();
	enemy->SetPos({ 3.0f,-3.0f,0 });
	m_objList.push_back(enemy);

	// �J�����̏�����
	std::shared_ptr<TPS> tps = std::make_shared<TPS>();;
	tps->SetTarget(player);
	player->SetCamera(tps);
	m_objList.push_back(tps);
}
