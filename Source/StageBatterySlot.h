#pragma once

#include "Graphics/Model.h"
#include "Stage.h"

class StageBatterySlot : public Stage
{
public:
    StageBatterySlot();
    ~StageBatterySlot()override;


    //�X�V����
    void Update(float elapsedTime)override;

    //�`�揈��
    void Render(ID3D11DeviceContext* dc, Shader* shader)override;

    //���C�L���X�g
    bool RayCast(const DirectX::XMFLOAT3& start,
        const DirectX::XMFLOAT3& end, HitResult& hit)override;


    Model* GetModel()const { return model; }

private:
    Model* model = nullptr;

    
};
