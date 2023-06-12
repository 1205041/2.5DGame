#pragma once

class SceneBase;

class SceneManager
{
public:
	enum class SceneType
	{
		Title,
		Game,
		Result,
	};

	void PreUpdate();
	void Update();
	void PostUpdate();

	void PreDraw();
	void Draw();
	void DrawSprite();

	void DrawDebug();
	void Init() { ChangeScene(SceneType::Game); }

	void SetNextScene(const SceneType& _nextScene) { m_nextSceneType = _nextScene; }

	const std::list<std::shared_ptr<KdGameObject>>& GetObjList();
	void AddObj(const std::shared_ptr<KdGameObject>& _obj);

private:
	void ChangeScene(const SceneType& _sceneType);

	// 現在のシーンを管理しているポインタ(生ポ)
	SceneBase* m_curtScene = nullptr;

	// 現在のシーンを管理している変数
	SceneType m_curtSceneType = SceneType::Game;

	// 次のシーンを管理している変数
	SceneType m_nextSceneType = m_curtSceneType;

// シングルトンパターン
private:
	SceneManager() {}
	~SceneManager() {}

public:
	static SceneManager& Instance()
	{
		static SceneManager instance;
		return instance;
	}
};
