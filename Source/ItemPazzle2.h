#pragma once
#include "Graphics/Model.h"
#include "Item.h"

//�R���e�i
class ItemPazzle2 : public Item
{
public:
    ItemPazzle2();
    ~ItemPazzle2() override;

    //�X�V����
    void Update(float elapsedTime)override;

    //�`�揈��
    void Render(ID3D11DeviceContext* dc, Shader* shader)override;

    void CollisionNodeVsPlayer(const char* nodeName, float nodeRadius);
private:
    Model* model = nullptr;
    float turnSpeed = DirectX::XMConvertToRadians(1);
};
