#pragma once

class ObjBase :public KdGameObject
{
public:
	ObjBase() { Init(); }
	~ObjBase() {}

	// �X�V�Ɋւ���֐�
	virtual void PreUpdate()	override{}
	virtual void Update()		override{}
	virtual void PostUpdate()	override{}

	// �`��Ɋւ���֐�
	virtual void PreDraw()						override{}
	virtual void GenerateDepthMapFromLight()	override{}
	virtual void DrawLit()						override{}
	virtual void DrawUnLit()					override{}
	virtual void DrawBright()					override{}
	virtual void DrawSprite()					override{}
	virtual void PostDraw()						override{}
	
	virtual void Init()			override{}

	void DrawDebug()final { m_debugWire.Draw(); }

	void RegistHitObj(const std::shared_ptr<KdGameObject>& _obj) { m_wpHitObjList.push_back(_obj); }
protected:
	// ���f���ƃ|���S���ƃJ�����̃X�}�|
	std::shared_ptr<KdModelWork>		m_spModel = nullptr;
	std::shared_ptr<KdSquarePolygon>	m_spPoly = nullptr;
	std::shared_ptr<KdCamera>			m_spCamera = nullptr;
	
	Math::Matrix scaleMat = Math::Matrix::Identity;	// �g�k�s��
	Math::Matrix rotMat = Math::Matrix::Identity;	// ��]�s��
	Math::Matrix transMat = Math::Matrix::Identity;	// ���W�s��
	
	// �d�͂ƃA�j���[�V����
	float			m_gravity = 0.0f;
	float			m_anime = 0.0f;
	
	// �L�����̈ړ����x
	Math::Vector3	m_nowPos;
	Math::Vector3	m_moveVec;
	
	// �����蔻��p�ϐ�
	std::vector<std::weak_ptr<KdGameObject>>	m_wpHitObjList;
	const float		enableStepHigh	= 0.2f;
	float			maxOverLap		= 0.0f;
	int				notHitCnt		= 0;
	bool			hit				= false;
	Math::Vector3	groundPos;
	
	// �f�o�b�N�p
	KdDebugWireFrame m_debugWire;
};