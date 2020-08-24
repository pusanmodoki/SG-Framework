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
	// �ʐ��Z�b�g
	int numFaces = 200 * 200 * 2;

	m_faces = new Face[numFaces];

	Face* pface = m_faces;

	// �C���f�b�N�X����
	for (unsigned int y = 0; y < 200; y++)
	{
		for (unsigned int x = 0; x < 200; x++)
		{
			int count = (200 + 1)*y + x;			// ������W�̃C���f�b�N�X

			// �㔼��
			pface->indexes[0] = count;
			pface->indexes[1] = count + 1;
			pface->indexes[2] = count + 1 + (200 + 1);

			pface++;

			// ������
			pface->indexes[0] = count;
			pface->indexes[1] = count + (200 + 1) + 1;
			pface->indexes[2] = count + (200 + 1);

			pface++;
		}
	}



	float azimuth = 0.0f;			// ���ʊp
	float elevation = 0.0f;			// �p

	m_vertexes = new Vertex::Polygon[(200 + 1) * (200 + 1)];
	Vertex::Polygon* pvtx = m_vertexes;

	Color colors[4] = { Const::Color::red,
	 Const::Color::green,
	Const::Color::blue,
	Const::Color::yellow };
	int colorIndex = 0;

	Vector3	normal;
	// ���ʊp�Ƌp���狅���b�V���̒��_�f�[�^���쐬
	for (unsigned int y = 0; y <= 200; y++) {
		elevation = (MathAF::cPi * (float)y) / (float)200;    // �p���Z�b�g
		float r = 1.0f * sinf(elevation);					// �p�ɉ��������a���v�Z


		colorIndex = Random::RangeToInt(0, 3);
		for (unsigned int x = 0; x <= 200; x++) {
			azimuth = (2 * MathAF::cPi  * (float)x) / (float)200 ;	// ���ʊp���Z�b�g

			// ���_���W�Z�b�g
			pvtx->position.x = r * cosf(azimuth);
			pvtx->position.y = radius * cosf(elevation);
			pvtx->position.z = r * sinf(azimuth);

			pvtx->color = colors[colorIndex];
			// �@���x�N�g���Z�b�g
			//Normalize(pvtx->Pos, normal);		// �@�����v�Z
			//pvtx-> = normal;				// �@�����Z�b�g

			// ����
			pvtx++;
		}
	}


	// �R���X�^���g�o�b�t�@����
	D3D11_BUFFER_DESC bd;
	D3D11_SUBRESOURCE_DATA InitData;

	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;								// �o�b�t�@�g�p���@
	bd.ByteWidth = sizeof(Vertex::Polygon)*(200 + 1) * (200 + 1);									// �o�b�t�@�̑傫
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;					// �R���X�^���g�o�b�t�@
	bd.CPUAccessFlags = 0;										// CPU�A�N�Z�X�s�v

	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = m_vertexes;

	HRESULT hr = Graphics::device->CreateBuffer(&bd, &InitData, &m_vertexBuffer);
	if (FAILED(hr)) {
		MessageBox(nullptr, L"CreateBuffer(constant buffer) error", L"Error", MB_OK);
	//	return false;
	}



	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;								// �o�b�t�@�g�p���@
	bd.ByteWidth = sizeof(uint)*(200) * (200) * 2 * 3;									// �o�b�t�@�̑傫
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;					// �R���X�^���g�o�b�t�@
	bd.CPUAccessFlags = 0;										// CPU�A�N�Z�X�s�v

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

