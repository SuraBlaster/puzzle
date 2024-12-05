#pragma once

#include <DirectXMath.h>
#include "Graphics/Model.h"

struct HitResult
{
    DirectX::XMFLOAT3 position = { 0,0,0 };
    DirectX::XMFLOAT3 normal = { 0,0,0 };
    DirectX::XMFLOAT3 rotation = { 0,0,0 };
    float distance = 0.0f;
    int materialIndex = -1;
};

class Collision
{
public:
    //���Ƌ��̓����蔻��
    static bool IntersectSphereVsSphere(
        const DirectX::XMFLOAT3& positionA,
        float radiusA,
        const DirectX::XMFLOAT3& positionB,
        float radiusB,
        DirectX::XMFLOAT3& outPositionB
    );

    //�~���Ɖ~���̌�������
    static bool IntersectCylinderVsCylinder(
        const DirectX::XMFLOAT3& positionA,
        float radiusA,
        float heightA,
        const DirectX::XMFLOAT3& positionB,
        float radiusB,
        float heightB,
        DirectX::XMFLOAT3& outpositionB
    );

    //���Ɖ~���̌�������
    static bool IntersectSphereVsCylinder(
        const DirectX::XMFLOAT3& spherePosition,
        float sphereRadius,
        const DirectX::XMFLOAT3& cylinderPosition,
        float cylinderRadius,
        float cylinderHeight,
        DirectX::XMFLOAT3& outCylinderposition
    );

    //���C�ƃ��f���̌�������
    

    static bool IntersectRayVsModel(
        const DirectX::XMFLOAT3& start,
        const DirectX::XMFLOAT3& end,
        const Model* model,
        HitResult& result);

   //���C�ƃ��f���̌�������
    static bool IntersectRayVsDoor(
        const DirectX::XMFLOAT3& start,
        const DirectX::XMFLOAT3& end,
        const Model* model,
        HitResult& result);

    static bool IsPointInTriangle(
        const DirectX::XMVECTOR& P,
        const DirectX::XMVECTOR& A,
        const DirectX::XMVECTOR& B,
        const DirectX::XMVECTOR& C,
        const DirectX::XMVECTOR& N);

    static void HandleDoorCollision(const std::string& doorName, const HitResult& result);
   
};