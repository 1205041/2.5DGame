#include "GameScene.h"

// キャラクタ
#include "../../Object/Player/Player.h"
#include "../../Object/Enemy/Enemy.h"

// 地形
#include "../../Object/Ground/Ground.h"
#include "../../Object/SkySphere/SkySphere.h"

void GameScene::Event()
{

	/* ------------------ */
	/* ここから↓はカメラ */
	/* ------------------ */
	if (!m_upCamera) { return; }

	// カメラの座標行列
	Math::Vector3 playerPos = Math::Vector3::Zero;
	if (!m_wpPlayer.expired()) 
	{
		playerPos = m_wpPlayer.lock()->GetPos();
	}

	// ターゲットの行列
	Math::Matrix targetMat = Math::Matrix::Identity;
	if (!m_wpTarget.expired())
	{
		std::shared_ptr<const KdGameObject> spTarget = m_wpTarget.lock();
		if (spTarget)
		{
			targetMat = Math::Matrix::CreateTranslation(spTarget->GetPos());
		}
	}

	// 行列合成
	UpdateRotateByMouse();
	m_rotMat = GetRotationMatrix();
	Math::Matrix mat = m_localMat * m_rotMat * targetMat;

	m_upCamera->SetCameraMatrix(mat);
	/* ※ この段階では更新されません ！！ */
}

void GameScene::Init()
{
	/* シーンオブジェクトの初期化 */
	// キャラ
	std::shared_ptr<Player> player;
	player = std::make_shared<Player>();
	m_objList.push_back(player);
	m_wpPlayer = player;

	std::shared_ptr<Enemy> enemy;
	enemy = std::make_shared<Enemy>();
	m_objList.push_back(enemy);

	// 地形
	std::shared_ptr<Ground> ground;
	ground = std::make_shared<Ground>();
	m_objList.push_back(ground);

	std::shared_ptr<SkySphere> skySp;
	skySp = std::make_shared<SkySphere>();
	m_objList.push_back(skySp);

	// 基準点(ターゲット)の目線の位置
	m_localMat = Math::Matrix::CreateTranslation(Math::Vector3(0.0f, 1.f, -10));

	// カーソル初期化
	m_FixMousePos.x = 640;
	m_FixMousePos.y = 360;

	SetCursorPos(m_FixMousePos.x, m_FixMousePos.y);

	// カメラの初期化
	if (!m_upCamera) { m_upCamera = std::make_unique<KdCamera>(); }
	m_upCamera->SetProjectionMatrix(60);
}
