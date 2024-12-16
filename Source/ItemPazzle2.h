#pragma once
#include "Graphics/Model.h"
#include "Item.h"

//コンテナ
class ItemPazzle2 : public Item
{
public:
    ItemPazzle2();
    ~ItemPazzle2() override;

    //更新処理
    void Update(float elapsedTime)override;

    //描画処理
    void Render(ID3D11DeviceContext* dc, Shader* shader)override;

    void CollisionNodeVsPlayer(const char* nodeName, float nodeRadius);
private:
    Model* model = nullptr;
    float turnSpeed = DirectX::XMConvertToRadians(1);
};
