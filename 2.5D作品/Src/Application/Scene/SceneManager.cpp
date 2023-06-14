#include "SceneManager.h"

#include "SceneBase/SceneBase.h"
#include "TitleScene/TitleScene.h"
#include "GameScene/GameScene.h"
#include "ResultScene/ResultScene.h"

void SceneManager::PreUpdate()
{
	// シーン切替
	if (m_curtSceneType != m_nextSceneType)
	{
		ChangeScene(m_nextSceneType);
	}

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

void SceneManager::AddObject(const std::shared_ptr<KdGameObject>& _obj)
{
	m_curtScene->AddObject(_obj);
}

void SceneManager::ChangeScene(const SceneType& _sceneType)
{
	// 現在のシーンを削除
//	if (m_currentScene) { delete m_currentScene; }
	KdSafeDelete(m_curtScene);

	// 次のシーンを作成し、現在のシーンにする
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