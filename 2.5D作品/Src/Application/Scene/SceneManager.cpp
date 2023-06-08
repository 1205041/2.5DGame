#include "SceneManager.h"
#include "SceneBase/SceneBase.h"

#include "TitleScene/TitleScene.h"
#include "GameScene/GameScene.h"
#include "ResultScene/ResultScene.h"

void SceneManager::PreUpdate()
{
	// シーン切替
	if (m_curtSceneType != m_nextSceneType) { ChangeScene(m_nextSceneType); }

	m_curtScene->PreUpdate();
}

void SceneManager::Update()
{
	m_curtScene->Update();
}

void SceneManager::PostUpdate()
{
	m_curtScene->PostUpdate();
}

void SceneManager::PreDraw()
{
	m_curtScene->PreDraw();
}

void SceneManager::Draw()
{
	m_curtScene->Draw();
}

void SceneManager::DrawSprite()
{
	m_curtScene->DrawSprite();
}

void SceneManager::DrawDebug()
{
	m_curtScene->DrawDebug();
}

const std::list<std::shared_ptr<KdGameObject>>& SceneManager::GetObjList()
{
	// TODO: return ステートメントをここに挿入します
	return m_curtScene->GetObjList();
}

void SceneManager::AddObj(const std::shared_ptr<KdGameObject>& _obj)
{
	m_curtScene->AddObj(_obj);
}

void SceneManager::ChangeScene(const SceneType& _sceneType)
{
	// 現在のシーンの削除
	KdSafeDelete(m_curtScene);

	// シーン遷移(次のシーン作成・現在のシーンにする)
	switch (_sceneType)
	{
	case SceneType::Title:
		m_curtScene = new TitleScene();
		break;
	case SceneType::Game:
		m_curtScene = new GameScene();
		break;
	case SceneType::Result:
		m_curtScene = new ResultScene();
		break;
	}
	m_curtSceneType = _sceneType;
}
