#include "GameScene.h"

#include "../../Object/ObjBase.h"

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
	std::shared_ptr<Ground> spGround;
	spGround = std::make_shared<Ground>();
	m_objList.push_back(spGround);

	std::shared_ptr<SkySphere> spSkySp;
	spSkySp = std::make_shared<SkySphere>();
	m_objList.push_back(spSkySp);

	// キャラ
	std::shared_ptr<Player> spPlayer;
	spPlayer = std::make_shared<Player>();
	spPlayer->SetPos({ -3.0f,-4.0f,0 });
	spPlayer->RegistHitObj(spGround);
	m_objList.push_back(spPlayer);

	std::shared_ptr<Enemy> spEnemy;
	spEnemy = std::make_shared<Enemy>();
	spEnemy->SetPos({ 3.0f,-4.0f,0 });
	spEnemy->RegistHitObj(spGround);
	spEnemy->RegistHitObj(spPlayer);
	spPlayer->RegistHitObj(spEnemy);
	m_objList.push_back(spEnemy);
	
	// カメラの初期化
	std::shared_ptr<TPS> spTps = std::make_shared<TPS>();;
	spTps->SetTarget(spPlayer);
	spPlayer->SetCamera(spTps);
	m_objList.push_back(spTps);
}
