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

	// 入力更新処理
	InputMoveCube();

	// 速度更新処理
	UpdateVelocity(elapsedTime);

	// 衝突判定処理
	Collision();

	//オブジェクト行列更新
	UpdateTransform();	

	//形が同じになったら扉が開く。
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
/// ステージ用ルートキューブ
/// </summary>
StageRootCube::StageRootCube()
{
	model = new Model("Data/Model/Cube/Cube.mdl");
	scale.x = scale.y = scale.z = 0.004f;

	m_cubeType = eCubeType::Root;
}

//子キューブを選択する処理
void StageRootCube::SelectChild(DirectX::XMINT3 pos) {
	DirectX::XMFLOAT3 scale = { 0.004f, 0.004f, 0.004f };

	// 現在選択中のキューブのスケールをリセット
	if (m_selected != nullptr) {
		m_selected->SetScale(scale);
	}

	// ラップアラウンドで位置を更新
	selectedIndex.x = (selectedIndex.x + pos.x + 3) % 3;
	selectedIndex.y = (selectedIndex.y + pos.y + 3) % 3;
	selectedIndex.z = (selectedIndex.z + pos.z + 3) % 3;

	// 選択された位置にキューブが存在するか確認
	auto it = m_childrenMap.find(selectedIndex);
	if (it != m_childrenMap.end() && it->second != nullptr) {
		m_selected = it->second;
		m_selected->SetScale({ 0.002f , 0.002f , 0.002f });
	}
	else {
		m_selected = nullptr;
	}
}
// 選択したキューブを仮移動位置で設置場所を選択する
void StageRootCube::SelectMovePosition(DirectX::XMINT3 pos)
{
	if (m_selected == nullptr) return;

	// 仮移動位置を計算
	targetPos = {
		selectedIndex.x + pos.x,
		selectedIndex.y + pos.y,
		selectedIndex.z + pos.z
	};

	// 範囲外の場合はラップアラウンドで補正
	targetPos.x = (targetPos.x + 3) % 3;
	targetPos.y = (targetPos.y + 3) % 3;
	targetPos.z = (targetPos.z + 3) % 3;

	// 位置が占有されているか確認
	if (IsPositionOccupied(targetPos)) return;

	// m_childrenMapを更新して仮移動位置にキューブを移動させる
	auto it = m_childrenMap.find(selectedIndex);
	if (it != m_childrenMap.end()) {
		auto selectedCube = it->second; // キューブを一時的に保存
		m_childrenMap.erase(selectedIndex);

		// キューブを新しい位置に移動
		m_childrenMap[targetPos] = selectedCube;
		selectedCube->SetPosition({ (float)targetPos.x, (float)targetPos.y, (float)targetPos.z });
		selectedIndex = targetPos;
	}

	// 新しい位置に対応するキューブがあるか再確認
	auto It = m_childrenMap.find(selectedIndex);
	if (It != m_childrenMap.end() && It->second != nullptr) {
		m_selected = It->second;
		m_selected->SetScale({ 0.0034f,0.0034f,0.0034f });
	}
	else {
		m_selected = nullptr;
	}
}
// 選択された子キューブを仮移動位置に移動させる
void StageRootCube::MoveSelectedChild()
{
	if (m_selected == nullptr) return;

	// 選択されたキューブの位置を更新
	m_selected->SetPosition({ (float)targetPos.x, (float)targetPos.y, (float)targetPos.z });

	// selectedIndex と m_childrenMap を新しい位置に更新
	m_childrenMap.erase(selectedIndex);
	m_childrenMap[targetPos] = m_selected;
	selectedIndex = targetPos;
	
	// 位置にキューブが存在するか再確認
	auto it = m_childrenMap.find(selectedIndex);
	if (it != m_childrenMap.end() && it->second != nullptr) {
		m_selected = it->second;
		m_selected->SetScale({ 0.0034f,0.0034f,0.0034f });
	}
	else {
		m_selected = nullptr;
	}
}


