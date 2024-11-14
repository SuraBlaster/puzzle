#include "RewindManager.h"

//�X�V����
void RewindManager::Update(float elapsedTime, DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 angle,DirectX::XMFLOAT3 velocity,Model& model)
{
    elapsedtime += elapsedTime * 2.4f;

    if (elapsedtime >= stateSaveInterval)
    {
        //������������擾
        currentState.position = position;
        currentState.angle = angle;
        currentState.velocity = velocity;

        // �A�j���[�V���������擾
        currentState.animationIndex = model.GetCurrentAnimationIndex();
        currentState.animationTime = model.GetCurrentAnimationSeconds();

        //�ۑ�
        pastStates.push_back(currentState);
        elapsedtime = 0.0f;
    }
}

//�����߂�����
void RewindManager::RewindTime(float elapsedTime,float rewindTime,Model& model)
{
    while (!pastStates.empty() && rewindTime > 0.0f)
    {
        //�Ō�����畜��
        currentState = pastStates.back();
        pastStates.pop_back();
        rewindTime -= stateSaveInterval;

        //�A�j���[�V��������
        model.PlayAnimation(currentState.animationIndex, true);
        model.UpdateAnimation(currentState.animationTime);
        
        //�O�̂��߃f�[�^����ł����break
        if (pastStates.empty())
        {
            break;
        }
    }
}
