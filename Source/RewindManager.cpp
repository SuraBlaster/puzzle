#include "RewindManager.h"

void RewindManager::Update(float elapsedTime, DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 angle)
{
    elapsedtime += elapsedTime;

    if (elapsedtime >= stateSaveInterval)
    {
        currentState.position = position;
        currentState.angle = angle;
        pastStates.push_back(currentState);
        elapsedtime = 0.0f;
    }
}

void RewindManager::RewindTime(float rewindTime)
{
    while (!pastStates.empty() && rewindTime > 0.0f)
    {
      
        currentState = pastStates.back();
        pastStates.pop_back();
        rewindTime -= stateSaveInterval;

        if (pastStates.empty()) break;
    }
}
