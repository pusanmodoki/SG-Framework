/*----------------------------------------------------------------------------------
GUIを管理するGUI class
------------------------------------------------------------------------------------*/
#include "GUI.hpp"
#include "../08_Input/Input.hpp"

// Framework namespace
namespace SGFramework
{
	//message color->Error
	const ImColor GUI::m_cErrorColor = { 0.9f, 0.1f, 0.1f, 1.0f };
	//message color->Worning
	const ImColor GUI::m_cWorningColor = { 0.9f, 0.9f, 0.1f, 1.0f };
	//message color->Info
	const ImColor GUI::m_cInfoColor = { 0.7f, 0.7f, 0.7f, 1.0f };
	//message color->FrameworkMessage
	const ImColor GUI::m_cFrameworkMessageColor = { 0.1f, 0.9f, 0.9f, 1.0f };

	//WindowProcedure用条件変数
	ConditionVariable::Auto GUI::m_copyCompletedCondition;
	//Log用DebugStream
	std::unordered_map<std::thread::id, GUI::DebugStream> GUI::m_debugStreams;
	//Log用ConsoleMessage
	std::vector<GUI::ConsoleMessage> GUI::m_drawConsoleMessages;
	//Vertex shader
	SharedPointer<Asset::VertexShaderAsset> GUI::m_vertexShader;
	//Pixel shader
	SharedPointer<Asset::PixelShaderAsset> GUI::m_pixelShader;
	//WindowProcedure用入力バッファ
	std::vector<uint> GUI::m_inputCharacters;
	//マウスカーソル変数保存バッファ
	ImGuiMouseCursor GUI::m_lastMouseCursor = ImGuiMouseCursor_COUNT;
	//WindowProcedure用ホイールバッファ
	float GUI::m_inputMouseWheel = 0.0f;
	//true->描画
	std::atomic_bool GUI::m_isDrawReady = false;
	//true->バッファクリア
	std::atomic_bool GUI::m_isClearDebugStreams = false;
	//true->GUI有効
	std::atomic_bool GUI::m_isUsing = false;
	//WindowProcedure用マウスボタンバッファ
	bool GUI::m_inputMouseDowns[5] = {};
	//WindowProcedure用キーバッファ
	bool GUI::m_inputKeyDowns[512] = {};


