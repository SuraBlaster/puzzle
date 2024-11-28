#pragma once

#include <vector>
#include "Stage.h"
#include <set>

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
    
    //�X�V����
    void Update(float elapsedTime);

    //�X�V����
    void Render(ID3D11DeviceContext* dc, Shader* shader);

    //�X�e�[�W�o�^
    void Register(Stage* stage);

    //�X�e�[�W�S�폜
    void Clear();

    //�w��̃X�e�[�W����
    void Remove(Stage* stage);

    //���C�L���X�g
    bool RayCast(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end,
        HitResult& hit);

    

private:
    std::vector<Stage*> stages;

    std::set<Stage*> removes;
};