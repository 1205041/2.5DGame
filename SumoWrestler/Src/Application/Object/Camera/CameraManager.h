#pragma once

class CameraBase;

class CameraManager
{
public:
	// カメラ情報
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
	
	// この関数ではシーン切替がまだ行われない！
	void SetNextScene(const CameraType& _nextScene) { m_nextCameraType = _nextScene; }

	// オブジェクトリストを取得
	const std::list<std::shared_ptr<KdGameObject>>& GetObjList();
	void AddObject(const std::shared_ptr<KdGameObject>& _obj);
private:
	void ChangeScene(const CameraType& _sceneType);

	// 現在のカメラを管理しているポインタ
	CameraBase* m_curtCamera = nullptr;

	// 現在のカメラを管理している変数
	CameraType m_curtCameraType = CameraType::Tracking;

	// 次のカメラを管理している変数
	CameraType m_nextCameraType = m_curtCameraType;

/* --- シングルトンパターン --- */
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
