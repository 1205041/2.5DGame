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

	// �^�[�Q�b�g�̍s��
	Math::Matrix targetMat = Math::Matrix::Identity;
	if (!m_wpTarget.expired())
	{
		std::shared_ptr<const KdGameObject> spTarget = m_wpTarget.lock();
		if (spTarget)
		{
			targetMat = Math::Matrix::CreateTranslation(spTarget->GetPos());
		}
	}

	// �s�񍇐�
	UpdateRotateByMouse();
	m_rotMat = GetRotationMatrix();
	Math::Matrix mat = m_localMat * m_rotMat * targetMat;

	m_upCamera->SetCameraMatrix(mat);
	/* �� ���̒i�K�ł͍X�V����܂��� �I�I */
}

void GameScene::Init()
{
	/* �V�[���I�u�W�F�N�g�̏����� */
	// �L����
	std::shared_ptr<Player> player;
	player = std::make_shared<Player>();
	m_objList.push_back(player);
	m_wpPlayer = player;

	std::shared_ptr<Enemy> enemy;
	enemy = std::make_shared<Enemy>();
	m_objList.push_back(enemy);

	// �n�`
	std::shared_ptr<Ground> ground;
	ground = std::make_shared<Ground>();
	m_objList.push_back(ground);

	std::shared_ptr<SkySphere> skySp;
	skySp = std::make_shared<SkySphere>();
	m_objList.push_back(skySp);

	// ��_(�^�[�Q�b�g)�̖ڐ��̈ʒu
	m_localMat = Math::Matrix::CreateTranslation(Math::Vector3(0.0f, 1.f, -10));

	// �J�[�\��������
	m_FixMousePos.x = 640;
	m_FixMousePos.y = 360;

	SetCursorPos(m_FixMousePos.x, m_FixMousePos.y);

	// �J�����̏�����
	if (!m_upCamera) { m_upCamera = std::make_unique<KdCamera>(); }
	m_upCamera->SetProjectionMatrix(60);
}
