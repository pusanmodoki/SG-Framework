/*----------------------------------------------------------------------------------
Vertex structures
------------------------------------------------------------------------------------*/
#ifndef SGFRAMEWORK_HEADER_VERTEX_HPP_
#define SGFRAMEWORK_HEADER_VERTEX_HPP_
#include "../../02_CustomVariable/CustomVariable.hpp"

//Framework namespace
namespace SGFramework
{
	//Vertex structures
	namespace Vertex
	{
		//Polygon Vertex
		struct Polygon
		{
			//default
			Polygon() = default;
			//----------------------------------------------------------------------------------
			//[コンストラクタ]
			//代入初期化する
			//引数1: position
			//引数2: color
			//引数3: uv
			Polygon(const Vector3& position, const Color& color, const UV& uv/*, const UV& uvNormalMap*/)
				: position(position), color(color), uv(uv)/*, uvNormalMap(uvNormalMap)*/ {}

			Vector3 position = Const::Vector3::zero;	//Position
			Color color = Const::Color::white;			//Color
			UV uv = Const::UV::leftTop;					//UV
			//UV uvNormalMap = Const::UV::leftTop;
		};

		//Model vertex
		struct Model
		{
			//default
			Model() = default;
			//----------------------------------------------------------------------------------
			//[コンストラクタ]
			//代入初期化する
			//引数1: position
			//引数2: normal
			//引数3: uv
			//引数4: bone weight
			//引数5: bone indexes
			Model(const Vector3& position, const Vector3& normal, const UV& uv0,
				const UV& uv1, const Vector4& weight, const Vector4Int& boneIndexesVector)
				: position(position), normal(normal)//, weight(weight), boneIndexes(boneIndexes) 
			{
				uvArray[0] = uv0;
				uvArray[1] = uv1;
				
				for (int i = 0; i < 4; ++i)
					boneIndexes[i] = static_cast<uint>(boneIndexesVector[i]);
			}

			static constexpr uint cNumSupportedUVs = 2;
			static constexpr uint cNumSupportedBones = 8;
			static constexpr uint cNumMaxOneModelBoneIndexes = 999;
			static constexpr int cIdentityBoneIndex = 999;
			static constexpr int cInvalidBoneIndex = -1;

			//次に初期化すべきBoneIndex(無効->-1) (read function property)
			SGF_FUNCTION_PROPERTY int getInitialBoneIndex()
			{
				return boneIndexes[0] == cInvalidBoneIndex ? 0
					: boneIndexes[1] == cInvalidBoneIndex ? 1
					: boneIndexes[2] == cInvalidBoneIndex ? 2
					: boneIndexes[3] == cInvalidBoneIndex ? 3
					: boneIndexes[4] == cInvalidBoneIndex ? 4
					: boneIndexes[5] == cInvalidBoneIndex ? 5
					: boneIndexes[6] == cInvalidBoneIndex ? 6
					: boneIndexes[7] == cInvalidBoneIndex ? 7
					: cInvalidBoneIndex;
			}

			Vector3 position = Const::Vector3::zero;					//Position
			Vector3 normal = Const::Vector3::zero;					//Normal
			//UVs       
			std::array<UV, cNumSupportedUVs> uvArray
				= { Const::UV::leftTop,Const::UV::leftTop };
			//Weights
			std::array<float, cNumSupportedBones> weights =
			{ 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f };
			//Bone Indexes
			std::array<int, cNumSupportedBones> boneIndexes = { -1, -1, -1, -1, -1, -1, -1, -1 };
		};

		//ImGui Vertex
		struct ImGui
		{
			//default
			ImGui() = default;
			//----------------------------------------------------------------------------------
			//[コンストラクタ]
			//代入初期化する
			//引数1: position
			//引数2: uv
			//引数3: color
			ImGui(const Vector2& position, const UV& uv, uint color)
				: position(position), uv(uv), color(color){}

			Vector2 position = Const::Vector2::zero;	//Position
			UV uv = Const::UV::leftTop;					//UV
			uint color;												//Color
		};
	}
}
#endif // !SGFRAMEWORK_HEADER_VERTEX_HPP_