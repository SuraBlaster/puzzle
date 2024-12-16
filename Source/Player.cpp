#include "Player.h"
#include <imgui.h>
#include "Input/Input.h"
#include "Camera.h"
#include "Graphics/Graphics.h"
#include "Collision.h"
#include "ProjectileStraight.h"
#include <ProjectileHoming.h>
#include "RewindManager.h"
static Player* instance = nullptr;

Player& Player::Instance()
{
    return *instance;
}

//�R���X�g���N�^
Player::Player()
{
    model = new Model("Data/Model/Player/Player.mdl");

    //�C���X�^���X�|�C���^�擾
    instance = this;

    //�X�P�[�����O
    scale.x = scale.y = scale.z = 0.7f;

    position.z = -3;

    //�q�b�g�G�t�F�N�g�ǂݍ���
    hitEffect = new Effect("Data/Effect/thunder.efk");

    //�ҋ@�X�e�[�g�֑J��
    TransitionIdleState();
}

void Player::DrawDebugPrimitive()
{
    DebugRenderer* debugRenderer = Graphics::Instance().GetDebugRenderer();
    
    //�Փ˔���p�̃f�o�b�O�~����`��
    //debugRenderer->DrawCylinder(position, radius, height, DirectX::XMFLOAT4(0, 0, 0, 1));

    //projectileManager.DrawDebugPrimitive();

    //�U���Փ˗p�̍���m�[�h�̃f�o�b�O����`��
    if (attackCollisionFlag)
    {
        Model::Node* leftHandBone = model->FindNode("mixamorig:LeftHand");
        debugRenderer->DrawSphere(DirectX::XMFLOAT3(
            leftHandBone->worldTransform._41,
            leftHandBone->worldTransform._42,
            leftHandBone->worldTransform._43),
            leftHandRadius,
            DirectX::XMFLOAT4(1, 0, 0, 1)
        );
    }
    
}


Player::~Player()
{
    delete hitEffect;

    if (model != nullptr)
    {
        delete model;
        model = nullptr;
    }
    
}

//�X�V����
void Player::Update(float elapsedTime)
{
    GamePad& gamePad = Input::Instance().GetGamePad();

    //C�L�[���������Ƃ�
    if (gamePad.GetButton() & GamePad::BTN_X)
    {
        //�����߂�����
        RewindManager::Instance().RewindTime(elapsedTime,0.1f, Player::GetModel());
        position = RewindManager::Instance().currentState.position;
        angle = RewindManager::Instance().currentState.angle;
        velocity = RewindManager::Instance().currentState.velocity;
    }
    else if (gamePad.GetButtonUp())
    {
        if (InputMove(elapsedTime))
        {
            Player::GetModel().PlayAnimation(Anim_Walking, true);
        }
        else
        {
            Player::GetModel().PlayAnimation(Anim_Idle, true);
        }     
    }
    else
    {
        //0.1�b���Ƃ�position,angle,velocity,animation��ۑ�
        RewindManager::Instance().Update(elapsedTime, position, angle, velocity, Player::GetModel());
    }


    switch (state)
    {
    case State::Idle:
        UpdateIdleState(elapsedTime);
        break;
    case State::Move:
        UpdateMoveState(elapsedTime);
        break;
    }

    UpdateTransform();
    
    //���͑��x�X�V
    UpdateVelocity(elapsedTime);

    UpdateInvincibleTimer(elapsedTime);

    projectileManager.Update(elapsedTime);

    model->UpdateAnimation(elapsedTime);

    model->UpdateTransform(transform);
}

bool Player::InputMove(float elapsedTime)
{
    //�i�s�x�N�g���擾
    DirectX::XMFLOAT3 moveVec = GetMoveVec();

    Move(moveVec.x, moveVec.z, moveSpeed);

    Turn(elapsedTime, moveVec.x ,moveVec.z, turnSpeed);

    return moveVec.x != 0 || moveVec.z != 0;
}

bool Player::InputAttack()
{
    GamePad& gamePad = Input::Instance().GetGamePad();

    if (gamePad.GetButtonDown() & GamePad::BTN_B)
    {
        return true;
    }
    return false;
}







void Player::TransitionIdleState()
{
    state = State::Idle;

    //�ҋ@�A�j���[�V�����Đ�
    model->PlayAnimation(Anim_Idle, true);
}

void Player::UpdateIdleState(float elapsedTime)
{

    if (InputMove(elapsedTime))
    {
        TransitionMoveState();
    }

    //�ړ����͏���
    InputMove(elapsedTime);

}

void Player::TransitionMoveState()
{
    state = State::Move;

    //����A�j���[�V�����Đ�
    model->PlayAnimation(Anim_Walking, true);
}

void Player::UpdateMoveState(float elapsedTime)
{
    if (!InputMove(elapsedTime))
    {
        TransitionIdleState();
    }

    //�ړ����͏���
    InputMove(elapsedTime);
 
}








//�W�����v����
bool Player::InputJump()
{
    GamePad& gamePad = Input::Instance().GetGamePad();
    if (gamePad.GetButtonDown() & GamePad::BTN_A)
    {
        if (jumpCount < jumpLimit)
        {
            jumpCount++;
            Jump(jumpSpeed);

            return true;
            
        }
        
    }
    return false;
}



void Player::Rewind(float elapsedTime,float rewindTime)
{
   
}

//�`�揈��
void Player::Render(ID3D11DeviceContext* dc, Shader* shader)
{
    shader->Draw(dc, model);

    projectileManager.Render(dc, shader);
}

void Player::DrawDebugGUI()
{


    
}

DirectX::XMFLOAT3 Player::GetMoveVec()const
{
    GamePad& gamePad = Input::Instance().GetGamePad();
    float ax = gamePad.GetAxisLX();
    float ay = gamePad.GetAxisLY();

    Camera& camera = Camera::Instance();
    const DirectX::XMFLOAT3& cameraRight = camera.GetRight();
    const DirectX::XMFLOAT3& cameraFront = camera.GetFront();
  
    float cameraRightX = cameraRight.x;
    float cameraRightZ = cameraRight.z;
    float cameraRightLength = sqrtf(cameraRightX * cameraRightX + cameraRightZ * cameraRightZ);

    if (cameraRightLength > 0.0f)
    {
        cameraRightX /= cameraRightLength;
        cameraRightZ /= cameraRightLength;
    }

    float cameraFrontX = cameraFront.x;
    float cameraFrontZ = cameraFront.z;
    float cameraFrontLength = sqrtf(cameraFrontX * cameraFrontX + cameraFrontZ * cameraFrontZ);

    if (cameraFrontLength > 0.0f)
    {
        cameraFrontX /= cameraFrontLength;
        cameraFrontZ /= cameraFrontLength;
    }

    DirectX::XMFLOAT3 vec;
    vec.x = ax * cameraRightX + ay * cameraFrontX;
    vec.z = ay * cameraFrontZ + ax * cameraRightZ;
    vec.y = 0;


    return vec;
}

