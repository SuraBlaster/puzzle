#pragma once

#include "Graphics/Model.h"
#include "Stage.h"

class StageMain : public Stage
{
public:
    StageMain();
    ~StageMain()override;


    //更新処理
    void Update(float elapsedTime)override;

    //描画処理
    void Render(ID3D11DeviceContext* dc, Shader* shader)override;

    //レイキャスト
    bool RayCast(const DirectX::XMFLOAT3& start,
        const DirectX::XMFLOAT3& end, HitResult& hit)override;
private:
    Model* model = nullptr;
};