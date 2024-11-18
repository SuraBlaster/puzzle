#include "Cube.h"
#include "Input/Input.h"
#include <random>
#include "imgui.h"
#include "SceneManager.h"
#include "SceneLoading.h"
#include "SceneTitle.h"


Cube::Cube()
{

}

Cube::~Cube()
{
	for (auto child : m_children)
		delete child;

	delete model;
}

void Cube::Update(float elapsedTime)
{
	// ���͍X�V����
	InputMoveCube();

	// ���x�X�V����
	UpdateVelocity(elapsedTime);

	// �Փ˔��菈��
	Collision();

	//�I�u�W�F�N�g�s��X�V
	UpdateTransform();	

	if (StageRootCube* rootCube = dynamic_cast<StageRootCube*>(this)) {
		if (rootCube->AreCubesAtExcludedPositionsAbsent()) {
			elevator->UpdateOpenState();
		}
	}

	for (auto child : m_children) {
		child->Update(elapsedTime);
	}

	model->UpdateTransform(transform);
}

void Cube::Render(ID3D11DeviceContext* dc, Shader* shader)
{
	shader->Draw(dc, model);

	for (auto child : m_children)
		child->Render(dc, shader);

	// �f�o�b�O�\��
	{
		ImGui::Separator();
		StageRootCube* rootCube = StageCubeManager::Instance().GetRootCube();

		if (ImGui::TreeNode("Rootcube"))
		{
			if (rootCube->m_selected != nullptr)
			{
				DirectX::XMFLOAT3 selectedPos = rootCube->m_selected->GetPosition();
				int pos[3] = { static_cast<int>(selectedPos.x), static_cast<int>(selectedPos.y), static_cast<int>(selectedPos.z) };
				ImGui::InputInt3("m_serected Position", pos);  // �\���p�Ɉʒu�𐮐��z��ɕϊ�
			}
			ImGui::TreePop();
		}
	}
}

/// <summary>
/// �X�e�[�W�p���[�g�L���[�u
/// </summary>
StageRootCube::StageRootCube()
{
	model = new Model("Data/Model/Cube/Cube.mdl");

	m_cubeType = eCubeType::Root;
}

//�q�L���[�u��I�����鏈��
void StageRootCube::SelectChild(DirectX::XMINT3 pos) {
	DirectX::XMFLOAT3 scale = { 1.0f, 1.0f, 1.0f };

	// ���ݑI�𒆂̃L���[�u�̃X�P�[�������Z�b�g
	if (m_selected != nullptr) {
		m_selected->SetScale(scale);
	}

	// ���b�v�A���E���h�ňʒu���X�V
	selectedIndex.x = (selectedIndex.x + pos.x + 3) % 3;
	selectedIndex.y = (selectedIndex.y + pos.y + 3) % 3;
	selectedIndex.z = (selectedIndex.z + pos.z + 3) % 3;

	// �I�����ꂽ�ʒu�ɃL���[�u�����݂��邩�m�F
	auto it = m_childrenMap.find(selectedIndex);
	if (it != m_childrenMap.end() && it->second != nullptr) {
		m_selected = it->second;
		m_selected->SetScale({ 0.5f, 0.5f, 0.5f });
	}
	else {
		m_selected = nullptr;
	}
}
// �I�������L���[�u�����ړ��ʒu�Őݒu�ꏊ��I������
void StageRootCube::SelectMovePosition(DirectX::XMINT3 pos)
{
	if (m_selected == nullptr) return;

	// ���ړ��ʒu���v�Z
	targetPos = {
		selectedIndex.x + pos.x,
		selectedIndex.y + pos.y,
		selectedIndex.z + pos.z
	};

	// �͈͊O�̏ꍇ�̓��b�v�A���E���h�ŕ␳
	targetPos.x = (targetPos.x + 3) % 3;
	targetPos.y = (targetPos.y + 3) % 3;
	targetPos.z = (targetPos.z + 3) % 3;

	// �ʒu����L����Ă��邩�m�F
	if (IsPositionOccupied(targetPos)) return;

	// m_childrenMap��V�����ʒu�ɍX�V
	m_childrenMap.erase(selectedIndex);
	m_childrenMap[targetPos] = m_selected;

	// selectedIndex��m_childrenMap�̐V�����ʒu���X�V
	selectedIndex = targetPos;

	// �ʒu�ɃL���[�u�����݂��邩�Ċm�F
	auto it = m_childrenMap.find(selectedIndex);
	if (it != m_childrenMap.end() && it->second != nullptr) {
		m_selected = it->second;
		m_selected->SetScale({ 0.5f, 0.5f, 0.5f });
	}
	else {
		m_selected = nullptr;
	}
}
// �I�����ꂽ�q�L���[�u�����ړ��ʒu�Ɉړ�������
void StageRootCube::MoveSelectedChild()
{
	if (m_selected == nullptr) return;

	// �I�����ꂽ�L���[�u�̈ʒu���X�V
	m_selected->SetPosition({ (float)targetPos.x, (float)targetPos.y, (float)targetPos.z });

	// selectedIndex �� m_childrenMap ��V�����ʒu�ɍX�V
	m_childrenMap.erase(selectedIndex);
	m_childrenMap[targetPos] = m_selected;
	selectedIndex = targetPos;

	// �ʒu�ɃL���[�u�����݂��邩�Ċm�F
	auto it = m_childrenMap.find(selectedIndex);
	if (it != m_childrenMap.end() && it->second != nullptr) {
		m_selected = it->second;
		m_selected->SetScale({ 0.5f, 0.5f, 0.5f });
	}
	else {
		m_selected = nullptr;
	}
}


