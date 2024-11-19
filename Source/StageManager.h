#pragma once

#include <vector>
#include "Stage.h"

class StageManager
{
private:
    StageManager(){}
    ~StageManager(){}

public:
    //�B��̃C���X�^���X�擾
    static StageManager& Instance()
    {
        static StageManager instance;
        return instance;
    }

    //�s��X�V����
    void UpdateTransform();
    
    //�X�V����
    void Update(float elapsedTime);

    //�X�V����
    void Render(ID3D11DeviceContext* dc, Shader* shader);

    //�X�e�[�W�o�^
    void Register(Stage* stage);

    //�X�e�[�W�S�폜
    void Clear();

    //���C�L���X�g
    bool RayCast(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end,
        HitResult& hit);

private:
    std::vector<Stage*> stages;

    DirectX::XMFLOAT3 position = { 0, 0, 0 };
    DirectX::XMFLOAT3 angle = { 0, 0, 0 };
    DirectX::XMFLOAT3 scale = { 1, 1, 1 };
    DirectX::XMFLOAT4X4 transform{
        1,0,0,0,
        0,1,0,0,
        0,0,1,0,
        0,0,0,1
    };
};