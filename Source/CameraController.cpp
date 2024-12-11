#include "CameraController.h"
#include "Camera.h"
#include "Input/Input.h"
#include "Input/Cube.h"
#include "Player.h"
#include"Mathf.h"


void CameraController::Update(float elapsedTime)
{
    Camera& camera = Camera::Instance();

    UpdateHorizontalRotation(elapsedTime);

    // 垂直方向回転の更新
    UpdateVerticalRotation(elapsedTime);

    // カメラの変換行列を計算
    DirectX::XMMATRIX Transform = DirectX::XMMatrixRotationRollPitchYaw(angle.x, angle.y, angle.z);

    // 新しいカメラ位置を計算
    DirectX::XMVECTOR Front = Transform.r[2];
    DirectX::XMFLOAT3 front;
    DirectX::XMStoreFloat3(&front, Front);

    DirectX::XMFLOAT3 eye;
    eye.x = target.x - front.x * range;
    eye.y = target.y - front.y * range;
    eye.z = target.z - front.z * range;

    eye.x = Mathf::Lerp(Camera::Instance().GetEye().x, eye.x, elapsedTime * 10.0f);
    eye.y = Mathf::Lerp(Camera::Instance().GetEye().y, eye.y, elapsedTime * 10.0f);
    eye.z = Mathf::Lerp(Camera::Instance().GetEye().z, eye.z, elapsedTime * 10.0f);

    // カメラの視点を更新
    camera.SetLookAt(eye, target, DirectX::XMFLOAT3(0, 1, 0));
}

void CameraController::UpdateHorizontalRotation(float elapsedTime)
{
    static int lastInputDirection = 0; // -1: 反時計回り, +1: 時計回り, 0: 未入力

    if (isRotating)
    {
        // 回転中の場合、新しい入力を無視して処理を進める
        angle.y += (targetAngleY - angle.y) * rollSpeed * elapsedTime * 5;

        if (std::abs(targetAngleY - angle.y) < 0.01f)
        {
            angle.y = targetAngleY;
            isRotating = false;

            // 回転完了後に cameraPointPosition を更新
            if (lastInputDirection == 1)
            {
                cameraPointPosition = (cameraPointPosition + 1) % 4; // 時計回り
            }
            else if (lastInputDirection == -1)
            {
                cameraPointPosition = (cameraPointPosition - 1 + 4) % 4; // 反時計回り
            }

            lastInputDirection = 0; // 入力方向をリセット
        }
    }
    else
    {

        GamePad& gamePad = Input::Instance().GetGamePad();
        float ax = gamePad.GetAxisRX();
        float ay = gamePad.GetAxisRY();
        // 新しい回転入力の検出
        if (abs(ax) > 0.1f)
        {
            if (ax < 0)
            {
                targetAngleY += DirectX::XM_PIDIV2;
                isRotating = true;
                lastInputDirection = 1; // 時計回りを記録
            }
            else
            {
                targetAngleY -= DirectX::XM_PIDIV2;
                isRotating = true;
                lastInputDirection = -1; // 反時計回りを記録
            }
        }
    }
}

void CameraController::UpdateVerticalRotation(float elapsedTime)
{
    GamePad& gamePad = Input::Instance().GetGamePad();
    float speed = rollSpeed * elapsedTime;  // 回転速度の計算


    // 垂直方向の回転処理
    if (gamePad.GetAxisRY())
    {
        angle.x += gamePad.GetAxisRY() * speed;  // Y軸の入力を使って垂直回転

        // 最大・最小角度の制限
        if (angle.x > maxAngleX)
        {
            angle.x = maxAngleX;  // 上方向に制限
        }

        if (angle.x < minAngleX)
        {
            angle.x = minAngleX;  // 下方向に制限
        }
    }
}
DirectX::XMFLOAT3 CameraController::TargetPosition(
    const DirectX::XMFLOAT3& playerPos,
    const DirectX::XMINT3& mapKey,
    float length,
    bool& isCubeView)  // 呼び出し元と連携して状態を共有
{
    GamePad& gamepad = Input::Instance().GetGamePad();
    StageRootCube* rootcube = StageCubeManager::Instance().GetRootCube();

    if (!rootcube)
    {
        // キューブが無い場合はプレイヤー視点に戻す
        isCubeView = false;  // 呼び出し元に状態を反映
        return playerPos;
    }

    Cube* targetCube = rootcube->GetChildAt(mapKey);
    DirectX::XMFLOAT3 targetCubePos = targetCube
        ? targetCube->GetPosition()
        : DirectX::XMFLOAT3(static_cast<float>(mapKey.x), static_cast<float>(mapKey.y), static_cast<float>(mapKey.z));

    float distance = DirectX::XMVectorGetX(DirectX::XMVector3Length(
        DirectX::XMVectorSubtract(DirectX::XMLoadFloat3(&playerPos), DirectX::XMLoadFloat3(&targetCubePos))));

    if (!isCubeView && distance <= length && (gamepad.GetButtonDown() & GamePad::BTN_ENTER))
    {
        isCubeView = true;  // 状態を更新
        return targetCubePos;
    }

    if (isCubeView && (gamepad.GetButtonDown() & GamePad::BTN_BACKSPACE))
    {
        isCubeView = false;  // 状態を更新
        return playerPos;
    }

    return isCubeView ? targetCubePos : playerPos;
}