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
    //�X�v���C�g������
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

    //��ʃN���A&�����_�[�^�[�Q�b�g�ݒ�
    FLOAT color[] = { 0.0f,0.0f,0.5f,1.0f };
    dc->ClearRenderTargetView(rtv, color);
    dc->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
    dc->OMSetRenderTargets(1, &rtv, dsv);

    //2D�X�v���C�g�`��
    {
        float screenWidth = static_cast<float>(graphics.GetScreenWidth());
        float screenHeight = static_cast<float>(graphics.GetScreenHeight());
        float textureWidth = static_cast<float>(bg->GetTextureWidth());
        float textureHeight = static_cast<float>(bg->GetTextureHeight());
        //�^�C�g���X�v���C�g�`��
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
        //�^�C�g���X�v���C�g�`��
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
