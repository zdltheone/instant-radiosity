#include "app.h"
using namespace std;

App::~App()
{
	ReleaseCOM(_deviceContext);
	ReleaseCOM(_device);
}
void App::RunApp() {
	InitApp();
	Run();
}

void App::InitApp()
{
	_window = new Window();
	_window->createWindow(L"InstantRadiosity");

	_direct3D = new D3D();
	_direct3D->InitD3D(_window->getWidth(), _window->getHeight(), _window->getHandle());

	_device = _direct3D->GetDevice();
	_deviceContext = _direct3D->GetDeviceContext();


	//_basic = new ShaderProgram(_device, _deviceContext);
	//_basic->CompileShaders(L"media/hlsl/basic.vsh", L"media/hlsl/basic.psh");
}

void App::Update() {


}


void App::Render() {
	_direct3D->BeginScene(0.25f);

	_direct3D->EndScene();
}

void App::Run()
{
	bool done = false;
	MSG msg;

	while(!done)
	{
		if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if(msg.message == WM_QUIT) { done = true; }
			else
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{
			Update();
			Render();
		}
	}
}