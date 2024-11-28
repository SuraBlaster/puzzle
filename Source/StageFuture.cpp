#include "StageFuture.h"

//コンストラクタ
StageFuture::StageFuture()
{

    model = new Model("Data/Model/Stage/Future1F.mdl");

    scale.x = scale.y = scale.z = 0.01f;
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
    //行列更新処理
    UpdateTransform();

    model->UpdateTransform(transform);
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