#include "Ground.h"

void Ground::Update()
{
	/* ==================== */
	/* 当たり判定(球判定用) */
	/* ==================== */
	KdCollider::SphereInfo sphereInfo;		// 球判定用の変数
	sphereInfo.m_sphere.Center = Math::Vector3(0, -5.0f, 0);	// 球の中心位置を設定
	sphereInfo.m_sphere.Radius = 8.0f;							// 球の半径を設定
	sphereInfo.m_type = KdCollider::TypeBump;	// 当たり判定をしたいタイプを設定

	/* === デバック用(球) === */
	m_debugWire.AddDebugSphere
	(
		sphereInfo.m_sphere.Center,
		sphereInfo.m_sphere.Radius
	);

	// 球に当たったオブジェクト情報を格納するリスト
	std::list<KdCollider::CollisionResult> retSphereList;

	// 球と当たり判定
	for (auto& obj : SceneManager::Instance().GetObjList())
	{
		obj->Intersects(sphereInfo, &retSphereList);
	}

	// 球に当たったリストから一番近いオブジェクトを検出
	float	maxOverLap = 0.0f;
	bool	hit = false;
	for (auto& ret : retSphereList)
	{
		// 一番めり込んだオブジェクトを探す
		if (maxOverLap < ret.m_overlapDistance)
		{
			maxOverLap = ret.m_overlapDistance;
			hit = true;
		}
	}
	if (hit)
	{
		
	}
	else 
	{

	}
}

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

// 陰影のあるオブジェクト(不透明な物体や2Dキャラ)
void Ground::DrawLit()
{
	if (!m_model) { return; }
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

void Ground::DrawDebug()
{
	m_debugWire.Draw();
}

