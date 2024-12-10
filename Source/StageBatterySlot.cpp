#include "StageBatterySlot.h"
#include "EraManager.h"
#include <Input/Input.h>
StageBatterySlot::StageBatterySlot()
{
    model = new Model("Data/Model/Stage/BatterySlot.mdl");

    scale.x = scale.y = scale.z = 0.01f;

    position.x = -3;
    position.y = 1.5;
    position.z = -9;
}

StageBatterySlot::~StageBatterySlot()
{
    //���f����j��
    delete model;
}

void StageBatterySlot::Update(float elapsedTime)
{
    GamePad& gamepad = Input::Instance().GetGamePad();

    if (EraManager::Instance().GetPlayerHasBattery() || EraManager::Instance().GetBatteryFuture() == false || EraManager::Instance().GetBatteryPast() == false)
    {
        scale.x = scale.y = scale.z = 0.005f;
    }
    else
    {
        scale.x = scale.y = scale.z = 0.0f;
    }

    UpdateTransform();

    model->UpdateTransform(transform);
}

void StageBatterySlot::Render(ID3D11DeviceContext* dc, Shader* shader)
{
    //�V�F�[�_�[�Ƀ��f����`�悵�Ă��炤
    shader->Draw(dc, model);
}

bool StageBatterySlot::RayCast(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, HitResult& hit)
{
    return Collision::IntersectRayVsModel(start, end, model, hit);
}

