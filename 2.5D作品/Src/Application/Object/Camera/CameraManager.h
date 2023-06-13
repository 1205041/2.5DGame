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

	// 現在のシーンを管理しているポインタ(生ポ)
	CameraBase* m_curtCamera = nullptr;

	// 現在のシーンを管理している変数
	CameraType m_curtCameraType = CameraType::TPS;

	// 次のシーンを管理している変数
	CameraType m_nextCameraType = m_curtCameraType;

	// シングルトンパターン
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
