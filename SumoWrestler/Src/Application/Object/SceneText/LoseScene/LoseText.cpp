#include "LoseText.h"

void LoseText::PostUpdate()
{
	// �g�k�s��
	scaleMat = Math::Matrix::CreateScale(5.0f, 5.0f, 5.0f);

	// ��]�s��
	rotMat = Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(90));
	
	// ���W�s��
	transMat = Math::Matrix::CreateTranslation({ 0,0,0 });

	// �s�񍇐�(�r�q�s)
	m_mWorld = scaleMat * rotMat * transMat;
}

void LoseText::DrawUnLit()
{
	if (!m_spPoly) { return; }

	KdShaderManager::Instance().m_HD2DShader.DrawPolygon(*m_spPoly, m_mWorld);
}

void LoseText::Init()
{
	if (!m_spPoly)
	{
		m_spPoly = std::make_shared<KdSquarePolygon>();
		m_spPoly->SetMaterial(KdAssets::Instance().m_textures.GetData("Asset/Textures/Result/lose.png"));
	}
}