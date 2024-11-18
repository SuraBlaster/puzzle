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
	// 入力更新処理
	InputMoveCube();

	// 速度更新処理
	UpdateVelocity(elapsedTime);

	// 衝突判定処理
	Collision();

	//オブジェクト行列更新
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

	// デバッグ表示
	{
		ImGui::Separator();
		StageRootCube* rootCube = StageCubeManager::Instance().GetRootCube();

		if (ImGui::TreeNode("Rootcube"))
		{
			if (rootCube->m_selected != nullptr)
			{
				DirectX::XMFLOAT3 selectedPos = rootCube->m_selected->GetPosition();
				int pos[3] = { static_cast<int>(selectedPos.x), static_cast<int>(selectedPos.y), static_cast<int>(selectedPos.z) };
				ImGui::InputInt3("m_serected Position", pos);  // 表示用に位置を整数配列に変換
			}
			ImGui::TreePop();
		}
	}
}

/// <summary>
/// ステージ用ルートキューブ
/// </summary>
StageRootCube::StageRootCube()
{
	model = new Model("Data/Model/Cube/Cube.mdl");

	m_cubeType = eCubeType::Root;
}

//子キューブを選択する処理
void StageRootCube::SelectChild(DirectX::XMINT3 pos) {
	DirectX::XMFLOAT3 scale = { 1.0f, 1.0f, 1.0f };

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
		m_selected->SetScale({ 0.5f, 0.5f, 0.5f });
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

	// m_childrenMapを新しい位置に更新
	m_childrenMap.erase(selectedIndex);
	m_childrenMap[targetPos] = m_selected;

	// selectedIndexとm_childrenMapの新しい位置を更新
	selectedIndex = targetPos;

	// 位置にキューブが存在するか再確認
	auto it = m_childrenMap.find(selectedIndex);
	if (it != m_childrenMap.end() && it->second != nullptr) {
		m_selected = it->second;
		m_selected->SetScale({ 0.5f, 0.5f, 0.5f });
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
		m_selected->SetScale({ 0.5f, 0.5f, 0.5f });
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

//選択されたキューブを移動または仮移動位置を選択
void StageRootCube::InputMoveCube()
{
	GamePad& gamePad = Input::Instance().GetGamePad();

	DirectX::XMINT3 position{};

	/*※キューブ移動の流れ
	 WASDキーでキューブを選択
			 ↓
	選択したキューブをENTERキーで移動開始
			  ↓
	WASDキーで移動場所を選択
			  ↓
	移動場所をENTERで決定する*/


	// 移動するキューブが選択された後に移動場所を選択
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
	// 移動モードかどうかで処理を分ける
	if (isEnterKey)
		SelectMovePosition(position);  // 移動モードなら仮移動位置を選択
	else
		SelectChild(position);  // 通常モードならキューブを選択

	// Enterキーで移動モードの切り替え
	if (gamePad.GetButtonDown() & GamePad::BTN_ENTER) {
		if (isEnterKey) {
			MoveSelectedChild();  // 移動確定
			SelectChild(position); // 移動後に選択を行う
		}
		isEnterKey = !isEnterKey;  // Enterキーが押されるたびに移動モードを切り替える
	}
}

// <summary>
 //ステージ用床キューブ
 //</summary>
StageFloorCube::StageFloorCube()
{
	model = new Model("Data/Model/Cube/Cube.mdl");

	m_cubeType = eCubeType::Floor;
}

/// <summary>
/// 動かない障害物用キューブ
/// </summary>
ObstacleCube::ObstacleCube()
{
	model = new Model("Data/Model/Cube/Cube.mdl");

	m_cubeType = eCubeType::Obstacle;
}

/// <summary>
/// ステージ用キューブ管理
/// </summary>
void StageCubeManager::Initialize() {
    m_rootCube = new StageRootCube();
    m_rootCube->SetAngle({ 0, 0, 0 });
    m_rootCube->SetPosition({ 0, -5, 0 });

    // 3x3x3の全位置をリストに格納
    std::vector<DirectX::XMINT3> allPositions;
    for (int z = 0; z < 3; ++z) {
        for (int y = 0; y < 3; ++y) {
            for (int x = 0; x < 3; ++x) {
                allPositions.push_back({ x, y, z });
            }
        }
    }

    // 12個の位置をランダムに除外する
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(allPositions.begin(), allPositions.end(), g);  // ランダムに並べ替え

    // 最初の12個を除外する位置として選ぶ
    std::unordered_set<DirectX::XMINT3, XMINT3Hash> excludedPositions(allPositions.begin(), allPositions.begin() + 12);

    // 残りの位置にキューブを生成
    for (int z = 0; z < 3; ++z) {
        for (int y = 0; y < 3; ++y) {
            for (int x = 0; x < 3; ++x) {
                DirectX::XMINT3 pos = { x, y, z };
                
                // 除外リストに含まれている位置かどうかをチェック
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
