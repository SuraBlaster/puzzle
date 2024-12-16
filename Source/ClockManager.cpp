#include "ClockManager.h"
#include "EraManager.h"
void ClockManager::Update(float elapsedTime) {
    // 時計の更新処理
    for (auto& clock : clocks) {
        clock->Update(elapsedTime);
        
        // 時計に関連する長針と短針の更新処理
        if (clock->GetLongHand()) {
            clock->GetLongHand()->Update(elapsedTime);
        }
        if (clock->GetShortHand()) {
            clock->GetShortHand()->Update(elapsedTime);
        }
    }

    //全ての時計が12自方向に向いてるか
    if (AllClocksTo12Clock()) {
        EraManager::Instance().SetDropPazzle4Position({ 0, 0, -5 });
        EraManager::Instance().SetDropPazzle4(true);
    }
}

void ClockManager::Render(ID3D11DeviceContext* dc, Shader* shader) {
    // 時計の描画処理
    for (auto& clock : clocks) {
        clock->Render(dc, shader);

        // 時計に関連する長針と短針の描画処理
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
    // Clockを削除する前に、長針と短針も削除
    for (Clock* clock : clocks) {
        if (clock->GetLongHand()) {
            delete clock->GetLongHand();  // LongHandの削除
        }
        if (clock->GetShortHand()) {
            delete clock->GetShortHand();  // ShortHandの削除
        }
        delete clock;  // Clock本体の削除
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
        // 各Clockが12時方向を向いているか確認
        if (!clock->IsAlignedTo12Clock())
        {
            return false;  // 1つでも12時方向を向いていない場合、falseを返す
        }
    }
    return true;  // すべての時計が12時方向を向いている場合、trueを返す
}

// 長針と短針の削除処理
void ClockManager::ClearHands(LongHand* longHand, ShortHand* shortHand) {
    if (longHand) {
        delete longHand;
    }
    if (shortHand) {
        delete shortHand;
    }
}