//�ړ���ɃL���[�u�����邩�m�F����֐�
bool StageRootCube::IsPositionOccupied(const DirectX::XMINT3& position)
{
	for (auto child : m_children)
	{
		if (child == nullptr) continue;  // �󂫈ʒu�Ȃ�X�L�b�v

		DirectX::XMFLOAT3 childPos = child->GetPosition();
		if (static_cast<int>(childPos.x) == position.x &&
			static_cast<int>(childPos.y) == position.y &&
			static_cast<int>(childPos.z) == position.z)
		{
			return true;
		}
	}
	return false;
}


bool StageRootCube::AreCubesAtExcludedPositionsAbsent() {

	// ���O�ʒu���X�g
	const std::vector<DirectX::XMINT3> excludedPositions = {
		{0, 0, 0}, {2, 0, 0}, {1, 1, 0}, {0, 2, 0}, {2, 2, 0},
		{0, 0, 2}, {0, 1, 1}, {0, 2, 2}, {2, 0, 2}, {2, 1, 1}, {2, 2, 2},
		{1, 1, 2}
	};

	// �e���O�ʒu�ɃL���[�u�����݂��邩���`�F�b�N
	for (const auto& pos : excludedPositions) {
		// m_childrenMap����ʒu�ɑΉ�����L���[�u���擾
		if (m_childrenMap.find(pos) != m_childrenMap.end()) {
			return false; // 1�ł��L���[�u���������ꍇ�Afalse��Ԃ�
		}
	}


	return true; // ���ׂĂ̈ʒu�ɃL���[�u���Ȃ������ꍇ�Atrue��Ԃ�
}