	//----------------------------------------------------------------------------------
	//[StratUp]
	//初期化する
	//引数1: HWND
	//引数2: Load vertex shader path(Debug)
	//引数3: Load pixel shader path(Debug)
	//引数4: Load vertex shader path(Release)
	//引数5: Load pixel shader path(Release)
	void GUI::StratUp(HWND hWnd, const ReadElement::Pack& vertexShaderDebugPath,
		const ReadElement::Pack& pixelShaderDebugPath, const ReadElement::Pack& vertexShaderReleasePath,
		const ReadElement::Pack& pixelShaderReleasePath, const ReadElement::Pack& isUsing,
		const ReadElement::Pack& fontSizePerDpi)
	{
		//初期化
		m_isUsing = isUsing[0].valueInt != 0 ? true : false;

		//Not Using->終了
		if (IS_FALSE(m_isUsing.load())) return;

		//DebugStream登録(Unknown)
		RegisterDebugStream(L"UnknownThread of Framework", std::thread::id());

		//以下[End Pointe 1]までimgui-master->sumpleのコピー

		IMGUI_CHECKVERSION();

		ImGui::CreateContext();

		ImGui::StyleColorsDark();

		ImGuiIO& io = ImGui::GetIO();
		io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;         // We can honor GetMouseCursor() values (optional)
		io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;          // We can honor io.WantSetMousePos requests (optional, rarely used)
		io.BackendPlatformName = "imgui_impl_win32";
		io.ImeWindowHandle = hWnd;

		// Keyboard mapping. ImGui will use those indices to peek into the io.KeysDown[] array that we will update during the application lifetime.
		io.KeyMap[ImGuiKey_Tab] = VK_TAB;
		io.KeyMap[ImGuiKey_LeftArrow] = VK_LEFT;
		io.KeyMap[ImGuiKey_RightArrow] = VK_RIGHT;
		io.KeyMap[ImGuiKey_UpArrow] = VK_UP;
		io.KeyMap[ImGuiKey_DownArrow] = VK_DOWN;
		io.KeyMap[ImGuiKey_PageUp] = VK_PRIOR;
		io.KeyMap[ImGuiKey_PageDown] = VK_NEXT;
		io.KeyMap[ImGuiKey_Home] = VK_HOME;
		io.KeyMap[ImGuiKey_End] = VK_END;
		io.KeyMap[ImGuiKey_Insert] = VK_INSERT;
		io.KeyMap[ImGuiKey_Delete] = VK_DELETE;
		io.KeyMap[ImGuiKey_Backspace] = VK_BACK;
		io.KeyMap[ImGuiKey_Space] = VK_SPACE;
		io.KeyMap[ImGuiKey_Enter] = VK_RETURN;
		io.KeyMap[ImGuiKey_Escape] = VK_ESCAPE;
		io.KeyMap[ImGuiKey_KeyPadEnter] = VK_RETURN;
		io.KeyMap[ImGuiKey_A] = 'A';
		io.KeyMap[ImGuiKey_C] = 'C';
		io.KeyMap[ImGuiKey_V] = 'V';
		io.KeyMap[ImGuiKey_X] = 'X';
		io.KeyMap[ImGuiKey_Y] = 'Y';
		io.KeyMap[ImGuiKey_Z] = 'Z';

		//[End Pointe 1]

		//フォントサイズの計算 (scale * (dpi.x + dpi.y))
		Vector2 toFloat = Graphics::screen.dpi->ToFloat();
		float fontSize = fontSizePerDpi[0].valueFloat * ((toFloat.x + toFloat.y) * 0.5f);
		//メイリオ設定
		io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\meiryo.ttc", fontSize, nullptr, io.Fonts->GetGlyphRangesJapanese());

		//StratUp Graphics (Debug or Releaseでshader pathが変わるので分岐)
		try
		{
#if defined(DEBUG) || defined(_DEBUG)
			StratUpGraphicsX11(vertexShaderDebugPath[0].valueString, pixelShaderDebugPath[0].valueString);
#else
			StratUpGraphicsX11(vertexShaderReleasePath[0].valueString, pixelShaderReleasePath[0].valueString);
#endif	
		}
		catch (...)
		{
			throw;
		}
	}
	//----------------------------------------------------------------------------------
	//[ShutDown]
	//終了処理
	void GUI::ShutDown()
	{	
		//Not Using->終了
		if (IS_FALSE(m_isUsing.load())) return;

		//Updateに入らなくさせるため初期化
		AtomicOperation::Init(m_isDrawReady, false);
		AtomicOperation::Init(m_isUsing, false);
		//スレッドロック防止のためこちらからもNotifyOne (GUIのみロックが多発したので応急処置)
		m_copyCompletedCondition.NotifyOne();

		//ここですぐコンテキストを消すと参照エラーを起こす可能性があるので少し待つ
		std::this_thread::sleep_for(std::chrono::milliseconds(500));

		//release shader
		m_vertexShader.WaiverAndReset();
		m_pixelShader.WaiverAndReset();

		//ShutDown Graphics 
		ShutDownGraphicsX11();

		//コンテキスト削除
		ImGui::DestroyContext();
	}

	//----------------------------------------------------------------------------------
	//[UpdateWindowProcedure]
	//更新を行う WindowProcedure ver
	void GUI::UpdateWindowProcedure()
	{
		ImGuiIO& io = ImGui::GetIO();

		//バッファからImGuiへコピー
		io.MouseWheel = m_inputMouseWheel;
		m_inputMouseWheel = 0.0f;
		memcpy_s(io.MouseDown, 5, m_inputMouseDowns, 5);
		memcpy_s(io.KeysDown, 512, m_inputKeyDowns, 512);
		for (auto& e : m_inputCharacters)
			io.AddInputCharacter(e);
		m_inputCharacters.clear();

		//終了イベント
		m_copyCompletedCondition.NotifyOne();
	}
	//----------------------------------------------------------------------------------
	//[ShutdownWindowProcedure]
	//終了処理 (Window Procedure)
	void GUI::ShutdownWindowProcedure()
	{
		//終了イベント, メインスレッド停止防止
		m_copyCompletedCondition.NotifyOne();
	}

	//----------------------------------------------------------------------------------
	//[Sync]
	//同期を行う
	void GUI::Sync()
	{
		SyncDebugStream();
	}

