/*----------------------------------------------------------------------------------
入力を管理するInput class
------------------------------------------------------------------------------------*/
#ifndef SGFRAMEWORK_HEADER_INPUT_HPP_
#define SGFRAMEWORK_HEADER_INPUT_HPP_
#include <array>
#include <exception>
#include <stdexcept>
#include <Windows.h>
#include <Xinput.h>
#include <dwmapi.h>
#include "../02_CustomVariable/CustomVariable.hpp"
#include "../03_MathAF/MathAF.hpp"
#include "../12_GUI/GUI.hpp"
#pragma comment(lib,"xinput.lib")			
#pragma comment(lib,"dwmapi.lib")			

//Framework namespace
namespace SGFramework
{
	//friend用
	class Application;
	class Thread;
	
	//入力を管理するInput class
	//各入力関数はメンバーにある構造体の中にstatic関数として格納されている
	class Input
	{
	public:
		//friend
		friend class Application;
		friend class Thread;
		friend class GUI;
		//prototype
		struct AxisPack;

		DELETE_ALL_DEFAULT_CLASS_FUNCTIONS(Input);

		//連射間隔秒数, 全Input共通
		static GetOnlyProperty<float> rapidInterval;
		
		//Any input function
		struct Any
		{
			//friend
			friend class Input;

			//----------------------------------------------------------------------------------
			//[GetAny]
			//return: 全てのキー, ボタンのうちどれかが押されていたらtrue
			//				isAnyModeがtrueでない場合は必ずfalseを返す
			inline static bool GetAny();
			//----------------------------------------------------------------------------------
			//[GetAnyDown]
			//return: 全てのキー, ボタンのうちどれかが押された瞬間ならtrue
			//				isAnyModeがtrueでない場合は必ずfalseを返す
			inline static bool GetAnyDown();
			//----------------------------------------------------------------------------------
			//[GetAnyUp]
			//return: 全てのキー, ボタンのうちどれかが離れた瞬間ならtrue
			//				isAnyModeがtrueでない場合は必ずfalseを返す
			inline static bool GetAnyUp();
			//----------------------------------------------------------------------------------
			//[SetAnyMode]
			//Any系関数の有効化 or 無効化を設定する, defaultではfalse
			//全ての入力を確認する必要があるので必要な時以外はfalseに設定することを推奨
			//引数1: true = 有効化, false = 無効化
			inline static void SetAnyMode(bool mode);

			//Any系関数が有効か否か
			static GetOnlyProperty<std::atomic<bool>> isAnyMode;
		};
		//Keybord input function
		struct Keybord
		{
			//----------------------------------------------------------------------------------
			//[GetKey]
			//return: 対象キーが押されていたら0より大きい値を返す, そうでない場合は0
			//引数1:  KeyCode::〇〇
			inline static byte GetKey(KeyCode::Enum keyCode);
			//----------------------------------------------------------------------------------
			//[GetKey]
			//return: 対象キーが押されていたら0より大きい値を返す, そうでない場合は0
			//引数1:  VK_〇〇
			inline static byte GetKey(int vKey);
			//----------------------------------------------------------------------------------
			//[GetKeyDown]
			//return: 対象キーが押された瞬間なら0より大きい値を返す, そうでない場合は0
			//引数1:  KeyCode::〇〇
			inline static byte GetKeyDown(KeyCode::Enum keyCode);
			//----------------------------------------------------------------------------------
			//[GetKeyDown]
			//return: 対象キーが押された瞬間なら0より大きい値を返す, そうでない場合は0
			//引数1:  VK_〇〇
			inline static byte GetKeyDown(int vKey);
			//----------------------------------------------------------------------------------
			//[GetKeyUp]
			//return: 対象キーが離れた瞬間なら0より大きい値を返す, そうでない場合は0
			//引数1:  KeyCode::〇〇
			inline static byte GetKeyUp(KeyCode::Enum keyCode);
			//----------------------------------------------------------------------------------
			//[GetKeyUp]
			//return: 対象キーが離れた瞬間なら0より大きい値を返す, そうでない場合は0
			//引数1:  VK_〇〇
			inline static byte GetKeyUp(int vKey);
			//----------------------------------------------------------------------------------
			//[GetRapid]
			//return: GetKey == trueのかつ前回の成功時から連射間隔秒数が経過した時のみtrueを返す
			//				trueと判断された場合連射間隔秒数は自動で初期化される
			//引数1:  KeyCode::〇〇
			inline static bool GetRapid(KeyCode::Enum keyCode);
			//----------------------------------------------------------------------------------
			//[GetRapid]
			//return: GetKey == trueのかつ前回の成功時から連射間隔秒数が経過した時のみtrueを返す
			//				trueと判断された場合連射間隔秒数は自動で初期化される
			//引数1:  VK_〇〇
			inline static bool GetRapid(int vKey);

			//GetKeyが成功を返した場合に入っている値
			static constexpr byte cGetKeyCheckBit = 0x80;
			//GetKeyDownが成功を返した場合に入っている値
			static constexpr byte cGetKeyDownCheckBit = 0x40;
			//GetKeyUpが成功を返した場合に入っている値
			static constexpr byte cGetKeyUpCheckBit = 0x20;
		};

		//Mouse input function
		struct Mouse
		{
			//----------------------------------------------------------------------------------
			//[GetButton]
			//return: 対象ボタンが押されていたら0より大きい値を返す, そうでない場合は0
			//引数1:  MouseCode::〇〇
			inline static byte GetButton(MouseCode::Enum mouseCode);
			//----------------------------------------------------------------------------------
			//[GetButton]
			//return: 対象キーが押されていたら0より大きい値を返す, そうでない場合は0
			//引数1:  VK_〇〇
			inline static byte GetButton(int vKey);
			//----------------------------------------------------------------------------------
			//[GetButtonDown]
			//return: 対象キーが押された瞬間なら0より大きい値を返す, そうでない場合は0
			//引数1:  MouseCode::〇〇
			inline static byte GetButtonDown(MouseCode::Enum mouseCode);
			//----------------------------------------------------------------------------------
			//[GetButtonDown]
			//return: 対象キーが押された瞬間なら0より大きい値を返す, そうでない場合は0
			//引数1:  VK_〇〇
			inline static byte GetButtonDown(int vKey);
			//----------------------------------------------------------------------------------
			//[GetButtonUp]
			//return: 対象キーが離れた瞬間なら0より大きい値を返す, そうでない場合は0
			//引数1:  MouseCode::〇〇
			inline static byte GetButtonUp(MouseCode::Enum mouseCode);
			//----------------------------------------------------------------------------------
			//[GetButtonUp]
			//return: 対象キーが離れた瞬間なら0より大きい値を返す, そうでない場合は0
			//引数1:  VK_〇〇
			inline static byte GetButtonUp(int vKey);
			//----------------------------------------------------------------------------------
			//[GetRapid]
			//return: GetButton == trueのかつ前回の成功時から連射間隔秒数が経過した時のみtrueを返す
			//				trueと判断された場合連射間隔秒数は自動で初期化される
			//引数1:  MouseCode::〇〇
			inline static bool GetRapid(MouseCode::Enum mouseCode);
			//----------------------------------------------------------------------------------
			//[GetRapid]
			//return: GetButton == trueのかつ前回の成功時から連射間隔秒数が経過した時のみtrueを返す
			//				trueと判断された場合連射間隔秒数は自動で初期化される
			//引数1:  VK_〇〇
			inline static bool GetRapid(int vKey);

			//----------------------------------------------------------------------------------
			//[GetPosition]
			//return: ウィンドウの大きさを縦横-1.0f ~ 1.0fとした場合の現在のマウス座標
			inline static Vector2 GetPosition();
			//----------------------------------------------------------------------------------
			//[GetPositionToWindow]
			//return: ウィンドウの大きさを縦横0.0f ~ 1.0fとした場合の現在のマウス座標
			inline static Vector2 GetPositionToWindow();
			//----------------------------------------------------------------------------------
			//[GetAcceleration]
			//return: 前フレーム比のポジション移動量, 各要素の絶対値がdedZone以下の場合必ず0.0fとなる
			inline static Vector2 GetAcceleration();
			//----------------------------------------------------------------------------------
			//[GetAcceleration]
			//return: dedZoneの影響を受けない前フレーム比のポジション移動量
			inline static Vector2 GetRawAcceleration();
			//----------------------------------------------------------------------------------
			//[GetWheelDelta]
			//return: 現在のwheel遷移状態 (奥 -> +1~, 手前 -> -1~)
			inline static short GetWheelDelta();

			//cGetButtonが成功を返した場合に入っている値
			static constexpr byte cGetButtonCheckBit = 0x80;
			//cGetButtonDownが成功を返した場合に入っている値
			static constexpr byte cGetButtonDownCheckBit = 0x40;
			//cGetButtonUpが成功を返した場合に入っている値
			static constexpr byte cGetButtonUpCheckBit = 0x20;

			//ウィンドウの大きさを0.0f ~ 1.0fとした場合の現在のマウス座標
			static GetOnlyProperty<Vector2> position;
			//dedZoneの影響を受けない前フレームからのポジションの移動量
			static GetOnlyProperty<Vector2> rawAcceleration;
			//GetAccelerationに使用されるデッドゾーン
			static GetOnlyProperty<float> dedZone;
			//現在のwheel遷移状態 (奥 -> +1~, 手前 -> -1~)
			static GetOnlyProperty<short> wheel;
			//trueの場合マウスカーソルがウィンドウの外へでなくなる
			static GetOnlyProperty<bool> isMouseInsideFrame;
		};

		//Joystick input function
		struct Joystick
		{
			//friend
			friend class Input;

			//----------------------------------------------------------------------------------
			//[GetButton]
			//return: 対象ボタンが押されていたら0より大きい値を返す, そうでない場合は0
			//引数1: JoystickCode::〇〇
			//引数2: 取得に使用するジョイスティックの番号 (0 ~ 4), 例外処理は行わない, default = 0
			inline static ushort GetButton(JoystickCode::Enum joystickCode, uint useStick = 0);
			//----------------------------------------------------------------------------------
			//[GetButtonDown]
			//return: 対象キーが押された瞬間なら0より大きい値を返す, そうでない場合は0
			//引数1: JoystickCode::〇〇
			//引数2: 取得に使用するジョイスティックの番号 (0 ~ 4), 例外処理は行わない, default = 0
			inline static ushort GetButtonDown(JoystickCode::Enum joystickCode, uint useStick = 0);
			//----------------------------------------------------------------------------------
			//[GetButtonUp]
			//return: 対象キーが離れた瞬間なら0より大きい値を返す, そうでない場合は0
			//引数1: JoystickCode::〇〇
			//引数2: 取得に使用するジョイスティックの番号 (0 ~ 4), 例外処理は行わない, default = 0
			inline static ushort GetButtonUp(JoystickCode::Enum joystickCode, uint useStick = 0);
			//----------------------------------------------------------------------------------
			//[GetRapid]
			//return: GetButton == trueのかつ前回の成功時から連射間隔秒数が経過した時のみtrueを返す
			//				trueと判断された場合連射間隔秒数は自動で初期化される
			//引数1: JoystickCode::〇〇
			//引数2: 取得に使用するジョイスティックの番号 (0 ~ 4), 例外処理は行わない, default = 0
			inline static bool GetRapid(JoystickCode::Enum joystickCode, uint useStick = 0);

			//----------------------------------------------------------------------------------
			//[GetLTrigger]
			//return: 0.0f ~ 1.0fの値となるLトリガーの現在値, 絶対値がdedZone以下の場合必ず0.0fを返却する
			//引数1: 取得に使用するジョイスティックの番号 (0 ~ 4), 例外処理は行わない, default = 0
			inline static float GetLTrigger(uint useStick = 0);
			//----------------------------------------------------------------------------------
			//[GetRawLTrigger]
			//return: 0.0f ~ 1.0fの値となるdedZoneを考慮しないLトリガーの現在値
			//引数1: 取得に使用するジョイスティックの番号 (0 ~ 4), 例外処理は行わない, default = 0
			inline static float GetRawLTrigger(uint useStick = 0);
			//----------------------------------------------------------------------------------
			//[GetLTrigger]
			//return: 0.0f ~ 1.0fの値を取るLトリガーの前フレーム比の移動量,
			//				比較に使われる値はdedZoneの影響を受ける
			//引数1: 取得に使用するジョイスティックの番号 (0 ~ 4), 例外処理は行わない, default = 0
			inline static float GetLTriggerAcceleration(uint useStick = 0);
			//----------------------------------------------------------------------------------
			//[GetRawLTriggerAcceleration]
			//return: 0.0f ~ 1.0fの値となるdedZoneを考慮しないLトリガーの前フレーム比移動量
			//引数1: 取得に使用するジョイスティックの番号 (0 ~ 4), 例外処理は行わない, default = 0
			inline static float GetRawLTriggerAcceleration(uint useStick = 0);

			//----------------------------------------------------------------------------------
			//[GetRTrigger]
			//return: 0.0f ~ 1.0fの値となるRトリガーの現在値, 絶対値がdedZone以下の場合必ず0.0fを返却する
			//引数1: 取得に使用するジョイスティックの番号 (0 ~ 4), 例外処理は行わない, default = 0
			inline static float GetRTrigger(uint useStick = 0);
			//----------------------------------------------------------------------------------
			//[GetRawRTrigger]
			//return: 0.0f ~ 1.0fの値となるdedZoneを考慮しないRトリガーの現在値
			//引数1: 取得に使用するジョイスティックの番号 (0 ~ 4), 例外処理は行わない, default = 0
			inline static float GetRawRTrigger(uint useStick = 0);
			//----------------------------------------------------------------------------------
			//[GetRTriggerAcceleration]
			//return: 0.0f ~ 1.0fの値を取るRトリガーの前フレーム比の移動量,
			//				比較に使われる値はdedZoneの影響を受ける
			//引数1: 取得に使用するジョイスティックの番号 (0 ~ 4), 例外処理は行わない, default = 0
			inline static float GetRTriggerAcceleration(uint useStick = 0);
			//----------------------------------------------------------------------------------
			//[GetRawRTriggerAcceleration]
			//return: 0.0f ~ 1.0fの値となるdedZoneを考慮しないRトリガーの前フレーム比移動量
			//引数1: 取得に使用するジョイスティックの番号 (0 ~ 4), 例外処理は行わない, default = 0
			inline static float GetRawRTriggerAcceleration(uint useStick = 0);

