#pragma once
#include "Graphics/Shader.h"
#include "Graphics/Model.h"
#include "Stage.h"
#include "Collision.h"
#include <SceneGame.h>

//移動床ステージ
class StageElevator2 : public Stage
{
public:
	StageElevator2();
	~StageElevator2();

	//更新処理
	void Update(float elapsedTile)override;

	//描画処理
	void Render(ID3D11DeviceContext* dc, Shader* shader)override;

	//レイキャスト
	bool RayCast(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, HitResult& hit)override;

	//スタート位置設定
	void SetStartPoint(const DirectX::XMFLOAT3& start) { this->start = start; }

	//ゴール位置設定
	void SetGoalPoint(const DirectX::XMFLOAT3& goal) { this->goal = goal; }

	//トルク設定
	void SetTorque(const DirectX::XMFLOAT3& torque) { this->torque = torque; }

	void SetPosition(const DirectX::XMFLOAT3 posititon) { this->position = position; }

	//扉の通常状態更新処理
	void UpdateIdleState();

	//扉の閉じる更新処理
	void UpdateCloseState();

	//扉の開く更新処理
	void UpdateOpenState();

	bool CollisionNodeVsPlayer(const char* nodeName, float nodeRadius);

private:
	//行列更新処理
	void UpdateTransform();

	//アニメーション
	enum Animation
	{
		Anim_Close,
		Anim_Open,
	};

	//ステート
	enum class State
	{
		Idle,
		Close,
		Open,
	};

	//通常ステートに遷移
	void IdleState();

	//閉じるステートに遷移
	void CloseState();

	//開くステートに遷移
	void OpenState();

private:
	Model* model = nullptr;
	DirectX::XMFLOAT3 position = { 0,0,0 };
	DirectX::XMFLOAT3 angle = { 0,0,0 };
	DirectX::XMFLOAT3 scale = { 1,1,1 };
	DirectX::XMFLOAT3 oldAngle = { 0,0,0 };
	//変換行列44
	DirectX::XMFLOAT4X4 transform = {
		1,0,0,0,
		0,1,0,0,
		0,0,1,0,
		0,0,0,1
	};

	DirectX::XMFLOAT4X4 oldTransform = {
		 1,0,0,0,
		 0,1,0,0,
		 0,0,1,0,
		 0,0,0,1
	};

	DirectX::XMFLOAT3 torque = { 0,0,0 };
	DirectX::XMFLOAT3 start = { 0,0,0 };
	DirectX::XMFLOAT3 goal = { 0,0,0 };

	float height = 5.0f;
	float radius = 5.0f;

	State state = State::Idle;

	int timer;

	SceneGame::Era era;

};