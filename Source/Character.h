#pragma once

#include <DirectXMath.h>
#include <Graphics/Model.h>
#include <cmath>
class Character
{
public:
    Character() = default;
    virtual ~Character() = default;

    void UpdateTransform();

    //�ʒu�擾
    const DirectX::XMFLOAT3& GetPosition() const { return position; }

    //�ʒu�ݒ�
    void SetPosition(const DirectX::XMFLOAT3& position) { this->position = position; }

    //��]�擾
    const DirectX::XMFLOAT3& GetAngle() const { return angle; }

    //��]�ݒ�
    void SetAngle(const DirectX::XMFLOAT3& angle) { this->angle = angle; }

    //�X�P�[���ݒ�
    const DirectX::XMFLOAT3& GetScale() const { return scale; }

    //�X�P�[���擾
    void SetScale(const DirectX::XMFLOAT3& scale) { this->scale = scale; }

    //���a�擾
    float GetRadius() const { return radius; }

    DirectX::XMFLOAT3 GetForwardPosition(float distance) {
        using namespace DirectX;

        // Yaw�i������]�j�Ɋ�Â��ĕ����x�N�g�����v�Z
        XMVECTOR directionVec = XMVectorSet(
            sinf(angle.y), // X����
            0.0f,               // Y�����i�����ړ��̂݁j
            cosf(angle.y), // Z����
            0.0f                // W����
        );

        // �����x�N�g���𐳋K��
        directionVec = XMVector3Normalize(directionVec);

        // �����x�N�g���ɋ������|����
        XMVECTOR forwardVec = XMVectorScale(directionVec, distance);

        // ���݈ʒu���x�N�g���ɕϊ�
        XMVECTOR positionVec = XMLoadFloat3(&position);

        // ���݈ʒu�Ɉړ��������Z
        XMVECTOR resultVec = XMVectorAdd(positionVec, forwardVec);

        // ���ʂ�XMFLOAT3�ɕϊ����ĕԂ�
        XMFLOAT3 forwardPosition;
        XMStoreFloat3(&forwardPosition, resultVec);

        return forwardPosition;
    }
    
    
    //�n�ʂɐݒu���Ă��邩
    bool IsGround() const { return isGround; }

    //�_���[�W��^����
    bool ApplyDamage(int damege);

    //�����擾
    float GetHeight() const { return height; }

    //�Ռ���^����
    void AddImpulse(const DirectX::XMFLOAT3& impulse);

    //���N��Ԃ��擾
    int GetHealth() const { return health; }

    //�ő匒�N��Ԃ��擾
    int GetMaxHealth() const { return maxHealth; }

protected:
    //�ړ�����
    void Move(float vx, float vz, float speed);

    //��]����
    void Turn(float elapsedTime, float vx, float vz, float speed);

    //�W�����v����
    void Jump(float speed);

    //���͏����X�V
    void UpdateVelocity(float elapsedTime);

    void UpdateInvincibleTimer(float elapsedTime);
    //���n�����Ƃ��ɌĂ΂��
    virtual void OnLanding() {}

    //�_���[�W���󂯂����ɌĂ΂��
    virtual void OnDamaged(){}

    //���S���ɌĂ΂��
    virtual void OnDead(){}

    

private:
    //�������͍X�V����
    void UpdateVerticalVelocity(float elapsedFrame);

    //�����ړ��X�V����
    void UpdateVerticalMove(float elapsedTime);

    //�������͍X�V����
    void UpdateHorizontalVelocity(float elapsedFrame);

    //�����ړ��X�V����
    void UpdateHorizontalMove(float elapsedTime);
protected:
    DirectX::XMFLOAT3 position = { 0, 0, 0 };
    DirectX::XMFLOAT3 angle    = { 0, 0, 0 };
    DirectX::XMFLOAT3 scale    = { 1, 1, 1 };
    DirectX::XMFLOAT4X4 transform{
        1,0,0,0,
        0,1,0,0,
        0,0,1,0,
        0,0,0,1
    };
    float radius = 0.5f;
    
    float gravity = -1.0f;

    DirectX::XMFLOAT3 velocity = { 0,0,0 };

    bool isGround = false;

    float height = 2.0f;

    int health = 5;

    int maxHealth = 5;

    float invincibleTimer = 1.0f;

    float invincibleTime = 0.0f;

    float friction = 0.5f;

    float acceleration = 1.0f;

    float maxMoveSpeed = 5.0f;

    float moveVecX = 0.0f;

    float moveVecZ = 0.0f;

    float airControl = 0.3f;

    float stepOffset = 1.0f;

    float slopeRate = 1.0f;
};