			//----------------------------------------------------------------------------------
			//[GetLStick]
			//return: -1.0f ~ 1.0fの値となるLスティックの現在値, 各要素の絶対値がdedZone以下の場合必ず0.0fとなる
			//引数1: 取得に使用するジョイスティックの番号 (0 ~ 4), 例外処理は行わない, default = 0
			inline static Vector2 GetLStick(uint useStick = 0);
			//----------------------------------------------------------------------------------
			//[GetRawLStick]
			//return: -1.0f ~ 1.0fの値となるdedZoneを考慮しないLスティックの現在値
			//引数1: 取得に使用するジョイスティックの番号 (0 ~ 4), 例外処理は行わない, default = 0
			inline static Vector2 GetRawLStick(uint useStick = 0);
			//----------------------------------------------------------------------------------
			//[GetLStickAcceleration]
			//return: -1.0f ~ 1.0fの値を取るLスティックの前フレーム比の移動量,
			//				比較に使われる値はdedZoneの影響を受ける
			//引数1: 取得に使用するジョイスティックの番号 (0 ~ 4), 例外処理は行わない, default = 0
			inline static Vector2 GetLStickAcceleration(uint useStick = 0);
			//----------------------------------------------------------------------------------
			//[GetRawLStickAcceleration]
			//return: -1.0f ~ 1.0fの値となるdedZoneを考慮しないLスティックの前フレーム比移動量
			//引数1: 取得に使用するジョイスティックの番号 (0 ~ 4), 例外処理は行わない, default = 0
			inline static Vector2 GetRawLStickAcceleration(uint useStick = 0);

			//----------------------------------------------------------------------------------
			//[GetRStick]
			//return: -1.0f ~ 1.0fの値となるRスティックの現在値, 各要素の絶対値がdedZone以下の場合必ず0.0fとなる
			//引数1: 取得に使用するジョイスティックの番号 (0 ~ 4), 例外処理は行わない, default = 0
			inline static Vector2 GetRStick(uint useStick = 0);
			//----------------------------------------------------------------------------------
			//[GetRawRStick]
			//return: -1.0f ~ 1.0fの値となるdedZoneを考慮しないRスティックの現在値
			//引数1: 取得に使用するジョイスティックの番号 (0 ~ 4), 例外処理は行わない, default = 0
			inline static Vector2 GetRawRStick(uint useStick = 0);
			//----------------------------------------------------------------------------------
			//[GetRStickAcceleration]
			//return: -1.0f ~ 1.0fの値を取るRスティックの前フレーム比の移動量,
			//				比較に使われる値はdedZoneの影響を受ける
			//引数1: 取得に使用するジョイスティックの番号 (0 ~ 4), 例外処理は行わない, default = 0
			inline static Vector2 GetRStickAcceleration(uint useStick = 0);
			//----------------------------------------------------------------------------------
			//[GetRawRStickAcceleration]
			//return: -1.0f ~ 1.0fの値となるdedZoneを考慮しないRスティックの前フレーム比移動量
			//引数1: 取得に使用するジョイスティックの番号 (0 ~ 4), 例外処理は行わない, default = 0
			inline static Vector2 GetRawRStickAcceleration(uint useStick = 0);

			//----------------------------------------------------------------------------------
			//[GetHighVibration]
			//return: 0.0f ~ 1.0fの値となる現在の高周波振動値
			//引数1: 取得に使用するジョイスティックの番号 (0 ~ 4), 例外処理は行わない, default = 0
			inline static float GetHighVibration(uint useStick = 0);
			//----------------------------------------------------------------------------------
			//[GetLowVibration]
			//return: 0.0f ~ 1.0fの値となる現在の低周波振動値
			//引数1: 取得に使用するジョイスティックの番号 (0 ~ 4), 例外処理は行わない, default = 0
			inline static float GetLowVibration(uint useStick = 0);

			//----------------------------------------------------------------------------------
			//[SetHighVibration]
			//高周波振動値を設定する, 実行は1フレーム後に行われるが、FadeOut中の場合は登録に失敗する
			//各コントローラーでHigh, Low各一つずつしか設定する事はできない
			//return: 設定が完了すればtrue, 失敗(すでに設定されている)の場合false
			//引数1: 設定する振動値, 0.0f ~ 1.0fにClampされる
			//引数2: 取得に使用するジョイスティックの番号 (0 ~ 4), 接続されていない場合失敗, default = 0
			//引数3: trueの場合, 既に設定済みの場合も強制的に設定する, default = false
			inline static bool SetHighVibration(float setVibration, uint useStick = 0, bool isForced = false);
			//----------------------------------------------------------------------------------
			//[SetLowVibration]
			//低周波振動値を設定する, 実行は1フレーム後に行われるが、FadeOut中の場合は登録に失敗する
			//各コントローラーでHigh, Low各一つずつしか設定する事はできない
			//return: 設定が完了すればtrue, 失敗(すでに設定されている)の場合false
			//引数1: 設定する振動値, 0.0f ~ 1.0fにClampされる
			//引数2: 取得に使用するジョイスティックの番号 (0 ~ 4), 接続されていない場合失敗, default = 0
			//引数3: trueの場合, 既に設定済みの場合も強制的に設定する, default = false
			inline static bool SetLowVibration(float setVibration, uint useStick = 0, bool isForced = false);

			//----------------------------------------------------------------------------------
			//[SetHighVibrationFadeOut]
			//高周波振動値を設定値から0.0fへフェードアウトするように設定する
			//実行は1フレーム後に行われるが、FadeOut中の場合は登録に失敗する
			//実行順序は setVibration設定 -> waitReachSeconds経過 -> reachZeroSecondsかけて0.0fまで減少
			//各コントローラーでHigh, Low各一つずつしか設定する事はできない
			//return: 設定が完了すればtrue, 失敗(すでに設定されている or FadeOut中)の場合false
			//引数1: 設定する振動値, 0.0f ~ 1.0fにClampされる
			//引数2: 減少し始めるまでの待機時間
			//引数3: 0.0fになるまでかかる所要時間
			//引数4: 取得に使用するジョイスティックの番号 (0 ~ 4), 接続されていない場合失敗, default = 0
			//引数5: trueの場合, 既に設定済みの場合も強制的に設定する, default = false
			inline static bool SetHighVibrationFadeOut(float setVibration, float waitReachSeconds, float reachZeroSeconds, uint useStick = 0, bool isForced = false);
			//----------------------------------------------------------------------------------
			//[SetLowVibrationFadeOut]
			//低周波振動値を設定値から0.0fへフェードアウトするように設定する, 実行は1フレーム後に行われる
			//実行は1フレーム後に行われるが、FadeOut中の場合は登録に失敗する
			//実行順序は setVibration設定 -> waitReachSeconds経過 -> reachZeroSecondsかけて0.0fまで減少
			//各コントローラーでHigh, Low各一つずつしか設定する事はできない
			//return: 設定が完了すればtrue, 失敗(すでに設定されている or FadeOut中)の場合false
			//引数1: 設定する振動値, 0.0f ~ 1.0fにClampされる
			//引数2: 減少し始めるまでの待機時間
			//引数3: 0.0fになるまでかかる所要時間
			//引数4: 取得に使用するジョイスティックの番号 (0 ~ 4), 接続されていない場合失敗, default = 0
			//引数5: trueの場合, 既に設定済みの場合も強制的に設定する, default = false
			inline static bool SetLowVibrationFadeOut(float setVibration, float waitReachSeconds, float reachZeroSeconds, uint useStick = 0, bool isForced = false);

			//----------------------------------------------------------------------------------
			//[ResetVibration]
			//現在の振動設定値をHigh Lowともに0.0fまでフェードアウトさせる
			//この関数は既に設定されているか否かに関わらず強制的に実行される
			//引数1: 0.0fになるまでかかる所要時間
			//引数2: 取得に使用するジョイスティックの番号 (0 ~ 4), 例外処理は行わない, default = 0
			inline static void ResetVibration(float reachZeroSpeedPerSeconds, uint useStick = 0);
			//----------------------------------------------------------------------------------
			//[ResetAllVibration]
			//全てのジョイスティックで現在の振動設定値をHigh Lowともに0.0fまでフェードアウトさせる
			//この関数は既に設定されているか否かに関わらず強制的に実行される
			//引数1: 0.0fになるまでかかる所要時間
			inline static void ResetAllVibration(float reachZeroSpeedPerSeconds);

			//----------------------------------------------------------------------------------
			//[SetAutoReConnectCallback]
			//接続数の変更が行われた時のコールバック関数を設定する
			//引数1: コールバック関数, 引数1: 新しいジョイスティックの接続数(uint)
			inline static void SetAutoReConnectCallback(const std::function<void(uint joystickCount)>& callback);

			//最大接続数
			static constexpr uint cJoystickMax = 4;
			//デッドゾーン
			static GetOnlyProperty<float> dedZone;
			//現在の接続数
			static GetOnlyProperty<byte> numUseJoysticks;
			//現在の接続数 > 0
			static GetOnlyProperty<bool> isUseJoystick;
		};
		//Window input function
		struct Window
		{
			//----------------------------------------------------------------------------------
			//[GetPosition]
			//return: 画面解像度を 0.0f ~ 1.0fとした場合のウィンドウの中心座標
			inline static Vector2 GetPosition();
			//----------------------------------------------------------------------------------
			//[GetSize]
			//return: 画面解像度を 0.0f ~ 1.0fとした場合のウィンドウのサイズ
			inline static Vector2 GetSize();
			//----------------------------------------------------------------------------------
			//[GetAcceleration]
			//return: 画面解像度を 0.0f ~ 1.0fとした場合の前フレームからのウィンドウ移動量
			//				各要素の絶対値がdedZone以下の場合必ず0.0fとなる
			inline static Vector2 GetAcceleration();
			//----------------------------------------------------------------------------------
			//[GetAcceleration]
			//return: 画面解像度を 0.0f ~ 1.0fとした場合のdedZoneの影響を受けない前フレームからのウィンドウ移動量
			inline static Vector2 GetRawAcceleration();

			//画面解像度を 0.0f ~ 1.0fとした場合のウィンドウの中心座標
			static GetOnlyProperty<Vector2> position;
			//画面解像度を 0.0f ~ 1.0fとした場合のウィンドウのサイズ
			static GetOnlyProperty<Vector2> size;
			//画面解像度を 0.0f ~ 1.0fとした場合のdedZoneの影響を受けない前フレームからのウィンドウ移動量
			static GetOnlyProperty<Vector2> rawAcceleration;
			//デッドゾーン
			static GetOnlyProperty<float> dedZone;
		};

		//Axis input function
		struct Axis
		{
		public:
			//----------------------------------------------------------------------------------
			//[GetAxisHorizontal]
			//仮想HorizontalAxis軸の取得を行う, 取得する値は[Keybord->A (-), D (+)], [Joystick->LStick.x]
			//return: 各取得値の中で最も絶対値が大きい値
			//引数1: 取得に使用するジョイスティックの番号 (0 ~ 4), 例外処理は行わない, default = 0
			inline static float GetAxisHorizontal(uint useJoystick = 0);
			//----------------------------------------------------------------------------------
			//[GetRawAxisHorizontal]
			//仮想HorizontalAxis軸の取得をdedZoneの影響を受けずに行う
			//取得する値は[Keybord->A (-), D (+)], [Joystick->RawLStick.x]
			//return: 各取得値の中で最も絶対値が大きい値
			//引数1: 取得に使用するジョイスティックの番号 (0 ~ 4), 例外処理は行わない, default = 0
			inline static float GetRawAxisHorizontal(uint useJoystick = 0);
			//----------------------------------------------------------------------------------
			//[GetAxisVertical]
			//仮想VerticalAxis軸の取得を行う, 取得する値は[Keybord->W (-), S (+)], [Joystick->LStick.y]
			//return: 各取得値の中で最も絶対値が大きい値
			//引数1: 取得に使用するジョイスティックの番号 (0 ~ 4), 例外処理は行わない, default = 0
			inline static float GetAxisVertical(uint useJoystick = 0);
			//----------------------------------------------------------------------------------
			//[GetRawAxisVertical]
			//仮想VerticalAxis軸の取得をdedZoneの影響を受けずに行う
			//取得する値は[Keybord->W (-), S (+)], [Joystick->RawLStick.y]
			//return: 各取得値の中で最も絶対値が大きい値
			//引数1: 取得に使用するジョイスティックの番号 (0 ~ 4), 例外処理は行わない, default = 0
			inline static float GetRawAxisVertical(uint useJoystick = 0);
			//----------------------------------------------------------------------------------
			//[GetAxisJump]
			//仮想JumpAxis軸の取得を行う, 取得する値は[Keybord->Space (+)], [Joystick->A (+)]
			//return: 各取得値の中で最も絶対値が大きい値
			//引数1: 取得に使用するジョイスティックの番号 (0 ~ 4), 例外処理は行わない, default = 0
			inline static float GetAxisJump(uint useJoystick = 0);
			//----------------------------------------------------------------------------------
			//[GetAxisAction]
			//仮想ActionAxis軸の取得を行う, 取得する値は[Mouse->Left (+)], [Joystick->A (+)]
			//return: 各取得値の中で最も絶対値が大きい値
			//引数1: 取得に使用するジョイスティックの番号 (0 ~ 4), 例外処理は行わない, default = 0
			inline static float GetAxisAction(uint useJoystick = 0);
			//----------------------------------------------------------------------------------
			//[GetAxisUser]
			//事前にRegisterAxisUserで登録された仮想Axis軸の取得を行う
			//取得する値は登録時のAxisPackに準ずる
			//return: 各取得値の中で最も絶対値が大きい値
			//throw: nameのエレメントが登録されていない場合
			//引数1: 登録時に使用したname (map->key)
			//引数2: 取得に使用するジョイスティックの番号 (0 ~ 4), 例外処理は行わない, default = 0
			inline static float GetAxisUser(const sgstring& name, uint useJoystick = 0);
			//----------------------------------------------------------------------------------
			//[GetRawAxisUser]
			//事前にRegisterAxisUserで登録された仮想Axis軸の取得をdedZoneの影響を受けずに行う
			//取得する値は登録時のAxisPackに準ずる
			//AxisPack->axisCodesが設定されていない場合, 結果はGetAxisUserと同じになる
			//return: 各取得値の中で最も絶対値が大きい値
			//throw: nameのエレメントが登録されていない場合
			//引数1: 登録時に使用したname (map->key)
			//引数2: 取得に使用するジョイスティックの番号 (0 ~ 4), 例外処理は行わない, default = 0
			inline static float GetRawAxisUser(const sgstring& name, uint useJoystick = 0);
			//----------------------------------------------------------------------------------
			//[RegisterAxisUser]
			//仮想Axis軸の登録を行う, name (map->key)がまだ登録されていない時にのみ登録を行う
			//return: 登録が成功したらtrue, 失敗した場合はfalse
			//引数1: 使用時に引数として使われるname (map->key)
			//引数2: 設定するAxis軸
			inline static bool RegisterAxisUser(const sgstring& name, const AxisPack& axisPack);

