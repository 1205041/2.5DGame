#include "GameScene.h"

// �L�����N�^
#include "../../Object/Player/Player.h"
#include "../../Object/Enemy/Enemy.h"

// �n�`
#include "../../Object/Ground/Ground.h"
#include "../../Object/SkySphere/SkySphere.h"

void GameScene::Event()
{

	/* ------------------ */
	/* �������火�̓J���� */
	/* ------------------ */
	if (!m_upCamera) { return; }

	// �J�����̍��W�s��
	Math::Vector3 playerPos = Math::Vector3::Zero;
	if (!m_wpPlayer.expired()) 
	{
		playerPos = m_wpPlayer.lock()->GetPos();
	}

	

	m_upCamera->SetCameraMatrix(mat);
	/* �� ���̒i�K�ł͍X�V����܂��� �I�I */
}

void GameScene::Init()
{
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
	player->RegistHitObj(ground);
	m_objList.push_back(player);
	m_wpPlayer = player;

	std::shared_ptr<Enemy> enemy;
	enemy = std::make_shared<Enemy>();
	m_objList.push_back(enemy);

	// �J�����̏�����
	m_upCamera = std::make_unique<KdCamera>();
	m_upCamera->SetProjectionMatrix(60);
}