//�I�����ꂽ�L���[�u���ړ��܂��͉��ړ��ʒu��I��
void StageRootCube::InputMoveCube()
{
	GamePad& gamePad = Input::Instance().GetGamePad();

	DirectX::XMINT3 position{};

	/*���L���[�u�ړ��̗���
	 WASD�L�[�ŃL���[�u��I��
			 ��
	�I�������L���[�u��ENTER�L�[�ňړ��J�n
			  ��
	WASD�L�[�ňړ��ꏊ��I��
			  ��
	�ړ��ꏊ��ENTER�Ō��肷��*/


	// �ړ�����L���[�u���I�����ꂽ��Ɉړ��ꏊ��I��
	if (gamePad.IsSpaceHeld())
	{
		if (gamePad.GetButtonDown() & GamePad::KEY_W) { position.z = 1; }
		else if (gamePad.GetButtonDown() & GamePad::KEY_S) { position.z = -1; }
	}
	else
	{
		if (gamePad.GetButtonDown() & GamePad::KEY_D) { position.x = 1; }
		else if (gamePad.GetButtonDown() & GamePad::KEY_A) { position.x = -1; }
		else if (gamePad.GetButtonDown() & GamePad::KEY_W) { position.y = 1; }
		else if (gamePad.GetButtonDown() & GamePad::KEY_S) { position.y = -1; }
	}
	// �ړ����[�h���ǂ����ŏ����𕪂���
	if (isEnterKey)
		SelectMovePosition(position);  // �ړ����[�h�Ȃ牼�ړ��ʒu��I��
	else
		SelectChild(position);  // �ʏ탂�[�h�Ȃ�L���[�u��I��

	// Enter�L�[�ňړ����[�h�̐؂�ւ�
	if (gamePad.GetButtonDown() & GamePad::BTN_ENTER) {
		if (isEnterKey) {
			MoveSelectedChild();  // �ړ��m��
			SelectChild(position); // �ړ���ɑI�����s��
		}
		isEnterKey = !isEnterKey;  // Enter�L�[��������邽�тɈړ����[�h��؂�ւ���
	}
}

// <summary>
 //�X�e�[�W�p���L���[�u
 //</summary>
StageFloorCube::StageFloorCube()
{
	model = new Model("Data/Model/Cube/Cube.mdl");

	m_cubeType = eCubeType::Floor;
}

/// <summary>
/// �����Ȃ���Q���p�L���[�u
/// </summary>
ObstacleCube::ObstacleCube()
{
	model = new Model("Data/Model/Cube/Cube.mdl");

	m_cubeType = eCubeType::Obstacle;
}

/// <summary>
/// �X�e�[�W�p�L���[�u�Ǘ�
/// </summary>
void StageCubeManager::Initialize() {
    m_rootCube = new StageRootCube();
    m_rootCube->SetAngle({ 0, 0, 0 });
    m_rootCube->SetPosition({ 0, -5, 0 });

    // 3x3x3�̑S�ʒu�����X�g�Ɋi�[
    std::vector<DirectX::XMINT3> allPositions;
    for (int z = 0; z < 3; ++z) {
        for (int y = 0; y < 3; ++y) {
            for (int x = 0; x < 3; ++x) {
                allPositions.push_back({ x, y, z });
            }
        }
    }

    // 12�̈ʒu�������_���ɏ��O����
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(allPositions.begin(), allPositions.end(), g);  // �����_���ɕ��בւ�

    // �ŏ���12�����O����ʒu�Ƃ��đI��
    std::unordered_set<DirectX::XMINT3, XMINT3Hash> excludedPositions(allPositions.begin(), allPositions.begin() + 12);

    // �c��̈ʒu�ɃL���[�u�𐶐�
    for (int z = 0; z < 3; ++z) {
        for (int y = 0; y < 3; ++y) {
            for (int x = 0; x < 3; ++x) {
                DirectX::XMINT3 pos = { x, y, z };
                
                // ���O���X�g�Ɋ܂܂�Ă���ʒu���ǂ������`�F�b�N
                if (excludedPositions.find(pos) == excludedPositions.end()) {
                    Cube* cube = new StageFloorCube();
                    cube->SetPosition({ static_cast<float>(x), static_cast<float>(y), static_cast<float>(z) });
                    m_rootCube->AddChild(cube);
                    static_cast<StageRootCube*>(m_rootCube)->m_childrenMap[pos] = cube;
                }
            }
        }
    }
}

void StageCubeManager::Finalize()
{
	if (m_rootCube != nullptr)
	{
		delete m_rootCube;
		m_rootCube = nullptr;
	}
}
