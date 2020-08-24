/*----------------------------------------------------------------------------------
GUI���Ǘ�����GUI class
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

	//GUI���Ǘ�����GUI class
	class GUI final
	{
	private:
		//Log header�̃T�C�Y
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
		//(framework�Ǌ��O����ĂԂƑS�ē���̃X�g���[����ԋp����̂Ŕr�������ɒ���)
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
		//�eDebugStream��reserve size
		static constexpr size_t m_cDebugDrawReserveSize = 300;
		//Sync�֐��Ŏg�p����o�b�t�@�[��
		static constexpr size_t m_cBufferLength = 2000;
		//message color->Error
		static const ImColor m_cErrorColor;
		//message color->Worning
		static const ImColor m_cWorningColor;
		//message color->Info
		static const ImColor m_cInfoColor;
		//message color->FrameworkMessage
		static const ImColor m_cFrameworkMessageColor;

		//Debug�pStream����������DebugStream structure
		struct DebugStream
		{
		public:
			//----------------------------------------------------------------------------------
			//[Constructor]
			DebugStream(const std::string& threadName) : threadName(threadName), stream() { }
			
			std::string threadName;		//�X���b�h�� (���b�Z�[�W�Ɏg�p)
			std::wstringstream stream;	//�X�g���[��
		};
		//ConsoleWindow�ɕ`�悷��Message���i�[����ConsoleMessage structure
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
		//����������
		//����1: HWND
		//����2: Load vertex shader path(Debug)
		//����3: Load pixel shader path(Debug)
		//����4: Load vertex shader path(Release)
		//����5: Load pixel shader path(Release)
		static void StratUp(HWND hWnd, const ReadElement::Pack& vertexShaderDebugPath,
			const ReadElement::Pack& pixelShaderDebugPath, const ReadElement::Pack& vertexShaderReleasePath,
			const ReadElement::Pack& pixelShaderReleasePath, const ReadElement::Pack& isUsing,
			const ReadElement::Pack& fontSizePerDpi);
		//----------------------------------------------------------------------------------
		//[ShutDown]
		//�I������
		static void ShutDown();

		//----------------------------------------------------------------------------------
		//[UpdateWindowProcedure]
		//�X�V���s�� WindowProcedure ver
		static void UpdateWindowProcedure();
		//----------------------------------------------------------------------------------
		//[ShutdownWindowProcedure]
		//�I������ (Window Procedure)
		static void ShutdownWindowProcedure();
		
		//----------------------------------------------------------------------------------
		//[Sync]
		//�������s��
		static void Sync();
		
		//----------------------------------------------------------------------------------
		//[Update]
		//�X�V���s�� (Graphics & Audio Thread)
		static void Update();
		//----------------------------------------------------------------------------------
		//[UpdateMousePosition]
		//�}�E�X�|�W�V�����̍X�V���s��
		static void UpdateMousePosition();
		//----------------------------------------------------------------------------------
		//[UpdateMouseCousor]
		//�}�E�X�J�[�\���̍X�V���s��
		static bool UpdateMouseCousor();
		//----------------------------------------------------------------------------------
		//[UpdateJoystick]
		//Joystick�̍X�V���s��
		static void UpdateJoystick();

		//----------------------------------------------------------------------------------
		//[DrawGUI]
		//�`����s��
		static void DrawGUI();


		//Debug Streams


		//----------------------------------------------------------------------------------
		//[RegisterDebugStream]
		//�X���b�h��DebugStream��o�^
		//����1: thread name
		//����2: this_thread::get_id() or thread.get_id()
		inline static void RegisterDebugStream(const sgstring& threadName, std::thread::id key);
		//----------------------------------------------------------------------------------
		//[BuildDebugStream]
		//DebugStream��o�^������ɍ\�z����
		static void BuildDebugStream();
		//----------------------------------------------------------------------------------
		//[SyncDebugStream]
		//DebugStream����ConsoleMessage�֕ϊ����ۑ�����
		static void SyncDebugStream();
		//----------------------------------------------------------------------------------
		//[DrawDebugStream]
		//DebugStream(ConsoleMessage)��`�悷��
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
		//����1: draw layer
		static void ShowTimeLayers(const WeakPointer<Time::TimeLayer>& layer);
		//----------------------------------------------------------------------------------
		//[ShowAudioLayers]
		//DebugStream->Draw Audio Submix Layers
		//����1: draw layer
		static void ShowAudioLayers(const WeakPointer<Audio::SubmixLayer>& layer);
		//----------------------------------------------------------------------------------
		//[ShowScene]
		//DebugStream->Draw Sub Scenes
		//����1: draw scene
		static void ShowScene(const UniquePointer<Scene::BaseScene>& scene);
		//----------------------------------------------------------------------------------
		//[ShowTransform]
		//DebugStream->Draw Transforms
		//����1: draw transform
		static void ShowTransform(Transform* transform);

		
		//X11 Functions
		
		
		//----------------------------------------------------------------------------------
		//[StratUpGraphicsX11]
		//DirectX11���g�p���鏉�������s��
		//����1: vertex shader path
		//����1: pixel shader path
		static void StratUpGraphicsX11(const sgstring& vertexShaderName,
			const sgstring& pixelShaderName);
		//----------------------------------------------------------------------------------
		//[ShutDownGraphicsX11]
		//DirectX11���g�p����I���������s��
		static void ShutDownGraphicsX11();
		//----------------------------------------------------------------------------------
		//[DrawGraphicsX11]
		//ImGui::Render��̕`�揈�����s��
		static void DrawGraphicsX11();
		//----------------------------------------------------------------------------------
		//[SetRendererStatesX11]
		//DrawGraphicsX11->Set Renderer States
		static void SetRendererStatesX11(ImDrawData* drawData);


		//WindowProcedure�p�����ϐ�
		static ConditionVariable::Auto m_copyCompletedCondition;
		//Log�pDebugStream
		static std::unordered_map<std::thread::id, DebugStream> m_debugStreams;
		//Log�pConsoleMessage
		static std::vector<GUI::ConsoleMessage> m_drawConsoleMessages;
		//Vertex shader
		static SharedPointer<Asset::VertexShaderAsset> m_vertexShader;
		//Pixel shader
		static SharedPointer<Asset::PixelShaderAsset> m_pixelShader;
		//WindowProcedure�p���̓o�b�t�@
		static std::vector<uint> m_inputCharacters;
		//�}�E�X�J�[�\���ϐ��ۑ��o�b�t�@
		static ImGuiMouseCursor	 m_lastMouseCursor;
		//WindowProcedure�p�z�C�[���o�b�t�@
		static float m_inputMouseWheel;
		//true->�`��
		static std::atomic_bool m_isDrawReady;
		//true->�o�b�t�@�N���A
		static std::atomic_bool m_isClearDebugStreams;
		//true->GUI�L��
		static std::atomic_bool m_isUsing;
		//WindowProcedure�p�}�E�X�{�^���o�b�t�@
		static bool m_inputMouseDowns[5];
		//WindowProcedure�p�L�[�o�b�t�@
		static bool m_inputKeyDowns[512];
	};

	//debug log stream<static> (reference property)
	//(framework�Ǌ��O����ĂԂƑS�ē���̃X�g���[����ԋp����̂Ŕr�������ɒ���)
	SGF_FUNCTION_PROPERTY std::wstringstream& GUI::getLogStream()
	{
		auto find = m_debugStreams.find(std::this_thread::get_id());

		if (find != m_debugStreams.end()) return find->second.stream;
		else return m_debugStreams.at(std::thread::id()).stream;
	}
	//----------------------------------------------------------------------------------
	//[RegisterDebugStream]
	//�X���b�h��DebugStream��o�^
	//����1: thread name
	//����2: this_thread::get_id() or thread.get_id()
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
		//Header���胁�b�Z�[�W->Color & Header��message�ƍ�����console message�쐬
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
		//Header�Ȃ����b�Z�[�W->message�݂̂�console message�쐬
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