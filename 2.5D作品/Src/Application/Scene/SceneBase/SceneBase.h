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

	const std::list<std::shared_ptr<KdGameObject>>& GetObjList() { return m_objList; }
	void AddObj(const std::shared_ptr<KdGameObject>& _obj) { m_objList.push_back(_obj); }

protected:
	virtual void Init() {}
	virtual void Event() {}

	std::list<std::shared_ptr<KdGameObject>> m_objList;
	std::unique_ptr<KdCamera> m_upCamera = nullptr;

private:
};