	//----------------------------------------------------------------------------------
	//[Update]
	//更新を行う (Graphics & Audio Thread)
	void GUI::Update()
	{
		//Not Using->終了
		if (IS_FALSE(m_isUsing)) return;
		
		//以下リネームや若干の修正があるものの基本的にはimgui-master->sumpleのコピー

		ImGuiIO& io = ImGui::GetIO();
		IM_ASSERT(io.Fonts->IsBuilt() && "Font atlas not built! It is generally built by the renderer back-end. Missing call to renderer _NewFrame() function? e.g. ImGui_ImplOpenGL3_NewFrame().");

		//更新メッセージ送信
		SendNotifyMessage(SGFramework::Window::HwndHolder::main, WM_SGFRAMEWORK_UPDATE_GUI, 0, 0);

		// Setup display size (every frame to accommodate for window resizing)
		RECT rect;
		::GetClientRect(Window::HwndHolder::main, &rect);
		io.DisplaySize = ImVec2((float)(rect.right - rect.left), (float)(rect.bottom - rect.top));

		// Setup time step
		io.DeltaTime = Time::unScaledWorldDeltaTime;

		// Read keyboard modifiers inputs
		io.KeyCtrl = (::GetKeyState(VK_CONTROL) & 0x8000) != 0;
		io.KeyShift = (::GetKeyState(VK_SHIFT) & 0x8000) != 0;
		io.KeyAlt = (::GetKeyState(VK_MENU) & 0x8000) != 0;
		io.KeySuper = false;
		// io.KeysDown[], io.MousePos, io.MouseDown[], io.MouseWheel: filled by the WndProc handler below.

		//Update OS mouse position
		UpdateMousePosition();

		//現在は使用していない↓

		// Update OS mouse cursor with the cursor requested by imgui
		//ImGuiMouseCursor mouse_cursor = io.MouseDrawCursor ? ImGuiMouseCursor_None : ImGui::GetMouseCursor();
		//if (m_lastMouseCursor != mouse_cursor)
		//{
		//	m_lastMouseCursor = mouse_cursor;
		//	UpdateMouseCousor();
		//}
		
		//Update OS joystick
		UpdateJoystick();

		//WindowProcedureの更新終了を待機
		m_copyCompletedCondition.Wait();

		//gui new frame
		ImGui::NewFrame();

		//draw ready!!
		AtomicOperation::Init(m_isDrawReady, true);
	}
	//----------------------------------------------------------------------------------
	//[UpdateMousePosition]
	//マウスポジションの更新を行う
	void GUI::UpdateMousePosition()
	{
		//以下リネームや若干の修正があるものの基本的にはimgui-master->sumpleのコピー
		ImGuiIO& io = ImGui::GetIO();

		// Set OS mouse position if requested (rarely used, only when ImGuiConfigFlags_NavEnableSetMousePos is enabled by user)
		if (io.WantSetMousePos)
		{
			POINT pos = { static_cast<int>(io.MousePos.x), static_cast<int>(io.MousePos.y) };
			::ClientToScreen(Window::HwndHolder::main, &pos);
			::SetCursorPos(pos.x, pos.y);
		}

		// Set mouse position
		io.MousePos = ImVec2(-FLT_MAX, -FLT_MAX);
		POINT pos;
		if (HWND active_window = ::GetForegroundWindow())
			if (active_window == Window::HwndHolder::main || ::IsChild(active_window, Window::HwndHolder::main))
				if (::GetCursorPos(&pos) && ::ScreenToClient(Window::HwndHolder::main, &pos))
					io.MousePos = ImVec2(static_cast<float>(pos.x), static_cast<float>(pos.y));
	}
	//----------------------------------------------------------------------------------
	//[UpdateMouseCousor]
	//マウスカーソルの更新を行う
	bool GUI::UpdateMouseCousor()
	{
		//以下リネームや若干の修正があるものの基本的にはimgui-master->sumpleのコピー
		ImGuiIO& io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_NoMouseCursorChange)
			return false;

