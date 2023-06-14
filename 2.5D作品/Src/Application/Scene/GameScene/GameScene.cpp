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
	
	// �J�����̍��W�s��
	Math::Matrix transMat;
	transMat = Math::Matrix::CreateTranslation(Math::Vector3(0, 1, -3) + m_player->GetPos());
	m_camera.SetCameraMatrix(transMat);
	/* �� ���̒i�K�ł͍X�V����܂��� �I�I */
}

void GameScene::PreDraw()
{
	// �J�����X�V
	m_camera.SetToShader();
}

void GameScene::Draw()
{
	// ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== =====
	// �����Ղ�I�u�W�F�N�g(�s�����ȕ��̂�2D�L����)��Begin��End�̊Ԃɂ܂Ƃ߂�Draw����
	KdShaderManager::Instance().m_HD2DShader.BeginGenerateDepthMapFromLight();
	{

	}
	KdShaderManager::Instance().m_HD2DShader.EndGenerateDepthMapFromLight();


	// ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== =====
	// �A�e�̂���I�u�W�F�N�g(�s�����ȕ��̂�2D�L����)��Begin��End�̊Ԃɂ܂Ƃ߂�Draw����
	KdShaderManager::Instance().m_HD2DShader.BeginLit();
	{
		m_enemy->Draw();
		m_player->Draw();

		m_ground->Draw();
	}
	KdShaderManager::Instance().m_HD2DShader.EndLit();


	// ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== =====
	// �A�e�̂Ȃ��I�u�W�F�N�g(�����ȕ������܂ޕ��̂�G�t�F�N�g)��Begin��End�̊Ԃɂ܂Ƃ߂�Draw����
	KdShaderManager::Instance().m_HD2DShader.BeginUnLit();
	{

	}
	KdShaderManager::Instance().m_HD2DShader.EndUnLit();


	// ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== =====
	// �����I�u�W�F�N�g(�������I�u�W�F�N�g��G�t�F�N�g)��Begin��End�̊Ԃɂ܂Ƃ߂�Draw����
	KdShaderManager::Instance().m_postProcessShader.BeginBright();
	{

	}
	KdShaderManager::Instance().m_postProcessShader.EndBright();
}

void GameScene::Init()
{
	// �J�����̏�����
	// ����p
	m_camera.SetProjectionMatrix(60);

	// �L����
	// ���̉����R���X�g���N�^�������I�ɓ���
	m_player = std::make_shared<Player>();
	m_player->SetGameScene(this);
	m_enemy = std::make_shared<Enemy>();
	m_enemy->SetGameScene(this);

	// �n�`
	m_ground = std::make_shared<Ground>();
}