		private:
			//----------------------------------------------------------------------------------
			//[BaseGetAxisFloat]
			//GetAxisのStickありverで用いるBase関数
			//return: GetAxisの結果
			//引数1: axis1 (+)
			//引数2: axis1 (-)
			//引数3: axis2
			inline static float BaseGetAxisFloat(byte axis1Puls, byte axis1Minus, float axis2);
			//----------------------------------------------------------------------------------
			//[BaseGetAxisByte]
			//GetAxisのStickなしverで用いるBase関数
			//return: GetAxisの結果
			//引数1: axis1 (+)
			//引数2: axis2 (-)
			inline static float BaseGetAxisByte(ushort axis1Puls, byte axis2Puls);
			//----------------------------------------------------------------------------------
			//[GetAxisPackKeyStatesPuls]
			//GetAxisUserでkeyAndMouseCodeWithPlusのAxis軸取得に使用する
			//return: 全て取得した結果最も絶対値の大きい値 (ない場合0.0f)
			//引数1: keyAndMouseCodeWithPlus
			inline static float GetAxisPackKeyStatesPuls(const std::vector<int>& keyCodes);
			//----------------------------------------------------------------------------------
			//[GetAxisPackKeyStatesMinus]
			//GetAxisUserでkeyAndMouseCodeWithMinusのAxis軸取得に使用する
			//return: 全て取得した結果最も絶対値の大きい値 (ない場合0.0f)
			//引数1: keyAndMouseCodeWithMinus
			inline static float GetAxisPackKeyStatesMinus(const std::vector<int>& keyCodes);
			//----------------------------------------------------------------------------------
			//[GetAxisPackJoystickButtomsPuls]
			//GetAxisUserでjoystickCodeWithPlusのAxis軸取得に使用する
			//return: 全て取得した結果最も絶対値の大きい値 (ない場合0.0f)
			//引数1: joystickCodeWithPlus
			inline static float GetAxisPackJoystickButtomsPuls(const std::vector<JoystickCode::Enum>& joystickCodes, uint useStick);
			//----------------------------------------------------------------------------------
			//[GetAxisPackJoystickButtomsMinus]
			//GetAxisUserでGetAxisPackJoystickButtomsMinusのAxis軸取得に使用する
			//return: 全て取得した結果最も絶対値の大きい値 (ない場合0.0f)
			//引数1: joystickCodeWithMinus
			inline static float GetAxisPackJoystickButtomsMinus(const std::vector<JoystickCode::Enum>& joystickCodes, uint useStick);
			//----------------------------------------------------------------------------------
			//[GetAxisPackAxisCodes]
			//GetAxisUserでaxisCodesのAxis軸取得に使用する
			//return: 全て取得した結果最も絶対値の大きい値 (ない場合0.0f)
			//引数1: axisCodes
			//引数2: 使用するstick
			inline static float GetAxisPackAxisCodes(const std::vector<AxisCode::Enum>& axisCodes, uint useStick);
			//----------------------------------------------------------------------------------
			//[GetRawAxisPackAxisCodes]
			//GetAxisUserでaxisCodesのAxis軸取得に使用する (Raw ver)
			//return: 全て取得した結果最も絶対値の大きい値 (ない場合0.0f)
			//引数1: axisCodes
			//引数2: 使用するstick
			inline static float GetRawAxisPackAxisCodes(const std::vector<AxisCode::Enum>& axisCodes, uint useStick);
		};

		//RegisterAxisUserに使用するAxisPack struct
		struct AxisPack
		{
			//default
			inline AxisPack() = default;

			//----------------------------------------------------------------------------------
			//[SetKeyAndMouseCodeWithPuls]
			//keyAndMouseCodeWithPlusに複数要素をまとめて登録する
			//引数1: 登録するパラメーターパック (KeyCode::〇〇, MosueCode::〇〇)
			template < typename... Args >
			inline void SetKeyAndMouseCodeWithPuls(const Args&... args);
			//----------------------------------------------------------------------------------
			//[SetKeyAndMouseCodeWithMinus]
			//keyAndMouseCodeWithMinus複数要素をまとめて登録する
			//引数1: 登録するパラメーターパック (KeyCode::〇〇, MosueCode::〇〇)
			template < typename... Args >
			inline void SetKeyAndMouseCodeWithMinus(const Args&... args);
			//----------------------------------------------------------------------------------
			//[SetJoystickCodeWithPuls]
			//joystickCodeWithPlus複数要素をまとめて登録する
			//引数1: 登録するパラメーターパック (JoystickCode::〇〇)
			template < typename... Args >
			inline void SetJoystickCodeWithPuls(const Args&... args);
			//----------------------------------------------------------------------------------
			//[SetJoystickCodeWithMinus]
			//joystickCodeWithMinus複数要素をまとめて登録する
			//引数1: 登録するパラメーターパック (JoystickCode::〇〇)
			template < typename... Args >
			inline void SetJoystickCodeWithMinus(const Args&... args);
			//----------------------------------------------------------------------------------
			//[SetAxisCodes]
			//axisCodes複数要素をまとめて登録する, 登録するのは各取得関数となる
			//引数1: 登録するパラメーターパック (AxisCode::〇〇)
			template < typename... Args >
			inline void SetAxisCodes(const Args&... args);

			std::vector<int> keyAndMouseCodeWithPlus;					//KeyCode::〇〇, MosueCode::〇〇 (+)
			std::vector<int> keyAndMouseCodeWithMinus;				//KeyCode::〇〇, MosueCode::〇〇 (-)
			std::vector<JoystickCode::Enum> joystickCodeWithPlus;	//JoystickCode::〇〇 (+)
			std::vector<JoystickCode::Enum> joystickCodeWithMinus;		//JoystickCode::〇〇 (-)
			std::vector<AxisCode::Enum> axisCodes;						//AxisCode::〇〇 (各取得関数)
		};


	private:
		//SetVibration用キュー
		struct VibrationQueue
		{
			//default
			inline VibrationQueue() = default;
			//----------------------------------------------------------------------------------
			//[SetValue]
			//値を初期化する
			//引数1: target
			//引数2: speed
			//引数3: wait
			//引数4: useStick
			//引数5: cSet or cFadeOut or cWaitState
			//引数6: isForced
			inline void SetValue(float target, float speed, float wait, byte useStick, byte type, bool isForced)
			{
				this->target = target;
				this->speed = speed;
				this->wait = wait;
				this->elapased = 0.0f;
				this->useStick = useStick;
				this->type = type;
				this->isLive = true;
				this->isForced = isForced;
			}

			//Set state bit
			static constexpr byte cSet = 0x01;
			//Fade out state bit
			static constexpr byte cFadeOut = 0x02;
			//Wait state bit
			static constexpr byte cWaitState = 0x10;
			//Start state bit
			static constexpr byte StartState = 0x20;
		
			float target = 0.0f;		//設定値	
			float speed = 0.0f;		//減少速度 per seconds
			float wait = 0.0f;		//待機秒数
			float elapased = 0.0f;	//待機秒数計測
			byte useStick = 0;		//使用ジョイスティック (バイト余ってるしあると便利)
			byte type = 0;			//ステート保存フラグ
			bool isLive = false;		//有効か否か
			bool isForced = false;	//強制か否か
		};
		//連射用
		struct Rapid
		{
			//----------------------------------------------------------------------------------
			//[コンストラクタ]
			//値を初期化する
			//引数1: 秒数
			//引数2: フラグ
			inline Rapid(float value, bool isEnd) : value(value), isEnd(isEnd) {}
			float value = 0.0f;			//秒数
			std::atomic_bool isEnd;	//フラグ
		};

		//----------------------------------------------------------------------------------
		//[StartUp]
		//起動時初期化を行う
		//引数1: 連射間隔
		//引数2: マウスのデッドゾーン
		//引数3: ジョイスティックのデッドゾーン
		//引数4: ウィンドウのデッドゾーン
		//引数5: マウスをウィンドウ内に留めるか
		static void StartUp(const ReadElement::Pack& rapidInterval, const ReadElement::Pack& mouseDedZone, 
			const ReadElement::Pack& joystickDedZone, const ReadElement::Pack& windowDedZone, const ReadElement::Pack& isMouseInsideFrame);
		//----------------------------------------------------------------------------------
		//[Update]
		//更新を行う
		static void Update();
		//----------------------------------------------------------------------------------
		//[Shutdown]
		//終了処理を行う
		static void Shutdown();

		//----------------------------------------------------------------------------------
		//[UpdateWindowProcedure]
		//更新を行う WindowProcedure ver
		static void UpdateWindowProcedure();
		//----------------------------------------------------------------------------------
		//[ReceiveInputMessageWithWindowProcedure]
		//WindowProcedureで入力Messageを受信する
		//引数1: Message
		//引数2: WPARAM
		//引数3: LPARAM
		inline static void ReceiveInputMessageWithWindowProcedure(uint message, WPARAM wParam, LPARAM lParam);
		//----------------------------------------------------------------------------------
		//[ShutdownWindowProcedure]
		//終了処理を行う WindowProcedure ver
		static void ShutdownWindowProcedure();

		//----------------------------------------------------------------------------------
		//[UpdateAny]
		//Anyの更新を行う
		static void UpdateAny();
		//----------------------------------------------------------------------------------
		//[UpdateKeybordStateAndSendMessage]
		//KeybordStateの更新とUpdateメッセージの送信を行う
		static void UpdateKeybordStateAndSendMessage();
		//----------------------------------------------------------------------------------
		//[UpdateMouse]
		//Mouseの更新を行う (WindowProcedure内)
		static void UpdateMouse();
		//----------------------------------------------------------------------------------
		//[UpdateJoystick]
		//Joystickの更新を行う
		static void UpdateJoystick();
		//----------------------------------------------------------------------------------
		//[UpdateWindow]
		//Windowの更新を行う (WindowProcedure内)
		static void UpdateWindow();
		//----------------------------------------------------------------------------------
		//[ReConnectJoyStick]
		// 接続数を更新する
		static void ReConnectJoyStick();

		//----------------------------------------------------------------------------------
		//[DecodingKeyboradMessage]
		//キーボードMessageを解読してGetKeyをセットする
		//return: 成功ならtrue
		//引数1: Message
		//引数2: WPARAM
		inline static bool DecodingKeyboradMessage(uint message, WPARAM wParam);
		//----------------------------------------------------------------------------------
		//[DecodingMouseMessage]
		//マウスMessageを解読してGetKeyをセットする
		//引数1: Message
		//引数2: WPARAM
		//引数3: LPARAM
		inline static void DecodingMouseMessage(uint message, WPARAM wParam, LPARAM lParam);
		//----------------------------------------------------------------------------------
		//[DecodingMouseLRButtonMessage]
		//DecodingMouseMessage内でLRボタンの解読に使用
		//引数1: Message
		//引数2: WPARAM
		inline static void DecodingMouseLRButtonMessage(uint message, WPARAM wParam);
		//----------------------------------------------------------------------------------
		//[DecodingMouseOtherMessage]
		//DecodingMouseMessage内でLRボタン以外の解読に使用
		//引数1: Message
		//引数2: WPARAM
		inline static void DecodingMouseOtherMessage(uint message, WPARAM wParam);
		//----------------------------------------------------------------------------------
		//[SetKeyDown]
		//GetKeyDownを設定する
		//引数1: vKey
		inline static void SetKeyDown(uint64 vKey);
		//----------------------------------------------------------------------------------
		//[SetKeyUp]
		//GetKeyUpを設定する
		//引数1: vKey
		inline static void SetKeyUp(uint64 vKey);
		//----------------------------------------------------------------------------------
		//[SetKeyDown]
		//GetKeyDownを設定する
		//引数1: vKey
		inline static void SetKeyDown(int vKey);
		//----------------------------------------------------------------------------------
		//[SetKeyUp]
		//GetKeyUpを設定する
		//引数1: vKey
		inline static void SetKeyUp(int vKey);

		static constexpr float m_cJoystickTriggerMax = 255.0f;															//トリガー最大値
		static constexpr float m_cJoystickStickMax = 32767.0f;															//スティック最大値
		static constexpr float m_cJoystickVibrationMax = 65535.0f;													//振動最大値
		static constexpr uint m_cNowState = 0;																				//配列[現在]
		static constexpr uint m_cOldState = 1;																				//配列[1フレーム前]
		static constexpr uint m_cMessageState = 1;																		//配列[RawInput]
		static constexpr uint m_cHighState = 0;																				//配列[高周波]
		static constexpr uint m_cLowState = 1;																				//配列[低周波]
		static constexpr uint m_cKeyStatesVkeyMask = VariableLimit::cUshortMax;							//全てのキーのマスク		
		static constexpr uint m_cKeyStatesEmplaceDown = 0x10000000;											//Donwビット
		static constexpr uint m_cKeyStatesEmplaceUp = 0x20000000;												//Upビット	
		static constexpr uint m_cKeyStateSize = 256;																		//配列サイズ
		static constexpr uint m_cJoystickStateSize = sizeof(XINPUT_STATE) * Joystick::cJoystickMax;	//配列サイズ		
		static constexpr ushort m_cJoystickAllButtonMask = 0xF3FF;													//全てのボタンのマスク		
		static constexpr byte m_cNextChangeAnyInfoBit = 0x10;														//次フレーム更新ビット		
		static constexpr byte m_cNextChangeAnyValueBit = 0x01;													//更新する値ビット

