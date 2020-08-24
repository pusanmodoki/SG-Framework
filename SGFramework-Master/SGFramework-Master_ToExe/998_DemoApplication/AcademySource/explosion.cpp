//*****************************************************************************
//!	@file	explosion.cpp
//!	@brief	
//!	@note	���������̂��߂̊֐��Q(1�̒��_�o�b�t�@�ɂ܂Ƃ߂�)
//!	@author	
//*****************************************************************************

//-----------------------------------------------------------------------------
//	Include header files.
//-----------------------------------------------------------------------------
#include	<windows.h>
#include	<DirectXMath.h>
#include	"explosion.h"
#include	"DatFileLoader.h"
#include	"shader.h"
#include	"DX11util.h"
#include	"dx11mathutil.h"
#include	"DX11Settransform.h"

//-----------------------------------------------------------------------------
//	�O���[�o���ϐ�
//-----------------------------------------------------------------------------
MyVertex	*g_LocalVertex = nullptr;			// ���_���
int			g_NumVertex=0;						// ���_��
MyConnect	*g_Connect = nullptr;				// �������
int			g_NumFace=0;						// �ʐ�
MyTriangle	*g_pTriangle = nullptr;				// �g���C�A���O���̐擪

ID3D11ShaderResourceView*  g_srv;				// �V�F�[�_�[���\�[�X�r���[

ID3D11VertexShader* g_pVertexShader=nullptr;	// ���_�V�F�[�_�[
ID3D11PixelShader*  g_pPixelShader=nullptr;		// �s�N�Z���V�F�[�_�[
ID3D11InputLayout*  g_pVertexLayout=nullptr;	// ���_���C�A�E�g

ID3D11Buffer*		g_pVertexBuffer = nullptr;	// ���_�o�b�t�@

MyVertex*			g_firstpvertex = nullptr;	// 
//==============================================================================
//!	@fn		InitTriangleVertexBuffer
//!	@brief	�΂�΂�ȂR�p�`���_�����܂Ƃ߂����_�o�b�t�@���쐬����
//!	@param�@MyTriangle�\���̔z��@�@	
//!	@retval	�Ȃ�
//==============================================================================
void InitTriangleVertexBuffer(MyTriangle *mytriangle) {

	MyVertex* svpv;

	// �Â��̂��c���Ă�����폜
	if (g_firstpvertex){
		delete[] g_firstpvertex;
	}

	g_firstpvertex = new MyVertex [g_NumFace * 3];
	svpv = g_firstpvertex;
	for (int i = 0; i < g_NumFace; i++) {
		svpv->Pos = mytriangle[i].Vertex[0].Pos;
		svpv->col = mytriangle[i].Vertex[0].col;
		svpv->tu = mytriangle[i].Vertex[0].tu;
		svpv->tv = mytriangle[i].Vertex[0].tv;

		svpv++;
		svpv->Pos = mytriangle[i].Vertex[1].Pos;
		svpv->col = mytriangle[i].Vertex[1].col;
		svpv->tu = mytriangle[i].Vertex[1].tu;
		svpv->tv = mytriangle[i].Vertex[1].tv;

		svpv++;
		svpv->Pos = mytriangle[i].Vertex[2].Pos;
		svpv->col = mytriangle[i].Vertex[2].col;
		svpv->tu = mytriangle[i].Vertex[2].tu;
		svpv->tv = mytriangle[i].Vertex[2].tv;
		svpv++;
	}

	// ���_�o�b�t�@�쐬(CPU���ŏ������݉\)
	bool sts = CreateVertexBufferWrite(
		GetDX11Device(),
		sizeof(MyVertex),		// �P���_������o�C�g��
		g_NumFace * 3,			// ���_��
		g_firstpvertex,			// ���_�f�[�^�i�[�������擪�A�h���X
		&g_pVertexBuffer);		// ���_�o�b�t�@
	if (!sts) {
		MessageBox(NULL, L"CreateBuffer(vertex buffer) error", L"Error", MB_OK);
		return;
	}
}

