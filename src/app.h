#ifndef _app_h_
#define _app_h_

#include "util.h"
#include "window.h"
#include "direct3D_11.h"
#include "shaderProgram.h"
#include "raytracer.h"
#include "geometryObjects.h"
#include "xmlLoader.h"
#include <vector>
#include <iostream>
#include <map>

using namespace std;

class App
{
public:
	~App();
	void RunApp();
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
    ID3D11SamplerState* _sampler;
    ID3D11Buffer* _vb;
    ID3D11Buffer* _ib;

    RayTracer* _raytracer;
};

#endif