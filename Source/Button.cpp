#include "Button.h"
#include "ClockManager.h"
#include "Clock.h"
#include "Player.h"
#include "Collision.h"
#include "Input/Input.h"

Button::Button()
{
    buttonModel = std::make_unique<Model>("Data/Model/Button/button.mdl");
    scale.x = scale.y = scale.z = 0.005f;
}

void Button::Update(float elapsedTime)
{
    GamePad& gamePad = Input::Instance().GetGamePad();

    if (gamePad.GetButtonDown() & GamePad::BTN_1)
    {
        UpdatePushA(elapsedTime);
    }
    else if (gamePad.GetButtonDown() & GamePad::BTN_2)
    {
        UpdatePushB(elapsedTime);
    }
    else if (gamePad.GetButtonDown() & GamePad::BTN_3)
    {
        UpdatePushC(elapsedTime);
    }

    UpdateTransform();

    buttonModel->UpdateAnimation(elapsedTime);

    //行列更新処理
    buttonModel->UpdateTransform(transform);
}

void Button::Render(ID3D11DeviceContext* dc, Shader* shader)
{
    shader->Draw(dc, buttonModel.get());
}

void Button::Finalize()
{
    buttonModel.reset();
}


void Button::DrawDebugPrimitive()
{
    DebugRenderer* debugRenderer = Graphics::Instance().GetDebugRenderer();

    //判定衝突用のデバッグ球を描画
    //debugRenderer->DrawSphere(position, radius, DirectX::XMFLOAT4(0, 0, 0, 1));

    //衝突判定用のデバッグ円柱を描画
    debugRenderer->DrawCylinder(position, radius, height, DirectX::XMFLOAT4(0, 0, 0, 1));


}

void Button::TransitionPushA()
{
    state = State::BottonA;

    buttonModel->PlayAnimation(PushAnimationA, false);

}

void Button::TransitionPushB()
{
    state = State::BottonB;

    buttonModel->PlayAnimation(PushAnimationB, false);

}

void Button::TransitionPushC()
{
    state = State::BottonC;

    buttonModel->PlayAnimation(PushAnimationC, false);

}



void Button::UpdatePushA(float elapsedTime)
{
    TransitionPushC();
};
void Button::UpdatePushB(float elapsedTime)
{
    TransitionPushB();
}
void Button::UpdatePushC(float elapsedTime)
{
    TransitionPushA();
}DirectX::XMFLOAT3 Button::PlayerVsButtonTargetView(
    DirectX::XMFLOAT3& playerPos,
    DirectX::XMFLOAT3& buttonPos,
    float len,
    bool& isButtonView)
{
    GamePad& gamePad = Input::Instance().GetGamePad();

    // プレイヤーとボタン間の距離を計算
    float dist = DirectX::XMVectorGetX(DirectX::XMVector3Length(
        DirectX::XMVectorSubtract(DirectX::XMLoadFloat3(&playerPos), DirectX::XMLoadFloat3(&buttonPos))));

    // ボタン視点への切り替え
    if (!isButtonView && dist <= len && (gamePad.GetButtonDown() & GamePad::BTN_ENTER))
    {
        isButtonView = true;
        return buttonPos;
    }

    // プレイヤー視点への戻り
    if (isButtonView && (gamePad.GetButtonDown() & GamePad::BTN_BACKSPACE))
    {
        isButtonView = false;
        return playerPos;
    }

    // 現在の視点に応じて位置を返す
    return isButtonView ? buttonPos : playerPos;
}

