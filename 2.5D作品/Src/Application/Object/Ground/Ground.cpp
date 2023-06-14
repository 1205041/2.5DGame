#include "Ground.h"

void Ground::Draw()
{
	// �|��(�n��)
	KdShaderManager::Instance().m_HD2DShader.DrawModel(*m_model, m_mat);

}

void Ground::Init()
{
	//	�|��(�n��)������
	m_model = std::make_shared<KdModelWork>();
	m_model->SetModelData(KdAssets::Instance().m_modeldatas.GetData("Asset/Models/Stage/Stage.gltf"));

	// �g�k�s��
	Math::Matrix scaleMat;
	scaleMat = Math::Matrix::CreateScale(5.0f, 1.0f, 1.0f);

	// ���W�s��
	Math::Matrix transMat;
	transMat = Math::Matrix::CreateTranslation({ 0,-2,0 });

	// �s�񍇐�(�r�q�s)
	m_mat = scaleMat * transMat;

	// �����蔻��p
	m_collider.RegisterCollisionShape("StageModel", m_model, KdCollider::TypeGround);
}

