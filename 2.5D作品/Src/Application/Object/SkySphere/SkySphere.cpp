#include "SkySphere.h"

void SkySphere::Draw()
{
}

void SkySphere::Init()
{
	m_model = std::make_shared<KdModelWork>();
	m_model->SetModelData(KdAssets::Instance().m_modeldatas.GetData("Asset/Models/SkySphere/"));

	// Šgks—ñ
	Math::Matrix scaleMat;
	scaleMat = Math::Matrix::CreateScale(5.0f, 1.0f, 1.0f);

	// À•Ws—ñ
	Math::Matrix transMat;
	transMat = Math::Matrix::CreateTranslation({ 0,-2,0 });

	// s—ñ‡¬(‚r‚q‚s)
	m_mat = scaleMat * transMat;

	// “–‚½‚è”»’è—p
	m_collider.RegisterCollisionShape("StageModel", m_model, KdCollider::TypeGround);
}
