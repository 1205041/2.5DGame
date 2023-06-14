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

	// �I�u�W�F�N�g���X�g���擾
	const std::list<std::shared_ptr<KdGameObject>>& GetObjList() { return m_objList; }
	void AddObject(const std::shared_ptr<KdGameObject>& _obj) { m_objList.push_back(_obj); }

protected:
	virtual void Init() {}
	virtual void Event() {}

	/* �ϐ� */
	// �I�u�W�F�N�g���X�g
	std::list<std::shared_ptr<KdGameObject>> m_objList;

	// �J����(��]�����Ȃ��I)
	std::unique_ptr<KdCamera> m_camera = nullptr;
};
