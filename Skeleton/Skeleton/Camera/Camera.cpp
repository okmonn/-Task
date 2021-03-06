#include "Camera.h"
#include "../Window/Window.h"
#include "../Device/Device.h"
#include "../etc/Release.h"
#include "../etc/Func.h"

// コンストラクタ
Camera::Camera(std::weak_ptr<Window>win, std::weak_ptr<Device> dev)  :
	dev(dev), win(win)
{
	vp = {};

	InitVP();
}

// デストラクタ
Camera::~Camera()
{
}

// VPの初期化
void Camera::InitVP(void)
{
	//ダミー宣言
	FLOAT pos = 0.0f;
	//カメラの位置
	DirectX::XMVECTOR eye    = { 0.0f, pos, -1.0f };
	//カメラの焦点 
	DirectX::XMVECTOR target = { 0.0f, pos,  0.0f };
	//カメラの上方向
	DirectX::XMVECTOR upper  = { 0.0f, 1.0f, 0.0f };

	DirectX::XMStoreFloat4x4(&vp.view, DirectX::XMMatrixLookAtLH(eye, target, upper));
	DirectX::XMStoreFloat4x4(&vp.projection, DirectX::XMMatrixPerspectiveFovLH(RAD(90.0f), (float)win.lock()->GetX() / (float)win.lock()->GetY(), 0.01f, 500.0f));

	vp.eye = { 0.0f, pos, -1.0f };

	vp.lightPos = vp.eye;
	DirectX::XMStoreFloat4x4(&vp.lightView, DirectX::XMMatrixLookAtLH(eye, target, upper));
	DirectX::XMStoreFloat4x4(&vp.lightProjection, DirectX::XMMatrixOrthographicLH(40.0f, 40.0f, 0.01f, 500.0f));
}

float Camera::Magnitude(const DirectX::XMFLOAT3 & i)
{
	return std::sqrt(i.x * i.x + i.y * i.y + i.z * i.z);
}

DirectX::XMFLOAT3 Camera::Normal(const DirectX::XMFLOAT3 & i)
{
	auto m = Magnitude(i);
	return { i.x / m, i.y / m, i.z / m };
}

// ビューの更新
void Camera::ChangeView(const DirectX::XMFLOAT3 & eye, const DirectX::XMFLOAT3 & target, const DirectX::XMFLOAT3 & up, const DirectX::XMFLOAT3 & lightPos)
{
	//カメラの位置
	DirectX::XMVECTOR eyeVec = { eye.x,    eye.y,    eye.z };
	//カメラの焦点 
	DirectX::XMVECTOR tarVec = { target.x, target.y, target.z };
	//カメラの上方向
	DirectX::XMVECTOR upVec  = { up.x,     up.y,     up.z };

	DirectX::XMStoreFloat4x4(&vp.view, DirectX::XMMatrixLookAtLH(eyeVec, tarVec, upVec));
	vp.eye = eye;

	DirectX::XMFLOAT3 vec = Normal(lightPos);
	float m = Magnitude({ target.x - eye.x, target.y - eye.y, target.z - eye.z });
	vp.lightPos = target;
	vp.lightPos.x += vec.x * m;
	vp.lightPos.y += vec.y * m;
	vp.lightPos.z += vec.z * m;

	DirectX::XMStoreFloat4x4(&vp.lightView, DirectX::XMMatrixLookAtLH(DirectX::XMLoadFloat3(&vp.lightPos), tarVec, upVec));
}
