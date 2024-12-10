#include "StageBatteryFuture.h"
#include "EraManager.h"
#include <Input/Input.h>

StageBatteryFuture::StageBatteryFuture()
{
    model = new Model("Data/Model/Stage/BatteryBox.mdl");

    scale.x = scale.y = scale.z = 0.005f;

    position.x = -3;
    position.y = 1;
    position.z = -9;
}

StageBatteryFuture::~StageBatteryFuture()
{
    //モデルを破棄
    delete model;
}

void StageBatteryFuture::Update(float elapsedTime)
{
    GamePad& gamepad = Input::Instance().GetGamePad();

    if (EraManager::Instance().GetPlayerHasBattery() || EraManager::Instance().GetBatteryPast() == true) {
        scale.x = scale.y = scale.z = 0.0f;
    }
    else {
        scale.x = scale.y = scale.z = 0.005f;
    }

    UpdateTransform();

    model->UpdateTransform(transform);
}

void StageBatteryFuture::Render(ID3D11DeviceContext* dc, Shader* shader)
{
    //シェーダーにモデルを描画してもらう
    shader->Draw(dc, model);
}

bool StageBatteryFuture::RayCast(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, HitResult& hit)
{
    return Collision::IntersectRayVsModel(start, end, model, hit);
}