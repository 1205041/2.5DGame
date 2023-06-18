#include "GameScene.h"

#include "../../Object/Player/Player.h"
#include "../../Object/Enemy/Enemy.h"

#include "../../Object/Ground/Ground.h"
#include "../../Object/Scaffold/Scaffold.h"
#include "../../Object/SkySphere/SkySphere.h"

void GameScene::Event()
{
	// �J�����̍��W�s��
	Math::Vector3 playerPos = Math::Vector3::Zero;
	if (!m_wpPlayer.expired()) 
	{
		playerPos = m_wpPlayer.lock()->GetPos();
	}

	Math::Matrix transMat = Math::Matrix::Identity;
	transMat = Math::Matrix::CreateTranslation(Math::Vector3(0, 2.5f, -3) + playerPos);

	// �J�����̉�]�s��
	Math::Matrix rotMat = Math::Matrix::Identity;
	rotMat = Math::Matrix::CreateRotationX(DirectX::XMConvertToRadians(30));

	// �s�񍇐�(�g�k(S) * ��](R) * ���W(T))
	Math::Matrix mat = rotMat * transMat;

	m_camera->SetCameraMatrix(mat);
	/* �� ���̒i�K�ł͍X�V����܂��� �I�I */

}

void GameScene::Init()
{
	// �J�����̏�����
	m_camera = std::make_unique<KdCamera>();
	m_camera->SetProjectionMatrix(60);

	// �I�u�W�F�N�g�̏�����
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
}
