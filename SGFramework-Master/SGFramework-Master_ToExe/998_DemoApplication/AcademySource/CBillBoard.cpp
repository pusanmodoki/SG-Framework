#include	"CBillBoard.h"
#include	"Shader.h"
#include	"dx11mathutil.h"
#include	"DX11Settransform.h"
#include "../SGFrameworkToExeProject/SGFramwork.hpp"

// �r���{�[�h�̒��_���W���v�Z
void CBillBoard::CalcVertex(){
		m_Vertex[0].x = -m_XSize/2;
		m_Vertex[0].y = -m_YSize/2;
		m_Vertex[0].z =0.0f;
		m_Vertex[0].color = m_Color;

		m_Vertex[1].x = -m_XSize/2;
		m_Vertex[1].y =  m_YSize/2;
		m_Vertex[1].z =0.0f;
		m_Vertex[1].color = m_Color;

		m_Vertex[2].x = m_XSize / 2;
		m_Vertex[2].y = -m_YSize / 2;
		m_Vertex[2].z = 0;
		m_Vertex[2].color = m_Color;

		m_Vertex[3].x = m_XSize/2;
		m_Vertex[3].y = m_YSize/2;
		m_Vertex[3].z =0.0f;
		m_Vertex[3].color = m_Color;

		if (m_vbuffer==nullptr) {
			// ���_�o�b�t�@�쐬�i��ŕύX�\�ȁj
			bool sts = CreateVertexBufferWrite(m_dev, sizeof(MyVertex), 4, m_Vertex, &m_vbuffer);
			if (!sts) {
				MessageBox(nullptr, L"create vertex buffer erro(CBillBoard)", L"error", MB_OK);
			}
		}
		else {
			// �쐬�ς݂Ȃ� ���e������������ 
			D3D11_MAPPED_SUBRESOURCE pData;

			HRESULT hr = m_devcontext->Map(m_vbuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData);
			if (SUCCEEDED(hr)) {
				memcpy_s(pData.pData, pData.RowPitch, (void*)(m_Vertex), sizeof(MyVertex) * 4);
				m_devcontext->Unmap(m_vbuffer, 0);
			}
		}
}

// �T�C�Y���Z�b�g
void CBillBoard::SetSize(float x, float y){
	m_XSize = x;
	m_YSize = y;
	CalcVertex();
}

// �J���[�l���Z�b�g
void CBillBoard::SetColor(XMFLOAT4  col){
	m_Color = col;
	CalcVertex();
}

// �ʒu���Z�b�g
void CBillBoard::SetPosition(float x,float y,float z){
	m_x=x;
	m_y=y;
	m_z=z;
}

// �r���{�[�h�p�̍s��𐶐�
void CBillBoard::CalcBillBoardMatrix(const DirectX::XMFLOAT4X4& cameramat){

	m_mat._11 =cameramat._11;
	m_mat._12 =cameramat._21;
	m_mat._13 =cameramat._31;

	m_mat._21 =cameramat._12;
	m_mat._22 =cameramat._22; 
	m_mat._23 =cameramat._32;

	m_mat._31 =cameramat._13;
	m_mat._32 =cameramat._23; 
	m_mat._33 =cameramat._33;

	m_mat._41 = m_x;
	m_mat._42 = m_y;
	m_mat._43 = m_z;

	m_mat._14=0;
	m_mat._24=0;
	m_mat._34=0;
	m_mat._44=1;
}

// �`��
void CBillBoard::Draw() {
	// �y�o�b�t�@������
	void TurnOffZbuffer();

	// �V�F�[�_�[���\�[�X�r���[���s�N�Z���V�F�[�_�[�փZ�b�g
	m_devcontext->PSSetShaderResources(0, 1, &m_srv);

	// ���[���h�ϊ��s��
	DX11SetTransform::GetInstance()->SetTransform(DX11SetTransform::TYPE::WORLD, m_mat);

	// ���_�o�b�t�@���Z�b�g
	unsigned int stride = sizeof(MyVertex);
	unsigned  offset = 0;

	// ���_�o�b�t�@���Z�b�g
	m_devcontext->IASetVertexBuffers(0, 1, &m_vbuffer, &stride, &offset);
	// �C���f�b�N�X�o�b�t�@���Z�b�g
//	m_devcontext->IASetIndexBuffer(nullptr, DXGI_FORMAT_R32_UINT, 0);
	// �g�|���W�[���Z�b�g
	m_devcontext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// ���_�t�H�[�}�b�g���Z�b�g
	m_devcontext->IASetInputLayout(m_pVertexLayout);
	
	// 4�p�`�`��
	m_devcontext->VSSetShader(m_pVertexShader, nullptr, 0);		// ���_�V�F�[�_�[���Z�b�g
	m_devcontext->PSSetShader(m_pPixelShader, nullptr, 0);		// �s�N�Z���V�F�[�_�[���Z�b�g
	m_devcontext->Draw(4,0);									// 4���_�`��(���_�o�b�t�@�݂̂ŕ`��)

	// �y�o�b�t�@�L����
	void TurnOnZbuffer();
}

// �r���{�[�h��`��
void CBillBoard::DrawBillBoard(const XMFLOAT4X4& cameramat){

	// �J�����s�񂩂�@�r���{�[�h�p�̃}�g���b�N�X���쐬
	CalcBillBoardMatrix(cameramat);

	//�A���t�@�u�����f�B���O���Z�b�g����
//	SetBlendStateSrcAlpha();

	// �`��
	Graphics::SendDrawFunction(MAKE_COMMAND(Draw()), 3);

	//�A���t�@�u�����f�B���O���Z�b�g����
//	SetBlendStateDefault();
}

