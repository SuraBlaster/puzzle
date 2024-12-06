#pragma once

#include "Graphics/Model.h"
#include "Stage.h"


class StagePazzle3 : public Stage
{
public:
    StagePazzle3();
    ~StagePazzle3()override;

    //�X�V����
    void Update(float elapsedTime)override;

    //�`�揈��
    void Render(ID3D11DeviceContext* dc, Shader* shader)override;

    //���C�L���X�g
    bool RayCast(const DirectX::XMFLOAT3& start,
        const DirectX::XMFLOAT3& end, HitResult& hit)override;

    void InputMove();
private:
    Model* model = nullptr;

    float prevAxisLX = 0.0f;
    float prevAxisLY = 0.0f;
};