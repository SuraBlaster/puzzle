#pragma once

#include <vector>
#include "Stage.h"

class StageManager
{
private:
    StageManager(){}
    ~StageManager(){}

public:
    //唯一のインスタンス取得
    static StageManager& Instance()
    {
        static StageManager instance;
        return instance;
    }

    //行列更新処理
    void UpdateTransform();
    
    //更新処理
    void Update(float elapsedTime);

    //更新処理
    void Render(ID3D11DeviceContext* dc, Shader* shader);

    //ステージ登録
    void Register(Stage* stage);

    //ステージ全削除
    void Clear();

    //レイキャスト
    bool RayCast(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end,
        HitResult& hit);

private:
    std::vector<Stage*> stages;

    DirectX::XMFLOAT3 position = { 0, 0, 0 };
    DirectX::XMFLOAT3 angle = { 0, 0, 0 };
    DirectX::XMFLOAT3 scale = { 1, 1, 1 };
    DirectX::XMFLOAT4X4 transform{
        1,0,0,0,
        0,1,0,0,
        0,0,1,0,
        0,0,0,1
    };
};