		//ReConnectJoyStick時に呼ぶコールバック
		static std::function<void(uint joystickCount)> m_joystickCallback;
		//GetRapid用マップ
		static std::unordered_map<int, Rapid> m_keybordStateRapids;
		//GetRapid用vector
		static std::vector<int> m_addKeybordStateRapids;
		//GetRapid用マップ
		static std::array<std::unordered_map<ushort, Rapid>, Joystick::cJoystickMax> m_joystickRapids;
		//GetRapid用vector
		static std::vector <Tuple::Two<ushort, uint>> m_addJoystickRapids;
		//GetAxisUser用マップ
		static std::unordered_map<sgstring, AxisPack> m_axisPacks;
		//GetKeyDown or Up用vector
		static std::vector<uint> m_getKeyDownOrUpVkeys;
		//GetKeyState用配列
		static std::array<std::array<byte, m_cKeyStateSize>, 2>  m_keyStates;
		//XIpnput->Input用配列
		static std::array<std::array<XINPUT_STATE, Joystick::cJoystickMax>, 2> m_joystickStates;
		//XInput->Vibration用配列
		static std::array<XINPUT_VIBRATION, Joystick::cJoystickMax>  m_joystickVibrations;
		//XInput->Vibration->更新用配列
		static std::array<std::array<VibrationQueue, Joystick::cJoystickMax>, 2> m_joystickSetVibrations;
		//XInput->Vibration->更新用キュー配列
		static std::array<std::array<VibrationQueue, Joystick::cJoystickMax>, 2> m_joystickVibrationQueues;
		//マウスホイール
		static std::array<short, 2> m_mouseWheels;		
		//WindowProcedure用条件変数
		static ConditionVariable::Auto m_inputCompletedCondition;
		static Vector2 m_mousePosition;							//マウスの位置
		static Vector2 m_mouseAcceleration;					//マウス加速度
		static Vector2 m_windowPosition;						//ウィンドウのポジション
		static Vector2 m_windowSize;								//ウィンドウのサイズ
		static Vector2 m_windowAcceleration;					//ウィンドウ加速度
		static float m_rapidInterval;								//連射間隔
		static float m_mouseDedZone;							//マウスのデッドゾーン
		static float m_joystickDedZone;							//ジョイスティックのデッドゾーン
		static float m_windowDedZone;							//ウィンドウのデッドゾーン

		//次フレームAnyMode更新フラグ
		static std::atomic<byte> m_isNextChangeAnyMode;
		//Any有効か否か
		static std::atomic<bool> m_isAnyMode;
		//rapid atomic
		static std::atomic<bool> m_rapidAtomic;
		//joystick atomic
		static std::atomic<bool> m_joystickAtomic;
		//axis atomic
		static std::atomic<bool> m_axisAtomic;
		static byte m_joystickCount;								//ジョイスティック接続数	
		static bool m_isAny;											//GetAny
		static bool m_isAnyDown;									//GetAnyDown
		static bool m_isAnyUp;										//GetAnyUp
		static bool m_isUseJoystick;								//ジョイスティック接続数 > 0
		static bool m_isMouseInsideFrame;						//マウスをウィンドウ内に留めるか
	};

	//----------------------------------------------------------------------------------
	//[ReceiveInputMessageWithWindowProcedure]
	//WindowProcedureで入力Messageを受信する
	//引数1: Message
	//引数2: WPARAM
	//引数3: LPARAM
	void Input::ReceiveInputMessageWithWindowProcedure(uint message, WPARAM wParam, LPARAM lParam)
	{
		//ビット演算でキーボードMessageか判別, 解読
		if ((message & WM_KEYDOWN) && (((message & 0xFF) == 0x0) || (message & 0x05)))
			DecodingKeyboradMessage(message, wParam);
		//ビット演算でマウスMessageか判別, 解読
		else if ((message & WM_MOUSEMOVE) && (((message & 0xFF) == 0x0)
			|| ((message & 0xC) == 0xC) || (static_cast<byte>(message) <= 11 && static_cast<byte>(message) % 3 != 0)))
			DecodingMouseMessage(message, wParam, lParam);
	}
	//----------------------------------------------------------------------------------
	//[DecodingKeyboradMessage]
	//キーボードMessageを解読してGetKeyをセットする
	//引数1: Message
	//引数2: WPARAM
	inline bool Input::DecodingKeyboradMessage(uint message, WPARAM wParam)
	{
		//KeyDown
		if (message == WM_KEYDOWN || message == WM_SYSKEYDOWN)
		{
			SetKeyDown(wParam);
			GUI::m_inputKeyDowns[wParam] = true;
			return true;
		}
		//KeyUp
		else if (message == WM_KEYUP || message == WM_SYSKEYUP)
		{
			SetKeyUp(wParam);
			GUI::m_inputKeyDowns[wParam] = false;
			return true;
		}
		else
			return false;
	}
	//----------------------------------------------------------------------------------
	//[DecodingMouseMessage]
	//マウスMessageを解読してGetKeyをセットする
	//引数1: Message
	//引数2: WPARAM
	//引数3: LPARAM
	inline void Input::DecodingMouseMessage(uint message, WPARAM wParam, LPARAM lParam)
	{
		if (message <= WM_RBUTTONUP)
			DecodingMouseLRButtonMessage(message, wParam);
		else
			DecodingMouseOtherMessage(message, wParam);
	}
	//----------------------------------------------------------------------------------
	//[DecodingMouseLRButtonMessage]
	//DecodingMouseMessage内でLRボタンの解読に使用
	//引数1: Message
	//引数2: WPARAM
	inline void Input::DecodingMouseLRButtonMessage(uint message, WPARAM wParam)
	{
		//LButton
		if (message <= WM_LBUTTONUP)
		{
			//WM_LBUTTONDOWN
			if (message & 0x1)
			{
				SetKeyDown(MouseCode::Left);
				GUI::m_inputMouseDowns[0] = true;
			}
			//WM_LBUTTONUP
			else if (message & 0x2)
			{
				SetKeyUp(MouseCode::Left);
				GUI::m_inputMouseDowns[0] = false;
			}
		}
		//RButton
		else
		{
			//WM_RBUTTONUP
			if (message & 0x1)
			{ 
				SetKeyUp(MouseCode::Right);
				GUI::m_inputMouseDowns[1] = false;
			}
			//WM_RBUTTONDOWN
			else
			{
				SetKeyDown(MouseCode::Right);
				GUI::m_inputMouseDowns[1] = true;
			}
		}
	}
	//----------------------------------------------------------------------------------
	//[DecodingMouseOtherMessage]
	//DecodingMouseMessage内でLRボタン以外の解読に使用
	//引数1: Message
	//引数2: WPARAM
	inline void Input::DecodingMouseOtherMessage(uint message, WPARAM wParam)
	{
		//MButton
		if (message <= WM_MBUTTONUP)
		{
			//WM_MBUTTONDOWN
			if (message & 0x1)
			{
				SetKeyDown(MouseCode::Center);
				GUI::m_inputMouseDowns[2] = true;
			}
			//WM_MBUTTONUP
			else
			{
				SetKeyUp(MouseCode::Center);
				GUI::m_inputMouseDowns[2] = false;
			}
		}
		//XButton or Wheel
		else
		{
			//WM_XBUTTONDOWN
			if (message & 0x1)
			{
				SetKeyDown(MouseCode::Button5 - ((wParam >> 16) & 1));
				GUI::m_inputMouseDowns[4 - ((wParam >> 16) & 1)] = true;
			}
			//WM_XBUTTONUP
			else if(message & 0x4)
			{
				SetKeyUp(MouseCode::Button5 - ((wParam >> 16) & 1));
				GUI::m_inputMouseDowns[4 - ((wParam >> 16) & 1)] = false;
			}
			//WM_MOUSEWHEEL
			else
			{
				m_mouseWheels[m_cMessageState] += static_cast<short>((wParam >> 16) & 0xFFFF) / WHEEL_DELTA;
				GUI::m_inputMouseWheel = static_cast<float>(m_mouseWheels[m_cMessageState]);
			}
		}
	}
	//----------------------------------------------------------------------------------
	//[SetKeyDown]
	//GetKeyDownを設定する
	//引数1: vKey
	inline void Input::SetKeyDown(uint64 vKey)
	{
		if (m_keyStates[m_cMessageState][vKey] == 0)
		{
			m_getKeyDownOrUpVkeys.emplace_back(static_cast<uint>(vKey | m_cKeyStatesEmplaceDown));
			m_keyStates[m_cMessageState][vKey] = Keybord::cGetKeyDownCheckBit;
		}

		m_keyStates[m_cMessageState][vKey] |= Keybord::cGetKeyCheckBit;
	}
	//----------------------------------------------------------------------------------
	//[SetKeyUp]
	//GetKeyUpを設定する
	//引数1: vKey
	inline void Input::SetKeyUp(uint64 vKey)
	{
		m_keyStates[m_cMessageState][vKey] = Keybord::cGetKeyUpCheckBit;

		m_getKeyDownOrUpVkeys.emplace_back(static_cast<uint>(vKey | m_cKeyStatesEmplaceUp));
	}
	//----------------------------------------------------------------------------------
	//[SetKeyDown]
	//GetKeyDownを設定する
	//引数1: vKey
	inline void Input::SetKeyDown(int vKey)
	{
		if (m_keyStates[m_cMessageState][vKey] == 0)
		{
			m_getKeyDownOrUpVkeys.emplace_back(vKey | m_cKeyStatesEmplaceDown);
			m_keyStates[m_cMessageState][vKey] = Keybord::cGetKeyDownCheckBit;
		}

		m_keyStates[m_cMessageState][vKey] |= Keybord::cGetKeyCheckBit;
	}
	//----------------------------------------------------------------------------------
	//[SetKeyUp]
	//GetKeyUpを設定する
	//引数1: vKey
	inline void Input::SetKeyUp(int vKey)
	{
		m_keyStates[m_cMessageState][vKey] = Keybord::cGetKeyUpCheckBit;

		m_getKeyDownOrUpVkeys.emplace_back(vKey | m_cKeyStatesEmplaceUp);
	}



	//----------------------------------------------------------------------------------
	//Any
	//----------------------------------------------------------------------------------

	//----------------------------------------------------------------------------------
	//[GetAny]
	//return: 全てのキー, ボタンのうちどれかが押されていたらtrue
	//				isAnyModeがtrueでない場合は必ずfalseを返す
	inline bool Input::Any::GetAny() { return m_isAny; }
	//----------------------------------------------------------------------------------
	//[GetAnyDown]
	//return: 全てのキー, ボタンのうちどれかが押された瞬間ならtrue
	//				isAnyModeがtrueでない場合は必ずfalseを返す
	inline bool Input::Any::GetAnyDown() { return m_isAnyDown; }
	//----------------------------------------------------------------------------------
	//[GetAnyUp]
	//return: 全てのキー, ボタンのうちどれかが離れた瞬間ならtrue
	//				isAnyModeがtrueでない場合は必ずfalseを返す
	inline bool Input::Any::GetAnyUp() { return m_isAnyUp; };
	//----------------------------------------------------------------------------------
	//[SetAnyMode]
	//Any系関数の有効化 or 無効化を設定する, defaultではfalse
	//全ての入力を確認する必要があるので必要な時以外はfalseに設定することを推奨
	//引数1: true = 有効化, false = 無効化
	inline void Input::Any::SetAnyMode(bool mode)
	{
		AtomicOperation::Init(m_isNextChangeAnyMode, static_cast<byte>(m_cNextChangeAnyInfoBit | (IS_TRUE(mode) ? 1 : 0)));
	}



	//----------------------------------------------------------------------------------
	//Keybord
	//----------------------------------------------------------------------------------

	//----------------------------------------------------------------------------------
	//[GetKey]
	//return: 対象キーが押されていたら0より大きい値を返す, そうでない場合は0
	//引数1:  KeyCode::〇〇
	inline byte Input::Keybord::GetKey(KeyCode::Enum keyCode)
	{
		return m_keyStates[m_cNowState][keyCode] & Keybord::cGetKeyCheckBit;
	}
	//----------------------------------------------------------------------------------
	//[GetKey]
	//return: 対象キーが押されていたら0より大きい値を返す, そうでない場合は0
	//引数1:  VK_〇〇
	inline byte Input::Keybord::GetKey(int vKey)
	{
		return m_keyStates[m_cNowState][vKey] & Keybord::cGetKeyCheckBit;
	}
	//----------------------------------------------------------------------------------
	//[GetKeyDown]
	//return: 対象キーが押された瞬間なら0より大きい値を返す, そうでない場合は0
	//引数1:  KeyCode::〇〇
	inline byte Input::Keybord::GetKeyDown(KeyCode::Enum keyCode)
	{
		return m_keyStates[m_cNowState][keyCode] & cGetKeyDownCheckBit;
	}
	//----------------------------------------------------------------------------------
	//[GetKeyDown]
	//return: 対象キーが押された瞬間なら0より大きい値を返す, そうでない場合は0
	//引数1:  VK_〇〇
	inline byte Input::Keybord::GetKeyDown(int vKey)
	{
		return m_keyStates[m_cNowState][vKey] & cGetKeyDownCheckBit;
	}
	//----------------------------------------------------------------------------------
	//[GetKeyUp]
	//return: 対象キーが離れた瞬間なら0より大きい値を返す, そうでない場合は0
	//引数1:  KeyCode::〇〇
	inline byte Input::Keybord::GetKeyUp(KeyCode::Enum keyCode)
	{
		return m_keyStates[m_cNowState][keyCode] & cGetKeyUpCheckBit;
	}
	//----------------------------------------------------------------------------------
	//[GetKeyUp]
	//return: 対象キーが離れた瞬間なら0より大きい値を返す, そうでない場合は0
	//引数1:  VK_〇〇
	inline byte Input::Keybord::GetKeyUp(int vKey)
	{
		return m_keyStates[m_cNowState][vKey] & cGetKeyUpCheckBit;
	}
	//----------------------------------------------------------------------------------
	//[GetRapid]
	//return: GetKey == trueのかつ前回の成功時から連射間隔秒数が経過した時のみtrueを返す
	//				trueと判断された場合連射間隔秒数は自動で初期化される
	//引数1:  KeyCode::〇〇
	inline bool Input::Keybord::GetRapid(KeyCode::Enum keyCode)
	{
		//find
		auto it = m_keybordStateRapids.find(keyCode);

		//既に要素がある
		if (it != m_keybordStateRapids.end())
		{
			//取得値true && interval経過 && 終了フラグfalse->trueに書き換え成功
			if ((m_keyStates[m_cNowState][keyCode] & Keybord::cGetKeyCheckBit)
				&& it->second.value >= m_rapidInterval && IS_FALSE(it->second.isEnd.exchange(true)))
				return true;
			else
				return false;
		}
		//ない場合は登録
		else
		{
			AtomicOperation::LockAtomic(m_rapidAtomic);
			m_addKeybordStateRapids.emplace_back(keyCode);
			AtomicOperation::UnlockAtomic(m_rapidAtomic);
			return true;
		}
	}
	//----------------------------------------------------------------------------------
	//[GetRapid]
	//return: GetKey == trueのかつ前回の成功時から連射間隔秒数が経過した時のみtrueを返す
	//				trueと判断された場合連射間隔秒数は自動で初期化される
	//引数1:  VK_〇〇
	inline bool Input::Keybord::GetRapid(int vKey)
	{
		//find
		auto it = m_keybordStateRapids.find(vKey);

		//既に要素がある
		if (it != m_keybordStateRapids.end())
		{
			//取得値true && interval経過 && 終了フラグfalse->trueに書き換え成功
			if ((m_keyStates[m_cNowState][vKey] & Keybord::cGetKeyCheckBit)
				&& it->second.value >= m_rapidInterval && IS_FALSE(it->second.isEnd.exchange(true)))
				return true;
			else
				return false;
		}
		//ない場合は登録
		else
		{
			AtomicOperation::LockAtomic(m_rapidAtomic);
			m_addKeybordStateRapids.emplace_back(vKey);
			AtomicOperation::UnlockAtomic(m_rapidAtomic);
			return true;
		}
	}



