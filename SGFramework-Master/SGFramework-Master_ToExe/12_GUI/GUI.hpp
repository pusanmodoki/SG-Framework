/*----------------------------------------------------------------------------------
GUIを管理するGUI class
------------------------------------------------------------------------------------*/
#ifndef SGFRAMEWORK_HEADER_GUI_HPP_
#define SGFRAMEWORK_HEADER_GUI_HPP_
#include <Windows.h>
#include <iostream>
#include <thread>
#include "imgui.h"
#include "../02_CustomVariable/CustomVariable.hpp"
#include "../05_Graphics/Graphics.hpp"
#include "../06_Audio/Audio.hpp"
#include "../09_Scene/SceneManager.hpp"
#include "../09_Scene/Scene.hpp"

// Framework namespace
namespace SGFramework
{
	class Application;
	class Thread;
	class Input;

	//GUIを管理するGUI class
	class GUI final
	{
	private:
		//Log headerのサイズ
		static constexpr size_t m_cLogIdentificationSize = 23;

	public:
		friend class Application;
		friend class Thread;
		friend class Input;
		friend Graphics;
		friend int WINAPI ::WinMain(_In_ HINSTANCE hInstance, 
			_In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow);
	
		DELETE_ALL_DEFAULT_CLASS_FUNCTIONS(GUI);

		//debug log stream<static> (reference property)
		//(framework管轄外から呼ぶと全て同一のストリームを返却するので排他処理に注意)
		SGF_FUNCTION_PROPERTY static std::wstringstream& getLogStream();

		//Log->Error message header (GUI::getLogStream() << this << message)
		static constexpr wchar cLogErrorHeader[m_cLogIdentificationSize] = { L"E#}SGFIdentification{#" };
		//Log->Worning message header (GUI::getLogStream() << this << message)
		static constexpr wchar cLogWorningHeader[m_cLogIdentificationSize] = { L"W#}SGFIdentification{#" };
		//Log->Info message header (GUI::getLogStream() << this << message)
		static constexpr wchar cLogInfoHeader[m_cLogIdentificationSize] = { L"I#}SGFIdentification{#" };
		//Log->Framework message header (GUI::getLogStream() << this << message)
		static constexpr wchar cLogFrameworkMessageHeader[m_cLogIdentificationSize] = { L"F#}SGFIdentification{#" };

	private:
		//message header->Error
		static constexpr char m_cLogErrorString[10] = { "@Error!!" };
		//message header->Worning
		static constexpr char m_cLogWorningString[11] = { "@Worning!!" };
		//message header->Info
		static constexpr char m_cLogInfoString[6] = { "@Info" };
		//message header->Framework
		static constexpr char m_cLogFrameworkString[11] = { "@Framework" };
		//log header identification
		static constexpr wchar m_cLogHerderIdentification[m_cLogIdentificationSize - 1] = { L"#}SGFIdentification{#" };
		//log identification header->Error
		static constexpr wchar m_cLogHerderError = { L'E' };
		//log identification header->Worning
		static constexpr wchar m_cLogHerderWorning = { L'W' };
		//log identification header->Info
		static constexpr wchar m_cLogHerderInfo = { 'I' };
		//log identification header->Framework Message
		static constexpr wchar m_cLogHerderFramework = { L'F' };
		//各DebugStreamのreserve size
		static constexpr size_t m_cDebugDrawReserveSize = 300;
		//Sync関数で使用するバッファー長
		static constexpr size_t m_cBufferLength = 2000;
		//message color->Error
		static const ImColor m_cErrorColor;
		//message color->Worning
		static const ImColor m_cWorningColor;
		//message color->Info
		static const ImColor m_cInfoColor;
		//message color->FrameworkMessage
		static const ImColor m_cFrameworkMessageColor;

		//Debug用Streamを所持するDebugStream structure
		struct DebugStream
		{
		public:
			//----------------------------------------------------------------------------------
			//[Constructor]
			DebugStream(const std::string& threadName) : threadName(threadName), stream() { }
			
			std::string threadName;		//スレッド名 (メッセージに使用)
			std::wstringstream stream;	//ストリーム
		};
		//ConsoleWindowに描画するMessageを格納するConsoleMessage structure
		struct ConsoleMessage
		{
		public:
			//----------------------------------------------------------------------------------
			//[Constructor]
			//create message
			inline ConsoleMessage(const std::string& header, const sgstring& message);
			//----------------------------------------------------------------------------------
			//[Draw]
			//draw message
			inline void Draw();
			
			std::string message;	//message string
			ImVec4 color;			//message color
			bool isUsingColor;		//using color?
		};

