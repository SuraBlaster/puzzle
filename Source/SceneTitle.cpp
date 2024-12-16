#include "SceneTitle.h"
#include "SceneGame.h"
#include "Graphics/Graphics.h"
#include "SceneManager.h"
#include "Input/Input.h"
#include "SceneLoading.h"
#include "midleStage.h"
#include "Audio/Audio.h"
#include "SceneAdvanced.h"
#include "SceneCube.h"

void SceneTitle::Initialize()
{
    //スプライト初期化
    bg = new Sprite("Data/Sprite/title.jpg");
    logo = new Sprite("Data/Sprite/Title.png");
    textPush = new Sprite("Data/Sprite/PushAnyKey.png");
    TitleBGM = Audio::Instance().LoadAudioSource("Data/Audio/SceneTitle.wav");
    TitleBGM->Play(true);
}

void SceneTitle::Finalize()
{
    if (bg != nullptr)
    {
        delete bg;
        bg = nullptr;
    }
}

void SceneTitle::Update(float elapsedTime)
{
    GamePad& gamepad = Input::Instance().GetGamePad();


    if (gamepad.GetButtonDown() & GamePad::BTN_ENTER)
    {
        SceneManager::Instance().ChangeScene(new SceneLoading(new SceneCube));
    }

}

void SceneTitle::Render()
{
    Graphics& graphics = Graphics::Instance();
    ID3D11DeviceContext* dc = graphics.GetDeviceContext();
    ID3D11RenderTargetView* rtv = graphics.GetRenderTargetView();
    ID3D11DepthStencilView* dsv = graphics.GetDepthStencilView();

    //画面クリア&レンダーターゲット設定
    FLOAT color[] = { 0.0f,0.0f,0.5f,1.0f };
    dc->ClearRenderTargetView(rtv, color);
    dc->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
    dc->OMSetRenderTargets(1, &rtv, dsv);

    //2Dスプライト描画
    {
        float screenWidth = static_cast<float>(graphics.GetScreenWidth());
        float screenHeight = static_cast<float>(graphics.GetScreenHeight());
        float textureWidth = static_cast<float>(bg->GetTextureWidth());
        float textureHeight = static_cast<float>(bg->GetTextureHeight());
        //タイトルスプライト描画
        bg->Render(dc,
            0, 0, screenWidth, screenHeight,
            0, 0, textureWidth, textureHeight,
            0,
            1, 1, 1, 1);

    }

    {
        float screenWidth = static_cast<float>(graphics.GetScreenWidth() / 2);
        float screenHeight = static_cast<float>(graphics.GetScreenHeight());
        float textureWidth = static_cast<float>(logo->GetTextureWidth());
        float textureHeight = static_cast<float>(logo->GetTextureHeight());
        //タイトルスプライト描画
        logo->Render(dc,
            430, 0, 1024, 1024,
            0, 0, textureWidth, textureHeight,
            0,
            1, 1, 1, 0.9f);

    }

    {
        textPush->Render(dc,
            700, 700, textPush->GetTextureWidth(), textPush->GetTextureHeight(),
            0, 0, textPush->GetTextureWidth(), textPush->GetTextureHeight(),
            0,
            1, 1, 1, 1);
    }

}
