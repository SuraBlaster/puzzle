#pragma once

#include "Graphics/Model.h"
#include "Stage.h"


class StagePazzle4 : public Stage
{
public:
    StagePazzle4();
    ~StagePazzle4()override;

    //更新処理
    void Update(float elapsedTime)override;

    //描画処理
    void Render(ID3D11DeviceContext* dc, Shader* shader)override;

    //レイキャスト
    bool RayCast(const DirectX::XMFLOAT3& start,
        const DirectX::XMFLOAT3& end, HitResult& hit)override;

    void InputMove();
private:
    Model* model = nullptr;

    float prevAxisLX = 0.0f;
    float prevAxisLY = 0.0f;
};
