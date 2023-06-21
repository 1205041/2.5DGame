#pragma once

class CameraBase;

class CameraManager
{
public:
	// �J�������
	enum class CameraType
	{
		Tracking,
		TPS,
		FPS,
	};

	void PreUpdate();
	void Update();
	void PreDraw();
	void Init() { ChangeScene(CameraType::Tracking); }
	
	// ���̊֐��ł̓V�[���ؑւ��܂��s���Ȃ��I
	void SetNextScene(const CameraType& _nextScene) { m_nextCameraType = _nextScene; }

	// �I�u�W�F�N�g���X�g���擾
	const std::list<std::shared_ptr<KdGameObject>>& GetObjList();
	void AddObject(const std::shared_ptr<KdGameObject>& _obj);
private:
	void ChangeScene(const CameraType& _sceneType);

	// ���݂̃J�������Ǘ����Ă���|�C���^
	CameraBase* m_curtCamera = nullptr;

	// ���݂̃J�������Ǘ����Ă���ϐ�
	CameraType m_curtCameraType = CameraType::Tracking;

	// ���̃J�������Ǘ����Ă���ϐ�
	CameraType m_nextCameraType = m_curtCameraType;

/* --- �V���O���g���p�^�[�� --- */
private:
	CameraManager() {}
	~CameraManager() {}
public:
	static CameraManager& Instance()
	{
		static CameraManager instace;
		return instace;
	}
};