		//----------------------------------------------------------------------------------
		//[StratUp]
		//初期化する
		//引数1: HWND
		//引数2: Load vertex shader path(Debug)
		//引数3: Load pixel shader path(Debug)
		//引数4: Load vertex shader path(Release)
		//引数5: Load pixel shader path(Release)
		static void StratUp(HWND hWnd, const ReadElement::Pack& vertexShaderDebugPath,
			const ReadElement::Pack& pixelShaderDebugPath, const ReadElement::Pack& vertexShaderReleasePath,
			const ReadElement::Pack& pixelShaderReleasePath, const ReadElement::Pack& isUsing,
			const ReadElement::Pack& fontSizePerDpi);
		//----------------------------------------------------------------------------------
		//[ShutDown]
		//終了処理
		static void ShutDown();

		//----------------------------------------------------------------------------------
		//[UpdateWindowProcedure]
		//更新を行う WindowProcedure ver
		static void UpdateWindowProcedure();
		//----------------------------------------------------------------------------------
		//[ShutdownWindowProcedure]
		//終了処理 (Window Procedure)
		static void ShutdownWindowProcedure();
		
		//----------------------------------------------------------------------------------
		//[Sync]
		//同期を行う
		static void Sync();
		
		//----------------------------------------------------------------------------------
		//[Update]
		//更新を行う (Graphics & Audio Thread)
		static void Update();
		//----------------------------------------------------------------------------------
		//[UpdateMousePosition]
		//マウスポジションの更新を行う
		static void UpdateMousePosition();
		//----------------------------------------------------------------------------------
		//[UpdateMouseCousor]
		//マウスカーソルの更新を行う
		static bool UpdateMouseCousor();
		//----------------------------------------------------------------------------------
		//[UpdateJoystick]
		//Joystickの更新を行う
		static void UpdateJoystick();

		//----------------------------------------------------------------------------------
		//[DrawGUI]
		//描画を行う
		static void DrawGUI();


		//Debug Streams


		//----------------------------------------------------------------------------------
		//[RegisterDebugStream]
		//スレッドへDebugStreamを登録
		//引数1: thread name
		//引数2: this_thread::get_id() or thread.get_id()
		inline static void RegisterDebugStream(const sgstring& threadName, std::thread::id key);
		//----------------------------------------------------------------------------------
		//[BuildDebugStream]
		//DebugStreamを登録情報を基に構築する
		static void BuildDebugStream();
		//----------------------------------------------------------------------------------
		//[SyncDebugStream]
		//DebugStreamからConsoleMessageへ変換し保存する
		static void SyncDebugStream();
		//----------------------------------------------------------------------------------
		//[DrawDebugStream]
		//DebugStream(ConsoleMessage)を描画する
		static void DrawDebugStream();

		//----------------------------------------------------------------------------------
		//[DrawApplicationInfomations]
		//DebugStream->Draw Application Infomations 
		static void DrawApplicationInfomations();
		//----------------------------------------------------------------------------------
		//[DrawSceneInfomations]
		//DebugStream->Draw Scene Infomations
		static void DrawSceneInfomations();
		//----------------------------------------------------------------------------------
		//[DrawDebugConsole]
		//DebugStream->Draw Debug Console Window
		static void DrawDebugConsole();

		//----------------------------------------------------------------------------------
		//[ShowTimeLayers]
		//DebugStream->Draw Time Layers
		//引数1: draw layer
		static void ShowTimeLayers(const WeakPointer<Time::TimeLayer>& layer);
		//----------------------------------------------------------------------------------
		//[ShowAudioLayers]
		//DebugStream->Draw Audio Submix Layers
		//引数1: draw layer
		static void ShowAudioLayers(const WeakPointer<Audio::SubmixLayer>& layer);
		//----------------------------------------------------------------------------------
		//[ShowScene]
		//DebugStream->Draw Sub Scenes
		//引数1: draw scene
		static void ShowScene(const UniquePointer<Scene::BaseScene>& scene);
		//----------------------------------------------------------------------------------
		//[ShowTransform]
		//DebugStream->Draw Transforms
		//引数1: draw transform
		static void ShowTransform(Transform* transform);

		
		//X11 Functions
		
		
		//----------------------------------------------------------------------------------
		//[StratUpGraphicsX11]
		//DirectX11を使用する初期化を行う
		//引数1: vertex shader path
		//引数1: pixel shader path
		static void StratUpGraphicsX11(const sgstring& vertexShaderName,
			const sgstring& pixelShaderName);
		//----------------------------------------------------------------------------------
		//[ShutDownGraphicsX11]
		//DirectX11を使用する終了処理を行う
		static void ShutDownGraphicsX11();
		//----------------------------------------------------------------------------------
		//[DrawGraphicsX11]
		//ImGui::Render後の描画処理を行う
		static void DrawGraphicsX11();
		//----------------------------------------------------------------------------------
		//[SetRendererStatesX11]
		//DrawGraphicsX11->Set Renderer States
		static void SetRendererStatesX11(ImDrawData* drawData);


