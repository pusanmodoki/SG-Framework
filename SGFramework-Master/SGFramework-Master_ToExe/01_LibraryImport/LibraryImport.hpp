/*----------------------------------------------------------------------------------
ライブラリ関数のImportを行うLibraryImport.h
------------------------------------------------------------------------------------*/
#ifndef SGFRAMEWORK_HEADER_LIBRARYIMPORT_HPP_
#define SGFRAMEWORK_HEADER_LIBRARYIMPORT_HPP_

//----------------------------------------------------------------------------------
//システム系
//----------------------------------------------------------------------------------
//変数型のかたまり
#include "../02_CustomVariable/CustomVariable.hpp"
//計算
#include "../03_MathAF/MathAF.hpp"
//Asset管理
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
////計算
//#include "../03_Math/Math.h"
////回転
//#include "../04_Rotate/Rotate.h"
////DirectX 3D
//#include "../05_DirectX\DirectX.h"					
////DirectX3Dクラスが必要
//#ifdef _DIRECTX_H_
//	//テクスチャ管理
//	#include "../06_TextureManager/TextureManager.h"
//	//3DModel
//	#include "../07_ModelManager/ModelManager.h"
//#endif // _DIRECTX_H_
////音声再生
//#include "../08_AudioManager\AudioManager.h"	   
////ファイル更新管理
//#include "../09_FileManager/FileManager.h"
////インプット
//#include "../10_Input\Input.h"
////Windows関数
//#include "../11_WindowsFunction/WindowsFunction.h"
//
////----------------------------------------------------------------------------------
////ゲーム系
////----------------------------------------------------------------------------------
////タグ
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
////アニメーション
//#include "../17_Animator/Animator.h"
////Collider
//#include "../18_Collider/Collider.h"
////GameEffect
//#include "../19_GameEffect/GameEffect.h"
//

////----------------------------------------------------------------------------------
////ゲーム系
////----------------------------------------------------------------------------------
//
////<><><> GameObject2Dをもととしたクラス群 <><><>
////ゲームオブジェクト
//#include "GameObject\GameObject2D.h"				   
////ゲームオブジェクト + 移動 (GameObject2D必須)
//#include "GameObject\Movement\Movement2D.h"	   
////ゲームオブジェクト + 移動 + キャラステータス(GameObject2D & Movement必須)
//#include "GameObject\Movement\Character\Character2D.h"
////オブジェクト当たり判定 (GameObject2D必須)													
//#include "Collider\ColliderManager\ColliderManager.h"
////ゲームワールド(GameObject2D必須)
//#include "GameWorld\GameWorld2D.h"
////カメラ(GameObject2D必須)
//#include "Camera\Camera2D.h"
////<><><><><><><><> <><><><><><><><>
//
////オブジェクトアニメーション													 
//#include "Animator\Animator2D.h"		  
////パーティクル
//#include "Particle\Particle.h"						   
////タグ
//#include "Tag\Tag.h"

#endif // !SGFRAMEWORK_HEADER_LIBRARYIMPORT_HPP_