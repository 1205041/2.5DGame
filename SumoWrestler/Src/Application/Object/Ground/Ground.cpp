#include "Ground.h"

void Ground::PostUpdate()
{
	// 拡縮行列
	Math::Matrix scaleMat;
	scaleMat = Math::Matrix::CreateScale(0.5f, 0.5f, 0.5f);

	// 座標行列
	Math::Matrix transMat;
	transMat = Math::Matrix::CreateTranslation({ 0,-5.0f,0 });

	// 行列合成(ＳＲＴ)
	m_mWorld = scaleMat * transMat;
}

void Ground::DrawLit()
{
	// 板ポリ(地面)
	KdShaderManager::Instance().m_HD2DShader.DrawModel(*m_model, m_mWorld);
}

void Ground::Init()
{
	//	板ポリ(地面)初期化
	m_model = std::make_shared<KdModelWork>();
	m_model->SetModelData(KdAssets::Instance().m_modeldatas.GetData("Asset/Models/Stage/Stage.gltf"));

	// 当たり判定用
	m_pCollider = std::make_unique<KdCollider>();
	m_pCollider->RegisterCollisionShape("StageModel", m_model, KdCollider::TypeGround);
}

