#pragma once

class ObjBase :public KdGameObject
{
public:
	ObjBase() { Init(); }
	~ObjBase() {}

	// 更新に関する関数
	virtual void PreUpdate()	override{}
	virtual void Update()		override{}
	virtual void PostUpdate()	override{}

	// 描画に関する関数
	virtual void PreDraw()						override{}
	virtual void GenerateDepthMapFromLight()	override{}
	virtual void DrawLit()						override{}
	virtual void DrawUnLit()					override{}
	virtual void DrawBright()					override{}
	virtual void DrawSprite()					override{}
	virtual void PostDraw()						override{}
	
	virtual void Init()			override{}

	void DrawDebug()final { m_debugWire.Draw(); }

	void RegistHitObj(const std::shared_ptr<KdGameObject>& _obj) { m_wpHitObjList.push_back(_obj); }
protected:
	// 衝突判定とそれに伴う座標の更新
	void UpdateCollision();

	//srcから見たdestの角度を習得する関数
	float GetAngleDeg(Math::Vector3 _src, Math::Vector3 _dest);	//角度

	// モデルとポリゴンとカメラのスマポ
	std::shared_ptr<KdModelWork>		m_spModel	= nullptr;
	std::shared_ptr<KdSquarePolygon>	m_spPoly	= nullptr;
	std::shared_ptr<KdCamera>			m_spCamera	= nullptr;
	
	// 行列一覧
	Math::Matrix scaleMat	= Math::Matrix::Identity;	// 拡縮行列
	Math::Matrix rotMat		= Math::Matrix::Identity;	// 回転行列
	Math::Matrix transMat	= Math::Matrix::Identity;	// 座標行列
	
	// 重力
	float	m_gravity = 0.0f;

	// アニメーション
	// 歩く：8	,9	,8	,10
	int		Walk[4] = { 8,9,8,10 };
	float	m_anime	= 0.0f;
	
	// キャラの移動速度
	float			m_moveSpd	= 0.0f;
	Math::Vector3	m_nowPos	= Math::Vector3::Zero;
	Math::Vector3	m_moveVec	= Math::Vector3::Zero;
	
	// 当たり判定用変数
	std::vector<std::weak_ptr<KdGameObject>>	m_wpHitObjList;
	const float		enableStepHigh	= 0.2f;
	float			maxOverLap		= 0.0f;
	int				notHitCnt		= 0;
	bool			hit				= false;
	Math::Vector3	hitPos			= Math::Vector3::Zero;
	
	// デバック用
	KdDebugWireFrame m_debugWire;
};