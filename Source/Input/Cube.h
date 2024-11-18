#pragma once

#include "Graphics/Shader.h"
#include "Graphics/Model.h"
#include "Character.h"
#include "Collision.h"
#include <array>
#include <unordered_map>
#include <unordered_set>
#include "StageElevator.h"

//定義していないと要素検索、比較が上手く行かない。
//なのでハッシュテーブルに格納するためにoperator==を定義
namespace DirectX {
    inline bool operator==(const XMINT3& lhs, const XMINT3& rhs) {
        return lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z;
    }
}
struct XMINT3Hash {
    std::size_t operator()(const DirectX::XMINT3& pos) const {
        // XMINT3 の x, y, z を使ってハッシュを計算
        size_t h1 = std::hash<int>{}(pos.x);
        size_t h2 = std::hash<int>{}(pos.y);
        size_t h3 = std::hash<int>{}(pos.z);
        return h1 ^ (h2 << 1) ^ (h3 << 2); // ハッシュ値を組み合わせる
    }
};

enum class eCubeType : int {
    None = 0,
    Root = 100,
    Floor,
    Obstacle = 200,
};

class Cube : public Character {
public:
    Cube();
    ~Cube() override;

    void Update(float elapsedTime);
    void Render(ID3D11DeviceContext* dc, Shader* shader);

    void Cube::AddChild(Cube* child) {
        if (child == nullptr || child->m_parent != nullptr)
            return;  // nullptrやすでに親を持つ子を追加しない

        child->m_parent = this;
        m_children.push_back(child);
    }

    int GetChildSize() { return m_children.size(); }
    Cube* GetChild(int index) { return m_children.at(index); }
    eCubeType GetCubeType() { return m_cubeType; }
    void SetElevator(StageElevator* elevatorInstance) {
        elevator = elevatorInstance;
    }
    StageElevator* elevator = nullptr;

    virtual void UpdateVelocity(float elapsedTime) = 0;
    virtual void Collision() {}
    virtual void InputMoveCube() {};


protected:
    Model* model = nullptr;
    Cube* m_parent = nullptr;
    std::vector<Cube*> m_children;
    eCubeType m_cubeType = eCubeType::None;
    
   
};

class StageRootCube : public Cube {
public:
    StageRootCube();
    ~StageRootCube() {}

    void UpdateVelocity(float elapsedTime) override {}

    void SelectChild(DirectX::XMINT3 dir);
    void SelectMovePosition(DirectX::XMINT3 dir);
    void MoveSelectedChild();
    bool IsPositionOccupied(const DirectX::XMINT3& position);
    bool AreCubesAtExcludedPositionsAbsent();
    void InputMoveCube();
   
   

    Cube* m_selected = nullptr;
    DirectX::XMINT3 selectedIndex = {};
    DirectX::XMINT3 targetPos = {};
    
    bool isEnterKey = false;
    bool isSpaceKey = false;
    std::unordered_map<DirectX::XMINT3, Cube*, XMINT3Hash> m_childrenMap;

   

};

class StageFloorCube : public Cube {
public:
    StageFloorCube();
    ~StageFloorCube() {};
    void UpdateVelocity(float elapsedTime) {}
};

class ObstacleCube : public Cube {
public:
    ObstacleCube();
    ~ObstacleCube() {}
    void UpdateVelocity(float elapsedTime) {}
};

class StageCubeManager {
private:
    StageCubeManager() {}
    ~StageCubeManager() {}

public:
    static StageCubeManager& Instance() {
        static StageCubeManager instance;
        return instance;
    }

    void Initialize();
    void Finalize();

    StageRootCube* GetRootCube() { return (StageRootCube*)m_rootCube; }
 
private:
    Cube* m_rootCube;
};