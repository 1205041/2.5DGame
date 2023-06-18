#pragma once

class SceneBase;

class SceneManager
{
public:
	// シーン情報
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
	void DrawLit();
	void DrawSprite();

	void DrawDebug();

	void Init() { ChangeScene(SceneType::Game); }
	//	void Release();

		// この関数ではシーン切替がまだ行われない！
		// あくまで予約！！
	void SetNextScene(const SceneType& _nextScene) { m_nextSceneType = _nextScene; }

	// オブジェクトリストを取得
	const std::list<std::shared_ptr<KdGameObject>>& GetObjList();
	void AddObject(const std::shared_ptr<KdGameObject>& _obj);

private:
	void ChangeScene(const SceneType& _sceneType);

	// 現在のシーンを管理しているポインタ
	SceneBase* m_curtScene = nullptr;

	// 現在のシーンを管理している変数
	SceneType m_curtSceneType = SceneType::Game;

	// 次のシーンを管理している変数
	SceneType m_nextSceneType = m_curtSceneType;

/* --- シングルトンパターン --- */
private:
	SceneManager() { Init(); }
	~SceneManager() {}

public:
	static SceneManager& Instance()
	{
		static SceneManager instace;
		return instace;
	}
};