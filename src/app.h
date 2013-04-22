#ifndef _app_h_
#define _app_h_

#include "util.h"
#include "window.h"
#include "direct3D_11.h"
#include "shaderProgram.h"
#include <vector>
#include <iostream>
#include <map>

using namespace std;

class App
{
public:
	~App();
	void RunApp();
	void SwitchActive(string name);
private:
	void InitApp();
	void Update();
	void Render();
	void Run();
	
private:
	Window* _window;
	D3D* _direct3D;

	ID3D11DeviceContext* _deviceContext;
	ID3D11Device* _device;

	ShaderProgram* _basic;
};

#endif