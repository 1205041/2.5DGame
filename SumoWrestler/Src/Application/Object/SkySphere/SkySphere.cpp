#include "SkySphere.h"

void SkySphere::PostUpdate()
{
	// �g�k�s��
	scaleMat = Math::Matrix::CreateScale(15.0f, 15.0f, 15.0f);

	// ���W�s��
	transMat = Math::Matrix::CreateTranslation({ 0,-5.0f,0 });

	// �s�񍇐�(�r�q�s)
	m_mWorld = scaleMat * transMat;
}

// �A�e�̂Ȃ��I�u�W�F�N�g(�����ȕ������܂ޕ��̂�G�t�F�N�g)
void SkySphere::DrawUnLit()
{
	// �|��(�n��)
	if (!m_spModel) { return; }
	
	KdShaderManager::Instance().m_HD2DShader.DrawModel(*m_spModel, m_mWorld);
}

void SkySphere::Init()
{
	if (!m_spModel)
	{
		m_spModel = std::make_shared<KdModelWork>();
		m_spModel->SetModelData(KdAssets::Instance().m_modeldatas.GetData("Asset/Models/SkySphere/Doujyou/Doujyou.gltf"));
	}
}