// �r���{�[�h��`��
void CBillBoard::DrawBillBoardAdd(const XMFLOAT4X4& cameramat){

	// �J�����s�񂩂�@�r���{�[�h�p�̃}�g���b�N�X���쐬
	CalcBillBoardMatrix(cameramat);

	// �`��
	//�A���t�@�u�����f�B���O���Z�b�g����
	SetBlendStateOne();

	// �`��
	Draw();

	//�A���t�@�u�����f�B���O���Z�b�g����
	SetBlendStateDefault();

	//�A���t�@�u�����f�B���O�Ŕ������������Z�b�g����
}

void CBillBoard::SetBlendStateDefault() {
	float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	m_devcontext->OMSetBlendState(m_pBlendStateDefault, blendFactor, 0xffffffff);
}

void CBillBoard::SetBlendStateSrcAlpha() {
	float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	m_devcontext->OMSetBlendState(m_pBlendStateSrcAlpha, blendFactor, 0xffffffff);
}

void CBillBoard::SetBlendStateOne() {
	float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	m_devcontext->OMSetBlendState(m_pBlendStateOne, blendFactor, 0xffffffff);
}

void CBillBoard::CreateBlendStateSrcAlpha(){
	
	D3D11_BLEND_DESC BlendDesc;

	ZeroMemory(&BlendDesc, sizeof(BlendDesc));
	BlendDesc.AlphaToCoverageEnable = FALSE;
	BlendDesc.IndependentBlendEnable = FALSE;		// �z���0�Ԃ̂ݎg�p

	BlendDesc.RenderTarget[0].BlendEnable = TRUE;
	BlendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	BlendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_DEST_COLOR;
	BlendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	BlendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	BlendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	BlendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	BlendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	m_dev->CreateBlendState(&BlendDesc, &m_pBlendStateSrcAlpha);
}

void CBillBoard::CreateBlendStateOne() {

	D3D11_BLEND_DESC BlendDesc;

	ZeroMemory(&BlendDesc, sizeof(BlendDesc));
	BlendDesc.AlphaToCoverageEnable = FALSE;
	BlendDesc.IndependentBlendEnable = FALSE;		// �z���0�Ԃ̂ݎg�p

	BlendDesc.RenderTarget[0].BlendEnable = TRUE;
	BlendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	BlendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
	BlendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	BlendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	BlendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	BlendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	BlendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	m_dev->CreateBlendState(&BlendDesc, &m_pBlendStateOne);
}

void CBillBoard::CreateBlendStateDefault() {

	D3D11_BLEND_DESC BlendDesc;

	ZeroMemory(&BlendDesc, sizeof(BlendDesc));

	ZeroMemory(&BlendDesc, sizeof(BlendDesc));
	BlendDesc.AlphaToCoverageEnable = FALSE;
	BlendDesc.IndependentBlendEnable = FALSE;		// �z���0�Ԃ̂ݎg�p

	BlendDesc.RenderTarget[0].BlendEnable = FALSE;
	BlendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	BlendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ZERO;
	BlendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	BlendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	BlendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	BlendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	BlendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	m_dev->CreateBlendState(&BlendDesc, &m_pBlendStateDefault);
}

// �r���{�[�h��`��(�y���ŉ�])
void CBillBoard::DrawRotateBillBoard(const DirectX::XMFLOAT4X4 &cameramat, float angle){

	// �J�����s�񂩂�@�r���{�[�h�p�̃}�g���b�N�X���쐬
	CalcBillBoardMatrix(cameramat);

	// �y����]�s����쐬
	DirectX::XMFLOAT4X4 matRotZ;
	DirectX::XMFLOAT3 axisZ = { cameramat._13, cameramat._23, cameramat._33 };

	// �w�莲��]�̍s����쐬����
	DX11MtxRotationAxis(axisZ, angle, matRotZ);

	// �s����������A�ʒu���Z�b�g
	DX11MtxMultiply(m_mat, m_mat, matRotZ);
	m_mat._41 = m_x;
	m_mat._42 = m_y;
	m_mat._43 = m_z;
	
	//�A���t�@�u�����f�B���O���Z�b�g����
//	SetBlendStateSrcAlpha();

	// �`��
	Draw();

	//�A���t�@�u�����f�B���O���Z�b�g����
//	SetBlendStateSrcAlpha();

}

//	�e�N�X�`���ǂݍ���
bool CBillBoard::LoadTexTure(const char* filename){

		// ���łɓǂݍ��܂�Ă���e�N�X�`��������Ή������
		if(m_srv!=nullptr){
			m_srv->Release();
			m_srv=nullptr;
		}

		// �܂��͂v�h�b�t�@�C���ƍl���ēǂݍ���
		bool sts = CreatetSRVfromWICFile(filename,
			m_dev, m_devcontext, &m_srv);
		if (sts == false) {
			// TGA�t�@�C���������Ă݂�
			sts = CreatetSRVfromTGAFile(filename,
				m_dev, &m_srv);
			if (sts == false) {
				MessageBox(nullptr, L"textureload error(CBillBoard LoadTexture)", L"error", MB_OK);
				return false;
			}
		}

		return true;
}