//==============================================================================
//!	@fn		TriangleTransforms
//!	@brief	�΂�΂�ȂR�p�`���_���i���[�J�����W�j�����[���h���W�n�ɕϊ�
//!	@param�@���[���h�ϊ��s��(����)�@�@	
//!	@retval	�Ȃ�
//==============================================================================
void TriangleTransforms(const DirectX::XMFLOAT4X4& mat)
{
	int					i;
	MyVertex			*TransformWork;			// ���[���h���W�n�ɕϊ����邽�߂̃��[�N
	DirectX::XMFLOAT3	temp;

	int				idx0, idx1, idx2;
	DirectX::XMFLOAT3		normal;
	DirectX::XMFLOAT3		p0p1;
	DirectX::XMFLOAT3		p0p2;

	// �������ꂽ�O�p�`��񂪂���΍폜����
	if (g_pTriangle != nullptr){
		delete[] g_pTriangle;
		g_pTriangle = nullptr;
	}

	TransformWork = new MyVertex[g_NumVertex];		// ���W�ϊ��p���[�N�𒸓_������������

	// ���_�����ׂĎ��o�����[���h���W�ɕϊ�����
	for (i = 0; i<g_NumVertex; i++){
		TransformWork[i] = g_LocalVertex[i];		// ���[�J�����W�����W�ϊ��p���[�N�ɃZ�b�g

		temp = g_LocalVertex[i].Pos;

		DX11Vec3MulMatrix(temp, temp, mat);

		TransformWork[i].Pos = temp;				// ���W�l�̂݃��[���h���W�ŏ�������
	}

	g_pTriangle = new MyTriangle[g_NumFace];			// �O�p�`�̖ʐ����I�u�W�F�N�g����

	for (i = 0; i<g_NumFace; i++){						// �R�p�`�|���S���������[�v
		idx0 = g_Connect[i].idx[0];
		idx1 = g_Connect[i].idx[1];
		idx2 = g_Connect[i].idx[2];

		g_pTriangle[i].Vertex[0] = TransformWork[idx0];
		g_pTriangle[i].Vertex[0].col = DirectX::XMFLOAT4(1,1,1,1);		// �f�B�t���[�Y�l���Z�b�g����B�i�w�t�@�C���Ɋ܂܂�Ă��Ȃ��ׁj

		g_pTriangle[i].Vertex[1] = TransformWork[idx1];
		g_pTriangle[i].Vertex[1].col = DirectX::XMFLOAT4(1, 1, 1, 1);		// �f�B�t���[�Y�l���Z�b�g����B�i�w�t�@�C���Ɋ܂܂�Ă��Ȃ��ׁj

		g_pTriangle[i].Vertex[2] = TransformWork[idx2];
		g_pTriangle[i].Vertex[2].col = DirectX::XMFLOAT4(1, 1, 1, 1);		// �f�B�t���[�Y�l���Z�b�g����B�i�w�t�@�C���Ɋ܂܂�Ă��Ȃ��ׁj

		p0p1.x = g_pTriangle[i].Vertex[1].Pos.x - g_pTriangle[i].Vertex[0].Pos.x;
		p0p1.y = g_pTriangle[i].Vertex[1].Pos.y - g_pTriangle[i].Vertex[0].Pos.y;
		p0p1.z = g_pTriangle[i].Vertex[1].Pos.z - g_pTriangle[i].Vertex[0].Pos.z;

		p0p2.x = g_pTriangle[i].Vertex[2].Pos.x - g_pTriangle[i].Vertex[0].Pos.x;
		p0p2.y = g_pTriangle[i].Vertex[2].Pos.y - g_pTriangle[i].Vertex[0].Pos.y;
		p0p2.z = g_pTriangle[i].Vertex[2].Pos.z - g_pTriangle[i].Vertex[0].Pos.z;

		// �@�����v�Z
		DX11Vec3Cross(normal, p0p1, p0p2);			// �O�ς��v�Z
		DX11Vec3Normalize(normal,normal);			// �m�[�}���C�Y
		g_pTriangle[i].nx = normal.x;				// �@�����Z�b�g
		g_pTriangle[i].ny = normal.y;
		g_pTriangle[i].nz = normal.z;

		// �d�S�v�Z
		g_pTriangle[i].cx = (g_pTriangle[i].Vertex[0].Pos.x + g_pTriangle[i].Vertex[1].Pos.x + g_pTriangle[i].Vertex[2].Pos.x) / 3;
		g_pTriangle[i].cy = (g_pTriangle[i].Vertex[0].Pos.y + g_pTriangle[i].Vertex[1].Pos.y + g_pTriangle[i].Vertex[2].Pos.y) / 3;
		g_pTriangle[i].cz = (g_pTriangle[i].Vertex[0].Pos.z + g_pTriangle[i].Vertex[1].Pos.z + g_pTriangle[i].Vertex[2].Pos.z) / 3;

		// �d�S���W�����_����Ƃ������W�ɕϊ�����
		g_pTriangle[i].Vertex[0].Pos.x -= g_pTriangle[i].cx;
		g_pTriangle[i].Vertex[0].Pos.y -= g_pTriangle[i].cy;
		g_pTriangle[i].Vertex[0].Pos.z -= g_pTriangle[i].cz;

		g_pTriangle[i].Vertex[1].Pos.x -= g_pTriangle[i].cx;
		g_pTriangle[i].Vertex[1].Pos.y -= g_pTriangle[i].cy;
		g_pTriangle[i].Vertex[1].Pos.z -= g_pTriangle[i].cz;

		g_pTriangle[i].Vertex[2].Pos.x -= g_pTriangle[i].cx;
		g_pTriangle[i].Vertex[2].Pos.y -= g_pTriangle[i].cy;
		g_pTriangle[i].Vertex[2].Pos.z -= g_pTriangle[i].cz;
	}

	InitTriangleVertexBuffer(g_pTriangle);

	delete[] TransformWork;
	return;
}

