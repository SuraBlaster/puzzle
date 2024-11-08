#include "Player.h"
#include <imgui.h>
#include "Input/Input.h"
#include "Camera.h"
#include "Graphics/Graphics.h"
#include "EnemyManager.h"
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
    model = new Model("Data/Model/Jammo/Jammo.mdl");

    //�C���X�^���X�|�C���^�擾
    instance = this;

    //�X�P�[�����O
    scale.x = scale.y = scale.z = 0.01f;

    //�q�b�g�G�t�F�N�g�ǂݍ���
    hitEffect = new Effect("Data/Effect/thunder.efk");

    //�ҋ@�X�e�[�g�֑J��
    TransitionIdleState();
}

void Player::DrawDebugPrimitive()
{
    DebugRenderer* debugRenderer = Graphics::Instance().GetDebugRenderer();

    //����Փ˗p�̃f�o�b�O����`��
    //debugRenderer->DrawSphere(position, radius, DirectX::XMFLOAT4(0, 0, 0, 1));
    
    //�Փ˔���p�̃f�o�b�O�~����`��
    debugRenderer->DrawCylinder(position, radius, height, DirectX::XMFLOAT4(0, 0, 0, 1));

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
        RewindManager::Instance().RewindTime(0.1f, Player::GetModel());
        position = RewindManager::Instance().currentState.position;
        angle = RewindManager::Instance().currentState.angle;
        velocity = RewindManager::Instance().currentState.velocity;
    }
    else if (gamePad.GetButtonUp())
    {
        if (InputMove(elapsedTime))
        {
            Player::GetModel().PlayAnimation(Anim_Running, true);
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
    case State::Jump:
        UpdateJumpState(elapsedTime);
        break;
    case State::Land:
        UpdateLandState(elapsedTime);
        break;
    case State::Attack:
        UpdateAttackState(elapsedTime);
        break;
    case State::Damage:
        UpdateDamageState(elapsedTime);
        break;
    case State::Death:
        UpdateDeathState(elapsedTime);
        break;
    case State::Revive:
        UpdateReviveState(elapsedTime);
    }

    UpdateTransform();
    
    //���͑��x�X�V
    UpdateVelocity(elapsedTime);

    UpdateInvincibleTimer(elapsedTime);

    projectileManager.Update(elapsedTime);

    CollisionPlayerVsEnemies();

    CollisionprojectilesVsEnemies();

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

//�v���C���[�ƃG�l�~�[�Ƃ̏Փ˔���
void Player::CollisionPlayerVsEnemies()
{
    EnemyManager& enemyManager = EnemyManager::Instance();

    //���ׂĂ̓G�Ƒ�������ŏՓ˔���
    int enemyCount = enemyManager.GetEnemyCount();

    for (int i = 0; i < enemyCount; ++i)
    {
        Enemy* enemy = enemyManager.GetEnemy(i);

        DirectX::XMFLOAT3 outPosition;
        /*if (Collision::IntersectSphereVsSphere(
            Player::GetPosition(),
            Player::GetRadius(),
            enemy->GetPosition(),
            enemy->GetRadius(), outPosition))
        {
            enemy->SetPosition(outPosition);
        }*/

        if (Collision::IntersectCylinderVsCylinder(
            Player::GetPosition(),
            Player::GetRadius(),
            Player::GetHeight(),
            enemy->GetPosition(),
            enemy->GetRadius(),
            enemy->GetHeight(),
            outPosition))
        {
            DirectX::XMVECTOR P = DirectX::XMLoadFloat3(&position);
            DirectX::XMVECTOR E = DirectX::XMLoadFloat3(&enemy->GetPosition());
            DirectX::XMVECTOR V = DirectX::XMVectorSubtract(P, E);
            DirectX::XMVECTOR N = DirectX::XMVector3Normalize(V);
            DirectX::XMFLOAT3 normal;
            DirectX::XMStoreFloat3(&normal, N);

            if (normal.y > 0.8f)
            {
                //���W�����v
                Jump(jumpSpeed * 0.5f);
            }
            else
            {
                enemy->SetPosition(outPosition);
            }

            
        }

    }

}

void Player::CollisionNodeVsEnemies(const char* nodeName, float nodeRadius)
{
    //�m�[�h�擾
    Model::Node* node = model->FindNode(nodeName);

    //�m�[�h�ʒu�擾
    
    
    DirectX::XMMATRIX Nodetra = DirectX::XMLoadFloat4x4(&node->worldTransform);
    DirectX::XMVECTOR Nodepos = Nodetra.r[3];
    DirectX::XMFLOAT3 NodePosition;

    DirectX::XMStoreFloat3(&NodePosition, Nodepos);

    //�w��̃m�[�h�Ƃ��ׂĂ̓G�𓖂��蔻��ŏՓˏ���
    EnemyManager& enemyManager = EnemyManager::Instance();

    int enemyCount = enemyManager.GetEnemyCount();


    for (int i = 0; i < enemyCount; ++i)
    {
        // �G�̈ʒu�擾
        Enemy* enemy = enemyManager.GetEnemy(i);

            DirectX::XMFLOAT3 outPosition;

            if (Collision::IntersectSphereVsCylinder(
                NodePosition,
                nodeRadius,
                enemy->GetPosition(),
                enemy->GetRadius(),
                enemy->GetHeight(),
                outPosition))
            {
                enemy->SetPosition(outPosition);

                if (enemy->ApplyDamage(1))
                {
                    {
                        DirectX::XMFLOAT3 impulse{};

                        const float power = 10.0f;
                        const DirectX::XMFLOAT3& e = enemy->GetPosition();
                        const DirectX::XMFLOAT3& p = NodePosition;
                        float vx = e.x - p.x;
                        float vz = e.z - p.z;
                        float lengthXZ = sqrtf(vx * vx + vz * vz);
                        vx /= lengthXZ;
                        vz /= lengthXZ;

                        impulse.x = vx * power;
                        impulse.y = power * 0.5f;
                        impulse.z = vz * power;


                        enemy->AddImpulse(impulse);
                    }
                }

                {
                    DirectX::XMFLOAT3 e = enemy->GetPosition();
                    e.y += enemy->GetHeight() * 0.5f;
                    hitEffect->Play(e);
                }



            }
    }
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

    //�W�����v���͏���
    if (InputJump())
    {
        TransitionJumpState();
    }

    //�e�ۓ��͏���
    InputProjectile();

    //�U�����͏���
    if (InputAttack())
    {
        TransitionAttackState();
    }
}

void Player::TransitionMoveState()
{
    state = State::Move;

    //����A�j���[�V�����Đ�
    model->PlayAnimation(Anim_Running, true);
}

void Player::UpdateMoveState(float elapsedTime)
{
    if (!InputMove(elapsedTime))
    {
        TransitionIdleState();
    }

    //�ړ����͏���
    InputMove(elapsedTime);

    //�W�����v���͏���
    if (InputJump())
    {
        TransitionJumpState();
    }

    //�e�ۓ��͏���
    InputProjectile();

    //�U�����͏���
    if (InputAttack())
    {
        TransitionAttackState();
    }
}

void Player::TransitionJumpState()
{
    state = State::Jump;


    //�W�����v�A�j���[�V�����Đ�
    model->PlayAnimation(Anim_Jump, false);
}

void Player::UpdateJumpState(float elapsedTime)
{
    GamePad& gamePad = Input::Instance().GetGamePad();
    
    if (IsGround())
    {
        TransitionLandState();
    }

    if (InputJump())
    {
        //�W�����v�A�j���[�V�����Đ�
        model->PlayAnimation(Anim_Jump_Flip, false);
    }

    //�e�ۓ��͏���
    InputProjectile();
}

void Player::TransitionLandState()
{
    state = State::Land;

    //���n�A�j���[�V�����Đ�
    model->PlayAnimation(Anim_Landing, false);
}

void Player::UpdateLandState(float elapsedTime)
{
    if (!model->IsPlayAnimation())
    {
        TransitionIdleState();
    }
}

void Player::TransitionAttackState()
{
    state = State::Attack;

    //�U���A�j���[�V�����Đ�
    model->PlayAnimation(Anim_Attack, false);
}

void Player::UpdateAttackState(float elapsedTime)
{
    if (!model->IsPlayAnimation())
    {
        TransitionIdleState();
    }

    //�C�ӂ̃A�j���[�V�����Đ���Ԃł̂ݏՓ˔��菈��������
    float animationTime = model->GetCurrentAnimationSeconds();
    attackCollisionFlag = animationTime >= 0.3f && animationTime < 0.4f ? true : false;

    if (attackCollisionFlag)
    {
        //����m�[�h�ƃG�l�~�[�̏Փˏ���
        CollisionNodeVsEnemies("mixamorig:LeftHand", leftHandRadius);
    }
    
}

void Player::TransitionDamageState()
{
    state = State::Damage;

    //�_���[�W�A�j���[�V�����Đ�
    model->PlayAnimation(Anim_GetHit1, false);
}

void Player::UpdateDamageState(float elapsedTime)
{
    //�_���[�W�A�j���[�V�������I�������ҋ@�X�e�[�g�ɑJ��
    if (!model->IsPlayAnimation())
    {
        TransitionIdleState();
    }
}

void Player::TransitionDeathState()
{
    state = State::Death;

    //���S�A�j���[�V�����Đ�
    model->PlayAnimation(Anim_Death, false);
}

void Player::UpdateDeathState(float elapsedTime)
{
    if (!model->IsPlayAnimation())
    {
        //�{�^�����������畜���X�e�[�g�ɑJ��
        GamePad& gamePad = Input::Instance().GetGamePad();
        if (gamePad.GetButtonDown() & GamePad::BTN_A)
        {
            TransitionReviveState();
        }
    }
}

void Player::TransitionReviveState()
{
    state = State::Revive;

    //�̗͉�
    health = maxHealth;

    //�����A�j���[�V�����Đ�
    model->PlayAnimation(Anim_Revive, false);
}

void Player::UpdateReviveState(float elapsedTime)
{
    //�����A�j���[�V�����I�����ɑҋ@�X�e�[�g�ɑJ��
    if (!model->IsPlayAnimation())
    {
        TransitionIdleState();
    }
}



void Player::InputProjectile()
{
    GamePad& gamePad = Input::Instance().GetGamePad();

    if (gamePad.GetButtonDown() & GamePad::BTN_X)
    {
        DirectX::XMFLOAT3 dir;
        dir.x = sinf(angle.y);
        dir.y = 0.0f;
        dir.z = cosf(angle.y);

        DirectX::XMStoreFloat3(&dir, DirectX::XMVector3Normalize(DirectX::XMLoadFloat3(&dir)));
        
        DirectX::XMFLOAT3 pos;
        pos.x = Player::GetPosition().x;
        pos.y = Player::GetPosition().y + Player::GetHeight() / 2;
        pos.z = Player::GetPosition().z;

        ProjectileStraight* projectile = new ProjectileStraight(&projectileManager);
        projectile->Launch(dir, pos);
        //projectileManager.Register(projectile);

    }

    if (gamePad.GetButtonDown() & GamePad::BTN_Y)
    {
        DirectX::XMFLOAT3 dir;
        dir.x = sinf(angle.y);
        dir.y = 0.0f;
        dir.z = cosf(angle.y);

        DirectX::XMFLOAT3 pos;
        pos.x = position.x;
        pos.y = position.y + height / 2;
        pos.z = position.z;

        DirectX::XMFLOAT3 target;
        target.x = pos.x + dir.x * 1000.0f;
        target.y = pos.y + dir.y * 1000.0f;
        target.z = pos.z + dir.z * 1000.0f;

        float dist = FLT_MAX;
        EnemyManager& enemyManager = EnemyManager::Instance();
        int enemyCount = enemyManager.GetEnemyCount();

        for (int i = 0; i < enemyCount; ++i)
        {
            Enemy* enemy = EnemyManager::Instance().GetEnemy(i);
            DirectX::XMVECTOR P = DirectX::XMLoadFloat3(&position);
            DirectX::XMVECTOR E = DirectX::XMLoadFloat3(&enemy->GetPosition());
            DirectX::XMVECTOR V = DirectX::XMVectorSubtract(E, P);
            DirectX::XMVECTOR D = DirectX::XMVector3LengthSq(V);

            float d;
            DirectX::XMStoreFloat(&d, D);
            if (d < dist)
            {
                dist = d;
                target = enemy->GetPosition();
                target.y += enemy->GetHeight() * 0.5f;
            }
        }

        ProjectileHoming* projectile = new ProjectileHoming(&projectileManager);
        projectile->Launch(dir, pos, target);
    }
    
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

//���n�����Ƃ��ɌĂ΂��
void Player::OnLanding()
{
    jumpCount = 0;

    if (state != State::Damage && state != State::Death)
    {
        TransitionLandState();
    }
}

void Player::OnDamaged()
{
    //�_���[�W�X�e�[�g�ɑJ��
    TransitionDamageState();
}

void Player::OnDead()
{
    //���S�X�e�[�g�ɑJ��
    TransitionDeathState();
}

void Player::CollisionprojectilesVsEnemies()
{
    EnemyManager& enemyManager = EnemyManager::Instance();

    //�������菈��
    int projectileCount = projectileManager.GetProjectileCount();
    int enemyCount = enemyManager.GetEnemyCount();
    for (int i = 0; i < projectileCount; ++i)
    {
        Projectile* projectile = projectileManager.GetProjectile(i);

        for (int j = 0; j < enemyCount; ++j)
        {
            Enemy* enemy = enemyManager.GetEnemy(j);

            //�Փˏ���
            DirectX::XMFLOAT3 outPosition;
            if (Collision::IntersectSphereVsCylinder(
                projectile->GetPosition(),
                projectile->GetRadius(),
                enemy->GetPosition(),
                enemy->GetRadius(),
                enemy->GetHeight(),
                outPosition
            ))
            {
                if (enemy->ApplyDamage(1))
                {
                    {
                        DirectX::XMFLOAT3 impulse{};

                        const float power = 10.0f;
                        const DirectX::XMFLOAT3& e = enemy->GetPosition();
                        const DirectX::XMFLOAT3& p = projectile->GetPosition();
                        float vx = e.x - p.x;
                        float vz = e.z - p.z;
                        float lengthXZ = sqrtf(vx * vx + vz * vz);
                        vx /= lengthXZ;
                        vz /= lengthXZ;

                        impulse.x = vx * power;
                        impulse.y = power * 0.5f;
                        impulse.z = vz * power;
                        

                        enemy->AddImpulse(impulse);
                    }


                    projectile->Destroy();
                }

                {
                    DirectX::XMFLOAT3 e = enemy->GetPosition();
                    e.y += enemy->GetHeight() * 0.5f;
                    hitEffect->Play(e);
                }
           }
        }
    }
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
    ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(300, 300), ImGuiCond_FirstUseEver);

    if (ImGui::Begin("Player", nullptr, ImGuiWindowFlags_None))
    {
        if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
        {
            ImGui::InputFloat3("Position", &position.x);

            DirectX::XMFLOAT3 a;
            a.x = DirectX::XMConvertToDegrees(angle.x);
            a.y = DirectX::XMConvertToDegrees(angle.y);
            a.z = DirectX::XMConvertToDegrees(angle.z);

            ImGui::InputFloat3("Angle", &a.x);
            angle.x = DirectX::XMConvertToRadians(a.x);
            angle.y = DirectX::XMConvertToRadians(a.y);
            angle.z = DirectX::XMConvertToRadians(a.z);

            ImGui::InputFloat3("Scale", &scale.x);

            ImGui::InputFloat("invincible", &invincibleTimer);
        }
    }


    ImGui::End();
}

DirectX::XMFLOAT3 Player::GetMoveVec() const
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

