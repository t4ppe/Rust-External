#include "../Renderer/Drawer.h"
#include "../Renderer/ImGui/imgui_impl_dx11.h"
#include <winuser.h>
#include <d3d11.h>

#include "../xorstr.hpp"

#pragma comment(lib, "d3d11.lib")
extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace menu {
	static ID3D11Device* g_pd3dDevice = NULL;
	static ID3D11DeviceContext* g_pd3dDeviceContext = NULL;
	static IDXGISwapChain* g_pSwapChain = NULL;
	static ID3D11RenderTargetView* g_mainRenderTargetView = NULL;

	void CreateRenderTarget()
	{
		DXGI_SWAP_CHAIN_DESC sd;
		g_pSwapChain->GetDesc(&sd);

		ID3D11Texture2D* pBackBuffer;
		D3D11_RENDER_TARGET_VIEW_DESC render_target_view_desc;
		ZeroMemory(&render_target_view_desc, sizeof(render_target_view_desc));
		render_target_view_desc.Format = sd.BufferDesc.Format;
		render_target_view_desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		g_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
		g_pd3dDevice->CreateRenderTargetView(pBackBuffer, &render_target_view_desc, &g_mainRenderTargetView);
		g_pd3dDeviceContext->OMSetRenderTargets(1, &g_mainRenderTargetView, NULL);
		pBackBuffer->Release();
	}

	void CleanupRenderTarget()
	{
		if (g_mainRenderTargetView) { g_mainRenderTargetView->Release(); g_mainRenderTargetView = NULL; }
	}

	HRESULT CreateDeviceD3D(HWND hWnd)
	{
		DXGI_SWAP_CHAIN_DESC sd;
		{
			ZeroMemory(&sd, sizeof(sd));
			sd.BufferCount = 2;
			sd.BufferDesc.Width = 0;
			sd.BufferDesc.Height = 0;
			sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			sd.BufferDesc.RefreshRate.Numerator = 0;
			sd.BufferDesc.RefreshRate.Denominator = 1;
			sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
			sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
			sd.OutputWindow = hWnd;
			sd.SampleDesc.Count = 1;
			sd.SampleDesc.Quality = 0;
			sd.Windowed = TRUE;
			sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		}

		UINT createDeviceFlags = DXGI_SWAP_CHAIN_FLAG_DISPLAY_ONLY;
		D3D_FEATURE_LEVEL featureLevel;
		const D3D_FEATURE_LEVEL featureLevelArray[1] = { D3D_FEATURE_LEVEL_11_0, };
		if (D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, createDeviceFlags, featureLevelArray, 1, D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &featureLevel, &g_pd3dDeviceContext) != S_OK)
			return E_FAIL;

		CreateRenderTarget();

		return S_OK;
	}

	void CleanupDeviceD3D()
	{
		CleanupRenderTarget();
		if (g_pSwapChain) { g_pSwapChain->Release(); g_pSwapChain = NULL; }
		if (g_pd3dDeviceContext) { g_pd3dDeviceContext->Release(); g_pd3dDeviceContext = NULL; }
		if (g_pd3dDevice) { g_pd3dDevice->Release(); g_pd3dDevice = NULL; }
	}

	LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
			return true;

		switch (msg)
		{
		case WM_SIZE:
			if (g_pd3dDevice != NULL && wParam != SIZE_MINIMIZED)
			{
				ImGui_ImplDX11_InvalidateDeviceObjects();
				CleanupRenderTarget();
				g_pSwapChain->ResizeBuffers(0, (UINT)LOWORD(lParam), (UINT)HIWORD(lParam), DXGI_FORMAT_UNKNOWN, 0);
				CreateRenderTarget();
				ImGui_ImplDX11_CreateDeviceObjects();
			}
			return 0;
		case WM_SYSCOMMAND:
			if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
				return 0;
			break;
		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;
		}
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}

	namespace DisplayInformation
	{
		float FPSLock = 144.f;
		bool bMenu = true;
		float clearColor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
		bool WindowStatus = true;
		Vector2 Resolution;

		Vector2 GetDisplayResolution()
		{
			RECT Desktop;
			GetWindowRect(GetDesktopWindow(), &Desktop);
			Resolution.x = Desktop.right;
			Resolution.y = Desktop.bottom;
			return Resolution;
		}
	}

	void ChangeClickAbility(bool canclick, HWND CHWND)
	{
		long style = GetWindowLong(CHWND, GWL_EXSTYLE);
		if (canclick)
		{
			style &= ~WS_EX_LAYERED;
			SetWindowLong(CHWND, GWL_EXSTYLE, style);
			SetForegroundWindow(CHWND);
		}
		else
		{
			style |= WS_EX_LAYERED;
			SetWindowLong(CHWND, GWL_EXSTYLE, style);
		}
	}







