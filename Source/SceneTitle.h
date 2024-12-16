#pragma once

#include "Graphics/Sprite.h"
#include "Scene.h"
#include "Audio/AudioSource.h"

class SceneTitle : public Scene
{
public:
    SceneTitle() {}
    ~SceneTitle()override {}

    //‰Šú‰»
    void Initialize()override;

    //I—¹‰»
    void Finalize()override;

    //XVˆ—
    void Update(float elapsedTime)override;

    //•`‰æˆ—
    void Render()override;

private:
    Sprite* bg = nullptr;
    Sprite* logo = nullptr;
    Sprite* textPush = nullptr;
    std::unique_ptr<AudioSource> TitleBGM;
};
