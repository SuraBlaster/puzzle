#include "StageAlter.h"
#include "Player.h"
#include "EraManager.h"

StageAlter::StageAlter()
{
    model = new Model("Data/Model/Stage/Alter.mdl");

    scale.x = scale.y = scale.z = 0.005f;

    position.x = 10;

    position.z = 5;

    angle.y += DirectX::XMConvertToDegrees(45);
}

StageAlter::~StageAlter()
{
    delete model;
}

void StageAlter::Update(float elapsedTime)
{
    //やること名塩
    UpdateTransform();

    model->UpdateTransform(transform);

}

void StageAlter::Render(ID3D11DeviceContext* dc, Shader* shader)
{
    //シェーダーにモデルを描画してもらう
    shader->Draw(dc, model);
}

bool StageAlter::RayCast(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, HitResult& hit)
{
    return Collision::IntersectRayVsModel(start, end, model, hit);
}