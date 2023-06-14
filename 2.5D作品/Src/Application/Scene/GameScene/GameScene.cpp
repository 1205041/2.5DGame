#include "GameScene.h"

#include "../../Object/Player/Player.h"
#include "../../Object/Enemy/Enemy.h"

#include "../../Object/Ground/Ground.h"
//#include "../../Object/Scaffold/Scaffold.h"
//#include "../../Object/SkySphere/SkySphere.h"

void GameScene::Update()
{
	m_enemy->Update();
	m_player->Update();
	
	// カメラの座標行列
	Math::Matrix transMat;
	transMat = Math::Matrix::CreateTranslation(Math::Vector3(0, 1, -3) + m_player->GetPos());
	m_camera.SetCameraMatrix(transMat);
	/* ※ この段階では更新されません ！！ */
}

void GameScene::PreDraw()
{
	// カメラ更新
	m_camera.SetToShader();
}

void GameScene::Draw()
{
	// ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== =====
	// 光を遮るオブジェクト(不透明な物体や2Dキャラ)はBeginとEndの間にまとめてDrawする
	KdShaderManager::Instance().m_HD2DShader.BeginGenerateDepthMapFromLight();
	{

	}
	KdShaderManager::Instance().m_HD2DShader.EndGenerateDepthMapFromLight();


	// ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== =====
	// 陰影のあるオブジェクト(不透明な物体や2Dキャラ)はBeginとEndの間にまとめてDrawする
	KdShaderManager::Instance().m_HD2DShader.BeginLit();
	{
		m_enemy->Draw();
		m_player->Draw();

		m_ground->Draw();
	}
	KdShaderManager::Instance().m_HD2DShader.EndLit();


	// ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== =====
	// 陰影のないオブジェクト(透明な部分を含む物体やエフェクト)はBeginとEndの間にまとめてDrawする
	KdShaderManager::Instance().m_HD2DShader.BeginUnLit();
	{

	}
	KdShaderManager::Instance().m_HD2DShader.EndUnLit();


	// ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== =====
	// 光源オブジェクト(自ら光るオブジェクトやエフェクト)はBeginとEndの間にまとめてDrawする
	KdShaderManager::Instance().m_postProcessShader.BeginBright();
	{

	}
	KdShaderManager::Instance().m_postProcessShader.EndBright();
}

void GameScene::Init()
{
	// カメラの初期化
	// 視野角
	m_camera.SetProjectionMatrix(60);

	// キャラ
	// 実体化→コンストラクタが自動的に動く
	m_player = std::make_shared<Player>();
	m_player->SetGameScene(this);
	m_enemy = std::make_shared<Enemy>();
	m_enemy->SetGameScene(this);

	// 地形
	m_ground = std::make_shared<Ground>();
}