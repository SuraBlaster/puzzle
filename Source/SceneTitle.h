#pragma once

#include "Graphics/Sprite.h"
#include "Scene.h"
#include "Audio/AudioSource.h"

class SceneTitle : public Scene
{
public:
    SceneTitle() {}
    ~SceneTitle()override {}

    //������
    void Initialize()override;

    //�I����
    void Finalize()override;

    //�X�V����
    void Update(float elapsedTime)override;

    //�`�揈��
    void Render()override;

private:
    Sprite* bg = nullptr;
    Sprite* logo = nullptr;
    Sprite* textPush = nullptr;
    std::unique_ptr<AudioSource> TitleBGM;
};
