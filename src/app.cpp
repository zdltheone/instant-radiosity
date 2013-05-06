#include "app.h"
#include "instantRadiosity.h"

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

    _raytracer = new RayTracer(5);
	
    _basic = new ShaderProgram(_device, _deviceContext);
	_basic->CompileShaders(L"src/basic.vsh", L"src/basic.psh");

    D3D11_SAMPLER_DESC samplerDesc;
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.BorderColor[0] = 0;
	samplerDesc.BorderColor[1] = 0;
	samplerDesc.BorderColor[2] = 0;
	samplerDesc.BorderColor[3] = 0;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;


	_device->CreateSamplerState(&samplerDesc, &_sampler);
	_deviceContext->PSSetSamplers(0, 1, &_sampler);

    float verts[20] = 
        { -0.99f,  0.99f, 0.f, 0.f, 0.f,   //tl
           0.99f,  0.99f, 0.f, 1.f, 0.f,   //tr
           0.99f, -0.99f, 0.f, 1.f, 1.f,   //br
          -0.99f, -0.99f, 0.f, 0.f, 1.f }; //bl
    unsigned int indices[6] = {0, 1, 2, 2, 3, 0};

    D3D11_BUFFER_DESC vbd;
	memset(&vbd, 0, sizeof(D3D11_BUFFER_DESC));
	vbd.ByteWidth = sizeof(float) * 5 * 4;
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA vsrd;
	memset(&vsrd, 0, sizeof(D3D11_SUBRESOURCE_DATA));
	vsrd.pSysMem = verts;

	D3D11_BUFFER_DESC ibd;
	memset(&ibd, 0, sizeof(D3D11_BUFFER_DESC));
	ibd.ByteWidth = sizeof(unsigned int) * 6;
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;

	D3D11_SUBRESOURCE_DATA isrd;
	memset(&isrd, 0, sizeof(D3D11_SUBRESOURCE_DATA));
	isrd.pSysMem = indices;

    HRESULT hret;
	hret = _device->CreateBuffer(&vbd, &vsrd, &_vb);
	if(FAILED(hret)) {}

	hret = _device->CreateBuffer(&ibd, &isrd, &_ib);
	if(FAILED(hret)) {}


}

void App::Update() {


}


void App::Render() {
	_direct3D->BeginScene(0.25f);

    _basic->SetActive();

    UINT stride = sizeof(float) * 5;
	UINT offset = 0;

	_deviceContext->IASetVertexBuffers(0, 1, &_vb, &stride, &offset);
	_deviceContext->IASetIndexBuffer(_ib, DXGI_FORMAT_R32_UINT, 0);

	_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	_deviceContext->DrawIndexed(6, 0, 0);

	_direct3D->EndScene();
}

void App::Run()
{

    Scene* scene = XMLLoader::parseSceneXML("../src/Cornell Box.xml");
    Image* imageBuffer = new Image(320, 240);
    
<<<<<<< HEAD
    bool showVPLs = false;
    int samples = 100;
    int reflect = 5;
=======
    bool showVPLs = true;
    int samples = 500;
    int reflect = 20;
>>>>>>> cddd182b727b2db156b5fb823e873a868c50a029

    _raytracer->raytrace(scene, imageBuffer, samples, reflect, showVPLs);

   
    ID3D11Texture2D* imageTex;
    ID3D11ShaderResourceView* imageSRV;

    //Create the noise texture
	D3D11_TEXTURE2D_DESC td;
	D3D11_SHADER_RESOURCE_VIEW_DESC srvd;
	HRESULT hret;

	ZeroMemory(&td, sizeof(td));
    td.Width			= imageBuffer->getWidth();
	td.Height			= imageBuffer->getHeight();
	td.MipLevels		= 1;
	td.ArraySize		= 1;
	td.Format			= DXGI_FORMAT_R32G32B32A32_FLOAT;
	td.SampleDesc.Count	= 1;
	td.Usage			= D3D11_USAGE_IMMUTABLE;
	td.BindFlags		= D3D11_BIND_SHADER_RESOURCE;
	td.CPUAccessFlags	= 0;
	td.MiscFlags		= 0;

	D3D11_SUBRESOURCE_DATA srd;
    ZeroMemory(&srd, sizeof(srd));
	srd.pSysMem = imageBuffer->getBuffer();
	srd.SysMemPitch = imageBuffer->getWidth() * sizeof(XMFLOAT4);

	hret = _device->CreateTexture2D(&td, &srd, &imageTex);
	if(FAILED(hret)) {
		return;
	}

	srvd.Format						= td.Format;
	srvd.ViewDimension				= D3D11_SRV_DIMENSION_TEXTURE2D;
	srvd.Texture2D.MostDetailedMip	= 0;
	srvd.Texture2D.MipLevels		= 1;
	hret = _device->CreateShaderResourceView(imageTex, &srvd, &imageSRV);
	if(FAILED(hret)) {
		return;

	}

    _deviceContext->PSSetShaderResources(0, 1, &imageSRV);


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
		    Render();
		}
	}
}