		ImGuiMouseCursor imgui_cursor = ImGui::GetMouseCursor();
		if (imgui_cursor == ImGuiMouseCursor_None || io.MouseDrawCursor)
		{
			// Hide OS mouse cursor if imgui is drawing it or if it wants no cursor
			// ::SetCursor(NULL);
		}
		else
		{
			// Show OS mouse cursor
			LPTSTR win32_cursor = IDC_ARROW;
			switch (imgui_cursor)
			{
			case ImGuiMouseCursor_Arrow:        win32_cursor = IDC_ARROW; break;
			case ImGuiMouseCursor_TextInput:    win32_cursor = IDC_IBEAM; break;
			case ImGuiMouseCursor_ResizeAll:    win32_cursor = IDC_SIZEALL; break;
			case ImGuiMouseCursor_ResizeEW:     win32_cursor = IDC_SIZEWE; break;
			case ImGuiMouseCursor_ResizeNS:     win32_cursor = IDC_SIZENS; break;
			case ImGuiMouseCursor_ResizeNESW:   win32_cursor = IDC_SIZENESW; break;
			case ImGuiMouseCursor_ResizeNWSE:   win32_cursor = IDC_SIZENWSE; break;
			case ImGuiMouseCursor_Hand:         win32_cursor = IDC_HAND; break;
			case ImGuiMouseCursor_NotAllowed:   win32_cursor = IDC_NO; break;
			}
			//::SetCursor(::LoadCursor(NULL, win32_cursor));
		}
		return true;
	}
	//----------------------------------------------------------------------------------
	//[UpdateJoystick]
	//Joystickの更新を行う
	void GUI::UpdateJoystick()
	{
		//以下Inputに合わせて修正があるものの基本的にはimgui-master->sumpleのコピー

#ifndef IMGUI_IMPL_WIN32_DISABLE_GAMEPAD
		if (IS_FALSE(Input::Joystick::isUseJoystick())) return;
		
		ImGuiIO& io = ImGui::GetIO();
		ZeroMemory(io.NavInputs, sizeof(io.NavInputs));
		if ((io.ConfigFlags & ImGuiConfigFlags_NavEnableGamepad) == 0)
			return;

		io.BackendFlags &= ~ImGuiBackendFlags_HasGamepad;
		if (Input::Joystick::numUseJoysticks > 0)
		{
			io.BackendFlags |= ImGuiBackendFlags_HasGamepad;
			io.NavInputs[ImGuiNavInput_Activate] = Input::Joystick::GetButton(JoystickCode::A) ? 1.0f : 0.0f;
			io.NavInputs[ImGuiNavInput_Cancel] = Input::Joystick::GetButton(JoystickCode::B) ? 1.0f : 0.0f;
			io.NavInputs[ImGuiNavInput_Menu] = Input::Joystick::GetButton(JoystickCode::X) ? 1.0f : 0.0f;
			io.NavInputs[ImGuiNavInput_Input] = Input::Joystick::GetButton(JoystickCode::Y) ? 1.0f : 0.0f;
		
			io.NavInputs[ImGuiNavInput_DpadLeft] = Input::Joystick::GetButton(JoystickCode::Left) ? 1.0f : 0.0f;
			io.NavInputs[ImGuiNavInput_DpadRight] = Input::Joystick::GetButton(JoystickCode::Right) ? 1.0f : 0.0f;
			io.NavInputs[ImGuiNavInput_DpadUp] = Input::Joystick::GetButton(JoystickCode::Up) ? 1.0f : 0.0f;
			io.NavInputs[ImGuiNavInput_DpadDown] = Input::Joystick::GetButton(JoystickCode::Down) ? 1.0f : 0.0f;
		
			io.NavInputs[ImGuiNavInput_FocusPrev] = Input::Joystick::GetButton(JoystickCode::LB) ? 1.0f : 0.0f;
			io.NavInputs[ImGuiNavInput_FocusNext] = Input::Joystick::GetButton(JoystickCode::RB) ? 1.0f : 0.0f;
			io.NavInputs[ImGuiNavInput_TweakSlow] = io.NavInputs[ImGuiNavInput_FocusPrev];
			io.NavInputs[ImGuiNavInput_TweakFast] = io.NavInputs[ImGuiNavInput_FocusNext];

			Vector2 stick = Input::Joystick::GetLStick();
			float inversion = stick.x * -1;

			if (inversion > 0.0f && io.NavInputs[ImGuiNavInput_LStickLeft] < inversion)
				io.NavInputs[ImGuiNavInput_LStickLeft] = inversion;
			if (stick.x > 0.0f && io.NavInputs[ImGuiNavInput_LStickRight] < stick.x)
				io.NavInputs[ImGuiNavInput_LStickRight] = stick.x;

			inversion = stick.y * -1;
			if (inversion > 0.0f && io.NavInputs[ImGuiNavInput_LStickUp] < inversion)
				io.NavInputs[ImGuiNavInput_LStickUp] = inversion;
			if (stick.x > 0.0f && io.NavInputs[ImGuiNavInput_LStickDown] < stick.x)
				io.NavInputs[ImGuiNavInput_LStickDown] = stick.x;
		}
#endif // #ifndef IMGUI_IMPL_WIN32_DISABLE_GAMEPAD
	}


	//----------------------------------------------------------------------------------
	//[DrawGUI]
	//描画を行う
	void GUI::DrawGUI()
	{
		//Not Using or Not DrawReady->終了
		if (IS_FALSE(m_isUsing.load() & m_isDrawReady.load())) return;

		DrawDebugStream();
		ImGui::Render();
		DrawGraphicsX11();
	}
}