#include "CameraController.h"
#include "Camera.h"
#include "Input/Input.h"
#include "Player.h"
#include "Mathf.h"

void CameraController::Update(float elapsedTime)
{
    GamePad& gamePad = Input::Instance().GetGamePad();
    float ax = gamePad.GetAxisRX();
    float ay = gamePad.GetAxisRY();

    float speed = rollSpeed * elapsedTime;

    if (gamePad.GetAxisRX())
    {
       //X����]����
       angle.y += ax * speed;
    }

    if (gamePad.GetAxisRY())
    {
        //Y����]����
        angle.x += ay * speed;
    }

    //UpdateHorizontalRotation(elapsedTime);

    if (angle.x > maxAngleX)
    {
        angle.x = maxAngleX;
    }

    if (angle.x < minAngleX)
    {
        angle.x = minAngleX;
    }

    /*if (angle.y < -DirectX::XM_PI)
    {
        angle.y += DirectX::XM_2PI;
    }

    if (angle.y > DirectX::XM_PI)
    {
        angle.y -= DirectX::XM_2PI;
    }*/

    DirectX::XMMATRIX Transform = DirectX::XMMatrixRotationRollPitchYaw(angle.x, angle.y, angle.z);

    DirectX::XMVECTOR Front = Transform.r[2];
    DirectX::XMFLOAT3 front;
    DirectX::XMStoreFloat3(&front,Front);

    DirectX::XMFLOAT3 eye;
    eye.x = target.x - front.x * range;
    eye.y = target.y - front.y * range;
    eye.z = target.z - front.z * range;

    Camera::Instance().SetLookAt(eye, target, DirectX::XMFLOAT3(0, 1, 0));
}

void CameraController::UpdateHorizontalRotation(float elapsedTime)
{
    static int lastInputDirection = 0; // -1: �����v���, +1: ���v���, 0: ������

    if (isRotating)
    {
        // ��]���̏ꍇ�A�V�������͂𖳎����ď�����i�߂�
        angle.y += (targetAngleY - angle.y) * rollSpeed * elapsedTime * 5;

        if (std::abs(targetAngleY - angle.y) < 0.01f)
        {
            angle.y = targetAngleY;
            isRotating = false;

            // ��]������� cameraPointPosition ���X�V
            if (lastInputDirection == 1)
            {
                cameraPointPosition = (cameraPointPosition + 1) % 4; // ���v���d
            }
            else if (lastInputDirection == -1)
            {
                cameraPointPosition = (cameraPointPosition - 1 + 4) % 4; // �����v���
            }

            lastInputDirection = 0; // ���͕��������Z�b�g
        }
    }
    else
    {
        GamePad& gamePad = Input::Instance().GetGamePad();
        float ax = gamePad.GetAxisRX();
        float ay = gamePad.GetAxisRY();
        // �V������]���͂̌��o
        if (abs(ax) > 0.1f)
        {
            if (ax < 0)
            {
                targetAngleY += DirectX::XM_PIDIV2;
                isRotating = true;
                lastInputDirection = 1; // ���v�����L�^
            }
            else
            {
                targetAngleY -= DirectX::XM_PIDIV2;
                isRotating = true;
                lastInputDirection = -1; // �����v�����L�^
            }
        }
    }
}


void CameraController::UpdateVerticalRotation(float elapsedTime)
{
    GamePad& gamePad = Input::Instance().GetGamePad();
    float speed = rollSpeed * elapsedTime;  // ��]���x�̌v�Z


    // ���������̉�]����
    if (gamePad.GetAxisRY())
    {
        angle.x += gamePad.GetAxisRY() * speed;  // Y���̓��͂��g���Đ�����]

        // �ő�E�ŏ��p�x�̐���
        if (angle.x > maxAngleX)
        {
            angle.x = maxAngleX;  // ������ɐ���
        }

        if (angle.x < minAngleX)
        {
            angle.x = minAngleX;  // �������ɐ���
        }
    }
}

DirectX::XMFLOAT3 CameraController::TargetPosition(
    const DirectX::XMFLOAT3& playerPos,
    const DirectX::XMINT3& mapKey,
    float length,
    bool& isCubeView)  // �Ăяo�����ƘA�g���ď�Ԃ����L
{
    GamePad& gamepad = Input::Instance().GetGamePad();

    DirectX::XMFLOAT3 targetCubePos = { 0,0,0 };

    float distance = DirectX::XMVectorGetX(DirectX::XMVector3Length(
        DirectX::XMVectorSubtract(DirectX::XMLoadFloat3(&playerPos), DirectX::XMLoadFloat3(&targetCubePos))));

    if (!isCubeView && distance <= length && (gamepad.GetButtonDown() & GamePad::BTN_ENTER))
    {
        isCubeView = true;  // ��Ԃ��X�V
        return targetCubePos;
    }

    if (isCubeView && (gamepad.GetButtonDown() & GamePad::BTN_BACKSPACE))
    {
        isCubeView = false;  // ��Ԃ��X�V
        return playerPos;
    }

    return isCubeView ? targetCubePos : playerPos;
}