#pragma once

class CameraBase;

class CameraManager
{
public:
	enum class CameraType
	{
		FPS,
		TPS,
		Tracking,
	};

	void PreUpdate();
	void Update();
	void PostUpdate();

	void PreDraw();
	void Draw();
	void DrawSprite();

	void DrawDebug();
	void Init() { ChangeCamera(CameraType::TPS); }

	void SetNextScene(const CameraType& _nextCamera) { m_nextCameraType = _nextCamera; }

	const std::list<std::shared_ptr<KdGameObject>>& GetObjList();
	void AddObj(const std::shared_ptr<KdGameObject>& _obj);

private:
	void ChangeCamera(const CameraType& _cameraType);

	// ���݂̃V�[�����Ǘ����Ă���|�C���^(���|)
	CameraBase* m_curtCamera = nullptr;

	// ���݂̃V�[�����Ǘ����Ă���ϐ�
	CameraType m_curtCameraType = CameraType::TPS;

	// ���̃V�[�����Ǘ����Ă���ϐ�
	CameraType m_nextCameraType = m_curtCameraType;

	// �V���O���g���p�^�[��
private:
	CameraManager() {}
	~CameraManager() {}

public:
	static CameraManager& Instance()
	{
		static CameraManager instance;
		return instance;
	}
};
