/*----------------------------------------------------------------------------------
íËêîÇìZÇﬂÇΩConst namespace
------------------------------------------------------------------------------------*/
#ifndef SGFRAMEWORK_HEADER_CONST_VECTOR_HPP_
#define SGFRAMEWORK_HEADER_CONST_VECTOR_HPP_
#include "../Vector/Vector.hpp"

//Framework namespace
namespace SGFramework
{
	//Constant Values
	namespace Const
	{
		//Vector2 Constant Values
		namespace Vector2
		{
			//Vector2(0,0)
			constexpr SGFramework::Vector2 zero(0.0f, 0.0f);
			//Vector2(1,1,1,1)
			constexpr SGFramework::Vector2 one(1.0f, 1.0f);
			//Vector2(0,1)
			constexpr SGFramework::Vector2 up(0.0f, 1.0f);
			//Vector2(0,-1)
			constexpr SGFramework::Vector2 down(0.0f, -1.0f);
			//Vector2(1,0)
			constexpr SGFramework::Vector2 right(1.0f, 0.0f);
			//Vector2(-1,0)
			constexpr SGFramework::Vector2 left(-1.0f, 0.0f);
		}

		//Vector2Int Constant Values
		namespace Vector2Int
		{
			//Vector2Int(0,0)
			constexpr SGFramework::Vector2Int zero(0, 0);
			//Vector2Int(1,1)
			constexpr SGFramework::Vector2Int one(1, 1);
			//Vector2Int(0,1)
			constexpr SGFramework::Vector2Int up(0, 1);
			//Vector2Int(0,-1)
			constexpr SGFramework::Vector2Int down(0, -1);
			//Vector2Int(1,0)
			constexpr SGFramework::Vector2Int right(1, 0);
			//Vector2Int(-1,0)
			constexpr SGFramework::Vector2Int left(-1, 0);
		}

		//Vector3 Constant Values
		namespace Vector3
		{
			//Vector3(0,0,0)
			constexpr SGFramework::Vector3 zero(0.0f, 0.0f, 0.0f);
			//Vector3(1,1,1)
			constexpr SGFramework::Vector3 one(1.0f, 1.0f, 1.0f);
			//Vector3(0,1,0)
			constexpr SGFramework::Vector3 up(0.0f, 1.0f, 0.0f);
			//Vector3(0,-1,0)
			constexpr SGFramework::Vector3 down(0.0f, -1.0f, 0.0f);
			//Vector3(1,0,0)
			constexpr SGFramework::Vector3 right(1.0f, 0.0f, 0.0f);
			//Vector3(-1,0,0)
			constexpr SGFramework::Vector3 left(-1.0f, 0.0f, 0.0f);
			//Vector3(0,0,1)
			constexpr SGFramework::Vector3 foward(0.0f, 0.0f, 1.0f);
			//Vector3(0,0,-1)
			constexpr SGFramework::Vector3 back(0.0f, 0.0f, -1.0f);
			//Vector3(-1,1, 0)
			constexpr SGFramework::Vector3 leftTop(-1.0f, 1.0f, 0.0f);
			//Vector3(1,1,0)
			constexpr SGFramework::Vector3 rightTop(1.0f, 1.0f, 0.0f);
			//Vector3(-1,-1,0)
			constexpr SGFramework::Vector3 leftDown(-1.0f, -1.0f, 0.0f);
			//Vector3(1,-1,0)
			constexpr SGFramework::Vector3 rightDown(1.0f, -1.0f, 0.0f);
		}

		//Vector3Int Constant Values
		namespace Vector3Int
		{
			//Vector3Int(0,0,0)
			constexpr SGFramework::Vector3Int zero(0, 0, 0);
			//Vector3Int(1,1,1)
			constexpr SGFramework::Vector3Int one(1, 1, 1);
			//Vector3Int(0,1,0)
			constexpr SGFramework::Vector3Int up(0, 1, 0);
			//Vector3Int(0,-1,0)
			constexpr SGFramework::Vector3Int down(0, -1, 0);
			//Vector3Int(1,0,0)
			constexpr SGFramework::Vector3Int right(1, 0, 0);
			//Vector3Int(-1,0,0)
			constexpr SGFramework::Vector3Int left(-1, 0, 0);
			//Vector3Int(0,0,1)
			constexpr SGFramework::Vector3Int foward(0, 0, 1);
			//Vector3Int(0,0,-1)
			constexpr SGFramework::Vector3Int back(0, 0, -1);
		}

		//Vector4 Constant Values
		namespace Vector4
		{
			//Vector4(0,0,0,0)
			constexpr SGFramework::Vector4 zero(0.0f, 0.0f, 0.0f, 0.0f);
			//Vector4(1,1,1,1)
			constexpr SGFramework::Vector4 one(1.0f, 1.0f, 1.0f, 1.0f);
		}

		//Vector4Int Constant Values
		namespace Vector4Int
		{
			//Vector4Int(0,0,0,0)
			constexpr SGFramework::Vector4Int zero(0, 0, 0, 0);
			//Vector4Int(1,1,1,1)
			constexpr SGFramework::Vector4Int one(1, 1, 1, 1);
		}

		//Vector4x4 Constant Values
		namespace Vector4x4
		{
			//All 0
			constexpr SGFramework::Vector4x4 zero(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
			//All 1
			constexpr SGFramework::Vector4x4 one(1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);
			//_11, _22, _33,_44 = 1 , other = 0
			constexpr SGFramework::Vector4x4 identity(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);
		}
	}
}


#endif // !SGFRAMEWORK_HEADER_CONST_VECTOR_HPP_
