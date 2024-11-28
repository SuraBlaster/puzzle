#pragma once

#include "Graphics/Model.h"
#include "Stage.h"

class StageBatteryFuture : public Stage
{
public:
    StageBatteryFuture();
    ~StageBatteryFuture()override;


    //�X�V����
    void Update(float elapsedTime)override;

    //�`�揈��
    void Render(ID3D11DeviceContext* dc, Shader* shader)override;

    //���C�L���X�g
    bool RayCast(const DirectX::XMFLOAT3& start,
        const DirectX::XMFLOAT3& end, HitResult& hit)override;


    Model* GetModel()const { return model; }
    void SetModel(Model* newModel) { model = newModel; }

private:
    Model* model = nullptr;
};