		//WindowProcedure用条件変数
		static ConditionVariable::Auto m_copyCompletedCondition;
		//Log用DebugStream
		static std::unordered_map<std::thread::id, DebugStream> m_debugStreams;
		//Log用ConsoleMessage
		static std::vector<GUI::ConsoleMessage> m_drawConsoleMessages;
		//Vertex shader
		static SharedPointer<Asset::VertexShaderAsset> m_vertexShader;
		//Pixel shader
		static SharedPointer<Asset::PixelShaderAsset> m_pixelShader;
		//WindowProcedure用入力バッファ
		static std::vector<uint> m_inputCharacters;
		//マウスカーソル変数保存バッファ
		static ImGuiMouseCursor	 m_lastMouseCursor;
		//WindowProcedure用ホイールバッファ
		static float m_inputMouseWheel;
		//true->描画
		static std::atomic_bool m_isDrawReady;
		//true->バッファクリア
		static std::atomic_bool m_isClearDebugStreams;
		//true->GUI有効
		static std::atomic_bool m_isUsing;
		//WindowProcedure用マウスボタンバッファ
		static bool m_inputMouseDowns[5];
		//WindowProcedure用キーバッファ
		static bool m_inputKeyDowns[512];
	};

	//debug log stream<static> (reference property)
	//(framework管轄外から呼ぶと全て同一のストリームを返却するので排他処理に注意)
	SGF_FUNCTION_PROPERTY std::wstringstream& GUI::getLogStream()
	{
		auto find = m_debugStreams.find(std::this_thread::get_id());

		if (find != m_debugStreams.end()) return find->second.stream;
		else return m_debugStreams.at(std::thread::id()).stream;
	}
	//----------------------------------------------------------------------------------
	//[RegisterDebugStream]
	//スレッドへDebugStreamを登録
	//引数1: thread name
	//引数2: this_thread::get_id() or thread.get_id()
	inline void GUI::RegisterDebugStream(const sgstring& threadName, std::thread::id key)
	{
		static std::atomic_bool lock = false;

		AtomicOperation::LockGuard guard(lock);
		m_debugStreams.try_emplace(key, DebugStream(threadName.to_std_string()));
	}

	//----------------------------------------------------------------------------------
	//[Constructor]
	//create message
	inline GUI::ConsoleMessage::ConsoleMessage(const std::string& header, const sgstring& message) : message()
	{
		//Headerありメッセージ->Color & Headerをmessageと合成しconsole message作成
		if (sgstring::functions::IsContains(message, m_cLogHerderIdentification))
		{
			//Error
			if (message[0] == m_cLogHerderError)
			{
				this->message = m_cLogErrorString + header + message.to_std_string().substr(m_cLogIdentificationSize - 1);
				color = m_cErrorColor;
				isUsingColor = true;
			}
			//Worning
			else if (message[0] == m_cLogHerderWorning)
			{
				this->message = m_cLogWorningString + header + message.to_std_string().substr(m_cLogIdentificationSize - 1);
				color = m_cWorningColor;
				isUsingColor = true;
			}
			//Info
			else if (message[0] == m_cLogHerderInfo)
			{
				this->message = m_cLogInfoString + header + message.to_std_string().substr(m_cLogIdentificationSize - 1);
				color = m_cInfoColor;
				isUsingColor = true;
			}
			//Framework
			else if (message[0] == m_cLogHerderFramework)
			{
				this->message = m_cLogFrameworkString + header + message.to_std_string().substr(m_cLogIdentificationSize - 1);
				color = m_cFrameworkMessageColor;
				isUsingColor = true;
			}
		}
		//Headerなしメッセージ->messageのみでconsole message作成
		else
		{
			this->message = header + message.to_std_string();
			isUsingColor = false;
		}
	}
	//----------------------------------------------------------------------------------
	//[Draw]
	//draw message
	inline void GUI::ConsoleMessage::Draw()
	{
		//Draw Text
		if (IS_FALSE(isUsingColor)) ImGui::Text("%s", message.c_str());
		else ImGui::TextColored(color, "%s", message.c_str());
		
		//Separat
		ImGui::Separator();
	}
}
#endif //!SGFRAMEWORK_HEADER_GUI_HPP_