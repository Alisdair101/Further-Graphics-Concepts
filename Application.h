#pragma once

#include <windows.h>
#include <d3d11_1.h>
#include <d3dcompiler.h>
#include <directxmath.h>
#include <directxcolors.h>
#include "resource.h"
#include <vector>

#include "Structures.h"
#include "AICar.h"
#include "Car.h"
#include "Camera.h"
#include "CarParticleModel.h"
#include "DDSTextureLoader.h"
#include "GameObject.h"
#include "OBJLoader.h"

using namespace DirectX;

class Application
{
private:
	bool temp = true;

	HINSTANCE               _hInst;
	HWND                    _hWnd;
	D3D_DRIVER_TYPE         _driverType;
	D3D_FEATURE_LEVEL       _featureLevel;
	ID3D11Device*           _pd3dDevice;
	ID3D11DeviceContext*    _pImmediateContext;
	IDXGISwapChain*         _pSwapChain;
	ID3D11RenderTargetView* _pRenderTargetView;
	ID3D11VertexShader*     _pVertexShader;
	ID3D11PixelShader*      _pPixelShader;
	ID3D11InputLayout*      _pVertexLayout;

	ID3D11Buffer*           _pVertexBuffer;
	ID3D11Buffer*           _pIndexBuffer;

	ID3D11Buffer*           _pPlaneVertexBuffer;
	ID3D11Buffer*           _pPlaneIndexBuffer;

	ID3D11Buffer*           _pConstantBuffer;

	ID3D11DepthStencilView* _depthStencilView = nullptr;
	ID3D11Texture2D* _depthStencilBuffer = nullptr;

	// Textures
	ID3D11ShaderResourceView* _pTextureRV = nullptr;
	ID3D11ShaderResourceView* _pRaceTrackTex = nullptr;
	ID3D11ShaderResourceView* _pGrassTex = nullptr;
	ID3D11ShaderResourceView* _pMountainTex = nullptr;
	ID3D11ShaderResourceView* _pStoneTex = nullptr;
	ID3D11ShaderResourceView* _pSkyTex = nullptr;

	// Car Textures
	ID3D11ShaderResourceView* _pCarTex = nullptr;

	ID3D11SamplerState* _pSamplerLinear = nullptr;

	Light basicLight;

	// Camera
	Camera* _camera;
	float _cameraOrbitRadius = 7.0f;
	float _cameraOrbitRadiusMin = 2.0f;
	float _cameraOrbitRadiusMax = 50.0f;
	float _cameraOrbitAngleXZ = -90.0f;
	float _cameraSpeed = 2.0f;

	UINT _WindowHeight;
	UINT _WindowWidth;

	// Render dimensions - Change here to alter screen resolution
	UINT _renderHeight = 1080;
	UINT _renderWidth = 1920;

	ID3D11DepthStencilState* DSLessEqual;
	ID3D11RasterizerState* RSCullNone;

	ID3D11RasterizerState* CCWcullMode;
	ID3D11RasterizerState* CWcullMode;

	// Material Types
	Material shinyMaterial;
	Material noSpecMaterial;

	// Game Objects
	//GameObject* carBody;
	Car* car;
	Car* aiCar;

	// Other Objects
	Geometry cubeGeometry;
	GameObject* slidingCube;
	vector< GameObject* > _cubes;
	vector< GameObject* > bullets;
	int bulletIndex;
	GameObject* raceTrack;
	GameObject* groundPlane;
	GameObject* mountain;
	GameObject* skyBox;

	// Waypoints
	vector < GameObject* > waypoints;

	// Car GameObject properties
	/*XMFLOAT3 carPos;

	float carRotation;
	float carRotationSpeed;
	float carWheelRotation;*/

	// Cameras
	Camera* cameraMain;
	Camera* cameraCarMain;
	Camera* cameraCarAlternate;
	Camera* cameraTopView;
	Camera* cameraCarPerspective;
	Camera* normPerspective;

	XMFLOAT3 cameraCarPos;

	Camera* cameraCurrent;

	int camNum;

	float eyeX;
	float eyeY;
	float eyeZ;

private:
	HRESULT InitWindow(HINSTANCE hInstance, int nCmdShow);
	HRESULT InitDevice();
	void Cleanup();
	HRESULT CompileShaderFromFile(WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut);
	HRESULT InitShadersAndInputLayout();
	HRESULT InitVertexBuffer();
	HRESULT InitIndexBuffer();

	// From Semester 1
	void InitObjects();
	void InitCarObjects();
	void InitAIObjects();
	void InitWaypoints();
	void CarUpdate(float t);
	void CameraInput();
	void CheckFireBullet();

	void MoveForward(int objectNumber);

	// Draw Methods
	void CarDraw(ConstantBuffer* cb, ID3D11DeviceContext* _pImmediateContext);
	void CubeDraw(ConstantBuffer* cb, ID3D11DeviceContext* _pImmediateContext);
	void OtherDraw(ConstantBuffer* cb, ID3D11DeviceContext* _pImmediateContext);

public:
	Application();
	~Application();

	HRESULT Initialise(HINSTANCE hInstance, int nCmdShow);

	bool HandleKeyboard(MSG msg);

	void Update(float t);
	void Draw();
};

