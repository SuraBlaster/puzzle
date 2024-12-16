#include "StageSeed.h"
#include "Player.h"
#include "EraManager.h"

StageSeed::StageSeed()
{
    model = new Model("Data/Model/Stage/Seed.mdl");

    scale.x = scale.y = scale.z = 0.0f;

    position.x = -8;

    position.y = 1;
}

StageSeed::~StageSeed()
{
    delete model;
}

void StageSeed::Update(float elapsedTime)
{
    if (EraManager::Instance().GetPlayerSeed() == false)
    {
        EraManager::Instance().SetDropPazzle2Position({0,0,0});
        EraManager::Instance().SetDropPazzle2(true);
        scale.x = scale.y = scale.z = 0.1f;
    }


    UpdateTransform();

    model->UpdateTransform(transform);

}

void StageSeed::Render(ID3D11DeviceContext* dc, Shader* shader)
{
    //シェーダーにモデルを描画してもらう
    shader->Draw(dc, model);
}

bool StageSeed::RayCast(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, HitResult& hit)
{
    return Collision::IntersectRayVsModel(start, end, model, hit);
}



