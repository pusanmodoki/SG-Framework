//*****************************************************************************
//!	@file	explosion.h
//!	@brief	���������֘A
//!	@note	
//!	@author
//*****************************************************************************
#pragma once

//-----------------------------------------------------------------------------
//	Include header files.
//-----------------------------------------------------------------------------
#include	"dx11mathutil.h"
#include	"DatFileLoader.h"
#include	<DirectXMath.h>
//-----------------------------------------------------------------------------
// �}�N���錾
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// �v���g�^�C�v�錾
//-----------------------------------------------------------------------------

// ���_���
struct	MyVertex{
	DirectX::XMFLOAT3   Pos;
	DirectX::XMFLOAT4	col;
	float				tu, tv;
};

// �������
struct MyConnect{
	WORD		idx[3];
};

// �R�p�`���
struct	MyTriangle{
	MyVertex		Vertex[3];					// �R�p�`�̒��_
	float			xangle;						// ��]�p�x�w
	float			yangle;						// ��]�p�x�x
	float			zangle;						// ��]�p�x�y
	float			dxa;						// ��]�p�x�����w
	float			dya;						// ��]�p�x�����x
	float			dza;						// ��]�p�x�����y
	float			cx, cy, cz;					// �d�S���W
	float			nx, ny, nz;					// �@���x�N�g��

	DirectX::XMFLOAT4X4		mat;				// �ϊ��s��
	MyTriangle(){
		xangle = 0.0f;							// ��]�p�x�w������
		yangle = 0.0f;							// ��]�p�x�x������
		zangle = 0.0f;							// ��]�p�x�y������
		dxa = (float)((float)(rand() / (float)RAND_MAX)*10);	// �����������w
		dya = (float)((float)(rand() / (float)RAND_MAX)*10);	// �����������x
		dza = (float)((float)(rand() / (float)RAND_MAX)*10);	// �����������y
		DX11MatrixIdentity(mat);
	}
};

void TriangleTransforms(const DirectX::XMFLOAT4X4& mat);
void XfileToTriangle(DatFileLoader*	datfiledata);
void ExplosionInit(DatFileLoader*	datfiledata);
void ExplosionUpdate();
void ExplosionDraw();
void ExplosionExit();

//******************************************************************************
//	End of file.
//******************************************************************************
