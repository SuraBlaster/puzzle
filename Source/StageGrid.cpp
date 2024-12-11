#include "StageGrid.h"
#include "Player.h"
#include "EraManager.h"

StageGrid::StageGrid()
{
    model = new Model("Data/Model/Stage/Grid.mdl");

    scale.x = scale.y = scale.z = 0.005f;

    position.x = 10;

    position.y = 1.6;

    position.z = 5;
}

StageGrid::~StageGrid()
{
    delete model;
}

void StageGrid::Update(float elapsedTime)
{

    UpdateTransform();

    model->UpdateTransform(transform);

}

void StageGrid::Render(ID3D11DeviceContext* dc, Shader* shader)
{
    //シェーダーにモデルを描画してもらう
    shader->Draw(dc, model);
}

bool StageGrid::RayCast(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, HitResult& hit)
{
    return Collision::IntersectRayVsModel(start, end, model, hit);
}