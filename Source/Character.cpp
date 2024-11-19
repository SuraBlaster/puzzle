#include "Character.h"
#include "Mathf.h"
#include "StageManager.h"

//�s��X�V����
void Character::UpdateTransform()
{
    //�X�P�[���s����쐬
    DirectX::XMMATRIX S = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);
    //��]�s����쐬
    //DirectX::XMMATRIX R = DirectX::XMMatrixRotationRollPitchYaw(angle.x, angle.y, angle.z);
    DirectX::XMMATRIX X = DirectX::XMMatrixRotationX(angle.x);
    DirectX::XMMATRIX Y = DirectX::XMMatrixRotationY(angle.y);
    DirectX::XMMATRIX Z = DirectX::XMMatrixRotationZ(angle.z);
    DirectX::XMMATRIX R = Y * X * Z;
    //�ʒu�s����쐬
    DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(position.x, position.y, position.z);
    //3�̍s���g�ݍ��킹�A���[���h�s����쐬
    DirectX::XMMATRIX W = S * R * T;
    //�v�Z�������[���h���W�����o��
    DirectX::XMStoreFloat4x4(&transform, W);
}

bool Character::ApplyDamage(int damege)
{

    //���S���Ă���ꍇ�͌��N��Ԃ�ύX���Ȃ�
    if (health <= 0)return false;

    if (invincibleTime <= 0)
    {
        health -= damege;
        invincibleTime += 0.5f;
    }

    if (health <= 0)
    {
        OnDead();
    }
    else
    {
        OnDamaged();
    }

    return true;
}

void Character::AddImpulse(const DirectX::XMFLOAT3& impulse)
{
    velocity.x += impulse.x;
    velocity.y += impulse.y;
    velocity.z += impulse.z;

}

void Character::Move(float vx, float vz, float speed)
{
    // �ړ�����
   /* speed *= elapsedTime;
    position.x += vx * speed;
    position.z += vz * speed;*/

    moveVecX = vx;
    moveVecZ = vz;

    maxMoveSpeed = speed;

}

void Character::Turn(float elapsedTime, float vx, float vz, float speed)
{
    speed *= elapsedTime;

    //�������������ꍇ�͌v�Z���Ȃ�
    float length = sqrtf(vx * vx + vz * vz);
    if (length < 0.001f)return;

    //�P�ʃx�N�g����
    vx /= length;
    vz /= length;

    //�O���������߂�
    float frontX = sinf(angle.y);
    float frontZ = cosf(angle.y);

    //����
    float dot = (vx * frontX) + (vz * frontZ);

    //�␳�l
    float rot = 1.0 - dot;

    //��]���x����
    //speed *= rot;
    if (rot > speed)rot = speed;

    //�P�ʃx�N�g���̊O��
    float cross = (vz * frontX) - (vx * frontZ);

    if (cross < 0.0f)
    {
        //angle.y += speed;
        angle.y += rot;
    }
    else
    {
        //angle.y -= speed;
        angle.y -= rot;
    }

}

//�W�����v����
void Character::Jump(float speed)
{
    velocity.y += speed;
}

//���x�X�V����
void Character::UpdateVelocity(float elapsedTime)
{
    //�o�߃t���[��
    float elapsedFrame = 60.0f * elapsedTime;

    //�������͍X�V����
    UpdateVerticalVelocity(elapsedFrame);

    //�������͍X�V����
    UpdateHorizontalVelocity(elapsedFrame);

    //�����ړ��X�V����
    UpdateVerticalMove(elapsedTime);

    //�����ړ��X�V����
    UpdateHorizontalMove(elapsedTime);
}

void Character::UpdateInvincibleTimer(float elapsedTime)
{
    if (invincibleTime > 0.0f)
    {
        invincibleTime -= elapsedTime;
    }
}

void Character::UpdateVerticalVelocity(float elapsedFrame)
{
    //�d�͏���
    velocity.y += gravity * elapsedFrame;
}

void Character::UpdateVerticalMove(float elapsedTime)
{
    float my = velocity.y * elapsedTime;

    DirectX::XMFLOAT3 normal = { 0,1,0 };

    if (my < 0.0f)
    {
        DirectX::XMFLOAT3 start = { position.x, position.y + stepOffset, position.z };

        DirectX::XMFLOAT3 end = { position.x, position.y + my, position.z };

        HitResult hit;
        if (StageManager::Instance().RayCast(start, end, hit))
        {
            position = hit.position;
            normal = hit.normal;
            


            if (!isGround)
            {
                OnLanding();
                
            }
            isGround = true;
            velocity.y = 0.0f;

            angle.x += hit.rotation.x;
            angle.y += hit.rotation.y;
            angle.z += hit.rotation.z;

        }
        else
        {
            position.y += my;
            //isGround = false;
        }

        
    }
    else if (my > 0.0f)
    {
        position.y += my;
        isGround = false;
    }

    //�n�ʂ̌����ɉ����悤��XZ����]
    {
        //Y�����@���x�N�g�������Ɍ����悤�ɃI�C���[�p��]���Z�o����
        DirectX::XMFLOAT3 test = { angle.x,angle.y,angle.z };

        test.x = atan2(normal.z, normal.y);
        test.z = atan2(-normal.x, normal.y);

    
        //���`�⊮�Ŋ��炩�ɉ�]����
        angle.x = Mathf::Lerp(angle.x, test.x, 0.1f);
        angle.z = Mathf::Lerp(angle.z, test.z, 0.1f);

    }
}

