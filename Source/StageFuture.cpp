#include "StageFuture.h"

//コンストラクタ
StageFuture::StageFuture()
{

    model = new Model("Data/Model/Stage/Future1F.mdl");
}

//デストラクタ
StageFuture::~StageFuture()
{
    //ステージモデルを破棄
    delete model;
}

//更新処理
void StageFuture::Update(float elapsedTime)
{
    //やること名塩
}

void StageFuture::Render(ID3D11DeviceContext* dc, Shader* shader)
{
    //シェーダーにモデルを描画してもらう
    shader->Draw(dc, model);
}

bool StageFuture::RayCast(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, HitResult& hit)
{
    return Collision::IntersectRayVsModel(start, end, model, hit);
}