	//----------------------------------------------------------------------------------
	//Mouse
	//----------------------------------------------------------------------------------

	//----------------------------------------------------------------------------------
	//[GetButton]
	//return: 対象ボタンが押されていたら0より大きい値を返す, そうでない場合は0
	//引数1:  MouseCode::〇〇
	inline byte Input::Mouse::GetButton(MouseCode::Enum mouseCode)
	{
		return m_keyStates[m_cNowState][mouseCode] & cGetButtonCheckBit;
	}
	//----------------------------------------------------------------------------------
	//[GetButton]
	//return: 対象キーが押されていたら0より大きい値を返す, そうでない場合は0
	//引数1:  VK_〇〇
	inline byte Input::Mouse::GetButton(int vKey)
	{
		return m_keyStates[m_cNowState][vKey] & cGetButtonCheckBit;
	}
	//----------------------------------------------------------------------------------
	//[GetButtonDown]
	//return: 対象キーが押された瞬間なら0より大きい値を返す, そうでない場合は0
	//引数1:  MouseCode::〇〇
	inline byte Input::Mouse::GetButtonDown(MouseCode::Enum mouseCode)
	{
		return m_keyStates[m_cNowState][mouseCode] & cGetButtonDownCheckBit;
	}
	//----------------------------------------------------------------------------------
	//[GetButtonDown]
	//return: 対象キーが押された瞬間なら0より大きい値を返す, そうでない場合は0
	//引数1:  VK_〇〇
	inline byte Input::Mouse::GetButtonDown(int vKey)
	{
		return m_keyStates[m_cNowState][vKey] & cGetButtonDownCheckBit;
	}
	//----------------------------------------------------------------------------------
	//[GetButtonUp]
	//return: 対象キーが離れた瞬間なら0より大きい値を返す, そうでない場合は0
	//引数1:  MouseCode::〇〇
	inline byte Input::Mouse::GetButtonUp(MouseCode::Enum mouseCode)
	{
		return m_keyStates[m_cNowState][mouseCode] & cGetButtonUpCheckBit;
	}
	//----------------------------------------------------------------------------------
	//[GetButtonUp]
	//return: 対象キーが離れた瞬間なら0より大きい値を返す, そうでない場合は0
	//引数1:  VK_〇〇
	inline byte Input::Mouse::GetButtonUp(int vKey)
	{
		return m_keyStates[m_cNowState][vKey] & cGetButtonUpCheckBit;
	}
	//----------------------------------------------------------------------------------
	//[GetRapid]
	//return: GetButton == trueのかつ前回の成功時から連射間隔秒数が経過した時のみtrueを返す
	//				trueと判断された場合連射間隔秒数は自動で初期化される
	//引数1:  MouseCode::〇〇
	inline bool Input::Mouse::GetRapid(MouseCode::Enum mouseCode)
	{
		//find
		auto it = m_keybordStateRapids.find(mouseCode);
		
		//既に要素がある
		if (it != m_keybordStateRapids.end())
		{
			//取得値true && interval経過 && 終了フラグfalse->trueに書き換え成功
			if ((m_keyStates[m_cNowState][mouseCode] & cGetButtonCheckBit)
				&& it->second.value >= m_rapidInterval && IS_FALSE(it->second.isEnd.exchange(true)))
				return true;
			else
				return false;
		}
		//ない場合は登録
		else
		{
			AtomicOperation::LockAtomic(m_rapidAtomic);
			m_addKeybordStateRapids.emplace_back(mouseCode);
			AtomicOperation::UnlockAtomic(m_rapidAtomic);
			return true;
		}
	}
	//----------------------------------------------------------------------------------
	//[GetRapid]
	//return: GetButton == trueのかつ前回の成功時から連射間隔秒数が経過した時のみtrueを返す
	//				trueと判断された場合連射間隔秒数は自動で初期化される
	//引数1:  VK_〇〇
	inline bool Input::Mouse::GetRapid(int vKey)
	{
		//find
		auto it = m_keybordStateRapids.find(vKey);

		//既に要素がある
		if (it != m_keybordStateRapids.end())
		{
			//取得値true && interval経過 && 終了フラグfalse->trueに書き換え成功
			if ((m_keyStates[m_cNowState][vKey] & cGetButtonCheckBit)
				&& it->second.value >= m_rapidInterval && IS_FALSE(it->second.isEnd.exchange(true)))
				return true;
			else
				return false;
		}
		//ない場合は登録
		else
		{
			AtomicOperation::LockAtomic(m_rapidAtomic);
			m_addKeybordStateRapids.emplace_back(vKey);
			AtomicOperation::UnlockAtomic(m_rapidAtomic);
			return true;
		}
	}
	//----------------------------------------------------------------------------------
	//[GetPosition]
	//return: ウィンドウの大きさを縦横-1.0f ~ 1.0fとした場合の現在のマウス座標
	inline Vector2 Input::Mouse::GetPosition()
	{
		return m_mousePosition;
	}
	//----------------------------------------------------------------------------------
	//[GetPositionToWindow]
	//return: ウィンドウの大きさを縦横0.0f ~ 1.0fとした場合の現在のマウス座標
	inline Vector2 Input::Mouse::GetPositionToWindow()
	{
		return m_mousePosition / 2 + Const::Vector2::one;
	}
	//----------------------------------------------------------------------------------
	//[GetAcceleration]
	//return: 前フレーム比のポジション移動量, 各要素の絶対値がdedZone以下の場合必ず0.0fとなる
	inline Vector2 Input::Mouse::GetAcceleration()
	{
		Vector2 result;
		result.x = (MathAF::Absolute(m_mouseAcceleration.x) > m_mouseDedZone) ?
			m_mouseAcceleration.x : 0.0f;
		result.y = (MathAF::Absolute(m_mouseAcceleration.y) > m_mouseDedZone) ?
			m_mouseAcceleration.y : 0.0f;
		return result;
	}
	//----------------------------------------------------------------------------------
	//[GetAcceleration]
	//return: dedZoneの影響を受けない前フレーム比のポジション移動量
	inline Vector2 Input::Mouse::GetRawAcceleration()
	{
		return m_mouseAcceleration;
	}
	//----------------------------------------------------------------------------------
	//[GetWheelDelta]
	//return: 現在のwheel遷移状態 (奥 -> +1~, 手前 -> -1~)
	inline short Input::Mouse::GetWheelDelta()
	{
		return m_mouseWheels[m_cNowState];
	}



	//----------------------------------------------------------------------------------
	//Joystick
	//----------------------------------------------------------------------------------

