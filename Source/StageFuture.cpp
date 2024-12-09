#include "StageFuture.h"
#include "EraManager.h"

//コンストラクタ
StageFuture::StageFuture()
{

    difficulty = EraManager::Instance().GetDifficulty();

    switch (difficulty)
    {
    case Difficulty::Tutorial:
        {
            model = new Model("Data/Model/Stage/Future1_2F.mdl");
            scale.x = scale.y = scale.z = 0.005f;
        }
        break;
    case Difficulty::Beginner2:
        {
            model = new Model("Data/Model/Stage/Future3FDebris.mdl");
            scale.x = scale.y = scale.z = 0.005f;
        }
        break;
    case Difficulty::Advanced:
        {
            model = new Model("Data/Model/Stage/Future3F.mdl");
            scale.x = scale.y = scale.z = 0.005f;
        }
        break;
    }
    
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