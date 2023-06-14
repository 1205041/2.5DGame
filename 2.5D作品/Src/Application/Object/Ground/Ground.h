#pragma once

class Ground
{
public:
	Ground() { Init(); }
	~Ground() {}

	void Draw();
	void Init();

	const KdCollider& GetCollidr() { return m_collider; }
	Math::Matrix GetMat() { return m_mat; }

private:
	std::shared_ptr<KdModelWork> m_model;
	Math::Matrix	m_mat;

	KdCollider m_collider;
};