//==============================================================================
//!	@fn		XfileToTriangle
//!	@brief	�w�t�@�C�����΂�΂�ȂR�p�`�v���~�e�B�u�ɂ΂炷
//!	@param�@���b�V���I�u�W�F�N�g�|�C���^(����)�@�@	
//!	@param�@�f�o�C�X�I�u�W�F�N�g(����)�@�@	
//!	@retval	�Ȃ�
//==============================================================================
void XfileToTriangle(DatFileLoader*	datfiledata)
{
	int							length;			// ���_�t�H�[�}�b�g���i�o�C�g�j

	// �����p�̃��b�V�����쐬

	g_NumVertex = datfiledata->m_VertexSuu;			// ���_�����擾

	g_NumFace = datfiledata->m_IndexSuu / 3;		// �ʐ����Q�b�g

	length = sizeof(MyVertex);

	g_LocalVertex = new MyVertex[g_NumVertex];

	// ���_�����R�s�[
	for (int i = 0; i < g_NumVertex; i++) {
		g_LocalVertex[i].Pos = datfiledata->m_Vertex[i].m_Pos;		// ���W���o��
		g_LocalVertex[i].tu = datfiledata->m_Vertex[i].m_Tex.x;		// UV���W���o��
		g_LocalVertex[i].tv = datfiledata->m_Vertex[i].m_Tex.y;
		g_LocalVertex[i].col = DirectX::XMFLOAT4(1,1,1,1);
	}

	g_Connect = new MyConnect[g_NumFace];	// �ʐ����̌��������擾

	unsigned int idx = 0;
	for (int i = 0; i<g_NumFace; i++){		// �R�p�`�|���S���������[�v
		g_Connect[i].idx[0] = datfiledata->m_Index[idx++];
		g_Connect[i].idx[1] = datfiledata->m_Index[idx++];
		g_Connect[i].idx[2] = datfiledata->m_Index[idx++];
	}

	return;
}

