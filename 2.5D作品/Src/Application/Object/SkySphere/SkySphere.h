#pragma once

class SkySphere
{
public:
	SkySphere() { Init(); }
	~SkySphere() {};

	void Draw();
	void Init();

	const KdCollider& GetCollidr() { return m_collider; }
	const Math::Matrix& GetMat() { return m_mat; }

private:
	std::shared_ptr<KdModelWork> m_model;
	Math::Matrix m_mat;

	// 当たり判定用
	KdCollider m_collider;
};
