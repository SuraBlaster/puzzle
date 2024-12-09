#pragma once

#include "Graphics/Shader.h"
#include "Graphics/Model.h"
#include "Character.h"
#include "ProjectileManager.h"
#include "Effect.h"

class Player : public Character
{
public:
    DirectX::XMFLOAT3 GetMoveVec() const;

private:

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

    //�m�[�h�ƃG�l�~�[�̏Փˏ���
    void CollisionNodeVsEnemies(const char* nodeName, float nodeRadius);

protected:
    

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

    void CollisionprojectilesVsEnemies();

    //�����߂�
    void Player::Rewind(float elapsedTime,float rewindTime);
   
    Model& GetModel() { return *model; }


private:
    //�A�j���[�V����
    enum Animation
    {
        Anim_Idle,
        Anim_Walking,
    };

    enum class State
    {
        Idle,
        Move,
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