//移動先にキューブがあるか確認する関数
bool StageRootCube::IsPositionOccupied(const DirectX::XMINT3& position)
{
	for (auto child : m_children)
	{
		if (child == nullptr) continue;  // 空き位置ならスキップ

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

//過去のブロックと未来のブロックの形が同じかどうかチェック
bool StageRootCube::AreCubesAtExcludedPositionsAbsent() {

	// 除外位置リスト
	const std::vector<DirectX::XMINT3> excludedPositions = {
		{0, 0, 0}, {2, 0, 0}, {1, 1, 0}, {0, 2, 0}, {2, 2, 0},
		{0, 0, 2}, {0, 1, 1}, {0, 2, 2}, {2, 0, 2}, {2, 1, 1}, {2, 2, 2},
		{1, 1, 2}
	};

	// 各除外位置にキューブが存在するかをチェック
	for (const auto& pos : excludedPositions) {
		// m_childrenMapから位置に対応するキューブを取得
		if (m_childrenMap.find(pos) != m_childrenMap.end()) {
			return false; // 1つでもキューブがあった場合、falseを返す
		}
	}
	return true; // すべての位置にキューブがなかった場合、trueを返す
}


// カメラの回転角度に応じて移動させる向きを変更する
void StageRootCube::SelectionDirection(DirectX::XMINT3& position)
{
	GamePad& gamePad = Input::Instance().GetGamePad();

	CameraController* cameraController = StageCubeManager::Instance().GetCameraController();
	int cameraPointPosition = cameraController->GetCameraPointPosition();
	// 現在の位置を基準に移動量を加算する
	if (gamePad.IsSpaceHeld())
	{
		// 垂直移動（Y軸）
		if (gamePad.GetButtonDown() & GamePad::KEY_W)
		{
			position.y++; // 上方向 
		}
		else if (gamePad.GetButtonDown() & GamePad::KEY_S)
		{
			position.y--; // 下方向 
		}
	}
	else
	{
		if (gamePad.GetButtonDown() & GamePad::KEY_W)
		{
			// Wキーで前進方向を決定
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
			// Sキーで後退方向を決定
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
			// Dキーで右方向を決定
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
			// Aキーで左方向を決定
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

	// isCubeViewがONの場合、入力処理を行わない
	if (!GetIsCubeView()) {
		return;
	}

	// 現在選択されている位置を取得
	DirectX::XMINT3 position{};
	SelectionDirection(position);

	// Enterキーが押された場合の処理
	if (gamePad.GetButtonDown() & GamePad::BTN_ENTER)
	{
		if (isEnterKey)
		{
			MoveSelectedChild();  // 仮移動を確定する
			SelectChild(position); // 移動確定後、選択状態を更新
		}

		// 移動モードの切り替え
		isEnterKey = !isEnterKey;
		return; // Enterキー処理後、他の処理をスキップ
	}

	// Enterキー状態に応じた処理
	if (isEnterKey)
	{
		// 仮移動位置を選択
		SelectMovePosition(position);
	}
	else
	{
		// 通常選択モード
		SelectChild(position);
	}
}


/// <summary>
/// ステージ用キューブ管理
/// </summary>
void StageCubeManager::Initialize() {
	m_rootCube = new StageRootCube();
	m_rootCube->SetAngle({ 0, 0, 0 });
	m_rootCube->SetPosition({ 0, -100, 0 });
	m_cameraController = new CameraController();

	// EraManager から現在の時代を取得
	auto& eraManager = EraManager::Instance();
	auto state = eraManager.GetEra();  // 現在の時代
	
	// 3x3x3の全位置をリストに格納
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
		// Past: ランダムに12個除外
		std::random_device rd;
		std::mt19937 g(rd());
		std::shuffle(allPositions.begin(), allPositions.end(), g);  // ランダムに並べ替え

		excludedPositions.insert(allPositions.begin(), allPositions.begin() + 12);
	}

	else if (state == SceneGame::Era::Future) {
		// Future: 固定された12個の位置を除外
		const std::vector<DirectX::XMINT3> predefinedExcludedPositions = {
			{0, 0, 0}, {2, 0, 0}, {1, 1, 0}, {0, 2, 0}, {2, 2, 0},
			{0, 0, 2}, {0, 1, 1}, {0, 2, 2}, {2, 0, 2}, {2, 1, 1}, {2, 2, 2},
			{1, 1, 2}
		};
		excludedPositions.insert(predefinedExcludedPositions.begin(), predefinedExcludedPositions.end());
	}

	int createQube = 3;//生成するキューブの範囲
	// 残りの位置にキューブを生成
	for (int z = 0; z < createQube; ++z) {
		for (int y = 0; y < createQube; ++y) {

			for (int x = 0; x < createQube; ++x) {
				DirectX::XMINT3 pos = { x, y, z };

				// 除外リストに含まれている位置かどうかをチェック
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
		// すべてのキューブを解放する再帰関数
		std::function<void(Cube*)> deleteCube = [&](Cube* cube) {
			if (cube) {
				// 子キューブを先に解放
				for (Cube* child : cube->m_children) {
					deleteCube(child);
				}
				cube->m_children.clear(); // 子キューブリストをクリア
				delete cube; // キューブ自身を解放
			}
		};

		// ルートキューブを解放
		deleteCube(m_rootCube);
		m_rootCube = nullptr; // ルートキューブをnullptrに
	}

	// カメラコントローラーの解放
	if (m_cameraController) {
		delete m_cameraController;
		m_cameraController = nullptr;
	}
}