//recoded menu new
std::string timebuilt = __DATE__;
std::string timebuild2 = __TIME__;
std::string nigger = "rustplus.co : " + timebuilt + "  " + __TIME__ + std::to_string(NULL);


void testmenu()
{

	//ImGui::Begin((nigger.c_str()), reinterpret_cast<bool*>(true), ImGuiWindowFlags_NoCollapse);
	//ImGui::Begin((nigger.c_str()), NULL, ImVec2(650, 400), 1.00f, NULL);
	ImGui::SetNextWindowSize, (ImVec2(650, 400));
	ImGui::Begin((nigger.c_str()), NULL, ImVec2(650, 400), 1.f, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoBringToFrontOnFocus);
	float LineWitdh = 860;
	ImVec2 Location = ImGui::GetCursorScreenPos();
	float DynamicRainbow = 0.01;
	static float staticHue = 0;
	ImDrawList* draw_list = ImGui::GetWindowDrawList();
	staticHue -= DynamicRainbow;
	if (staticHue < -1.f) staticHue += 1.f;

	for (int i = 0; i < LineWitdh; i++)
	{
		float hue = staticHue + (1.f / (float)LineWitdh) * i;
		if (hue < 0.f) hue += 1.f;
		ImColor cRainbow = ImColor::HSV(hue, 1.f, 1.f);
		draw_list->AddRectFilled(ImVec2(Location.x + i, Location.y), ImVec2(Location.x + i + 1, Location.y + 4), cRainbow);
	}

	static int tabb = 0;
	ImGui::Text(" ");
	ImGui::Text(" ");
	{ImGui::SameLine();
	if (ImGui::Button(xorget("Aimbot"), ImVec2(150.0f, 0.0f)))
	{
		tabb = 0;
	}
	ImGui::SameLine();
	}
	ImGui::SameLine();

	if (ImGui::Button(("Visuals"), ImVec2(150.0f, 0.0f)))
	{
		tabb = 1;
	}
	ImGui::SameLine();
	if (ImGui::Button(("Weapon Stuff"), ImVec2(150.0f, 0.0f)))
	{
		tabb = 2;
	}
	{ImGui::SameLine();
	if (ImGui::Button(("Misc"), ImVec2(150.0f, 0.0f)))
	{
		tabb = 3;
	}

	//Aimbot Tab
	if (tabb == 0) {

		ImGui::Text(" ");
		ImGui::Text("Aimbot");

		if (ImGui::TreeNodeEx(xorget("Options"), ImGuiTreeNodeFlags_DefaultOpen)) {


			ImGui::Checkbox(xorget("Aimbot"), &config.aimbot);
			if (config.aimbot) {
				ImGui::Hotkey(xorget("Key"), &config.aimbot_key);
				ImGui::SliderInt(xorget("Fov"), &config.fov, 1, 50);
				ImGui::SliderFloat(xorget("Smoothing"), &config.aimbot_smooth, 0.1f, 1.f);
				ImGui::Text(" ");
				ImGui::Text(" ");
				ImGui::Checkbox(xorget("Crosshair"), &config.crosshair);
			}
		}
		ImGui::TreePop();
	}

	else if (tabb == 1) {

		ImGui::Text(" ");
		ImGui::Text("Visuals");

		if (ImGui::TreeNodeEx(xorget("Visuals"), ImGuiTreeNodeFlags_DefaultOpen)) {
			ImGui::Checkbox(xorget("Rainbow ESP"), &config.rainbow_esp);
			ImGui::Checkbox(xorget("ESP"), &config.DrawPlayers);
			ImGui::Checkbox(xorget("Health"), &config.test_esp);
			ImGui::SliderInt(xorget("Max Player Distance"), &config.max_dist, 1, 700);
			if (config.DrawPlayers) {
				if (ImGui::TreeNodeEx(xorget("Player Esp"))) {
					ImGui::Checkbox(xorget("Sleeper ESP"), &config.sleeper);
					ImGui::Checkbox(xorget("Box ESP"), &config.box_esp);
					if (config.box_esp) {
						ImGui::Checkbox(xorget("Cornered Box"), &config.cornered_box_esp);
						ImGui::Checkbox(xorget("Filled Box"), &config.filled_box_esp);
					}
					ImGui::Checkbox(xorget("Name ESP"), &config.name_esp);
					ImGui::Checkbox(xorget("Health ESP"), &config.health_esp);
					ImGui::Checkbox(xorget("Snapline ESP"), &config.snapline_esp);
					ImGui::Checkbox(xorget("Skeleton ESP"), &config.skeleton_esp);
					ImGui::Checkbox(xorget("Held Weapon ESP"), &config.held_weapon);
					ImGui::TreePop();
				}

				if (ImGui::TreeNodeEx(xorget("World Items"))) {
					ImGui::Checkbox(xorget("Node ESP"), &config.node_esp);
					if (config.node_esp) {
						if (ImGui::TreeNodeEx(xorget("Nodes"))) {
							ImGui::Checkbox(xorget("Sulfur ESP"), &config.sulfur_node);
							ImGui::Checkbox(xorget("Stone ESP"), &config.stone_node);
							ImGui::Checkbox(xorget("Metal ESP"), &config.metal_node);
							ImGui::SliderInt(xorget("Distance"), &config.max_dist_ore, 1, 400);
							ImGui::TreePop();
						}
					}
					ImGui::Checkbox(xorget("Dropped Item ESP"), &config.dropped_esp);
					ImGui::Checkbox(xorget("Corpse ESP"), &config.corpse_esp);
					ImGui::Checkbox(xorget("Stash ESP"), &config.stash_esp);
					ImGui::Checkbox(xorget("Vehicle ESP"), &config.heli_esp);
					ImGui::Checkbox(xorget("Airdrop ESP"), &config.airdrop);
					ImGui::Checkbox(xorget("Hemp ESP"), &config.hemp);
					ImGui::Checkbox(xorget("Food ESP"), &config.food_esp);
					ImGui::Checkbox(xorget("Cargo Ship ESP"), &config.cargo_esp);
					
					if (ImGui::TreeNodeEx(xorget("Heli ESP"))) {
						ImGui::Checkbox(xorget("Patrol Helicopter ESP"), &config.patrol_heli);
						if (config.patrol_heli)
						{
							ImGui::SliderInt(xorget("Distance"), &config.max_dist_vehicle, 1, 5000);
							ImGui::TreePop();
						}
					}
					
					ImGui::Checkbox(xorget("Tool cupboard ESP"), &config.tool_cupboard);
					ImGui::Checkbox(xorget("Hackable Crate ESP"), &config.hackable_crate);
					ImGui::Checkbox(xorget("Crate ESP"), &config.high_tier_crate);
					ImGui::TreePop();
				}
			}
		}
	}

	else if (tabb == 2) {
		ImGui::Text(" ");
		ImGui::Text("Weapon Options");

		if (ImGui::TreeNodeEx(xorget("Weapons"))) {

			ImGui::Checkbox(xorget("No Recoil"), &config.recoil);
			ImGui::Checkbox(xorget("No Spread"), &config.spread);
			ImGui::Checkbox(xorget("No Sway"), &config.sway);
			ImGui::Checkbox(xorget("Rapid Fire"), &config.fast_shoot);
			ImGui::Checkbox(xorget("Onetap Eoka"), &config.eoka);
			ImGui::Checkbox(xorget("Bow"), &config.bowshit);
		}
	}


	else if (tabb == 3) {
		ImGui::Text(" ");
		ImGui::Text("Misc");

		if (ImGui::TreeNodeEx(xorget("Localplayer"))) {

			ImGui::Checkbox(xorget("Spiderman / No Fall"), &config.spiderman);
			ImGui::Checkbox(xorget("Double Jump"), &config.doublejump);
			ImGui::Checkbox(xorget("Admin Mode"), &config.setadmin);
			ImGui::TreePop();
		}

		if (ImGui::TreeNodeEx(xorget("World"))) {
			ImGui::SliderFloat(xorget("Sky color"), &config.sky_color, 0.f, 100.f);
			ImGui::Checkbox(xorget("Time Changer"), &config.day_changer);
			if (config.day_changer)
				ImGui::SliderFloat(xorget("DayTime"), &config.day_time, 0.f, 24.f);


			ImGui::TreePop();
		}
	}

	ImGui::End();
	}
}


