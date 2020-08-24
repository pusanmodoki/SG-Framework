#pragma once
#include	<d3d11.h>
#include	"DatFile.h"
#include "../SGFrameworkToExeProject/SGFramwork.hpp"

class CModel {
private:
	DatFile					m_datfile;						// datファイル
	ID3D11VertexShader*     m_pVertexShader = nullptr;		// 頂点シェーダー入れ物
	ID3D11PixelShader*      m_pPixelShader = nullptr;		// ピクセルシェーダー入れ物
	ID3D11InputLayout*      m_pVertexLayout = nullptr;		// 頂点フォーマット定義

	ID3D11ShaderResourceView*	m_texSRV = nullptr;			// テクスチャＳＲＶ
	
	SharedPointer<Asset::VertexShaderAsset> vsAsset;
	SharedPointer<Asset::PixelShaderAsset> psAsset;

	float tt = 0;
	float ttt = 1; 
public:
	SharedPointer<Transform> transform;
	Vector4x4 matrix;
	bool Init(const char* filename, const char* vsfile, const char* psfile);
	bool Init2(const char*  filename, const sgstring& vsfile, const sgstring& psfile);
	void Uninit();
	void Uninit2();
	void Update();
	void Update2();
	void Draw(bool a);
	void Draw2(bool a);
	DatFileLoader* GetDatFileData() {
		return m_datfile.GetDatFileLoader();
	}
};
