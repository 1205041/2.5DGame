#include "CameraManager.h"

#include "Tracking/Tracking.h"
#include "TPS/TPS.h"
#include "FPS/FPS.h"

void CameraManager::PreUpdate()
{
	// ƒV[ƒ“Ø‘Ö
	if (m_curtCameraType != m_nextCameraType)
	{
		ChangeScene(m_nextCameraType);
	}
}

void CameraManager::Update()
{
	m_curtCamera->Update();
}

void CameraManager::PreDraw()
{
	m_curtCamera->PreDraw();
}
