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
            return clocks[index];  // 有効なインデックスの場合、その時計を返す
        }
        return nullptr;  // 無効なインデックスの場合はnullptrを返す
    }
    bool AllClocksTo12Clock();
private:
    std::vector<Clock*> clocks; // 複数のClockオブジェクトを管理
    std::set<Clock*> removes;   // 削除対象のClockオブジェクト

    // 長針と短針を削除する処理を追加
    void ClearHands(LongHand* longHand, ShortHand* shortHand);
};