#pragma once

#include "Graphics/Model.h"
#include "Stage.h"


class StageAlter : public Stage
{
public:
    StageAlter();
    ~StageAlter()override;

    //更新処理
    void Update(float elapsedTime)override;

    //描画処理
    void Render(ID3D11DeviceContext* dc, Shader* shader)override;

    //レイキャスト
    bool RayCast(const DirectX::XMFLOAT3& start,
        const DirectX::XMFLOAT3& end, HitResult& hit)override;

    //ノードとプレイヤーの衝突処理
    void CollisionNodeVsPlayer(const char* nodeName, float nodeRadius);

private:
    Model* model = nullptr;
};
