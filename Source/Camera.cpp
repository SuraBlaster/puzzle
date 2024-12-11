#include "Camera.h"
#include "Player.h"
#include "Input/Cube.h"


void Camera::SetLookAt(const DirectX::XMFLOAT3& eye, const DirectX::XMFLOAT3& focus, const
    DirectX::XMFLOAT3& up)
{
    DirectX::XMVECTOR Eye = DirectX::XMLoadFloat3(&eye);
    DirectX::XMVECTOR Focus = DirectX::XMLoadFloat3(&focus);
    DirectX::XMVECTOR Up = DirectX::XMLoadFloat3(&up);
    DirectX::XMMATRIX View = DirectX::XMMatrixLookAtLH(Eye, Focus, Up);
    DirectX::XMStoreFloat4x4(&view, View);

    //�r���[���t�s�񉻂����[���h�s��ɖ߂�
    DirectX::XMMATRIX World = DirectX::XMMatrixInverse(nullptr, View);
    DirectX::XMFLOAT4X4 world;
    DirectX::XMStoreFloat4x4(&world, World);

    this->right.x = world._11;
    this->right.y = world._12;
    this->right.z = world._13;

    this->up.x = world._21;
    this->up.y = world._22;
    this->up.z = world._23;

    this->front.x = world._31;
    this->front.y = world._32;
    this->front.z = world._33;

    this->eye = eye;
    this->focus = focus;
}

void Camera::SetParspectiveFov(float  forY, float aspect, float nearZ, float farZ)
{
    DirectX::XMMATRIX Projection = DirectX::XMMatrixPerspectiveFovLH(forY, aspect, nearZ, farZ);
    DirectX::XMStoreFloat4x4(&projection, Projection);
}
