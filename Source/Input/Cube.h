#pragma once

#include "Graphics/Shader.h"
#include "Graphics/Model.h"
#include "CameraController.h"
#include "Character.h"
#include "Collision.h"
#include <array>
#include <unordered_map>
#include <unordered_set>
#include "StageElevator.h"
#include "SceneGame.h"
#include "CameraController.h"
#include "Graphics/LambertShader.h"


//��`���Ă��Ȃ��Ɨv�f�����A��r����肭�s���Ȃ��B
//�Ȃ̂Ńn�b�V���e�[�u���Ɋi�[���邽�߂�operator==���`
namespace DirectX {
    inline bool operator==(const XMINT3& lhs, const XMINT3& rhs) {
        return lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z;
    }
}
struct XMINT3Hash {
    std::size_t operator()(const DirectX::XMINT3& pos) const {
        // XMINT3 �� x, y, z ���g���ăn�b�V�����v�Z
        size_t h1 = std::hash<int>{}(pos.x);
        size_t h2 = std::hash<int>{}(pos.y);
        size_t h3 = std::hash<int>{}(pos.z);
        return h1 ^ (h2 << 1) ^ (h3 << 2); // �n�b�V���l��g�ݍ��킹��
    }
};
enum class eCubeType : int {
    None = 0,
    Root = 100,
};

class Cube : public Character {
public:
    Cube();
    ~Cube() override;

    void Update(float elapsedTime);
    void Render(ID3D11DeviceContext* dc, Shader* shader);

    void Cube::AddChild(Cube* child) {
        if (child == nullptr || child->m_parent != nullptr)
            return;  // nullptr�₷�łɐe�����q��ǉ����Ȃ�

        child->m_parent = this;
        m_children.push_back(child);
    }

    int GetChildSize() { return m_children.size(); }
    Cube* GetChild(int index) { return m_children.at(index); }
    eCubeType GetCubeType() { return m_cubeType; }

    void SetElevator(StageElevator* elevatorInstance) {
        elevator = elevatorInstance;
    }
    void SetTemporaryMoving(bool temporary) {
        isTemporaryMoving = temporary;
        size = temporary ? 0.5f : 1.0f; // ���ړ���ԂȂ�0.5�A�ʏ�Ȃ�1.0
    }
  

    StageElevator* elevator = nullptr;

    virtual void UpdateVelocity(float elapsedTime) = 0;
    virtual void Collision() {}
    virtual void InputMoveCube() {};
    std::vector<Cube*> m_children;

protected:
    Model* model = nullptr;
    Cube* m_parent = nullptr;
    eCubeType m_cubeType = eCubeType::None;
    bool isTemporaryMoving = false;

    float size;
};

class StageRootCube : public Cube {
public:
    StageRootCube();
    ~StageRootCube() {}

    void UpdateVelocity(float elapsedTime) override {}
    void SelectChild(DirectX::XMINT3 dir);
    void SelectMovePosition(DirectX::XMINT3 dir);
    void MoveSelectedChild();
    void InputMoveCube();
    void SelectionDirection(DirectX::XMINT3& positiom);

    bool IsPositionOccupied(const DirectX::XMINT3& position);
    bool AreCubesAtExcludedPositionsAbsent();

    void SetIsCubeView(bool value)
    {
        isCubeView = value;
    }
    bool GetIsCubeView() const {
        return isCubeView;
    }

  
    Cube* GetChildAt(const DirectX::XMINT3& position)
    {
        auto it = m_childrenMap.find(position);
        if (it != m_childrenMap.end()) {
            return it->second; // �q�L���[�u���������ꍇ
        }
        return nullptr; // �w����W�Ɏq�L���[�u�����݂��Ȃ��ꍇ
    }

    std::unordered_map<DirectX::XMINT3, Cube*, XMINT3Hash> m_childrenMap;

private:
    Cube* m_selected = nullptr;
    DirectX::XMINT3 selectedIndex = {};
    DirectX::XMINT3 targetPos = {};
    DirectX::XMINT3 position{};

    bool isEnterKey = false;
    bool isSpaceKey = false;
    bool isCubeView;
   
};

class StageCubeManager {
private:
    StageCubeManager() {}
    ~StageCubeManager() { Finalize(); }

public:
    static StageCubeManager& Instance() {
        static StageCubeManager instance;
        return instance;
    }

    void Initialize();
    void Finalize();
  
    StageRootCube* GetRootCube() { return (StageRootCube*)m_rootCube; }
    CameraController* StageCubeManager::GetCameraController() { return m_cameraController; }

    CameraController* m_cameraController;
private:
    Cube* m_rootCube;
};