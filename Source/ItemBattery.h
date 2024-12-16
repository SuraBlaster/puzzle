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


private:

    //ノードとプレイヤーの衝突処理
    void CollisionNodeVsPlayer(const char* nodeName, float nodeRadius);

private:
    Model* model = nullptr;
    float turnSpeed = DirectX::XMConvertToRadians(1);
};