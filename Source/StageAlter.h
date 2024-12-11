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
    bool CollisionNodeVsPlayer(const char* nodeName, float nodeRadius);

    //扉の通常状態更新処理
    void UpdateIdleState();

    //扉の閉じる更新処理
    void UpdateCloseState();

    //扉の開く更新処理
    void UpdateOpenState();

    

private:
    Model* model = nullptr;

    //アニメーション
    enum Animation
    {
        Anim_Close,
        Anim_Open,
    };

    //ステート
    enum class State
    {
        Idle,
        Close,
        Open,
    };

    State state = State::Idle;

    //通常ステートに遷移
    void IdleState();

    //閉じるステートに遷移
    void CloseState();

    //開くステートに遷移
    void OpenState();
};
