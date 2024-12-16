#pragma once

#include "Graphics/Model.h"
#include "Item.h"

//�X���C��
class ItemBattery : public Item
{
public:
    ItemBattery();
    ~ItemBattery() override;

    //�X�V����
    void Update(float elapsedTime)override;

    //�`�揈��
    void Render(ID3D11DeviceContext* dc, Shader* shader)override;


private:

    //�m�[�h�ƃv���C���[�̏Փˏ���
    void CollisionNodeVsPlayer(const char* nodeName, float nodeRadius);

private:
    Model* model = nullptr;
    float turnSpeed = DirectX::XMConvertToRadians(1);
};