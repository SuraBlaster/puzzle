#include "ClockManager.h"
#include "EraManager.h"
void ClockManager::Update(float elapsedTime) {
    // ���v�̍X�V����
    for (auto& clock : clocks) {
        clock->Update(elapsedTime);
        
        // ���v�Ɋ֘A���钷�j�ƒZ�j�̍X�V����
        if (clock->GetLongHand()) {
            clock->GetLongHand()->Update(elapsedTime);
        }
        if (clock->GetShortHand()) {
            clock->GetShortHand()->Update(elapsedTime);
        }
    }

    //�S�Ă̎��v��12�������Ɍ����Ă邩
    if (AllClocksTo12Clock()) {
        EraManager::Instance().SetDropPazzle4Position({ 0, 0, -5 });
        EraManager::Instance().SetDropPazzle4(true);
    }
}

void ClockManager::Render(ID3D11DeviceContext* dc, Shader* shader) {
    // ���v�̕`�揈��
    for (auto& clock : clocks) {
        clock->Render(dc, shader);

        // ���v�Ɋ֘A���钷�j�ƒZ�j�̕`�揈��
        if (clock->GetLongHand()) {
            clock->GetLongHand()->Render(dc, shader);
        }
        if (clock->GetShortHand()) {
            clock->GetShortHand()->Render(dc, shader);
        }
    }
}

void ClockManager::AddClock(Clock* clock) {
    clocks.push_back(clock);
}

void ClockManager::Clear() {
    // Clock���폜����O�ɁA���j�ƒZ�j���폜
    for (Clock* clock : clocks) {
        if (clock->GetLongHand()) {
            delete clock->GetLongHand();  // LongHand�̍폜
        }
        if (clock->GetShortHand()) {
            delete clock->GetShortHand();  // ShortHand�̍폜
        }
        delete clock;  // Clock�{�̂̍폜
    }
    clocks.clear();
}

void ClockManager::Remove(Clock* clock) {
    removes.insert(clock);
}


bool ClockManager::AllClocksTo12Clock()
{
    for (const auto& clock : clocks)
    {
        // �eClock��12�������������Ă��邩�m�F
        if (!clock->IsAlignedTo12Clock())
        {
            return false;  // 1�ł�12�������������Ă��Ȃ��ꍇ�Afalse��Ԃ�
        }
    }
    return true;  // ���ׂĂ̎��v��12�������������Ă���ꍇ�Atrue��Ԃ�
}

// ���j�ƒZ�j�̍폜����
void ClockManager::ClearHands(LongHand* longHand, ShortHand* shortHand) {
    if (longHand) {
        delete longHand;
    }
    if (shortHand) {
        delete shortHand;
    }
}