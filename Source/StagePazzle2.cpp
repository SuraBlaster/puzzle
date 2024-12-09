#include "StagePazzle2.h"
#include "EraManager.h"
#include <Input/Input.h>

StagePazzle2::StagePazzle2()
{
    model = new Model("Data/Model/Stage/Pazzle2.mdl");

    scale.x = scale.y = scale.z = 0.0f;

    position.x = 0;

    position.y = 2;
}

StagePazzle2::~StagePazzle2()
{
    delete model;
}

void StagePazzle2::Update(float elapsedTime)
{
    GamePad& gamePad = Input::Instance().GetGamePad();
    if (gamePad.GetButtonDown() & GamePad::BTN_2)
    {
        EraManager::Instance().SetPazzle(Stage::Pazzle::Pazzle2);
        EraManager::Instance().SetPazzle2(false);
        scale = { 0.005f,0.005f,0.005f };
    }

    if (EraManager::Instance().GetPazzle() == Stage::Pazzle::Pazzle2)
    {
        InputMove();

        if (gamePad.GetButtonDown() & GamePad::BTN_ENTER)
        {
            EraManager::Instance().SetPazzle2(true);
            EraManager::Instance().SetPazzle2Position(position);
        }
    }
    else if (!(EraManager::Instance().GetPazzle() == Stage::Pazzle::Pazzle2) && EraManager::Instance().GetPazzle2() == false)
    {
        scale = { 0.0f,0.0f,0.0f };
    }

    UpdateTransform();

    model->UpdateTransform(transform);
}

void StagePazzle2::Render(ID3D11DeviceContext* dc, Shader* shader)
{
    shader->Draw(dc, model);
}

bool StagePazzle2::RayCast(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, HitResult& hit)
{
    return false;
}

void StagePazzle2::InputMove()
{
    GamePad& gamePad = Input::Instance().GetGamePad();
    float currentax = gamePad.GetAxisLX();
    float currentay = gamePad.GetAxisLY();

    if (prevAxisLY == 0.0f && abs(currentay) > 0.1f)
    {
        if (currentay < 0)
        {
            position.z -= 1;
        }
        else
        {
            position.z += 1;
        }
    }

    prevAxisLY = currentay;

    if (prevAxisLX == 0.0f && abs(currentax) > 0.1f)
    {
        if (currentax < 0)
        {
            position.x -= 1;
        }
        else
        {
            position.x += 1;
        }
    }

    prevAxisLX = currentax;

    if (gamePad.GetButtonDown() & GamePad::BTN_UP)
    {
        float angleX = DirectX::XMConvertToDegrees(angle.x);
        angleX += 90;
        angle.x = DirectX::XMConvertToRadians(angleX);
    }

    if (gamePad.GetButtonDown() & GamePad::BTN_DOWN)
    {
        float angleX = DirectX::XMConvertToDegrees(angle.x);
        angleX -= 90;
        angle.x = DirectX::XMConvertToRadians(angleX);
    }

    if (gamePad.GetButtonDown() & GamePad::BTN_LEFT)
    {
        float angleY = DirectX::XMConvertToDegrees(angle.y);
        angleY -= 90;
        angle.y = DirectX::XMConvertToRadians(angleY);
    }

    if (gamePad.GetButtonDown() & GamePad::BTN_RIGHT)
    {
        float angleY = DirectX::XMConvertToDegrees(angle.y);
        angleY += 90;
        angle.y = DirectX::XMConvertToRadians(angleY);
    }
}