#include "SkySphere.h"

void SkySphere::Update()
{
	m_cnt++;
	if (m_cnt > 360)
	{
		m_cnt = 0;
	}
}

void SkySphere::PostUpdate()
{
	// 拡縮行列
	scaleMat = Math::Matrix::CreateScale(25.0f, 25.0f, 25.0f);

	// 回転行列
	if (SceneManager::Instance().GetCurtSceneType() == SceneManager::SceneType::Title)
	{
		rotMat = Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(m_cnt));
	}
	else
	{
		rotMat = Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(0));
	}
	
	// 座標行列
	transMat = Math::Matrix::CreateTranslation({ 0,-5.0f,0 });

	// 行列合成(ＳＲＴ)
	m_mWorld = scaleMat * rotMat * transMat;
}

// 陰影のないオブジェクト(透明な部分を含む物体やエフェクト)
void SkySphere::DrawUnLit()
{
	// 板ポリ(地面)
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

	m_cnt = 0;
}
