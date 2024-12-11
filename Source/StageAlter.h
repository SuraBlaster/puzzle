#pragma once

#include "Graphics/Model.h"
#include "Stage.h"


class StageAlter : public Stage
{
public:
    StageAlter();
    ~StageAlter()override;

    //�X�V����
    void Update(float elapsedTime)override;

    //�`�揈��
    void Render(ID3D11DeviceContext* dc, Shader* shader)override;

    //���C�L���X�g
    bool RayCast(const DirectX::XMFLOAT3& start,
        const DirectX::XMFLOAT3& end, HitResult& hit)override;

    //�m�[�h�ƃv���C���[�̏Փˏ���
    bool CollisionNodeVsPlayer(const char* nodeName, float nodeRadius);

    //���̒ʏ��ԍX�V����
    void UpdateIdleState();

    //���̕���X�V����
    void UpdateCloseState();

    //���̊J���X�V����
    void UpdateOpenState();

    

private:
    Model* model = nullptr;

    //�A�j���[�V����
    enum Animation
    {
        Anim_Close,
        Anim_Open,
    };

    //�X�e�[�g
    enum class State
    {
        Idle,
        Close,
        Open,
    };

    State state = State::Idle;

    //�ʏ�X�e�[�g�ɑJ��
    void IdleState();

    //����X�e�[�g�ɑJ��
    void CloseState();

    //�J���X�e�[�g�ɑJ��
    void OpenState();
};
