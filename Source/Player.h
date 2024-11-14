#pragma once

#include "Graphics/Shader.h"
#include "Graphics/Model.h"
#include "Character.h"
#include "ProjectileManager.h"
#include "Effect.h"

class Player : public Character
{
private:
    DirectX::XMFLOAT3 GetMoveVec() const;

    //�ړ����͏���
    bool InputMove(float elapsedTime);

    //�U�����͏���
    bool InputAttack();

    void InputProjectile();

    void CollisionPlayerVsEnemies();

    //�ҋ@�X�e�[�g�֑J��
    void TransitionIdleState();

    //�ҋ@�X�e�[�g�X�V����
    void UpdateIdleState(float elapsedTime);

    //�ړ��X�e�[�g�֑J��
    void TransitionMoveState();

    //�ړ��X�e�[�g�X�V����
    void UpdateMoveState(float elapsedTime);

    //�W�����v�X�e�[�g�֑J��
    void TransitionJumpState();

    //�W�����v�X�e�[�g�X�V����
    void UpdateJumpState(float elapsedTime);

    //���n�X�e�[�g�֑J��
    void TransitionLandState();

    //���n�X�e�[�g�X�V����
    void UpdateLandState(float elapsedTime);

    //�U���X�e�[�g�ɑJ��
    void TransitionAttackState();

    //�U���X�e�[�g�X�V����
    void UpdateAttackState(float elapsedTime);

    //�_���[�W�X�e�[�g�ɑJ��
    void TransitionDamageState();

    //�_���[�W�X�e�[�g�X�V����
    void UpdateDamageState(float elapsedTime);

    //���S�X�e�[�g�ɑJ��
    void TransitionDeathState();

    //���S�X�e�[�g�X�V����
    void UpdateDeathState(float elapsedTime);

    //�����X�e�[�g�ɑJ��
    void TransitionReviveState();

    //�����X�e�[�g�X�V����
    void UpdateReviveState(float elapsedTime);

    //�m�[�h�ƃG�l�~�[�̏Փˏ���
    void CollisionNodeVsEnemies(const char* nodeName, float nodeRadius);

protected:
    //�_���[�W���󂯂����ɌĂ΂��
    void OnDamaged()override;

    //���S�����Ƃ��ɌĂ΂��
    void OnDead()override;

public:
    Player();
    ~Player()override;

    //�C���X�^���X�擾
    static Player& Instance();

    //�X�V����
    void Update(float elapsedTime);

    //�`�揈��
    void Render(ID3D11DeviceContext* dc, Shader* shader);

    //GUI�`��
    void DrawDebugGUI();

    //�f�o�b�O�v���~�e�B�u�`��
    void DrawDebugPrimitive();

    //�W�����v���͏���
    bool InputJump();

    //���n�����Ƃ��ɌĂ΂��
    void OnLanding() override;

    void CollisionprojectilesVsEnemies();

    //�����߂�
    void Player::Rewind(float elapsedTime,float rewindTime);
   
    Model& GetModel() { return *model; }

private:
    //�A�j���[�V����
    enum Animation
    {
        Anim_Attack,
        Anim_Death,
        Anim_Falling,
        Anim_GetHit1,
        Anim_GetHit2,
        Anim_Idle,
        Anim_Jump,
        Anim_Jump_Flip,
        Anim_Landing,
        Anim_Revive,
        Anim_Running,
        Anim_Walking,
    };

    enum class State
    {
        Idle,
        Move,
        Jump,
        Land,
        Attack,
        Damage,
        Death,
        Revive,
    };
private:
    ProjectileManager projectileManager;

    Model* model = nullptr;

    Effect* hitEffect = nullptr;

    bool attackCollisionFlag = false;

    float moveSpeed = 5.0f;

    float turnSpeed = DirectX::XMConvertToRadians(720);

    float jumpSpeed = 20.0f;

    int   jumpCount = 0;

    int   jumpLimit = 2;

    float leftHandRadius = 0.4f;

    State state = State::Idle;

};