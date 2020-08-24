#include	<windows.h>
#include	"CModel.h"
#include	"Shader.h"
#include "../SGFrameworkToExeProject/SGFramwork.hpp"
#include	"DX11util.h"
#include	"CCamera.h"
#include	"DX11Settransform.h"

struct HALVertex {
	float pos[3];
	float col[4];
};

HALVertex g_VertexList[]{
		{ { -0.5f,  0.5f, 0.5f }, { 1.0f, 0.0f, 0.0f, 1.0f } },
		{ {  0.5f, -0.5f, 0.5f }, { 0.0f, 1.0f, 0.0f, 1.0f } },
		{ { -0.5f, -0.5f, 0.5f }, { 0.0f, 0.0f, 1.0f, 1.0f } }
};
ID3D11Buffer* vBuffer;


bool CModel::Init(const char* filename, const char* vsfile, const char* psfile) {

	bool sts;

	// 飛行機のモデルデータを読み込み
	sts = m_datfile.Load(filename, SGFramework::Graphics::device(), SGFramework::Graphics::context());
	if (!sts) {
		char str[128];
		sprintf_s(str, 128,"%s load ERROR!!", filename);
		//MessageBox(nullptr, str, "error", MB_OK);
		return false;
	}

	// 飛行機用の頂点データの定義
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT,		0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",		0, DXGI_FORMAT_R32G32B32_FLOAT,	    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT,		0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	unsigned int numElements = ARRAYSIZE(layout);

	// 頂点シェーダーオブジェクトを生成、同時に頂点レイアウトも生成
	sts = CreateVertexShader(SGFramework::Graphics::device(),
		vsfile,
		"main",
		"vs_5_0",
		layout,
		numElements,
		&m_pVertexShader,
		&m_pVertexLayout);
	if (!sts) {
		MessageBox(nullptr, L"CreateVertexShader error", L"error", MB_OK);
		return false;
	}

	// ピクセルシェーダーを生成
	sts = CreatePixelShader(			// ピクセルシェーダーオブジェクトを生成
		SGFramework::Graphics::device(),		// デバイスオブジェクト
		psfile,
		"main",
		"ps_5_0",
		&m_pPixelShader);
	if (!sts) {
		MessageBox(nullptr, L"CreatePixelShader error", L"error", MB_OK);
		return false;
	}
	matrix = Const::Vector4x4::identity;
	matrix._41 -= 10.0f;
	return true;
}

bool CModel::Init2(const char * filename, const sgstring & vsfile, const sgstring & psfile)
{
	bool sts;

	// 飛行機のモデルデータを読み込み
	sts = m_datfile.Load(filename, SGFramework::Graphics::device(), SGFramework::Graphics::context());
	if (!sts) {
		char str[128];
		sprintf_s(str, 128, "%s load ERROR!!", filename);
		//MessageBox(nullptr, str, "error", MB_OK);
		return false;
	}

	transform = Transform::Instantiate();
	vsAsset = Asset::InstantiateAsset<Asset::VertexShaderAsset>(vsfile);
	psAsset = Asset::InstantiateAsset<Asset::PixelShaderAsset>(psfile);
	
	try
	{
		vsAsset->Open(Asset::VertexShaderAsset::CreateInputElements(VSInputElement("POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT),
			VSInputElement("NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT), VSInputElement("TEXCOORD", 0, DXGI_FORMAT_R32G32B32_FLOAT)));
		psAsset->Open();
	}
	catch (...)
	{
		throw 1;
	}
//	transform->accessWrite->worldPosition.x = -10.0f;


	D3D11_BUFFER_DESC bufferDesc;
	bufferDesc.ByteWidth = sizeof(HALVertex) * 3;
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA subResourceData;
	subResourceData.pSysMem = g_VertexList;
	subResourceData.SysMemPitch = 0;
	subResourceData.SysMemSlicePitch = 0;

	Graphics::device->CreateBuffer(&bufferDesc, &subResourceData, &vBuffer);

	//matrix = Const::Vector4x4::identity;
	//matrix._41 -= 10.0f;
	return true;
}

void CModel::Uninit() {
	// 頂点シェーダー解放
	if (m_pVertexShader) {
		m_pVertexShader->Release();
		m_pVertexShader = nullptr;
	}

	// ピクセルシェーダー解放
	if (m_pPixelShader) {
		m_pPixelShader->Release();
		m_pPixelShader = nullptr;
	}

	// 頂点レイアウト解放
	if (m_pVertexLayout) {
		m_pVertexLayout->Release();
		m_pVertexLayout = nullptr;
	}
}

void CModel::Uninit2()
{
	vsAsset->Close();
	psAsset->Close();
}

void CModel::Update() {
}

void CModel::Update2()
{
}

void CModel::Draw(bool a) {
	if (a)
	{
		tt += Time::unScaledWorldDeltaTime;
		if (tt > 2)
		{
			tt = 0;
			ttt = -ttt;
		}
		matrix._41 += 10 * Time::unScaledWorldDeltaTime * ttt;
		DX11SetTransform::GetInstance()->SetTransform(DX11SetTransform::TYPE::WORLD, matrix);
	}
	else
	{
		auto set = Vector3(Random::Range(-1000, 1000), Random::Range(-1000, 1000), Random::Range(-1000, 1000));
		matrix._41 += set.x;
		matrix._42 += set.y;
		matrix._43 += set.z;
		DX11SetTransform::GetInstance()->SetTransform(DX11SetTransform::TYPE::WORLD, matrix);
		matrix._41 -= set.x;
		matrix._42 -= set.y;
		matrix._43 -= set.z;
	}
	// 頂点フォーマットをセット
	(SGFramework::Graphics::context())->IASetInputLayout(m_pVertexLayout);

	// 頂点シェーダーをセット
	SGFramework::Graphics::context()->VSSetShader(m_pVertexShader, nullptr, 0);

	// ピクセルシェーダーをセット
	SGFramework::Graphics::context()->PSSetShader(m_pPixelShader, nullptr, 0);

	// モデル描画
	m_datfile.Draw(GetDX11DeviceContext());
}

void CModel::Draw2(bool a)
{
	if (a)
	{
		tt += Time::unScaledWorldDeltaTime;
		if (tt > 2)
		{
			tt = 0;
			ttt = -ttt;
		}
		//transform->accessWrite->worldPosition.x = 0 * Time::worldUnScaledDeltaTime * ttt;
//		transform->worldRotation *= Quaternion::MathQ::AngleAxis(Const::Vector3::up, 10 * Time::worldUnScaledDeltaTime);
		//transform->localScale = Const::Vector3::one * 0.01f;
		//transform->SyncDrawMatrix();
		transform->setDrawMatrix();
	}
	else
	{
		Vector3 set(Random::Range(-1000, 1000), Random::Range(-1000, 1000), Random::Range(-1000, 1000));

		//transform->Translate(set);
		//transform->SyncDrawMatrix();
		transform->setDrawMatrix();
		//transform->accessWrite->worldPosition -= set;
	}

	vsAsset->setShader();
	psAsset->setShader();

	// モデル描画
	m_datfile.Draw(GetDX11DeviceContext());

	//UINT strides = sizeof(Vertex);
	//UINT offsets = 0;
	//Graphics::context->IASetVertexBuffers(0, 1, &vBuffer, &strides, &offsets);
	//Graphics::context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//
	//Graphics::context->Draw(3, 0);
}
