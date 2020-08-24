/*----------------------------------------------------------------------------------
ColliderのTriggerを指定するTriggerCollider.h
------------------------------------------------------------------------------------*/
#ifndef SGFRAMEWORK_HEADER_COLLIDER_TRIGGER_HPP_
#define SGFRAMEWORK_HEADER_COLLIDER_TRIGGER_HPP_

//Framework namespace
namespace SGFramework
{
	//コライダーのトリガー
	enum class Triggers
	{
		TriggerNull = 0x0,	//初期値
		TriggerFalse = 0x1,	//制限なし
		TriggerTrue = 0x2,	//制限あり
	};

	//コライダーの種類
	enum class Colliders
	{
		Mesh,					//メッシュコライダー
		Ray,						//レイキャスト
		Box3D,					//ボックスコライダー (3D)
		Box2D,					//ボックスコライダー (2D)
		Sphere,					//スフィアコライダー (3D)
		Circle,					//サークルコライダー (2D)
	};
}
#endif // !SGFRAMEWORK_HEADER_COLLIDER_TRIGGER_HPP_
