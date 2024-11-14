#include "StageElevator.h"

StageElevator::StageElevator()
{
    scale.x = scale.y = scale.z = 0.005f;

    //ステージモデルを読み込み
    model = new Model("Data/Model/Elevator/Elevator.mdl");
}

StageElevator::~StageElevator()
{
    //ステージモデルを破棄
    delete model;

}

void StageElevator::Update(float elapsedTime)
{
    //前回の情報を保存
    oldTransform = transform;

    //スタートからゴールまでの距離を算出する
    DirectX::XMVECTOR Start = DirectX::XMLoadFloat3(&start);
    DirectX::XMVECTOR Goal = DirectX::XMLoadFloat3(&goal);
    DirectX::XMVECTOR Vec = DirectX::XMVectorSubtract(Goal, Start);
    DirectX::XMVECTOR Length = DirectX::XMVector3Length(Vec);

    float length;
    DirectX::XMStoreFloat(&length, Length);

    //スタートからゴールまでの間を1秒間で進む割合(0.0～1.0)を算出する
    float speed = moveSpeed * elapsedTime;
    float speedRate = speed / length;
    moveRate += speedRate;

    //ゴールに到達、またはスタートに戻った場合、移動方向を反転させる
    if (moveRate <= 0.0f || moveRate >= 1.0f)
    {
        moveSpeed = -moveSpeed;
    }

    //線形補完で位置を算出する
    DirectX::XMVECTOR Position = DirectX::XMVectorLerp(Start, Goal, moveRate);
    DirectX::XMStoreFloat3(&position, Position);

    //行列更新
    UpdateTransform();

    const DirectX::XMFLOAT4X4 transformIdentity = { 1,0,0,0 ,0,1,0,0 ,0,0,1,0 ,0,0,0,1 };
    //model->UpdateTransform(transformIdentity);
}

//行列更新処理
void StageElevator::Render(ID3D11DeviceContext* dc, Shader* shader)
{
    model->UpdateTransform(transform);

    shader->Draw(dc, model);
}

//レイキャスト
bool StageElevator::RayCast(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, HitResult& hit)
{
    return Collision::IntersectRayVsModel(start, end, model, hit);
}

//行列更新処理
void StageElevator::UpdateTransform()
{
    DirectX::XMMATRIX S = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);
    DirectX::XMMATRIX R = DirectX::XMMatrixRotationRollPitchYaw(angle.x, angle.y, angle.z);
    DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(position.x, position.y, position.z);
    DirectX::XMMATRIX W = S * R * T;
    DirectX::XMStoreFloat4x4(&transform, W);
}