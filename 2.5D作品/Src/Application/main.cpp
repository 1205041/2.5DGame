#include "main.h"

// エントリーポイント
// アプリケーションはこの関数から進行する
int WINAPI WinMain(HINSTANCE, HINSTANCE , LPSTR , int )
{
	// メモリリークを知らせる
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	// COM初期化
	if (FAILED(CoInitializeEx(nullptr, COINIT_MULTITHREADED)))
	{
		CoUninitialize();

		return 0;
	}

	// mbstowcs_s関数で日本語対応にするために呼ぶ
	setlocale(LC_ALL, "japanese");

	//===================================================================
	// 実行
	//===================================================================
	Application::Instance().Execute();

	// COM解放
	CoUninitialize();

	return 0;
}

// アプリケーション更新の前処理
void Application::PreUpdate()
{
	// 入力状況の更新
	KdInputManager::Instance().Update();

	KdShaderManager::Instance().WorkAmbientController().PreUpdate();
}

// アプリケーション更新
void Application::Update()
{
	
}

// アプリケーション更新の後処理
void Application::PostUpdate()
{
	// 3DSoundListnerの行列を更新
	KdAudioManager::Instance().SetListnerMatrix(KdShaderManager::Instance().GetCameraCB().mView.Invert());

}

// アプリケーション描画の前処理
void Application::PreDraw()
{
	KdDirect3D::Instance().ClearBackBuffer();

	KdShaderManager::Instance().WorkAmbientController().PreDraw();

	KdShaderManager::Instance().m_postProcessShader.PreDraw();
}

// アプリケーション描画
void Application::Draw()
{
	// ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== =====
	// 光を遮るオブジェクト(不透明な物体や2Dキャラ)はBeginとEndの間にまとめてDrawする
	KdShaderManager::Instance().m_HD2DShader.BeginGenerateDepthMapFromLight();
	{

	}
	KdShaderManager::Instance().m_HD2DShader.EndGenerateDepthMapFromLight();


	// ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== =====
	// 陰影のあるオブジェクト(不透明な物体や2Dキャラ)はBeginとEndの間にまとめてDrawする
	KdShaderManager::Instance().m_HD2DShader.BeginLit();
	{
		
	}
	KdShaderManager::Instance().m_HD2DShader.EndLit();


	// ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== =====
	// 陰影のないオブジェクト(透明な部分を含む物体やエフェクト)はBeginとEndの間にまとめてDrawする
	KdShaderManager::Instance().m_HD2DShader.BeginUnLit();
	{

	}
	KdShaderManager::Instance().m_HD2DShader.EndUnLit();


	// ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== =====
	// 光源オブジェクト(自ら光るオブジェクトやエフェクト)はBeginとEndの間にまとめてDrawする
	KdShaderManager::Instance().m_postProcessShader.BeginBright();
	{

	}
	KdShaderManager::Instance().m_postProcessShader.EndBright();
}

// アプリケーション描画の後処理
void Application::PostDraw()
{
	// 画面のぼかしや被写界深度処理の実施
	KdShaderManager::Instance().m_postProcessShader.PostEffectProcess();
}

// 2Dスプライトの描画
void Application::DrawSprite()
{
	// ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== =====
	// 2Dの描画はこの間で行う
	KdShaderManager::Instance().m_spriteShader.Begin();
	{

	}
	KdShaderManager::Instance().m_spriteShader.End();
}

// アプリケーション初期設定
bool Application::Init(int w, int h)
{
	//===================================================================
	// ウィンドウ作成
	//===================================================================
	if (!m_window.Create(w, h, "3D GameProgramming", "Window")) 
	{
		MessageBoxA(nullptr, "ウィンドウ作成に失敗", "エラー", MB_OK);
		return false;
	}

	//===================================================================
	// フルスクリーン確認
	//===================================================================
	bool bFullScreen = false;
	//if (MessageBoxA(m_window.GetWndHandle(), "フルスクリーンにしますか？", "確認", MB_YESNO | MB_ICONQUESTION | MB_DEFBUTTON2) == IDYES) { bFullScreen = true; }

	//===================================================================
	// Direct3D初期化
	//===================================================================

	// デバイスのデバッグモードを有効にする
	bool deviceDebugMode = false;
#ifdef _DEBUG
	deviceDebugMode = true;
#endif

	// Direct3D初期化
	std::string errorMsg;
	if (!KdDirect3D::Instance().Init(m_window.GetWndHandle(), w, h, deviceDebugMode, errorMsg)) 
	{
		MessageBoxA(m_window.GetWndHandle(), errorMsg.c_str(), "Direct3D初期化失敗", MB_OK | MB_ICONSTOP);
		return false;
	}

	// フルスクリーン設定
	if (bFullScreen) { KdDirect3D::Instance().WorkSwapChain()->SetFullscreenState(TRUE, 0); }

	//===================================================================
	// シェーダー初期化
	//===================================================================
	KdShaderManager::Instance().Init();

	KdAudioManager::Instance().Init();

	return true;
}

// アプリケーション実行
void Application::Execute()
{
	KdCSVData windowData("Asset/Data/WindowSettings.csv");
	const std::vector<std::string>& sizeData = windowData.GetLine(0);

	//===================================================================
	// 初期設定(ウィンドウ作成、Direct3D初期化など)
	//===================================================================
	if (!Application::Instance().Init(atoi(sizeData[0].c_str()), atoi(sizeData[1].c_str()))) { return; }

	//===================================================================
	// ゲームループ
	//===================================================================

	// 時間
	m_fpsController.Init();

	// ループ
	while (1)
	{
		// 処理開始時間Get
		m_fpsController.UpdateStartTime();

		// ゲーム終了指定があるときはループ終了
		if (m_endFlag) { break; }

		//=========================================
		// ウィンドウ関係の処理
		//=========================================

		// ウィンドウのメッセージを処理する
		m_window.ProcessMessage();

		// ウィンドウが破棄されてるならループ終了
		if (!m_window.IsCreated()) { break; }

		if (GetAsyncKeyState(VK_ESCAPE))
		{
//			if (MessageBoxA(m_window.GetWndHandle(), "本当にゲームを終了しますか？","終了確認", MB_YESNO | MB_ICONQUESTION | MB_DEFBUTTON2) == IDYES)
			{End(); }
		}

		//=========================================
		// アプリケーション更新処理
		//=========================================
		PreUpdate();
		Update();
		PostUpdate();

		//=========================================
		// アプリケーション描画処理
		//=========================================
		PreDraw();
		Draw();
		PostDraw();
		DrawSprite();

		// BackBuffer -> 画面表示
		KdDirect3D::Instance().WorkSwapChain()->Present(0, 0);

		//=========================================
		// フレームレート制御
		//=========================================

		m_fpsController.Update();
	}

	//===================================================================
	// アプリケーション解放
	//===================================================================
	Release();
}

// アプリケーション終了
void Application::Release()
{
	KdInputManager::Instance().Release();
	KdShaderManager::Instance().Release();
	KdAudioManager::Instance().Release();
	KdDirect3D::Instance().Release();

	// ウィンドウ削除
	m_window.Release();
}