//==============================================================================
//!	@fn		ExplosionInit
//!	@brief	�����̂��߂̏����������s��
//!	@param�@DatFileLoader�|�C���^(����)�@�@	
//!	@retval	�Ȃ�
//==============================================================================
void ExplosionInit(DatFileLoader*	datfiledata){

	// X�t�@�C�����o���o���ȂR�p�`�ɕ�������
	XfileToTriangle(datfiledata);
	bool sts = CreatetSRVfromTGAFile("assets/koa_1.tga",
		GetDX11Device(),
		&g_srv);
	if (!sts) {
		MessageBox(NULL, L"CreatetSRVfromTGAFile", L"Error", MB_OK);
	}

	// ���_�f�[�^�̒�`
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT,		0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR",		0, DXGI_FORMAT_R32G32B32A32_FLOAT,		0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT,		0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	unsigned int numElements = ARRAYSIZE(layout);

	// ���_�V�F�[�_�[�I�u�W�F�N�g�𐶐��A�����ɒ��_���C�A�E�g������
	sts = CreateVertexShader(GetDX11Device(),
		"shader/vstriangle.fx",
		"main",
		"vs_5_0",
		layout,
		numElements,
		&g_pVertexShader,
		&g_pVertexLayout);
	if (!sts) {
		MessageBox(nullptr, L"CreateVertexShader error", L"error", MB_OK);
		return;
	}

	// �s�N�Z���V�F�[�_�[�𐶐�
	sts = CreatePixelShader(			// �s�N�Z���V�F�[�_�[�I�u�W�F�N�g�𐶐�
		GetDX11Device(),				// �f�o�C�X�I�u�W�F�N�g
		"shader/pstriangle.fx",
		"main",
		"ps_5_0",
		&g_pPixelShader);
	if (!sts) {
		MessageBox(nullptr, L"CreatePixelShader error", L"error", MB_OK);
		return;
	}
}

//==============================================================================
//!	@fn		ExplosionUpdate
//!	@brief	�����̍X�V����
//!	@param�@�@�@	
//!	@retval	�Ȃ�
//==============================================================================
void ExplosionUpdate(){
	if (!g_pTriangle) return;
	MyVertex* pv = new MyVertex [g_NumFace*3];

	// �R�p�`�̐������[�v����
	for (int i = 0; i <g_NumFace; i++) {
		XMFLOAT3 angle;
		XMFLOAT3 trans = { 0,0,0 };

		angle.x = g_pTriangle[i].xangle;
		angle.y = g_pTriangle[i].yangle;
		angle.z = g_pTriangle[i].zangle;
		DX11MakeWorldMatrix(g_pTriangle[i].mat, angle, trans);		// ��]�p�x�ƕ��s�ړ��ʂ���s����쐬����

		// �ʒu���W���d�S�ʒu�ɕύX����
		g_pTriangle[i].mat._41 = g_pTriangle[i].cx;
		g_pTriangle[i].mat._42 = g_pTriangle[i].cy;
		g_pTriangle[i].mat._43 = g_pTriangle[i].cz;

		// �R�������W�ɍs����|����
		DX11Vec3MulMatrix(pv[i * 3 + 0].Pos, g_firstpvertex[i * 3 + 0].Pos, g_pTriangle[i].mat);
		DX11Vec3MulMatrix(pv[i * 3 + 1].Pos, g_firstpvertex[i * 3 + 1].Pos, g_pTriangle[i].mat);
		DX11Vec3MulMatrix(pv[i * 3 + 2].Pos, g_firstpvertex[i * 3 + 2].Pos, g_pTriangle[i].mat);

		// �t�u���W���Z�b�g����
		pv[i * 3 + 0].tu = g_firstpvertex[i * 3 + 0].tu;
		pv[i * 3 + 0].tv = g_firstpvertex[i * 3 + 0].tv;
		pv[i * 3 + 1].tu = g_firstpvertex[i * 3 + 1].tu;
		pv[i * 3 + 1].tv = g_firstpvertex[i * 3 + 1].tv;
		pv[i * 3 + 2].tu = g_firstpvertex[i * 3 + 2].tu;
		pv[i * 3 + 2].tv = g_firstpvertex[i * 3 + 2].tv;

		// ���񂾂񓧖��ɂ��Ă���
		for (int k = 0; k < 3; k++) {
			g_pTriangle[i].Vertex[k].col.w -= 0.01f;
			if (g_pTriangle[i].Vertex[k].col.w < 0.0f) {
				g_pTriangle[i].Vertex[k].col.w = 0.0f;
			}
		}

		
		// ���_�J���[���Z�b�g
		pv[i * 3 + 0].col = g_pTriangle[i].Vertex[0].col;
		pv[i * 3 + 1].col = g_pTriangle[i].Vertex[1].col;
		pv[i * 3 + 2].col = g_pTriangle[i].Vertex[2].col;

		// ��������R�[�h��ǉ�

		//�t���[���ԕb��
		const float deltaTime = 0.016f;
		
		// ��]�p�x�̑�������

		//��]�X�s�[�h
		float rotSpeed = 400.0f;

		g_pTriangle[i].xangle += rotSpeed * deltaTime;
		g_pTriangle[i].yangle += rotSpeed * deltaTime;
		g_pTriangle[i].zangle += rotSpeed * deltaTime;

		// �@�������ւ̈ړ�
		float moveSpeed = 1.5f;
		
		//�ړ��X�s�[�h
		g_pTriangle[i].cx += g_pTriangle[i].nx  * moveSpeed * deltaTime;
		g_pTriangle[i].cy += g_pTriangle[i].ny * moveSpeed * deltaTime;
		g_pTriangle[i].cz += g_pTriangle[i].nz * moveSpeed * deltaTime;

		// �����܂ŃR�[�h��ǉ�
	}

	D3D11_MAPPED_SUBRESOURCE pData;

	HRESULT hr = GetDX11DeviceContext()->Map(g_pVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData);
	if (SUCCEEDED(hr)) {
		memcpy_s(pData.pData, pData.RowPitch, (void*)(pv), sizeof(MyVertex) * g_NumFace*3);
		GetDX11DeviceContext()->Unmap(g_pVertexBuffer, 0);
	}

	delete[] pv;
}

