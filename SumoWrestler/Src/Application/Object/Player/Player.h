#pragma once
#include "../ObjBase.h"

class CameraBase;

class Player :public ObjBase
{
public:
	Player() { Init(); }
	~Player() {}

	void Update()					override;
	void PostUpdate()				override;

	void GenerateDepthMapFromLight()override;
	void DrawLit()					override;

	void Init()						override;

	void SetCamera(const std::shared_ptr<CameraBase>& _camera) { m_wpCamera = _camera; }
	
	void RegistHitObj(const std::shared_ptr<KdGameObject>& _obj) { m_wpHitObjList.push_back(_obj); }

private:
	void UpdateRotate(Math::Vector3& _srcMoveVec);

	// �Փ˔���Ƃ���ɔ������W�̍X�V
	void UpdateCollision();

	std::weak_ptr<CameraBase>					m_wpCamera;

	// �����蔻�胊�X�g
	std::vector<std::weak_ptr<KdGameObject>>	m_wpHitObjList;
	
	// �L�����N�^�[�̈ړ����x
	float			m_moveSpd = 0.0f;

	// ���[���h�s������ׂ̂��ꂼ��̉�]�p�x
	Math::Vector3	m_worldRot;
	
	bool			m_pushFlg = false;
};