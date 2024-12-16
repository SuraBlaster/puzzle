#include "Cube.h"
#include "Input/Input.h"
#include <random>
#include "imgui.h"
#include "SceneManager.h"
#include "SceneLoading.h"
#include "SceneTitle.h"
#include "EraManager.h"
#include <iostream>
#include "SceneGame.h"

Cube::Cube()
{

}

Cube::~Cube()
{
	delete model;
}

void Cube::Update(float elapsedTime)
{
	SceneGame::Era era = EraManager::Instance().GetEra();

	// ���͍X�V����
	InputMoveCube();

	// ���x�X�V����
	UpdateVelocity(elapsedTime);

	// �Փ˔��菈��
	Collision();

	//�I�u�W�F�N�g�s��X�V
	UpdateTransform();	

	//�`�������ɂȂ���������J���B
	if (StageRootCube* rootCube = dynamic_cast<StageRootCube*>(this)) {
		if (rootCube->AreCubesAtExcludedPositionsAbsent()) {

			EraManager::Instance().SetDropPazzle3Position({ -6,1,0 });
			EraManager::Instance().SetDropPazzle3(true);
		}
	}
	
	
	

	StageCubeManager::Instance().m_cameraController->UpdateHorizontalRotation(elapsedTime /15);

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
	 
     
}

/// <summary>
/// �X�e�[�W�p���[�g�L���[�u
/// </summary>
StageRootCube::StageRootCube()
{
	model = new Model("Data/Model/Cube/Cube.mdl");
	scale.x = scale.y = scale.z = 0.004f;

	m_cubeType = eCubeType::Root;
}

