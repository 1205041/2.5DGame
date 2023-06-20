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
	player = std::make_shared<Player>();
	player->RegistHitObj(ground);
	m_objList.push_back(player);

	std::shared_ptr<Enemy> enemy;
	enemy = std::make_shared<Enemy>();
	m_objList.push_back(enemy);

	// カメラの初期化
	std::shared_ptr<TPS> tps = std::make_shared<TPS>();;
	tps->SetTarget(player);
	player->SetCamera(tps);
	m_objList.push_back(tps);
}