	//----------------------------------------------------------------------------------
	//[GetButton]
	//return: 対象ボタンが押されていたら0より大きい値を返す, そうでない場合は0
	//引数1: JoystickCode::〇〇
	//引数2: 取得に使用するジョイスティックの番号 (0 ~ 4), 例外処理は行わない, default = 0
	inline ushort Input::Joystick::GetButton(JoystickCode::Enum joystickCode, uint useStick)
	{
		return (m_joystickStates[m_cNowState][useStick].Gamepad.wButtons & joystickCode);
	}
	//----------------------------------------------------------------------------------
	//[GetButtonDown]
	//return: 対象キーが押された瞬間なら0より大きい値を返す, そうでない場合は0
	//引数1: JoystickCode::〇〇
	//引数2: 取得に使用するジョイスティックの番号 (0 ~ 4), 例外処理は行わない, default = 0
	inline ushort Input::Joystick::GetButtonDown(JoystickCode::Enum joystickCode, uint useStick)
	{
		return (((m_joystickStates[m_cNowState][useStick].Gamepad.wButtons ^ m_joystickStates[m_cOldState][useStick].Gamepad.wButtons)
			&  m_joystickStates[m_cNowState][useStick].Gamepad.wButtons) & joystickCode);
	}
	//----------------------------------------------------------------------------------
	//[GetButtonUp]
	//return: 対象キーが離れた瞬間なら0より大きい値を返す, そうでない場合は0
	//引数1: JoystickCode::〇〇
	//引数2: 取得に使用するジョイスティックの番号 (0 ~ 4), 例外処理は行わない, default = 0
	inline ushort Input::Joystick::GetButtonUp(JoystickCode::Enum joystickCode, uint useStick)
	{
		return (((m_joystickStates[m_cNowState][useStick].Gamepad.wButtons ^ m_joystickStates[m_cOldState][useStick].Gamepad.wButtons)
			&  m_joystickStates[m_cOldState][useStick].Gamepad.wButtons) & joystickCode);
	}
	//----------------------------------------------------------------------------------
	//[GetRapid]
	//return: GetButton == trueのかつ前回の成功時から連射間隔秒数が経過した時のみtrueを返す
	//				trueと判断された場合連射間隔秒数は自動で初期化される
	//引数1: JoystickCode::〇〇
	//引数2: 取得に使用するジョイスティックの番号 (0 ~ 4), 例外処理は行わない, default = 0
	inline bool Input::Joystick::GetRapid(JoystickCode::Enum joystickCode, uint useStick)
	{
		//find
		auto it = m_joystickRapids[useStick].find(joystickCode);

		//既に要素がある
		if (it != m_joystickRapids[useStick].end())
		{
			//取得値true && interval経過 && 終了フラグfalse->trueに書き換え成功
			if ((m_joystickStates[m_cNowState][useStick].Gamepad.wButtons  & joystickCode)
				&& it->second.value >= m_rapidInterval && IS_FALSE(it->second.isEnd.exchange(true)))
				return true;
			else
				return false;
		}
		//ない場合登録する
		else
		{
			AtomicOperation::LockAtomic(m_rapidAtomic);
			m_addJoystickRapids.emplace_back(joystickCode, useStick);
			AtomicOperation::UnlockAtomic(m_rapidAtomic);
			return true;
		}
	}
	//----------------------------------------------------------------------------------
	//[GetLTrigger]
	//return: 0.0f ~ 1.0fの値となるLトリガーの現在値, 絶対値がdedZone以下の場合必ず0.0fを返却する
	//引数1: 取得に使用するジョイスティックの番号 (0 ~ 4), 例外処理は行わない, default = 0
	inline float Input::Joystick::GetLTrigger(uint useStick)
	{
		float now = static_cast<float>(m_joystickStates[m_cNowState][useStick].Gamepad.bLeftTrigger) / m_cJoystickTriggerMax;
		return (now > m_joystickDedZone) ? now : 0.0f;
	}
	//----------------------------------------------------------------------------------
	//[GetRawLTrigger]
	//return: 0.0f ~ 1.0fの値となるdedZoneを考慮しないLトリガーの現在値
	//引数1: 取得に使用するジョイスティックの番号 (0 ~ 4), 例外処理は行わない, default = 0
	inline float Input::Joystick::GetRawLTrigger(uint useStick)
	{
		return static_cast<float>(m_joystickStates[m_cNowState][useStick].Gamepad.bLeftTrigger) / m_cJoystickTriggerMax;
	}
	inline float Input::Joystick::GetLTriggerAcceleration(uint useStick)
	{
		float now = static_cast<float>(m_joystickStates[m_cNowState][useStick].Gamepad.bLeftTrigger) / m_cJoystickTriggerMax;
		float old = static_cast<float>(m_joystickStates[m_cOldState][useStick].Gamepad.bLeftTrigger) / m_cJoystickTriggerMax;

		if (now > m_joystickDedZone)
			return (old > m_joystickDedZone) ? now - old : now - m_joystickDedZone;
		else
			return (old > m_joystickDedZone) ? m_joystickDedZone - old : 0.0f;
	}
	//----------------------------------------------------------------------------------
	//[GetRawLTriggerAcceleration]
	//return: 0.0f ~ 1.0fの値となるdedZoneを考慮しないLトリガーの前フレーム比移動量
	//引数1: 取得に使用するジョイスティックの番号 (0 ~ 4), 例外処理は行わない, default = 0
	inline float Input::Joystick::GetRawLTriggerAcceleration(uint useStick)
	{
		return static_cast<float>(m_joystickStates[m_cNowState][useStick].Gamepad.bLeftTrigger) / m_cJoystickTriggerMax
			- static_cast<float>(m_joystickStates[m_cOldState][useStick].Gamepad.bLeftTrigger) / m_cJoystickTriggerMax;
	}
	//----------------------------------------------------------------------------------
	//[GetRTrigger]
	//return: 0.0f ~ 1.0fの値となるRトリガーの現在値, 絶対値がdedZone以下の場合必ず0.0fを返却する
	//引数1: 取得に使用するジョイスティックの番号 (0 ~ 4), 例外処理は行わない, default = 0
	inline float Input::Joystick::GetRTrigger(uint useStick)
	{
		float now = static_cast<float>(m_joystickStates[m_cNowState][useStick].Gamepad.bRightTrigger) / m_cJoystickTriggerMax;
		return (now > m_joystickDedZone) ? now : 0.0f;
	}
	//----------------------------------------------------------------------------------
	//[GetRawRTrigger]
	//return: 0.0f ~ 1.0fの値となるdedZoneを考慮しないRトリガーの現在値
	//引数1: 取得に使用するジョイスティックの番号 (0 ~ 4), 例外処理は行わない, default = 0
	inline float Input::Joystick::GetRawRTrigger(uint useStick)
	{
		return static_cast<float>(m_joystickStates[m_cNowState][useStick].Gamepad.bRightTrigger) / m_cJoystickTriggerMax;
	}
	//----------------------------------------------------------------------------------
	//[GetRTriggerAcceleration]
	//return: 0.0f ~ 1.0fの値を取るRトリガーの前フレーム比の移動量,
	//				比較に使われる値はdedZoneの影響を受ける
	//引数1: 取得に使用するジョイスティックの番号 (0 ~ 4), 例外処理は行わない, default = 0
	inline float Input::Joystick::GetRTriggerAcceleration(uint useStick)
	{
		float now = static_cast<float>(m_joystickStates[m_cNowState][useStick].Gamepad.bRightTrigger) / m_cJoystickTriggerMax;
		float old = static_cast<float>(m_joystickStates[m_cOldState][useStick].Gamepad.bRightTrigger) / m_cJoystickTriggerMax;

		if (now > m_joystickDedZone) return (old > m_joystickDedZone) ? now - old : now - m_joystickDedZone;
		else return (old > m_joystickDedZone) ? m_joystickDedZone - old : 0.0f;
	}
	//----------------------------------------------------------------------------------
	//[GetRawRTriggerAcceleration]
	//return: 0.0f ~ 1.0fの値となるdedZoneを考慮しないRトリガーの前フレーム比移動量
	//引数1: 取得に使用するジョイスティックの番号 (0 ~ 4), 例外処理は行わない, default = 0
	inline float Input::Joystick::GetRawRTriggerAcceleration(uint useStick)
	{
		return static_cast<float>(m_joystickStates[m_cNowState][useStick].Gamepad.bRightTrigger) / m_cJoystickTriggerMax
			- static_cast<float>(m_joystickStates[m_cOldState][useStick].Gamepad.bRightTrigger) / m_cJoystickTriggerMax;
	}
	//----------------------------------------------------------------------------------
	//[GetLStick]
	//return: -1.0f ~ 1.0fの値となるLスティックの現在値, 各要素の絶対値がdedZone以下の場合必ず0.0fとなる
	//引数1: 取得に使用するジョイスティックの番号 (0 ~ 4), 例外処理は行わない, default = 0
	inline Vector2 Input::Joystick::GetLStick(uint useStick)
	{
		Vector2 result(Vector2::CreateWithInt(m_joystickStates[m_cNowState][useStick].Gamepad.sThumbLX,
			m_joystickStates[m_cNowState][useStick].Gamepad.sThumbLY) / m_cJoystickStickMax);

		if (MathAF::Absolute(result.x) <= m_joystickDedZone) result.x = 0.0f;
		if (MathAF::Absolute(result.y) <= m_joystickDedZone) result.y = 0.0f;

		return result;
	}
	//----------------------------------------------------------------------------------
	//[GetRawLStick]
	//return: -1.0f ~ 1.0fの値となるdedZoneを考慮しないLスティックの現在値
	//引数1: 取得に使用するジョイスティックの番号 (0 ~ 4), 例外処理は行わない, default = 0
	inline Vector2 Input::Joystick::GetRawLStick(uint useStick)
	{
		return Vector2::CreateWithInt(m_joystickStates[m_cNowState][useStick].Gamepad.sThumbLX,
			m_joystickStates[m_cNowState][useStick].Gamepad.sThumbLY) / m_cJoystickStickMax;
	}
	//----------------------------------------------------------------------------------
	//[GetLStickAcceleration]
	//return: -1.0f ~ 1.0fの値を取るLスティックの前フレーム比の移動量,
	//				比較に使われる値はdedZoneの影響を受ける
	//引数1: 取得に使用するジョイスティックの番号 (0 ~ 4), 例外処理は行わない, default = 0
	inline Vector2 Input::Joystick::GetLStickAcceleration(uint useStick)
	{
		Vector2 nowResult(Vector2::CreateWithInt(m_joystickStates[m_cNowState][useStick].Gamepad.sThumbLX,
			m_joystickStates[m_cNowState][useStick].Gamepad.sThumbLY) / m_cJoystickStickMax);
		Vector2 oldResult(Vector2::CreateWithInt(m_joystickStates[m_cOldState][useStick].Gamepad.sThumbLX,
			m_joystickStates[m_cOldState][useStick].Gamepad.sThumbLY) / m_cJoystickStickMax);


		if (MathAF::Absolute(nowResult.x) > m_joystickDedZone)
		{
			if (MathAF::Absolute(oldResult.x) > m_joystickDedZone)
				nowResult.x = nowResult.x - oldResult.x;
			else if (nowResult.x > 0.0f)
				nowResult.x = nowResult.x - m_joystickDedZone;
			else
				nowResult.x = nowResult.x + m_joystickDedZone;
		}
		else
		{
			if (oldResult.x > m_joystickDedZone)
				nowResult.x = m_joystickDedZone - oldResult.x;
			else if (oldResult.x < m_joystickDedZone)
				nowResult.x = -m_joystickDedZone - oldResult.x;
			else
				nowResult.x = 0.0f;
		}

		return nowResult;
	}
	//----------------------------------------------------------------------------------
	//[GetRawLStickAcceleration]
	//return: -1.0f ~ 1.0fの値となるdedZoneを考慮しないLスティックの前フレーム比移動量
	//引数1: 取得に使用するジョイスティックの番号 (0 ~ 4), 例外処理は行わない, default = 0
	inline Vector2 Input::Joystick::GetRawLStickAcceleration(uint useStick)
	{
		return Vector2::CreateWithInt(m_joystickStates[m_cNowState][useStick].Gamepad.sThumbLX - m_joystickStates[m_cOldState][useStick].Gamepad.sThumbLX,
			m_joystickStates[m_cNowState][useStick].Gamepad.sThumbLY - m_joystickStates[m_cOldState][useStick].Gamepad.sThumbLY) / m_cJoystickStickMax;
	}
	//----------------------------------------------------------------------------------
	//[GetRStick]
	//return: -1.0f ~ 1.0fの値となるRスティックの現在値, 各要素の絶対値がdedZone以下の場合必ず0.0fとなる
	//引数1: 取得に使用するジョイスティックの番号 (0 ~ 4), 例外処理は行わない, default = 0
	inline Vector2 Input::Joystick::GetRStick(uint useStick)
	{
		Vector2 result(Vector2::CreateWithInt(m_joystickStates[m_cNowState][useStick].Gamepad.sThumbRX,
			m_joystickStates[m_cNowState][useStick].Gamepad.sThumbRY) / m_cJoystickStickMax);

		if (MathAF::Absolute(result.x) <= m_joystickDedZone) result.x = 0.0f;
		if (MathAF::Absolute(result.y) <= m_joystickDedZone) result.y = 0.0f;

		return result;
	}
	//----------------------------------------------------------------------------------
	//[GetRawRStick]
	//return: -1.0f ~ 1.0fの値となるdedZoneを考慮しないRスティックの現在値
	//引数1: 取得に使用するジョイスティックの番号 (0 ~ 4), 例外処理は行わない, default = 0
	inline Vector2 Input::Joystick::GetRawRStick(uint useStick)
	{
		return Vector2::CreateWithInt(m_joystickStates[m_cNowState][useStick].Gamepad.sThumbRX,
			m_joystickStates[m_cNowState][useStick].Gamepad.sThumbRY) / m_cJoystickStickMax;
	}
	//----------------------------------------------------------------------------------
	//[GetRStickAcceleration]
	//return: -1.0f ~ 1.0fの値を取るRスティックの前フレーム比の移動量,
	//				比較に使われる値はdedZoneの影響を受ける
	//引数1: 取得に使用するジョイスティックの番号 (0 ~ 4), 例外処理は行わない, default = 0
	inline Vector2 Input::Joystick::GetRStickAcceleration(uint useStick)
	{
		Vector2 nowResult(Vector2::CreateWithInt(m_joystickStates[m_cNowState][useStick].Gamepad.sThumbRX,
			m_joystickStates[m_cNowState][useStick].Gamepad.sThumbRY) / m_cJoystickStickMax);
		Vector2 oldResult(Vector2::CreateWithInt(m_joystickStates[m_cOldState][useStick].Gamepad.sThumbRX,
			m_joystickStates[m_cOldState][useStick].Gamepad.sThumbRY) / m_cJoystickStickMax);

		if (MathAF::Absolute(nowResult.x) > m_joystickDedZone)
		{
			if (MathAF::Absolute(oldResult.x) > m_joystickDedZone)
				nowResult.x = nowResult.x - oldResult.x;
			else if (nowResult.x > 0.0f)
				nowResult.x = nowResult.x - m_joystickDedZone;
			else
				nowResult.x = nowResult.x + m_joystickDedZone;
		}
		else
		{
			if (oldResult.x > m_joystickDedZone)
				nowResult.x = m_joystickDedZone - oldResult.x;
			else if (oldResult.x < m_joystickDedZone)
				nowResult.x = -m_joystickDedZone - oldResult.x;
			else
				nowResult.x = 0.0f;
		}

		return nowResult;
	}
	//----------------------------------------------------------------------------------
	//[GetRawRStickAcceleration]
	//return: -1.0f ~ 1.0fの値となるdedZoneを考慮しないRスティックの前フレーム比移動量
	//引数1: 取得に使用するジョイスティックの番号 (0 ~ 4), 例外処理は行わない, default = 0
	inline Vector2 Input::Joystick::GetRawRStickAcceleration(uint useStick)
	{
		return Vector2::CreateWithInt(m_joystickStates[m_cNowState][useStick].Gamepad.sThumbRX - m_joystickStates[m_cOldState][useStick].Gamepad.sThumbRX,
			m_joystickStates[m_cNowState][useStick].Gamepad.sThumbRY - m_joystickStates[m_cOldState][useStick].Gamepad.sThumbRY) / m_cJoystickStickMax;
	}
	//----------------------------------------------------------------------------------
	//[GetHighVibration]
	//return: 0.0f ~ 1.0fの値となる現在の高周波振動値
	//引数1: 取得に使用するジョイスティックの番号 (0 ~ 4), 例外処理は行わない, default = 0
	inline float Input::Joystick::GetHighVibration(uint useStick)
	{
		return static_cast<float>(m_joystickVibrations[useStick].wRightMotorSpeed) / m_cJoystickVibrationMax;;
	}
	//----------------------------------------------------------------------------------
	//[GetLowVibration]
	//return: 0.0f ~ 1.0fの値となる現在の低周波振動値
	//引数1: 取得に使用するジョイスティックの番号 (0 ~ 4), 例外処理は行わない, default = 0
	inline float Input::Joystick::GetLowVibration(uint useStick)
	{
		return static_cast<float>(m_joystickVibrations[useStick].wLeftMotorSpeed) / m_cJoystickVibrationMax;;
	}
	//----------------------------------------------------------------------------------
	//[SetHighVibration]
	//高周波振動値を設定する, 実行は1フレーム後に行われるが、FadeOut中の場合は登録に失敗する
	//各コントローラーでHigh, Low各一つずつしか設定する事はできない
	//return: 設定が完了すればtrue, 失敗(すでに設定されている or FadeOut中)の場合false
	//引数1: 設定する振動値, 0.0f ~ 1.0fにClampされる
	//引数2: 取得に使用するジョイスティックの番号 (0 ~ 4), 接続されていない場合失敗, default = 0
	//引数3: trueの場合, 既に設定済みの場合も強制的に設定する, default = false
	inline bool Input::Joystick::SetHighVibration(float setVibration, uint useStick, bool isForced)
	{
		if (useStick >= m_joystickCount) return false;

		setVibration = MathAF::Clamp01(setVibration);
		AtomicOperation::LockAtomic(m_joystickAtomic);

		//強制 or 登録可能
		if (IS_TRUE(isForced) | IS_FALSE(m_joystickSetVibrations[m_cHighState][useStick].isLive))
		{
			m_joystickVibrationQueues[m_cHighState][useStick].SetValue(setVibration, 0.0f, 0.0f, useStick, VibrationQueue::cSet, isForced);
			AtomicOperation::UnlockAtomic(m_joystickAtomic);
			return true;
		}

		AtomicOperation::UnlockAtomic(m_joystickAtomic);
		return false;
	}
	//----------------------------------------------------------------------------------
	//[SetLowVibration]
	//低周波振動値を設定する, 実行は1フレーム後に行われるが、FadeOut中の場合は登録に失敗する
	//各コントローラーでHigh, Low各一つずつしか設定する事はできない
	//return: 設定が完了すればtrue, 失敗(すでに設定されている or FadeOut中)の場合false
	//引数1: 設定する振動値, 0.0f ~ 1.0fにClampされる
	//引数2: 取得に使用するジョイスティックの番号 (0 ~ 4), 接続されていない場合失敗, default = 0
	//引数3: trueの場合, 既に設定済みの場合も強制的に設定する, default = false
	inline bool Input::Joystick::SetLowVibration(float setVibration, uint useStick, bool isForced)
	{
		if (useStick >= m_joystickCount) return false;

		setVibration = MathAF::Clamp01(setVibration);
		AtomicOperation::LockAtomic(m_joystickAtomic);

		//強制 or 登録可能
		if (IS_TRUE(isForced) | IS_FALSE(m_joystickSetVibrations[m_cLowState][useStick].isLive))
		{
			m_joystickVibrationQueues[m_cLowState][useStick].SetValue(setVibration, 0.0f, 0.0f, useStick, VibrationQueue::cSet, isForced);
			AtomicOperation::UnlockAtomic(m_joystickAtomic);
			return true;
		}
	
		AtomicOperation::UnlockAtomic(m_joystickAtomic);
		return false;
	}
	//----------------------------------------------------------------------------------
	//[SetHighVibrationFadeOut]
	//高周波振動値を設定値から0.0fへフェードアウトするように設定する
	//実行は1フレーム後に行われるが、FadeOut中の場合は登録に失敗する
	//実行順序は setVibration設定 -> waitReachSeconds経過 -> reachZeroSecondsかけて0.0fまで減少
	//各コントローラーでHigh, Low各一つずつしか設定する事はできない
	//return: 設定が完了すればtrue, 失敗(すでに設定されている or FadeOut中)の場合false
	//引数1: 設定する振動値, 0.0f ~ 1.0fにClampされる
	//引数2: 減少し始めるまでの待機時間
	//引数3: 0.0fになるまでかかる所要時間
	//引数4: 取得に使用するジョイスティックの番号 (0 ~ 4), 接続されていない場合失敗, default = 0
	//引数5: trueの場合, 既に設定済みの場合も強制的に設定する, default = false
	inline bool Input::Joystick::SetHighVibrationFadeOut(float setVibration, float waitReachSeconds, float reachZeroSeconds, uint useStick, bool isForced)
	{
		if (useStick >= m_joystickCount) return false;

		setVibration = MathAF::Clamp01(setVibration);
		AtomicOperation::LockAtomic(m_joystickAtomic);

		//強制 or 登録可能
		if (IS_TRUE(isForced) | IS_FALSE(m_joystickSetVibrations[m_cHighState][useStick].isLive))
		{
			m_joystickVibrationQueues[m_cHighState][useStick].SetValue(setVibration, setVibration / reachZeroSeconds, waitReachSeconds,
				useStick, VibrationQueue::cFadeOut | VibrationQueue::cWaitState | VibrationQueue::StartState, isForced);
			AtomicOperation::UnlockAtomic(m_joystickAtomic);
		}

		AtomicOperation::UnlockAtomic(m_joystickAtomic);
	}
	//----------------------------------------------------------------------------------
	//[SetLowVibrationFadeOut]
	//低周波振動値を設定値から0.0fへフェードアウトするように設定する			
	//実行は1フレーム後に行われるが、FadeOut中の場合は登録に失敗する
	//実行順序は setVibration設定 -> waitReachSeconds経過 -> reachZeroSecondsかけて0.0fまで減少
	//各コントローラーでHigh, Low各一つずつしか設定する事はできない
	//return: 設定が完了すればtrue, 失敗(すでに設定されている or FadeOut中)の場合false
	//引数1: 設定する振動値, 0.0f ~ 1.0fにClampされる
	//引数2: 減少し始めるまでの待機時間
	//引数3: 0.0fになるまでかかる所要時間
	//引数4: 取得に使用するジョイスティックの番号 (0 ~ 4), 接続されていない場合失敗, default = 0
	//引数5: trueの場合, 既に設定済みの場合も強制的に設定する, default = false
	inline bool Input::Joystick::SetLowVibrationFadeOut(float setVibration, float waitReachSeconds, float reachZeroSeconds, uint useStick, bool isForced)
	{
		if (useStick >= m_joystickCount) return false;

		setVibration = MathAF::Clamp01(setVibration);
		AtomicOperation::LockAtomic(m_joystickAtomic);

		//強制 or 登録可能
		if (IS_TRUE(isForced) | IS_FALSE(m_joystickSetVibrations[m_cLowState][useStick].isLive))
		{
			m_joystickVibrationQueues[m_cLowState][useStick].SetValue(setVibration, setVibration / reachZeroSeconds, waitReachSeconds,
				useStick, VibrationQueue::cFadeOut | VibrationQueue::cWaitState | VibrationQueue::StartState, isForced);
			AtomicOperation::UnlockAtomic(m_joystickAtomic);
		}

		AtomicOperation::UnlockAtomic(m_joystickAtomic);
	}
	//----------------------------------------------------------------------------------
	//[ResetVibration]
	//現在の振動設定値をHigh Lowともに0.0fまでフェードアウトさせる
	//この関数は既に設定されているか否かに関わらず強制的に実行される
	//引数1: 0.0fになるまでかかる所要時間
	//引数2: 取得に使用するジョイスティックの番号 (0 ~ 4), 例外処理は行わない, default = 0
	inline void Input::Joystick::ResetVibration(float reachZeroSpeedPerSeconds, uint useStick)
	{
		float target1 = static_cast<float>(m_joystickVibrations[useStick].wRightMotorSpeed) / m_cJoystickVibrationMax,
			target2 = static_cast<float>(m_joystickVibrations[useStick].wLeftMotorSpeed) / m_cJoystickVibrationMax;
		
		AtomicOperation::LockAtomic(m_joystickAtomic);
		if (target1 > 0.0f) m_joystickVibrationQueues[m_cHighState][useStick].SetValue(target1, reachZeroSpeedPerSeconds, 0.0f, useStick, VibrationQueue::cFadeOut, true);
		if (target2 > 0.0f) m_joystickVibrationQueues[m_cLowState][useStick].SetValue(target2, reachZeroSpeedPerSeconds, 0.0f, useStick, VibrationQueue::cFadeOut, true);
		AtomicOperation::UnlockAtomic(m_joystickAtomic);
	}
	//----------------------------------------------------------------------------------
	//[ResetAllVibration]
	//全てのジョイスティックで現在の振動設定値をHigh Lowともに0.0fまでフェードアウトさせる
	//この関数は既に設定されているか否かに関わらず強制的に実行される
	//引数1: 0.0fになるまでかかる所要時間
	inline void Input::Joystick::ResetAllVibration(float reachZeroSpeedPerSeconds)
	{
		float target1, target2;

		AtomicOperation::LockAtomic(m_joystickAtomic);
		for (uint i = 0; i < m_joystickCount; i++)
		{
			target1 = static_cast<float>(m_joystickVibrations[i].wRightMotorSpeed) / m_cJoystickVibrationMax;
			target2 = static_cast<float>(m_joystickVibrations[i].wLeftMotorSpeed) / m_cJoystickVibrationMax;
			
			if (target1 > 0.0f) m_joystickVibrationQueues[m_cHighState][i].SetValue(target1, reachZeroSpeedPerSeconds, 0.0f, i, VibrationQueue::cFadeOut, true);
			if (target2 > 0.0f) m_joystickVibrationQueues[m_cLowState][i].SetValue(target2, reachZeroSpeedPerSeconds, 0.0f, i, VibrationQueue::cFadeOut, true);
		}
		AtomicOperation::UnlockAtomic(m_joystickAtomic);
	}
	//----------------------------------------------------------------------------------
	//[SetAutoReConnectCallback]
	//接続数の変更が行われた時のコールバック関数を設定する
	//引数1: コールバック関数, 引数1: 新しいジョイスティックの接続数(uint)
	inline void Input::Joystick::SetAutoReConnectCallback(const std::function<void(uint joystickCount)>& callback)
	{
		AtomicOperation::LockAtomic(m_joystickAtomic);
		m_joystickCallback = callback;
		AtomicOperation::UnlockAtomic(m_joystickAtomic);
	}



