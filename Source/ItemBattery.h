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

    //�f�o�b�O�v���~�e�B�u�`��
    void DrawDebugPrimitive()override;

private:

    //�m�[�h�ƃv���C���[�̏Փˏ���
    void CollisionNodeVsPlayer(const char* nodeName, float nodeRadius);

private:
    Model* model = nullptr;
    DirectX::XMFLOAT3 targetPosition = { 0,0,0 };
    DirectX::XMFLOAT3 territoryOrigin = { 0,0,0 };
    float turnSpeed = DirectX::XMConvertToRadians(90);
};