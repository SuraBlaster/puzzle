#include "StageBatteryPast.h"
#include "EraManager.h"
#include <Input/Input.h>

StageBatteryPast::StageBatteryPast()
{
    model = new Model("Data/Model/Stage/BatteryBox.mdl");

    scale.x = scale.y = scale.z = 0.01f;

    position.x = 5;

    position.y = 0;
}

StageBatteryPast::~StageBatteryPast()
{
    //モデルを破棄
    delete model;
}

void StageBatteryPast::Update(float elapsedTime)
{
    GamePad& gamepad = Input::Instance().GetGamePad();

    if (EraManager::Instance().GetPlayerHasBattery() || EraManager::Instance().GetBatteryPast() == false) {
        scale.x = scale.y = scale.z = 0.0f;
    }
    else {
        scale.x = scale.y = scale.z = 0.01f;
    }
    
    UpdateTransform();

    model->UpdateTransform(transform);
}

void StageBatteryPast::Render(ID3D11DeviceContext* dc, Shader* shader)
{
    //シェーダーにモデルを描画してもらう
    shader->Draw(dc, model);
}

bool StageBatteryPast::RayCast(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, HitResult& hit)
{
    return Collision::IntersectRayVsModel(start, end, model, hit);
}



