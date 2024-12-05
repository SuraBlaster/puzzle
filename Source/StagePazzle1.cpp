#include "StagePazzle1.h"
#include <Input/Input.h>
StagePazzle1::StagePazzle1()
{
    model = new Model("Data/Model/Stage/Pazzle1.mdl");

    scale.x = scale.y = scale.z = 0.005f;

    position.x = 0;

    position.y = 0;
}

StagePazzle1::~StagePazzle1()
{
    delete model;
}

void StagePazzle1::Update(float elapsedTime)
{
    GamePad& gamePad = Input::Instance().GetGamePad();


    if (gamePad.GetButtonDown() & GamePad::KEY_W)
    {
        float angleX = DirectX::XMConvertToDegrees(angle.x);
        angleX += 90;
        angle.x = DirectX::XMConvertToRadians(angleX);
    }

    if (gamePad.GetButtonDown() & GamePad::BTN_DOWN)
    {
        float angleX = DirectX::XMConvertToDegrees(angle.x);
        angleX-= 90;
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

    

    UpdateTransform();

    model->UpdateTransform(transform);
}

void StagePazzle1::Render(ID3D11DeviceContext* dc, Shader* shader)
{
    shader->Draw(dc, model);
}

bool StagePazzle1::RayCast(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, HitResult& hit)
{
    return false;
}
