#include "TitleScene.h"

// �L�����N�^�[
#include "../../Object/Player/Player.h"

// �n�`
#include "../../Object/Ground/Ground.h"
#include "../../Object/SkySphere/SkySphere.h"

// �^�C�g���V�[��


// �J����
#include "../../Object/Camera/Tracking/Tracking.h"

void TitleScene::Event()
{
	// �V�[���ؑ�(Title��Game)
	if (GetAsyncKeyState(VK_RETURN) & 0x8000)
	{
		SceneManager::Instance().SetNextScene(SceneManager::SceneType::Game);
	}
}

void TitleScene::Init()
{
	ShowCursor(false);
}
