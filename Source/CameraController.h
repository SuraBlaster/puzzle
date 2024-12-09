#pragma once

#include<DirectXMath.h>

class CameraController
{
public:
    CameraController(){}
    ~CameraController(){}

    void Update(float elapsedTime);
    void UpdateHorizontalRotation(float elapsedTime);
    void UpdateVerticalRotation(float elapsedTime);


    void SetTarget(const DirectX::XMFLOAT3& target) { this->target = target; }
    const DirectX::XMFLOAT3& GetTarget() const { return target; }

    DirectX::XMFLOAT3 TargetPosition(const DirectX::XMFLOAT3& playerPos, const DirectX::XMINT3& mapKey, float length, bool& isCubeView);

    

    void SetAngle(const DirectX::XMFLOAT3& angle) { this->angle = angle; }
    void SetCameraPointPosition(const int& direction) { this->cameraPointPosition = direction; }

    const DirectX::XMFLOAT3& GetAngle() const { return angle; }

    int cameraPointPosition = 0;
private:
    DirectX::XMFLOAT3 target = { 0, 0, 0 };
    DirectX::XMFLOAT3 angle = { 0, 0, 0 };
    float             rollSpeed = DirectX::XMConvertToRadians(90);
    float             range = 10.0f;
    float             maxAngleX = DirectX::XMConvertToRadians(45);
    float             minAngleX = DirectX::XMConvertToRadians(5);
    bool              isRotating = false;
    float             targetAngleY = angle.y;
};
