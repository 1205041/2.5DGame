#include "SkySphere.h"

void SkySphere::PostUpdate()
{
	// 拡縮行列
	Math::Matrix scaleMat;
	scaleMat = Math::Matrix::CreateScale(20.0f, 20.0f, 20.0f);

	// 座標行列
	Math::Matrix transMat;
	transMat = Math::Matrix::CreateTranslation({ 0,-5.0f,0 });

	// 行列合成(ＳＲＴ)
	m_mWorld = scaleMat * transMat;
}

// 陰影のないオブジェクト(透明な部分を含む物体やエフェクト)
void SkySphere::DrawUnLit()
{
	if (!m_model) { return; }
	// 板ポリ(地面)
	KdShaderManager::Instance().m_HD2DShader.DrawModel(*m_model, m_mWorld);
}

void SkySphere::Init()
{
	m_model = std::make_shared<KdModelWork>();
	m_model->SetModelData(KdAssets::Instance().m_modeldatas.GetData("Asset/Models/SkySphere/Doujyou/Doujyou.gltf"));
}
