#include "SceneBase.h"

void SceneBase::PreUpdate()
{
	/* オブジェクトリストの整理 */
	auto it = m_objList.begin();
	while (it != m_objList.end())
	{
		// 期限切れ
		if ((*it)->IsExpired())
		{
			it = m_objList.erase(it);
		}
		else
		{
			++it;
		}
	}
}

void SceneBase::Update()
{
	for (auto& obj : m_objList) { obj->Update(); }

	Event();
}

void SceneBase::PostUpdate()
{
	for (auto& obj : m_objList) { obj->PostUpdate(); }
}

void SceneBase::PreDraw()
{
	if (!m_camera)return;

	// カメラ更新
	m_camera->SetToShader();
}

void SceneBase::Draw()
{
	// ===== ===== ===== ===== ===== ===== ===== =====
	// 光を遮るオブジェクト(不透明な物体や2Dキャラ)
	// はBeginとEndの間にまとめてDrawする
	// ===== ===== ===== ===== ===== ===== ===== =====
	KdShaderManager::Instance().m_HD2DShader.BeginGenerateDepthMapFromLight();
	{
		for (auto& obj : m_objList) { obj->GenerateDepthMapFromLight(); }
	}
	KdShaderManager::Instance().m_HD2DShader.EndGenerateDepthMapFromLight();

	// ===== ===== ===== ===== ===== ===== ===== =====
	// 陰影のあるオブジェクト(不透明な物体や2Dキャラ)
	// はBeginとEndの間にまとめてDrawする
	// ===== ===== ===== ===== ===== ===== ===== =====
	KdShaderManager::Instance().m_HD2DShader.BeginLit();
	{
		for (auto& obj : m_objList) { obj->DrawLit(); }
	}
	KdShaderManager::Instance().m_HD2DShader.EndLit();

	// ===== ===== ===== ===== ===== ===== ===== ===== ===== =====
	// 陰影のないオブジェクト(透明な部分を含む物体やエフェクト)
	// はBeginとEndの間にまとめてDrawする
	// ===== ===== ===== ===== ===== ===== ===== ===== ===== =====
	KdShaderManager::Instance().m_HD2DShader.BeginUnLit();
	{
		for (auto& obj : m_objList) { obj->DrawUnLit(); }
	}
	KdShaderManager::Instance().m_HD2DShader.EndUnLit();

	// ===== ===== ===== ===== ===== ===== ===== ===== =====
	// 光源オブジェクト(自ら光るオブジェクトやエフェクト)
	// はBeginとEndの間にまとめてDrawする
	// ===== ===== ===== ===== ===== ===== ===== ===== =====
	KdShaderManager::Instance().m_postProcessShader.BeginBright();
	{
		for (auto& obj : m_objList) { obj->DrawBright(); }
	}
	KdShaderManager::Instance().m_postProcessShader.EndBright();
}

void SceneBase::DrawSprite()
{
	// ===== ===== ===== =====
	// 2Dの描画はこの間で行う
	// ===== ===== ===== =====
	KdShaderManager::Instance().m_spriteShader.Begin();
	{
		for (auto& obj : m_objList) { obj->DrawSprite(); }
	}
	KdShaderManager::Instance().m_spriteShader.End();
}

void SceneBase::DrawDebug()
{
	// ===== ===== ===== ===== ===== ===== ===== ===== ===== =====
	// 陰影のないオブジェクト(透明な部分を含む物体やエフェクト)
	// はBeginとEndの間にまとめてDrawする
	// ===== ===== ===== ===== ===== ===== ===== ===== ===== =====
	KdShaderManager::Instance().m_HD2DShader.BeginUnLit();
	{
		for (auto& obj : m_objList) { obj->DrawDebug(); }
	}
	KdShaderManager::Instance().m_HD2DShader.EndUnLit();
}