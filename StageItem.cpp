#include "StageItem.h"

//コンストラクタ
StageItem::StageItem()
{
    model = new Model("Data/Model/Stage/Past1F.mdl");

    scale.x = scale.y = scale.z = 0.01f;
}

//デストラクタ
StageItem::~StageItem()
{
    //ステージモデルを破棄
    delete model;
}

//更新処理
void StageItem::Update(float elapsedTime)
{
    //やること名塩
    UpdateTransform();

    model->UpdateTransform(transform);
}

void StageItem::Render(ID3D11DeviceContext* dc, Shader* shader)
{
    //シェーダーにモデルを描画してもらう
    shader->Draw(dc, model);
}

bool StageItem::RayCast(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, HitResult& hit)
{
    return Collision::IntersectRayVsModel(start, end, model, hit);
}