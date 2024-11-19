#include "StagePast.h"

//コンストラクタ
StagePast::StagePast()
{
    model = new Model("Data/Model/Stage/Past1F.mdl");

    
}

//デストラクタ
StagePast::~StagePast()
{
    //ステージモデルを破棄
    delete model;
}

//更新処理
void StagePast::Update(float elapsedTime)
{
    //やること名塩
}

void StagePast::Render(ID3D11DeviceContext* dc, Shader* shader)
{
    //シェーダーにモデルを描画してもらう
    shader->Draw(dc, model);
}

bool StagePast::RayCast(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, HitResult& hit)
{
    return Collision::IntersectRayVsModel(start, end, model, hit);
}
