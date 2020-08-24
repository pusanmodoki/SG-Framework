#include "Sphere.hpp"


Sphere::Face* Sphere::m_faces = nullptr;
Vertex::Polygon* Sphere::m_vertexes = nullptr;
ID3D11Buffer* Sphere::m_vertexBuffer = nullptr;
ID3D11Buffer* Sphere::m_indexBuffer = nullptr;
SharedPointer<Asset::AudioAssetEffect3D> Sphere::m_effect;

void Sphere::InitModel()
{	
	if (m_indexBuffer) return;

	float radius = 1.0f;
	// 面数セット
	int numFaces = 200 * 200 * 2;

	m_faces = new Face[numFaces];

	Face* pface = m_faces;

	// インデックス生成
	for (unsigned int y = 0; y < 200; y++)
	{
		for (unsigned int x = 0; x < 200; x++)
		{
			int count = (200 + 1)*y + x;			// 左上座標のインデックス

			// 上半分
			pface->indexes[0] = count;
			pface->indexes[1] = count + 1;
			pface->indexes[2] = count + 1 + (200 + 1);

			pface++;

			// 下半分
			pface->indexes[0] = count;
			pface->indexes[1] = count + (200 + 1) + 1;
			pface->indexes[2] = count + (200 + 1);

			pface++;
		}
	}



	float azimuth = 0.0f;			// 方位角
	float elevation = 0.0f;			// 仰角

	m_vertexes = new Vertex::Polygon[(200 + 1) * (200 + 1)];
	Vertex::Polygon* pvtx = m_vertexes;

	Color colors[4] = { Const::Color::red,
	 Const::Color::green,
	Const::Color::blue,
	Const::Color::yellow };
	int colorIndex = 0;

	Vector3	normal;
	// 方位角と仰角から球メッシュの頂点データを作成
	for (unsigned int y = 0; y <= 200; y++) {
		elevation = (MathAF::cPi * (float)y) / (float)200;    // 仰角をセット
		float r = 1.0f * sinf(elevation);					// 仰角に応じた半径を計算


		colorIndex = Random::RangeToInt(0, 3);
		for (unsigned int x = 0; x <= 200; x++) {
			azimuth = (2 * MathAF::cPi  * (float)x) / (float)200 ;	// 方位角をセット

			// 頂点座標セット
			pvtx->position.x = r * cosf(azimuth);
			pvtx->position.y = radius * cosf(elevation);
			pvtx->position.z = r * sinf(azimuth);

			pvtx->color = colors[colorIndex];
			// 法線ベクトルセット
			//Normalize(pvtx->Pos, normal);		// 法線を計算
			//pvtx-> = normal;				// 法線をセット

			// 次へ
			pvtx++;
		}
	}


	// コンスタントバッファ生成
	D3D11_BUFFER_DESC bd;
	D3D11_SUBRESOURCE_DATA InitData;

	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;								// バッファ使用方法
	bd.ByteWidth = sizeof(Vertex::Polygon)*(200 + 1) * (200 + 1);									// バッファの大き
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;					// コンスタントバッファ
	bd.CPUAccessFlags = 0;										// CPUアクセス不要

	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = m_vertexes;

	HRESULT hr = Graphics::device->CreateBuffer(&bd, &InitData, &m_vertexBuffer);
	if (FAILED(hr)) {
		MessageBox(nullptr, L"CreateBuffer(constant buffer) error", L"Error", MB_OK);
	//	return false;
	}



	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;								// バッファ使用方法
	bd.ByteWidth = sizeof(uint)*(200) * (200) * 2 * 3;									// バッファの大き
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;					// コンスタントバッファ
	bd.CPUAccessFlags = 0;										// CPUアクセス不要

	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = m_faces;

	 hr = Graphics::device->CreateBuffer(&bd, &InitData, &m_indexBuffer);
	if (FAILED(hr)) {
		MessageBox(nullptr, L"CreateBuffer(constant buffer) error", L"Error", MB_OK);
		//	return false;

	}

	m_effect = Asset::InstantiateAsset<Asset::AudioAssetEffect3D>("Assets/Audio/DemoSE3D1.wav");
	m_effect->Open(Audio::submixMonoralRootLayer(), 200, 0.2f);
	m_effect->setVolume(1.5f);
}

