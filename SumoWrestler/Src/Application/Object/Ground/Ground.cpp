#include "Ground.h"

void Ground::PostUpdate()
{
	// 拡縮行列
	scaleMat = Math::Matrix::CreateScale(0.3f, 0.3f, 0.3f);

	// 座標行列
	transMat = Math::Matrix::CreateTranslation({ 0,-5.0f,0 });

	// 行列合成(ＳＲＴ)
	m_mWorld = scaleMat * transMat;
}

// 陰影のあるオブジェクト(不透明な物体や2Dキャラ)
void Ground::DrawLit()
{
	if (!m_spModel) { return; }
	// 板ポリ(地面)
	KdShaderManager::Instance().m_HD2DShader.DrawModel(*m_spModel, m_mWorld);
}

void Ground::Init()
{
	if (!m_spModel)
	{
		//	板ポリ(地面)初期化
		m_spModel = std::make_shared<KdModelWork>();
		m_spModel->SetModelData(KdAssets::Instance().m_modeldatas.GetData("Asset/Models/Stage/Stage.gltf"));
	}
	
	m_pCollider = std::make_unique<KdCollider>();
	m_pCollider->RegisterCollisionShape("StageModel", m_spModel, KdCollider::TypeGround);
}
