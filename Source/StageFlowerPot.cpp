#include "StageFlowerPot.h"
#include "Player.h"
#include "EraManager.h"

StageFlowerPot::StageFlowerPot()
{
    model = new Model("Data/Model/Stage/FlowerPot.mdl");

    scale.x = scale.y = scale.z = 0.005f;

    position.x = 1;

    position.y = 1;
}

StageFlowerPot::~StageFlowerPot()
{
    delete model;
}

void StageFlowerPot::Update(float elapsedTime)
{
    //やること名塩
    UpdateTransform();

    model->UpdateTransform(transform);

}

void StageFlowerPot::Render(ID3D11DeviceContext* dc, Shader* shader)
{
    //シェーダーにモデルを描画してもらう
    shader->Draw(dc, model);
}

bool StageFlowerPot::RayCast(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, HitResult& hit)
{
    return Collision::IntersectRayVsModel(start, end, model, hit);
}