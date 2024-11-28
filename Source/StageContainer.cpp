#include "StageContainer.h"

StageContainer::StageContainer()
{
    model = new Model("Data/Model/Stage/Container.mdl");

    scale.x = scale.y = scale.z = 0.005f;

    position.x = 10;

    position.y = 10;
}

StageContainer::~StageContainer()
{
    delete model;
}

void StageContainer::Update(float elapsedTime)
{
    //やること名塩
    UpdateTransform();

    model->UpdateTransform(transform);
}

void StageContainer::Render(ID3D11DeviceContext* dc, Shader* shader)
{
    //シェーダーにモデルを描画してもらう
    shader->Draw(dc, model);
}

bool StageContainer::RayCast(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, HitResult& hit)
{
    return Collision::IntersectRayVsModel(start, end, model, hit);
}
