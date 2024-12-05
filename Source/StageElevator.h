#include "Graphics/Shader.h"
#include "Graphics/Model.h"
#include "Stage.h"
#include "Collision.h"

//移動床ステージ
class StageElevator : public Stage
{
public:
	StageElevator();
	~StageElevator();

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

	//扉の開閉更新処理
	void UpdateOpenState();


private:
	//行列更新処理
	void UpdateTransform();

	//アニメーション
	enum Animation
	{
		Anim_Idle,
		Anim_Open
	};

	//ステート
	enum class State
	{
		Idle,
		Open,
	};

	//待機ステートに遷移
	void IdleState();

	//左の扉開閉ステートに遷移
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

};