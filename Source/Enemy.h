#pragma once

#include "Graphics/Shader.h"
#include "Character.h"

class Enemy : public Character
{
public:
    Enemy() {}
    ~Enemy() override {}

    //�X�V����
    virtual void Update(float elapsedTime) = 0;

    //�`�揈��
    virtual void Render(ID3D11DeviceContext* dc, Shader* shader) = 0;

    virtual void DrawDebugPrimitive();

    void Destroy();

};
