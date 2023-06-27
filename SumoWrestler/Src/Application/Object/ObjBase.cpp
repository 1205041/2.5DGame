#include "ObjBase.h"

float ObjBase::GetAngleDeg(Math::Vector3 _src, Math::Vector3 _dest)
{
	//手順1. 横方向の距離aと縦方向の距離bを減算で求める
	float a = _dest.x - _src.x;
	float b = _dest.z - _src.z;

	//手順2. atan2 関数を活用して角度を求める
	float rad = atan2(b, a);

	//手順3. 補正処理:必要に応じて、デグリー(度)に直す
	float deg = DirectX::XMConvertToDegrees(rad);

	//手順4.0～360に補正
	if (deg < 0) { deg += 360; }

	return deg;
}
