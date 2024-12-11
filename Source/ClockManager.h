#pragma once
#include <vector>
#include "Clock.h"
#include "Stage.h"
class ClockManager
{
public:
    ClockManager() {};
    ~ClockManager() { Clear(); };

    static ClockManager& Instance() {
        static ClockManager instance;
        return instance;
    }

    void Update(float elapsedTime);
    void Render(ID3D11DeviceContext* dc, Shader* shader);

    void AddClock(Clock* clock);
    void Clear();
    void Remove(Clock* clock);
    Clock* GetClock(int index) const
    {
        if (index >= 0 && index < clocks.size())
        {
            return clocks[index];  // �L���ȃC���f�b�N�X�̏ꍇ�A���̎��v��Ԃ�
        }
        return nullptr;  // �����ȃC���f�b�N�X�̏ꍇ��nullptr��Ԃ�
    }
    bool AllClocksTo12Clock();
private:
    std::vector<Clock*> clocks; // ������Clock�I�u�W�F�N�g���Ǘ�
    std::set<Clock*> removes;   // �폜�Ώۂ�Clock�I�u�W�F�N�g

    // ���j�ƒZ�j���폜���鏈����ǉ�
    void ClearHands(LongHand* longHand, ShortHand* shortHand);
};