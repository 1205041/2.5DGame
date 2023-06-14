#pragma once

class SceneBase
{
public:
	SceneBase() { Init(); }
	~SceneBase() {}

	void PreUpdate();
	void Update();
	void PostUpdate();

	void PreDraw();
	void Draw();
	void DrawSprite();

	void DrawDebug();

	// オブジェクトリストを取得
	const std::list<std::shared_ptr<KdGameObject>>& GetObjList() { return m_objList; }
	void AddObject(const std::shared_ptr<KdGameObject>& _obj) { m_objList.push_back(_obj); }

protected:
	virtual void Init() {}
	virtual void Event() {}

	/* 変数 */
	// オブジェクトリスト
	std::list<std::shared_ptr<KdGameObject>> m_objList;

	// カメラ(回転させない！)
	std::unique_ptr<KdCamera> m_camera = nullptr;
};
