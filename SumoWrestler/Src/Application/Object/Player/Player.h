#pragma once

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

	void SetCamera(const std::shared_ptr<SceneBase>& _camera) { m_wpCamera = _camera; }

private:
	void UpdateRotate(Math::Vector3& _srcMoveVec);

	// �Փ˔���Ƃ���ɔ������W�̍X�V
	void UpdateCollision();

	std::shared_ptr<KdSquarePolygon> m_spPoly = nullptr;
	std::weak_ptr<SceneBase>		 m_wpCamera;

	Math::Vector3	m_moveVec;
	float			m_anime = 0.0f;

	float			m_gravity = 0.0f;
	bool			m_jumpFlg = false;

	// ���[���h�s������ׂ̂��ꂼ��̉�]�p�x
	Math::Vector3	m_worldRot;

	Math::Vector3	nowPos;
	float			moveSpd = 0.0f;

	// �f�o�b�N�p
	KdDebugWireFrame m_debugWire;
};