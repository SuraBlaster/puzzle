#pragma once

#include "Graphics/Model.h"
#include "Item.h"

//スライム
class ItemBattery : public Item
{
public:
    ItemBattery();
    ~ItemBattery() override;

    //更新処理
    void Update(float elapsedTime)override;

    //描画処理
    void Render(ID3D11DeviceContext* dc, Shader* shader)override;

    //デバッグプリミティブ描画
    void DrawDebugPrimitive()override;

private:

    //ノードとプレイヤーの衝突処理
    void CollisionNodeVsPlayer(const char* nodeName, float nodeRadius);

private:
    Model* model = nullptr;
    DirectX::XMFLOAT3 targetPosition = { 0,0,0 };
    DirectX::XMFLOAT3 territoryOrigin = { 0,0,0 };
    float turnSpeed = DirectX::XMConvertToRadians(90);
};