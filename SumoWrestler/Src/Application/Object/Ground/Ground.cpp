#include "Ground.h"

void Ground::Update()
{
	/* ==================== */
	/* �����蔻��(������p) */
	/* ==================== */
	KdCollider::SphereInfo sphereInfo;		// ������p�̕ϐ�
	sphereInfo.m_sphere.Center = Math::Vector3(0, -5.0f, 0);	// ���̒��S�ʒu��ݒ�
	sphereInfo.m_sphere.Radius = 8.0f;							// ���̔��a��ݒ�
	sphereInfo.m_type = KdCollider::TypeBump;	// �����蔻����������^�C�v��ݒ�

	/* === �f�o�b�N�p(��) === */
	m_debugWire.AddDebugSphere
	(
		sphereInfo.m_sphere.Center,
		sphereInfo.m_sphere.Radius
	);

	// ���ɓ��������I�u�W�F�N�g�����i�[���郊�X�g
	std::list<KdCollider::CollisionResult> retSphereList;

	// ���Ɠ����蔻��
	for (auto& obj : SceneManager::Instance().GetObjList())
	{
		obj->Intersects(sphereInfo, &retSphereList);
	}

	// ���ɓ����������X�g�����ԋ߂��I�u�W�F�N�g�����o
	float	maxOverLap = 0.0f;
	bool	hit = false;
	for (auto& ret : retSphereList)
	{
		// ��Ԃ߂荞�񂾃I�u�W�F�N�g��T��
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
	// �g�k�s��
	Math::Matrix scaleMat;
	scaleMat = Math::Matrix::CreateScale(0.5f, 0.5f, 0.5f);

	// ���W�s��
	Math::Matrix transMat;
	transMat = Math::Matrix::CreateTranslation({ 0,-5.0f,0 });

	// �s�񍇐�(�r�q�s)
	m_mWorld = scaleMat * transMat;
}

// �A�e�̂���I�u�W�F�N�g(�s�����ȕ��̂�2D�L����)
void Ground::DrawLit()
{
	if (!m_model) { return; }
	// �|��(�n��)
	KdShaderManager::Instance().m_HD2DShader.DrawModel(*m_model, m_mWorld);
}

void Ground::Init()
{
	//	�|��(�n��)������
	m_model = std::make_shared<KdModelWork>();
	m_model->SetModelData(KdAssets::Instance().m_modeldatas.GetData("Asset/Models/Stage/Stage.gltf"));

	// �����蔻��p
	m_pCollider = std::make_unique<KdCollider>();
	m_pCollider->RegisterCollisionShape("StageModel", m_model, KdCollider::TypeGround);
}

void Ground::DrawDebug()
{
	m_debugWire.Draw();
}