	//----------------------------------------------------------------------------------
	//Window
	//----------------------------------------------------------------------------------

	//----------------------------------------------------------------------------------
	//[GetPosition]
	//return: 画面解像度を 0.0f ~ 1.0fとした場合のウィンドウの中心座標
	inline Vector2 Input::Window::GetPosition()
	{
		return m_windowPosition;
	}
	//----------------------------------------------------------------------------------
	//[GetSize]
	//return: 画面解像度を 0.0f ~ 1.0fとした場合のウィンドウのサイズ
	inline Vector2 Input::Window::GetSize()
	{
		return m_windowSize;
	}
	//----------------------------------------------------------------------------------
	//[GetAcceleration]
	//return: 画面解像度を 0.0f ~ 1.0fとした場合の前フレームからのウィンドウ移動量
	//				各要素の絶対値がdedZone以下の場合必ず0.0fとなる
	inline Vector2 Input::Window::GetAcceleration()
	{
		Vector2 result;
	
		result.x = (MathAF::Absolute(m_windowAcceleration.x) > m_windowDedZone) ?
			m_windowAcceleration.x : 0.0f;
		result.y = (MathAF::Absolute(m_windowAcceleration.y) > m_windowDedZone) ?
			m_windowAcceleration.y : 0.0f;
		return result;
	}
	//----------------------------------------------------------------------------------
	//[GetAcceleration]
	//return: 画面解像度を 0.0f ~ 1.0fとした場合のdedZoneの影響を受けない前フレームからのウィンドウ移動量
	inline Vector2 Input::Window::GetRawAcceleration()
	{
		return m_windowAcceleration;
	}



	//----------------------------------------------------------------------------------
	//Axis
	//----------------------------------------------------------------------------------

	//----------------------------------------------------------------------------------
	//[GetAxisHorizontal]
	//仮想HorizontalAxis軸の取得を行う, 取得する値は[Keybord->A (-), D (+)], [Joystick->LStick.x]
	//return: 各取得値の中で最も絶対値が大きい値
	//引数1: 取得に使用するジョイスティックの番号 (0 ~ 4), 例外処理は行わない, default = 0
	inline float Input::Axis::GetAxisHorizontal(uint useJoystick)
	{
		return BaseGetAxisFloat(Keybord::GetKey('A'), Keybord::GetKey('D'), Joystick::GetLStick(useJoystick).x);
	}
	//----------------------------------------------------------------------------------
	//[GetRawAxisHorizontal]
	//仮想HorizontalAxis軸の取得をdedZoneの影響を受けずに行う
	//取得する値は[Keybord->A (-), D (+)], [Joystick->RawLStick.x]
	//return: 各取得値の中で最も絶対値が大きい値
	//引数1: 取得に使用するジョイスティックの番号 (0 ~ 4), 例外処理は行わない, default = 0
	inline float Input::Axis::GetRawAxisHorizontal(uint useJoystick)
	{
		return BaseGetAxisFloat(Keybord::GetKey('A'), Keybord::GetKey('D'), Joystick::GetRawLStick(useJoystick).x);
	}
	//----------------------------------------------------------------------------------
	//[GetAxisVertical]
	//仮想VerticalAxis軸の取得を行う, 取得する値は[Keybord->W (-), S (+)], [Joystick->LStick.y]
	//return: 各取得値の中で最も絶対値が大きい値
	//引数1: 取得に使用するジョイスティックの番号 (0 ~ 4), 例外処理は行わない, default = 0
	inline float Input::Axis::GetAxisVertical(uint useJoystick)
	{
		return BaseGetAxisFloat(Keybord::GetKey('W'), Keybord::GetKey('S'), Joystick::GetLStick(useJoystick).y);
	}
	//----------------------------------------------------------------------------------
	//[GetRawAxisVertical]
	//仮想VerticalAxis軸の取得をdedZoneの影響を受けずに行う
	//取得する値は[Keybord->W (-), S (+)], [Joystick->RawLStick.y]
	//return: 各取得値の中で最も絶対値が大きい値
	//引数1: 取得に使用するジョイスティックの番号 (0 ~ 4), 例外処理は行わない, default = 0
	inline float Input::Axis::GetRawAxisVertical(uint useJoystick)
	{
		return BaseGetAxisFloat(Keybord::GetKey('W'), Keybord::GetKey('S'), Joystick::GetRawLStick(useJoystick).y);
	}
	//----------------------------------------------------------------------------------
	//[GetAxisJump]
	//仮想JumpAxis軸の取得を行う, 取得する値は[Keybord->Space (+)], [Joystick->A (+)]
	//return: 各取得値の中で最も絶対値が大きい値
	//引数1: 取得に使用するジョイスティックの番号 (0 ~ 4), 例外処理は行わない, default = 0
	inline float Input::Axis::GetAxisJump(uint useJoystick)
	{
		return BaseGetAxisByte(Joystick::GetButton(JoystickCode::A), Keybord::GetKey(VK_SPACE));
	}
	//----------------------------------------------------------------------------------
	//[GetAxisAction]
	//仮想ActionAxis軸の取得を行う, 取得する値は[Mouse->Left (+)], [Joystick->A (+)]
	//return: 各取得値の中で最も絶対値が大きい値
	//引数1: 取得に使用するジョイスティックの番号 (0 ~ 4), 例外処理は行わない, default = 0
	inline float Input::Axis::GetAxisAction(uint useJoystick)
	{
		return BaseGetAxisByte(Joystick::GetButton(JoystickCode::B), Mouse::GetButton(VK_LBUTTON));
	}
	//----------------------------------------------------------------------------------
	//[GetAxisUser]
	//事前にRegisterAxisUserで登録された仮想Axis軸の取得を行う
	//取得する値は登録時のAxisPackに準ずる
	//return: 各取得値の中で最も絶対値が大きい値
	//throw: nameのエレメントが登録されていない場合
	//引数1: 登録時に使用したname (map->key)
	//引数2: 取得に使用するジョイスティックの番号 (0 ~ 4), 例外処理は行わない, default = 0
	inline float Input::Axis::GetAxisUser(const sgstring& name, uint useJoystick)
	{
		AtomicOperation::LockGuard guard(m_axisAtomic);
		
		//find
		auto it = m_axisPacks.find(name);
		
		//要素がある
		if (it != m_axisPacks.end())
		{
			float results[5] = { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f};

			//各要素に結果を格納する
			results[0] = GetAxisPackKeyStatesPuls(it->second.keyAndMouseCodeWithPlus);
			results[1] = GetAxisPackKeyStatesMinus(it->second.keyAndMouseCodeWithMinus);
			results[2] = GetAxisPackJoystickButtomsPuls(it->second.joystickCodeWithPlus, useJoystick);
			results[3] = GetAxisPackJoystickButtomsMinus(it->second.joystickCodeWithMinus, useJoystick);
			results[4] = GetAxisPackAxisCodes(it->second.axisCodes, useJoystick);

			if (MathAF::Absolute(results[0]) == MathAF::Absolute(results[1]))
				results[0] = results[1] = 0.0f;
			if (MathAF::Absolute(results[2]) == MathAF::Absolute(results[3]))
				results[2] = results[3] = 0.0f;

			//最大値を求める
			for (int i = 1; i < 5; i++)
				results[0] = MathAF::MaxWithAbsolute(results[0], results[i]);

			//返却
			return results[0];
		}
		//ない場合throw
		else
			throw InvalidArgument(L"Error!! Input->Axis->GetAxisUser", L"Name element not found");
	}
	//----------------------------------------------------------------------------------
	//[GetRawAxisUser]
	//事前にRegisterAxisUserで登録された仮想Axis軸の取得をdedZoneの影響を受けずに行う
	//取得する値は登録時のAxisPackに準ずる
	//AxisPack->axisCodesが設定されていない場合, 結果はGetAxisUserと同じになる
	//return: 各取得値の中で最も絶対値が大きい値
	//throw: nameのエレメントが登録されていない場合
	//引数1: 登録時に使用したname (map->key)
	//引数2: 取得に使用するジョイスティックの番号 (0 ~ 4), 例外処理は行わない, default = 0
	inline float Input::Axis::GetRawAxisUser(const sgstring&  name, uint useJoystick)
	{
		AtomicOperation::LockGuard guard(m_axisAtomic);
		
		//find
		auto it = m_axisPacks.find(name);
	
		//要素がある
		if (it != m_axisPacks.end())
		{
			float results[5] = { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f };

			//各要素に結果を格納する
			results[0] = GetAxisPackKeyStatesPuls(it->second.keyAndMouseCodeWithPlus);
			results[1] = GetAxisPackKeyStatesMinus(it->second.keyAndMouseCodeWithMinus);
			results[2] = GetAxisPackJoystickButtomsPuls(it->second.joystickCodeWithPlus, useJoystick);
			results[3] = GetAxisPackJoystickButtomsMinus(it->second.joystickCodeWithMinus, useJoystick);
			results[4] = GetRawAxisPackAxisCodes(it->second.axisCodes, useJoystick);

			if (MathAF::Absolute(results[0]) == MathAF::Absolute(results[1]))
				results[0] = results[1] = 0.0f;
			if (MathAF::Absolute(results[2]) == MathAF::Absolute(results[3]))
				results[2] = results[3] = 0.0f;

			//最大値を求める
			for (int i = 1; i < 5; i++)
				results[0] = MathAF::MaxWithAbsolute(results[0], results[i]);

			//返却
			return results[0];
		}
		//ない場合throw
		else
			throw InvalidArgument(L"Error!! Input->Axis->GetRawAxisUser", L"Name element not found");
	}
	//----------------------------------------------------------------------------------
	//[RegisterAxisUser]
	//仮想Axis軸の登録を行う, name (map->key)がまだ登録されていない時にのみ登録を行う
	//return: 登録が成功したらtrue, 失敗した場合はfalse
	//引数1: 使用時に引数として使われるname (map->key)
	//引数2: 設定するAxis軸
	inline bool Input::Axis::RegisterAxisUser(const sgstring& name, const AxisPack& axisPack)
	{
		AtomicOperation::LockGuard guard(m_axisAtomic);
		
		//find
		auto it = m_axisPacks.find(name);

		//ない場合登録する
		if (it == m_axisPacks.end())
		{
			m_axisPacks.try_emplace(name, axisPack);
			return true;
		}
		else
			return false;
	}