//==============================================================================
//!	@fn		ExplosionDraw
//!	@brief	������`�悷��
//!	@param�@�f�o�C�X�I�u�W�F�N�g(����)�@�@	
//!	@retval	�Ȃ�
//==============================================================================
void ExplosionDraw(){

	XMFLOAT4X4 mtx;

	DX11MatrixIdentity(mtx);

	DX11SetTransform::GetInstance()->SetTransform(DX11SetTransform::TYPE::WORLD, mtx);

	// ���_�o�b�t�@���Z�b�g����
	unsigned int stride = sizeof(MyVertex);
	unsigned  offset = 0;
	GetDX11DeviceContext()->IASetVertexBuffers(0, 1, &g_pVertexBuffer, &stride, &offset);

	GetDX11DeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);		// �g�|���W�[���Z�b�g�i���v���~�e�B�u�^�C�v�j

																							// ���_���C�A�E�g�Z�b�g
	GetDX11DeviceContext()->IASetInputLayout(g_pVertexLayout);

	GetDX11DeviceContext()->VSSetShader(g_pVertexShader, nullptr, 0);	// ���_�V�F�[�_�[���Z�b�g
	GetDX11DeviceContext()->PSSetShader(g_pPixelShader, nullptr, 0);	// �s�N�Z���V�F�[�_�[���Z�b�g

	GetDX11DeviceContext()->PSSetShaderResources(0, 1, &g_srv);		// �V�F�[�_�[���\�[�X�r���[���s�N�Z���V�F�[�_�[�փZ�b�g

	GetDX11DeviceContext()->Draw(g_NumFace * 3, 0);
}


//==============================================================================
//!	@fn		ExplosionExit
//!	@brief	���������̌�n��
//!	@param�@�f�o�C�X�I�u�W�F�N�g(����)�@�@	
//!	@retval	�Ȃ�
//==============================================================================
void ExplosionExit(){

	// �������ꂽ�O�p�`��񂪂���΍폜����
	if (g_pTriangle != nullptr){
		delete[] g_pTriangle;
		g_pTriangle = nullptr;
	}

	if(g_Connect != nullptr){
		delete [] g_Connect;
		g_Connect = nullptr;
	}

	if(g_LocalVertex != nullptr){
		delete[] g_LocalVertex;
		g_LocalVertex = nullptr;
	}

	if (g_srv) {
		g_srv->Release();
		g_srv = nullptr;
	}

	if (g_pVertexBuffer) {
		g_pVertexBuffer->Release();
		g_pVertexBuffer = nullptr;
	}

	if (g_firstpvertex) {
		delete[] g_firstpvertex;
	}
}

//******************************************************************************
//	End of file.
//******************************************************************************
