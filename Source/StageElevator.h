#include "Graphics/Shader.h"
#include "Graphics/Model.h"
#include "Stage.h"
#include "Collision.h"

//�ړ����X�e�[�W
class StageElevator : public Stage
{
public:
	StageElevator();
	~StageElevator();

	//�X�V����
	void Update(float elapsedTile)override;

	//�`�揈��
	void Render(ID3D11DeviceContext* dc, Shader* shader)override;

	//���C�L���X�g
	bool RayCast(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, HitResult& hit)override;

	//�X�^�[�g�ʒu�ݒ�
	void SetStartPoint(const DirectX::XMFLOAT3& start) { this->start = start; }

	//�S�[���ʒu�ݒ�
	void SetGoalPoint(const DirectX::XMFLOAT3& goal) { this->goal = goal; }

	//�g���N�ݒ�
	void SetTorque(const DirectX::XMFLOAT3& torque) { this->torque = torque; }

	void SetPosition(const DirectX::XMFLOAT3 posititon) { this->position = position; }

	//���̊J�X�V����
	void UpdateOpenState();


private:
	//�s��X�V����
	void UpdateTransform();

	//�A�j���[�V����
	enum Animation
	{
		Anim_Idle,
		Anim_Open
	};

	//�X�e�[�g
	enum class State
	{
		Idle,
		Open,
	};

	//�ҋ@�X�e�[�g�ɑJ��
	void IdleState();

	//���̔��J�X�e�[�g�ɑJ��
	void OpenState();

private:
	Model* model = nullptr;
	DirectX::XMFLOAT3 position = { 0,0,0 };
	DirectX::XMFLOAT3 angle = { 0,0,0 };
	DirectX::XMFLOAT3 scale = { 1,1,1 };
	DirectX::XMFLOAT3 oldAngle = { 0,0,0 };
	//�ϊ��s��44
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