	//----------------------------------------------------------------------------------
	//[BaseGetAxisFloat]
	//GetAxisのStickありverで用いるBase関数
	//return: GetAxisの結果
	//引数1: axis1 (+)
	//引数2: axis1 (-)
	//引数3: axis2
	inline float Input::Axis::BaseGetAxisFloat(byte axis1Puls, byte axis1Minus, float axis2)
	{
		return MathAF::MaxWithAbsolute(axis2, 
			(axis1Puls) ?
				(axis1Minus) ?
					0.0f : 1.0f
			: (axis1Minus) ?
				-1.0f : 0.0f);
	}
	//----------------------------------------------------------------------------------
	//[BaseGetAxisByte]
	//GetAxisのStickなしverで用いるBase関数
	//return: GetAxisの結果
	//引数1: axis1 (+)
	//引数2: axis2 (-)
	inline float Input::Axis::BaseGetAxisByte(ushort axis1Puls, byte axis2Puls)
	{
		if (axis1Puls || axis2Puls) return 1.0f;
		else return 0.0f;
	}
	//----------------------------------------------------------------------------------
	//[GetAxisPackKeyStatesPuls]
	//GetAxisUserでkeyAndMouseCodeWithPlusのAxis軸取得に使用する
	//return: 全て取得した結果最も絶対値の大きい値 (ない場合0.0f)
	//引数1: keyAndMouseCodeWithPlus
	inline float Input::Axis::GetAxisPackKeyStatesPuls(const std::vector<int>& keyCodes)
	{
		for (const auto& e : keyCodes)
		{
			if (Keybord::GetKey(e))
				return 1.0f;
		}
		return 0.0f;
	}
	//----------------------------------------------------------------------------------
	//[GetAxisPackKeyStatesMinus]
	//GetAxisUserでkeyAndMouseCodeWithMinusのAxis軸取得に使用する
	//return: 全て取得した結果最も絶対値の大きい値 (ない場合0.0f)
	//引数1: keyAndMouseCodeWithMinus
	inline float Input::Axis::GetAxisPackKeyStatesMinus(const std::vector<int>& keyCodes)
	{
		for (const auto& e : keyCodes)
		{
			if (Keybord::GetKey(e))
				return -1.0f;
		}
		return 0.0f;
	}
	//----------------------------------------------------------------------------------
	//[GetAxisPackJoystickButtomsPuls]
	//GetAxisUserでjoystickCodeWithPlusのAxis軸取得に使用する
	//return: 全て取得した結果最も絶対値の大きい値 (ない場合0.0f)
	//引数1: joystickCodeWithPlus
	inline float Input::Axis::GetAxisPackJoystickButtomsPuls(const std::vector<JoystickCode::Enum>& joystickCodes, uint useStick)
	{
		for (const auto& e : joystickCodes)
		{
			if (Joystick::GetButton(e, useStick))
				return 1.0f;
		}
		return 0.0f;
	}
	//----------------------------------------------------------------------------------
	//[GetAxisPackJoystickButtomsMinus]
	//GetAxisUserでGetAxisPackJoystickButtomsMinusのAxis軸取得に使用する
	//return: 全て取得した結果最も絶対値の大きい値 (ない場合0.0f)
	//引数1: joystickCodeWithMinus
	inline float Input::Axis::GetAxisPackJoystickButtomsMinus(const std::vector<JoystickCode::Enum>& joystickCodes, uint useStick)
	{
		for (const auto& e : joystickCodes)
		{
			if (Joystick::GetButton(e, useStick))
				return -1.0f;
		}
		return 0.0f;
	}
	//----------------------------------------------------------------------------------
	//[GetAxisPackAxisCodes]
	//GetAxisUserでaxisCodesのAxis軸取得に使用する
	//return: 全て取得した結果最も絶対値の大きい値 (ない場合0.0f)
	//引数1: axisCodes
	//引数2: 使用するstick
	inline float Input::Axis::GetAxisPackAxisCodes(const std::vector<AxisCode::Enum>& axisCodes, uint useStick)
	{
		//0 = return, 1 = buf 
		float result[2] = { 0.0f,0.0f };

		for (const auto& e : axisCodes)
		{
			//Joystick
			if (e & AxisCode::m_cUpperBitJoystick)
			{
				//ビット演算で判別 & 取得
				if (e & AxisCode::m_cLowerBitJoystickLStickX)
					result[1] = Joystick::GetLStick(useStick).x;
				else if (e & AxisCode::m_cLowerBitJoystickLStickY)
					result[1] = Joystick::GetLStick(useStick).y;
				else if (e & AxisCode::m_cLowerBitJoystickRStickX)
					result[1] = Joystick::GetRStick(useStick).x;
				else if (e & AxisCode::m_cLowerBitJoystickRStickY)
					result[1] = Joystick::GetRStick(useStick).y;
				else if (e & AxisCode::m_cLowerBitJoystickLTrigger)
					result[1] = Joystick::GetLTrigger(useStick);
				else if (e & AxisCode::m_cLowerBitJoystickRTrigger)
					result[1] = Joystick::GetRTrigger(useStick);
				else
					result[1] = 0.0f;
			}
			//Mouse
			else if (e & AxisCode::m_cUpperBitMouse)
			{
				//ビット演算で判別 & 取得
				if (e & AxisCode::m_cLowerBitMouseAccelerationX)
					result[1] = Mouse::GetAcceleration().x;
				else if (e & AxisCode::m_cLowerBitMouseAccelerationY)
					result[1] = Mouse::GetAcceleration().y;
				else
					result[1] = 0.0f;
			}
			//Window
			else if (e & AxisCode::m_cUpperBitWindow)
			{
				//ビット演算で判別 & 取得
				if (e & AxisCode::m_cLowerBitWindowAccelerationX)
					result[1] = Window::GetAcceleration().x;
				else if (e & AxisCode::m_cLowerBitWindowAccelerationY)
					result[1] = Window::GetAcceleration().y;
				else
					result[1] = 0.0f;
			}

			//Max
			result[0] = MathAF::MaxWithAbsolute(result[0], result[1]);
		}

		//return
		return result[0];
	}
	//----------------------------------------------------------------------------------
	//[GetRawAxisPackAxisCodes]
	//GetAxisUserでaxisCodesのAxis軸取得に使用する (Raw ver)
	//return: 全て取得した結果最も絶対値の大きい値 (ない場合0.0f)
	//引数1: axisCodes
	//引数2: 使用するstick
	inline float Input::Axis::GetRawAxisPackAxisCodes(const std::vector<AxisCode::Enum>& axisCodes, uint useStick)
	{
		//0 = return, 1 = buf 
		float result[2] = { 0.0f,0.0f };
		
		for (const auto& e : axisCodes)
		{
			//Joystick
			if (e & AxisCode::m_cUpperBitJoystick)
			{
				//ビット演算で判別 & 取得
				if (e & AxisCode::m_cLowerBitJoystickLStickX)
					result[1] = Joystick::GetRawLStick(useStick).x;
				else if (e & AxisCode::m_cLowerBitJoystickLStickY)
					result[1] = Joystick::GetRawLStick(useStick).y;
				else if (e & AxisCode::m_cLowerBitJoystickRStickX)
					result[1] = Joystick::GetRawRStick(useStick).x;
				else if (e & AxisCode::m_cLowerBitJoystickRStickY)
					result[1] = Joystick::GetRawRStick(useStick).y;
				else if (e & AxisCode::m_cLowerBitJoystickLTrigger)
					result[1] = Joystick::GetRawLTrigger(useStick);
				else if (e & AxisCode::m_cLowerBitJoystickRTrigger)
					result[1] = Joystick::GetRawRTrigger(useStick);
				else
					result[1] = 0.0f;
			}
			//Mouse
			else if (e & AxisCode::m_cUpperBitMouse)
			{
				//ビット演算で判別 & 取得
				if (e & AxisCode::m_cLowerBitMouseAccelerationX)
					result[1] = Mouse::GetRawAcceleration().x;
				else if (e & AxisCode::m_cLowerBitMouseAccelerationY)
					result[1] = Mouse::GetRawAcceleration().y;
				else
					result[1] = 0.0f;
			}
			//Window
			else if (e & AxisCode::m_cUpperBitWindow)
			{
				//ビット演算で判別 & 取得
				if (e & AxisCode::m_cLowerBitWindowAccelerationX)
					result[1] = Window::GetRawAcceleration().x;
				else if (e & AxisCode::m_cLowerBitWindowAccelerationY)
					result[1] = Window::GetRawAcceleration().y;
				else
					result[1] = 0.0f;
			}

			//Max
			result[0] = MathAF::MaxWithAbsolute(result[0], result[1]);
		}
	
		//return
		return result[0];
	}



	//----------------------------------------------------------------------------------
	//AxisPack
	//----------------------------------------------------------------------------------

	//----------------------------------------------------------------------------------
	//[SetKeyAndMouseCodeWithPuls]
	//keyAndMouseCodeWithPlusに複数要素をまとめて登録する
	//引数1: 登録するパラメーターパック (KeyCode::〇〇, MosueCode::〇〇)
	template < typename... Args >
	inline void Input::AxisPack::SetKeyAndMouseCodeWithPuls(const Args&... args)
	{
		for (const auto& e : { args... })
			keyAndMouseCodeWithPlus.emplace_back(e);
	}
	//----------------------------------------------------------------------------------
	//[SetKeyAndMouseCodeWithMinus]
	//keyAndMouseCodeWithMinus複数要素をまとめて登録する
	//引数1: 登録するパラメーターパック (KeyCode::〇〇, MosueCode::〇〇)
	template < typename... Args >
	inline void Input::AxisPack::SetKeyAndMouseCodeWithMinus(const Args&... args)
	{
		for (const auto& e : { args... })
			keyAndMouseCodeWithMinus.emplace_back(e);
	}
	//----------------------------------------------------------------------------------
	//[SetJoystickCodeWithPuls]
	//joystickCodeWithPlus複数要素をまとめて登録する
	//引数1: 登録するパラメーターパック (JoystickCode::〇〇)
	template < typename... Args >
	inline void Input::AxisPack::SetJoystickCodeWithPuls(const Args&... args)
	{
		for (const auto& e : { args... })
			joystickCodeWithPlus.emplace_back(e);
	}
	//----------------------------------------------------------------------------------
	//[SetJoystickCodeWithMinus]
	//joystickCodeWithMinus複数要素をまとめて登録する
	//引数1: 登録するパラメーターパック (JoystickCode::〇〇)
	template < typename... Args >
	inline void Input::AxisPack::SetJoystickCodeWithMinus(const Args&... args)
	{
		for (const auto& e : { args... })
			joystickCodeWithMinus.emplace_back(e);
	}
	//----------------------------------------------------------------------------------
	//[SetAxisCodes]
	//axisCodes複数要素をまとめて登録する, 登録するのは各取得関数となる
	//引数1: 登録するパラメーターパック (AxisCode::〇〇)
	template < typename... Args >
	inline void Input::AxisPack::SetAxisCodes(const Args&... args)
	{
		for (const auto& e : { args... })
			axisCodes.emplace_back(e);
	}
}
#endif // !SGFRAMEWORK_HEADER_INPUT_HPP_