void Character::UpdateHorizontalVelocity(float elapsedFrame)
{

    float length = sqrtf(velocity.x * velocity.x + velocity.z * velocity.z);

    if (length > 0.0f)
    {
        float friction = this->friction * elapsedFrame;

        //�󒆂ɂ���Ƃ��͖��C�͂����炷
        //if (!&IsGround) friction *= airControl;

        if (length > friction)
        {
            float vx = velocity.x / length;
            float vz = velocity.z / length;

            velocity.x -= vx * friction;
            velocity.z -= vz * friction;
        }
        else
        {
            velocity.x = 0.0f;
            velocity.z = 0.0f;
        }
    }

    //XZ���ʂ̑��͉���
    if (length <= maxMoveSpeed)
    {
        float moveVecLength = sqrtf(moveVecX * moveVecX + moveVecZ * moveVecZ);

        if (moveVecLength > 0.0f)
        {
            //������
            float acceleration = this->acceleration * elapsedFrame;

            //�󒆂ɂ���Ƃ��͉����͂����炷
            //if (!&IsGround) friction *= airControl;

            //�ړ��x�N�g���ɂ���������
            velocity.x += moveVecX * acceleration;
            velocity.z += moveVecZ * acceleration;

            //�ő呬�x����
            float length = sqrtf(velocity.x * velocity.x + velocity.z * velocity.z);

            if (length > maxMoveSpeed)
            {
                float vx = velocity.x / length;
                float vz = velocity.z / length;

                velocity.x = vx * maxMoveSpeed;
                velocity.z = vz * maxMoveSpeed;
            }
        }
    }
    moveVecX = 0.0f;
    moveVecZ = 0.0f;
}

void Character::UpdateHorizontalMove(float elapsedTime)
{
    //�������͗ʌv�Z
    float velocityLengthXZ = (velocity.x * velocity.x + velocity.z * velocity.z);
    if (velocityLengthXZ > 0.0f)
    {
        //�����ړ��l
        float mx = velocity.x * elapsedTime;
        float mz = velocity.z * elapsedTime;

        //���C�̊J�n�ʒu�ƏI�_�ʒu
        DirectX::XMFLOAT3 start = { position.x,position.y + stepOffset,position.z };
        DirectX::XMFLOAT3 end = { position.x + mx,position.y,position.z + mz };

        //���C�L���X�g�ɂ��ǔ���
        HitResult hit;
        if (StageManager::Instance().RayCast(start, end, hit))
        {
            //�ǂ܂ł̃x�N�g��
            DirectX::XMVECTOR P = DirectX::XMLoadFloat3(&hit.position);
            DirectX::XMVECTOR E = DirectX::XMLoadFloat3(&end);
            DirectX::XMVECTOR PE = DirectX::XMVectorSubtract(E,P);

            //�ǂ̖@��
            DirectX::XMVECTOR N = DirectX::XMLoadFloat3(&hit.normal);
            DirectX::XMVECTOR A = DirectX::XMVector3Dot(DirectX::XMVectorNegate(PE), N);
            float a = DirectX::XMVectorGetX(A) + 0.001f;

            //�Ǎ���x�N�g��
            DirectX::XMVECTOR R = DirectX::XMVectorAdd(PE, DirectX::XMVectorScale(N, a));

            //�ǂ����̈ʒu����
            DirectX::XMVECTOR Q = DirectX::XMVectorAdd(P, R);
            DirectX::XMFLOAT3 q;
            DirectX::XMStoreFloat3(&q, Q);
            
            if (StageManager::Instance().RayCast(start, q, hit))
            {
                P = DirectX::XMLoadFloat3(&hit.position);
                DirectX::XMVECTOR S = DirectX::XMLoadFloat3(&start);
                DirectX::XMVECTOR PS = DirectX::XMVectorSubtract(S, P);
                DirectX::XMVECTOR V = DirectX::XMVector3Normalize(PS);
                P = DirectX::XMVectorAdd(P, DirectX::XMVectorScale(V, 0.001f));
                DirectX::XMStoreFloat3(&hit.position, P);
                position.x = hit.position.x;
                position.z = hit.position.z;
            }
            else
            {
                position.x = q.x;
                position.z = q.z;
            }
        }
        else
        {
            //�ړ�
            position.x += mx;
            position.z += mz;
        }
    }


}
