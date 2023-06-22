#pragma once

class CameraBase;

class Player :public KdGameObject
{
public:
	Player() { Init(); }
	~Player() {}

	void Update()					override;
	void PostUpdate()				override;

	void GenerateDepthMapFromLight()override;
	void DrawLit()					override;

	void Init()						override;

	void DrawDebug()				override;

	void SetCamera(const std::shared_ptr<CameraBase>& _camera) { m_wpCamera = _camera; }
	
	void RegistHitObj(const std::shared_ptr<KdGameObject>& _obj) { m_wpHitObjList.push_back(_obj); }

private:
	void UpdateRotate(Math::Vector3& _srcMoveVec);

	// �Փ˔���Ƃ���ɔ������W�̍X�V
	void UpdateCollision();

	std::shared_ptr<KdSquarePolygon>			m_spPoly = nullptr;
	std::weak_ptr<CameraBase>					m_wpCamera;

	std::vector<std::weak_ptr<KdGameObject>>	m_wpHitObjList;
	
	// �L�����N�^�[�̈ړ����x
	Math::Vector3	m_nowPos;
	Math::Vector3	m_moveVec;
	float			m_moveSpd = 0.0f;

	// ���[���h�s������ׂ̂��ꂼ��̉�]�p�x
	Math::Vector3	m_worldRot;
	float			m_gravity = 0.0f;
	bool			m_pushFlg = false;

	float			m_anime = 0.0f;

	// �����蔻��p�ϐ�
	const float		enableStepHigh	= 0.2f;
	float			maxOverLap		= 0.0f;
	int				notHitCnt		= 0;
	bool			hit				= false;
	Math::Vector3	groundPos;

	
	// �f�o�b�N�p
	KdDebugWireFrame m_debugWire;
};