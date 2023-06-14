#pragma once
#include "../BaseObject.h"

class Player :public BaseObject
{
public:
	Player() { Init(); }
	~Player() {}

	void Update()override;
	void Draw()override;
	void Init()override;

	Math::Vector3 GetPos() { return m_pos; }
	// もう一つの座標の取り方
//	m_pos;
//	m_mat.Translation();
 	// 使い方の一例(セッター,ゲッター共用として利用)
//	Math::Vector3 pos = m_mat.Translation();
//	m_mat.Translation({ 0,1,0 });

private:
	
};