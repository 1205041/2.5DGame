#include "GameScene.h"

// キャラクタ
#include "../../Object/Player/Player.h"
#include "../../Object/Enemy/Enemy.h"

// 地形
#include "../../Object/Ground/Ground.h"
#include "../../Object/SkySphere/SkySphere.h"

// カメラ
#include "../../Object/Camera/TPS/TPS.h"

void GameScene::Event()
{
	
	/* ※ この段階では更新されません ！！ */
}

void GameScene::Init()
{
	KdAudioManager::Instance().Play("Asset/Sounds/SE/GameStart.wav");
	KdAudioManager::Instance().Play("Asset/Sounds/BGM/Game.wav", true);

	/* オブジェクトの初期化 */
	// 地形
	std::shared_ptr<Ground> ground;
	ground = std::make_shared<Ground>();
	m_objList.push_back(ground);

	std::shared_ptr<SkySphere> skySp;
	skySp = std::make_shared<SkySphere>();
	m_objList.push_back(skySp);

	// キャラ
	std::shared_ptr<Player> player;
	std::shared_ptr<Enemy> enemy;

	player = std::make_shared<Player>();
	enemy = std::make_shared<Enemy>();

	player->SetPos({ -3.0f,-3.0f,0 });
	enemy->SetPos({ 3.0f,-3.0f,0 });

	player->RegistHitObj(ground);
	player->RegistHitObj(enemy);

	enemy->RegistHitObj(ground);
	enemy->RegistHitObj(player);

	m_objList.push_back(player);
	m_objList.push_back(enemy);

	// カメラの初期化
	std::shared_ptr<TPS> tps = std::make_shared<TPS>();;
	tps->SetTarget(player);
	player->SetCamera(tps);
	m_objList.push_back(tps);
}