//end of recode menu






	bool IntializeMainCanvasMenuOverlay()
	{
		Vector2 Resolution = DisplayInformation::GetDisplayResolution();

		if (1) {
			WNDCLASSEX WindowClass = { sizeof(WNDCLASSEX), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(NULL), NULL, LoadCursor(NULL, IDC_ARROW), NULL, NULL, _T("Kendra"), NULL };
			RegisterClassEx(&WindowClass);
			HWND MenuWindowHandle = CreateWindowEx(WS_EX_TRANSPARENT, _T("Kendra"), _T("Kendra"), WS_POPUP | WS_CHILD, 0, 0, DisplayInformation::GetDisplayResolution().x - 1, DisplayInformation::GetDisplayResolution().y - 1, NULL, NULL, WindowClass.hInstance, NULL);

			MARGINS margins = { -1 };
			DwmExtendFrameIntoClientArea(MenuWindowHandle, &margins);

			if (CreateDeviceD3D(MenuWindowHandle) < 0)
			{
				CleanupDeviceD3D();
				UnregisterClass(_T("Notepad"), WindowClass.hInstance); //paint
				return 0;
			}

			ImGui::CreateContext();
			ImGui_ImplDX11_Init(MenuWindowHandle, g_pd3dDevice, g_pd3dDeviceContext);
			ChangeClickAbility(true, MenuWindowHandle);

			Renderer::GetInstance()->Initialize();
			ImGuiIO& io = ImGui::GetIO();
			ImFont* imFont = io.Fonts->AddFontDefault();

			MSG msg;
			ZeroMemory(&msg, sizeof(msg));

			ShowWindow(MenuWindowHandle, SW_SHOWNORMAL);

			while (msg.message != WM_QUIT && !GetAsyncKeyState(VK_DELETE)) { //panic button
				SetWindowPos(MenuWindowHandle, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);

				if (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE)) {
					TranslateMessage(&msg);
					DispatchMessage(&msg);
					continue;
				}

				ImGui_ImplDX11_NewFrame();

				if (GetAsyncKeyState(VK_INSERT)) //Menu key to open
				{
					ShowWindow(MenuWindowHandle, SW_SHOWDEFAULT);
					UpdateWindow(MenuWindowHandle);
					DisplayInformation::bMenu = DisplayInformation::bMenu ? false : true;
					DisplayInformation::WindowStatus = DisplayInformation::WindowStatus ? false : true;
					Sleep(150);
				}

				if (DisplayInformation::bMenu) {
					//BuildMenuMain();
					testmenu();

					ChangeClickAbility(true, MenuWindowHandle);
				}
				else {
					ChangeClickAbility(false, MenuWindowHandle);
				}
				if (!DisplayInformation::WindowStatus) {
					ShowWindow(MenuWindowHandle, SWP_SHOWWINDOW);
				}

				Renderer::GetInstance()->BeginScene();
				g_pd3dDeviceContext->ClearRenderTargetView(g_mainRenderTargetView, reinterpret_cast<const float*>(&DisplayInformation::clearColor));
				std::lock_guard<std::mutex> lk(entity_mutex);
				rust::localplayer_thread();
				if (rust::local)
					rust::gettaggedobjects();

				if (rust::cam)
					rust::entity_esp_thread(imFont, Resolution);

				Renderer::GetInstance()->DrawScene();
				Renderer::GetInstance()->EndScene();
				ImGui::Render();
				ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
				g_pSwapChain->Present(0, 0);
				HWND DESKTOP = GetForegroundWindow();
				HWND MOVERDESK = GetWindow(DESKTOP, GW_HWNDPREV);
				SetWindowPos(MenuWindowHandle, MOVERDESK, NULL, NULL, NULL, NULL, SWP_NOMOVE | SWP_NOSIZE);
				UpdateWindow(MenuWindowHandle);
			}

			ImGui_ImplDX11_Shutdown();
			ImGui::DestroyContext();
			CleanupDeviceD3D();
			UnregisterClass(_T("Notepad"), WindowClass.hInstance); //paint
			return 0;
		}
	}
}