//�q�L���[�u��I�����鏈��
void StageRootCube::SelectChild(DirectX::XMINT3 pos) {
	DirectX::XMFLOAT3 scale = { 0.004f, 0.004f, 0.004f };

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
		m_selected->SetScale({ 0.002f , 0.002f , 0.002f });
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

	// m_childrenMap���X�V���ĉ��ړ��ʒu�ɃL���[�u���ړ�������
	auto it = m_childrenMap.find(selectedIndex);
	if (it != m_childrenMap.end()) {
		auto selectedCube = it->second; // �L���[�u���ꎞ�I�ɕۑ�
		m_childrenMap.erase(selectedIndex);

		// �L���[�u��V�����ʒu�Ɉړ�
		m_childrenMap[targetPos] = selectedCube;
		selectedCube->SetPosition({ (float)targetPos.x, (float)targetPos.y, (float)targetPos.z });
		selectedIndex = targetPos;
	}

	// �V�����ʒu�ɑΉ�����L���[�u�����邩�Ċm�F
	auto It = m_childrenMap.find(selectedIndex);
	if (It != m_childrenMap.end() && It->second != nullptr) {
		m_selected = It->second;
		m_selected->SetScale({ 0.0034f,0.0034f,0.0034f });
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
		m_selected->SetScale({ 0.0034f,0.0034f,0.0034f });
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

//�ߋ��̃u���b�N�Ɩ����̃u���b�N�̌`���������ǂ����`�F�b�N
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


// �J�����̉�]�p�x�ɉ����Ĉړ������������ύX����
void StageRootCube::SelectionDirection(DirectX::XMINT3& position)
{
	GamePad& gamePad = Input::Instance().GetGamePad();

	CameraController* cameraController = StageCubeManager::Instance().GetCameraController();
	int cameraPointPosition = cameraController->GetCameraPointPosition();
	// ���݂̈ʒu����Ɉړ��ʂ����Z����
	if (gamePad.IsSpaceHeld())
	{
		// �����ړ��iY���j
		if (gamePad.GetButtonDown() & GamePad::KEY_W)
		{
			position.y++; // ����� 
		}
		else if (gamePad.GetButtonDown() & GamePad::KEY_S)
		{
			position.y--; // ������ 
		}
	}
	else
	{
		if (gamePad.GetButtonDown() & GamePad::KEY_W)
		{
			// W�L�[�őO�i����������
			switch (cameraPointPosition)
			{
			case 0: position.z = 1; break; // front
			case 1: position.x = 1; break; // left
			case 2: position.z = -1; break;// back
			case 3: position.x = -1; break; // right
			}
		}
		else if (gamePad.GetButtonDown() & GamePad::KEY_S)
		{
			// S�L�[�Ō�ޕ���������
			switch (cameraPointPosition)
			{
			case 0:position.z = -1; break; // front
			case 1:position.x = -1; break; // left
			case 2:position.z = 1; break;  // back
			case 3:position.x = 1; break; // right
			}
		}
		else if (gamePad.GetButtonDown() & GamePad::KEY_D)
		{
			// D�L�[�ŉE����������
			switch (cameraPointPosition)
			{
			case 0:position.x = 1; break;  // front
			case 1:position.z = -1; break; // left
			case 2:position.x = -1; break; // back 
			case 3:position.z = 1; break; // right
			}
		}
		else if (gamePad.GetButtonDown() & GamePad::KEY_A)
		{
			// A�L�[�ō�����������
			switch (cameraPointPosition)
			{
			case 0:position.x = -1; break; // front
			case 1:position.z = 1; break;  // left
			case 2:position.x = 1; break; // back
			case 3:position.z = -1; break;  // right
			}
		}
	}
}
void StageRootCube::InputMoveCube()
{
	GamePad& gamePad = Input::Instance().GetGamePad();

	// isCubeView��ON�̏ꍇ�A���͏������s��Ȃ�
	if (!GetIsCubeView()) {
		return;
	}

	// ���ݑI������Ă���ʒu���擾
	DirectX::XMINT3 position{};
	SelectionDirection(position);

	// Enter�L�[�������ꂽ�ꍇ�̏���
	if (gamePad.GetButtonDown() & GamePad::BTN_ENTER)
	{
		if (isEnterKey)
		{
			MoveSelectedChild();  // ���ړ����m�肷��
			SelectChild(position); // �ړ��m���A�I����Ԃ��X�V
		}

		// �ړ����[�h�̐؂�ւ�
		isEnterKey = !isEnterKey;
		return; // Enter�L�[������A���̏������X�L�b�v
	}

	// Enter�L�[��Ԃɉ���������
	if (isEnterKey)
	{
		// ���ړ��ʒu��I��
		SelectMovePosition(position);
	}
	else
	{
		// �ʏ�I�����[�h
		SelectChild(position);
	}
}


/// <summary>
/// �X�e�[�W�p�L���[�u�Ǘ�
/// </summary>
void StageCubeManager::Initialize() {
	m_rootCube = new StageRootCube();
	m_rootCube->SetAngle({ 0, 0, 0 });
	m_rootCube->SetPosition({ 0, -100, 0 });
	m_cameraController = new CameraController();

	// EraManager ���猻�݂̎�����擾
	auto& eraManager = EraManager::Instance();
	auto state = eraManager.GetEra();  // ���݂̎���
	
	// 3x3x3�̑S�ʒu�����X�g�Ɋi�[
	std::vector<DirectX::XMINT3> allPositions;
	for (int z = 0; z < 3; ++z) {
		for (int y = 0; y < 3; ++y) {
			for (int x = 0; x < 3; ++x) {
				allPositions.push_back({ x, y, z });
			}
		}
	}

	std::unordered_set<DirectX::XMINT3, XMINT3Hash> excludedPositions;

	if (state == SceneGame::Era::Past) {
		// Past: �����_����12���O
		std::random_device rd;
		std::mt19937 g(rd());
		std::shuffle(allPositions.begin(), allPositions.end(), g);  // �����_���ɕ��בւ�

		excludedPositions.insert(allPositions.begin(), allPositions.begin() + 12);
	}

	else if (state == SceneGame::Era::Future) {
		// Future: �Œ肳�ꂽ12�̈ʒu�����O
		const std::vector<DirectX::XMINT3> predefinedExcludedPositions = {
			{0, 0, 0}, {2, 0, 0}, {1, 1, 0}, {0, 2, 0}, {2, 2, 0},
			{0, 0, 2}, {0, 1, 1}, {0, 2, 2}, {2, 0, 2}, {2, 1, 1}, {2, 2, 2},
			{1, 1, 2}
		};
		excludedPositions.insert(predefinedExcludedPositions.begin(), predefinedExcludedPositions.end());
	}

	int createQube = 3;//��������L���[�u�͈̔�
	// �c��̈ʒu�ɃL���[�u�𐶐�
	for (int z = 0; z < createQube; ++z) {
		for (int y = 0; y < createQube; ++y) {

			for (int x = 0; x < createQube; ++x) {
				DirectX::XMINT3 pos = { x, y, z };

				// ���O���X�g�Ɋ܂܂�Ă���ʒu���ǂ������`�F�b�N
				if (excludedPositions.find(pos) == excludedPositions.end()) {
					Cube* cube = new StageRootCube();
					cube->SetPosition({ static_cast<float>(x), static_cast<float>(y), static_cast<float>(z) });
					m_rootCube->AddChild(cube);
					static_cast<StageRootCube*>(m_rootCube)->m_childrenMap[pos] = cube;
				}
			}
		}
	}
}


void StageCubeManager::Finalize() {
	if (m_rootCube) {
		// ���ׂẴL���[�u���������ċA�֐�
		std::function<void(Cube*)> deleteCube = [&](Cube* cube) {
			if (cube) {
				// �q�L���[�u���ɉ��
				for (Cube* child : cube->m_children) {
					deleteCube(child);
				}
				cube->m_children.clear(); // �q�L���[�u���X�g���N���A
				delete cube; // �L���[�u���g�����
			}
		};

		// ���[�g�L���[�u�����
		deleteCube(m_rootCube);
		m_rootCube = nullptr; // ���[�g�L���[�u��nullptr��
	}

	// �J�����R���g���[���[�̉��
	if (m_cameraController) {
		delete m_cameraController;
		m_cameraController = nullptr;
	}
}
