/*----------------------------------------------------------------------------------
���C�u�����֐���Import���s��LibraryImport.h
------------------------------------------------------------------------------------*/
#ifndef SGFRAMEWORK_HEADER_LIBRARYIMPORT_HPP_
#define SGFRAMEWORK_HEADER_LIBRARYIMPORT_HPP_

//----------------------------------------------------------------------------------
//�V�X�e���n
//----------------------------------------------------------------------------------
//�ϐ��^�̂����܂�
#include "../02_CustomVariable/CustomVariable.hpp"
//�v�Z
#include "../03_MathAF/MathAF.hpp"
//Asset�Ǘ�
#include "../04_Asset/AssetManager.hpp"
#include "../04_Asset/InstantiateAsset.hpp"
//Graphics
#include "../05_Graphics/Graphics.hpp"
//Audio
#include "../06_Audio/Audio.hpp"
#include "../06_Audio/AudioAsset/AudioAssetMusic.hpp"
#include "../06_Audio/AudioAsset/AudioAssetEffect.hpp"
#include "../06_Audio/AudioAsset/AudioAssetEffect3D.hpp"
//Thread
#include "../07_Thread/Thread.hpp"
//Input
#include "../08_Input/Input.hpp"
//Scene
#include "../09_Scene/SceneManager.hpp"
//PhysX
#include "../10_Physics/PhysicsInclude.hpp"
//GameObject
#include "../11_GameObject/GameObject.hpp"
//GUI
#include "../12_GUI/GUI.hpp"
////�v�Z
//#include "../03_Math/Math.h"
////��]
//#include "../04_Rotate/Rotate.h"
////DirectX 3D
//#include "../05_DirectX\DirectX.h"					
////DirectX3D�N���X���K�v
//#ifdef _DIRECTX_H_
//	//�e�N�X�`���Ǘ�
//	#include "../06_TextureManager/TextureManager.h"
//	//3DModel
//	#include "../07_ModelManager/ModelManager.h"
//#endif // _DIRECTX_H_
////�����Đ�
//#include "../08_AudioManager\AudioManager.h"	   
////�t�@�C���X�V�Ǘ�
//#include "../09_FileManager/FileManager.h"
////�C���v�b�g
//#include "../10_Input\Input.h"
////Windows�֐�
//#include "../11_WindowsFunction/WindowsFunction.h"
//
////----------------------------------------------------------------------------------
////�Q�[���n
////----------------------------------------------------------------------------------
////�^�O
//#include "../12_Tag/Tag.h"
////GameObject												  
//#include "../13_GameObject/GameObject.h"		
//#include "../13_GameObject/GameObject2D/GameObject2D.h"		
//#include "../13_GameObject/GameObject3D/GameObject3D.h"		
//#include "../13_GameObject/FadeIOObject/FadeIOObject.h"			  
////Camera
//#include "../15_Camera/Camera.h"
////GameScene
//#include "../16_GameScene/GameScene.h"
//#include "../16_GameScene/GameSceneManager.h"
////�A�j���[�V����
//#include "../17_Animator/Animator.h"
////Collider
//#include "../18_Collider/Collider.h"
////GameEffect
//#include "../19_GameEffect/GameEffect.h"
//

////----------------------------------------------------------------------------------
////�Q�[���n
////----------------------------------------------------------------------------------
//
////<><><> GameObject2D�����ƂƂ����N���X�Q <><><>
////�Q�[���I�u�W�F�N�g
//#include "GameObject\GameObject2D.h"				   
////�Q�[���I�u�W�F�N�g + �ړ� (GameObject2D�K�{)
//#include "GameObject\Movement\Movement2D.h"	   
////�Q�[���I�u�W�F�N�g + �ړ� + �L�����X�e�[�^�X(GameObject2D & Movement�K�{)
//#include "GameObject\Movement\Character\Character2D.h"
////�I�u�W�F�N�g�����蔻�� (GameObject2D�K�{)													
//#include "Collider\ColliderManager\ColliderManager.h"
////�Q�[�����[���h(GameObject2D�K�{)
//#include "GameWorld\GameWorld2D.h"
////�J����(GameObject2D�K�{)
//#include "Camera\Camera2D.h"
////<><><><><><><><> <><><><><><><><>
//
////�I�u�W�F�N�g�A�j���[�V����													 
//#include "Animator\Animator2D.h"		  
////�p�[�e�B�N��
//#include "Particle\Particle.h"						   
////�^�O
//#include "Tag\Tag.h"

#endif // !SGFRAMEWORK_HEADER_LIBRARYIMPORT_HPP_