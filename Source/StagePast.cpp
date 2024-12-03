#include "StagePast.h"

//コンストラクタ
StagePast::StagePast()
{
    switch (difficulty)
    {
    case Difficulty::Tutorial:
        {
            model = new Model("Data/Model/Stage/Past1_2F.mdl");
            scale.x = scale.y = scale.z = 0.005f;
        }
        break;
    }
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
    UpdateTransform();

    model->UpdateTransform(transform);
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
