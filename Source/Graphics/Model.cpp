#include "Graphics/Graphics.h"
#include "Graphics/Model.h"
#include <functional>
#include "ResourceManager.h"

// �R���X�g���N�^
Model::Model(const char* filename)
{
	// ���\�[�X�ǂݍ���
	//resource = std::make_shared<ModelResource>();
	//resource->Load(Graphics::Instance().GetDevice(), filename);
	resource = ResourceManager::Instance().LoadModelResource(filename);

	// �m�[�h
	const std::vector<ModelResource::Node>& resNodes = resource->GetNodes();

	nodes.resize(resNodes.size());
	for (size_t nodeIndex = 0; nodeIndex < nodes.size(); ++nodeIndex)
	{
		auto&& src = resNodes.at(nodeIndex);
		auto&& dst = nodes.at(nodeIndex);

		dst.name = src.name.c_str();
		dst.parent = src.parentIndex >= 0 ? &nodes.at(src.parentIndex) : nullptr;
		dst.scale = src.scale;
		dst.rotate = src.rotate;
		dst.translate = src.translate;

		if (dst.parent != nullptr)
		{
			dst.parent->children.emplace_back(&dst);
		}
	}

	// �s��v�Z
	const DirectX::XMFLOAT4X4 transform = { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 };
	UpdateTransform(transform);
}

// �ϊ��s��v�Z
void Model::UpdateTransform(const DirectX::XMFLOAT4X4& transform)
{
	DirectX::XMMATRIX Transform = DirectX::XMLoadFloat4x4(&transform);

	for (Node& node : nodes)
	{
		// ���[�J���s��Z�o
		DirectX::XMMATRIX S = DirectX::XMMatrixScaling(node.scale.x, node.scale.y, node.scale.z);
		DirectX::XMMATRIX R = DirectX::XMMatrixRotationQuaternion(DirectX::XMLoadFloat4(&node.rotate));
		DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(node.translate.x, node.translate.y, node.translate.z);
		DirectX::XMMATRIX LocalTransform = S * R * T;

		// ���[���h�s��Z�o
		DirectX::XMMATRIX ParentTransform;
		if (node.parent != nullptr)
		{
			ParentTransform = DirectX::XMLoadFloat4x4(&node.parent->worldTransform);
		}
		else
		{
			ParentTransform = Transform;
		}
		DirectX::XMMATRIX WorldTransform = LocalTransform * ParentTransform;

		// �v�Z���ʂ��i�[
		DirectX::XMStoreFloat4x4(&node.localTransform, LocalTransform);
		DirectX::XMStoreFloat4x4(&node.worldTransform, WorldTransform);
	}
}

void Model::UpdateAnimation(float elapsedTime)
{
	//�Đ����łȂ��Ȃ珈�����Ȃ�
	if (!IsPlayAnimation())return;

	//�u�����h���̌v�Z
	float blendRate = 1.0f;
	if (animationBlendTime < animationBlendSeconds)
	{
		blendRate = animationBlendTime / animationBlendSeconds;
		animationBlendTime += elapsedTime;
	}


	//�w��̃A�j���[�V�����f�[�^���擾
	const std::vector<ModelResource::Animation>& animations = resource->GetAnimations();
	const ModelResource::Animation& animation = animations.at(currentAnimationIndex);

	//�A�j���[�V�����f�[�^����L�[�t���[���f�[�^���X�g���擾
	const std::vector<ModelResource::Keyframe>& keyframes = animation.keyframes;
	int keyCount = static_cast<int>(keyframes.size());
	for (int keyIndex = 0; keyIndex < keyCount - 1; ++keyIndex)
	{
		//���݂̎��Ԃ��ǂ̃L�[�t���[���̊Ԃɂ��邩���肷��
		const ModelResource::Keyframe& keyframe0 = keyframes.at(keyIndex);
		const ModelResource::Keyframe& keyframe1 = keyframes.at(keyIndex + 1);

		if (currentAnimationSeconds >= keyframe0.seconds &&
			currentAnimationSeconds < keyframe1.seconds)
		{
			//�Đ����ԂƃL�[�t���[���̎��Ԃ���⊮�����Z�o����
			float rate = (currentAnimationSeconds - keyframe0.seconds) / (keyframe1.seconds - keyframe0.seconds);

			int nodeCount = static_cast<int>(nodes.size());
			
			for (int nodeIndex = 0; nodeIndex < nodeCount; ++nodeIndex)
			{
				//2�̃L�[�t���[���Ԃ̕⊮�v�Z
				const ModelResource::NodeKeyData& key0 = keyframe0.nodeKeys.at(nodeIndex);
				const ModelResource::NodeKeyData& key1 = keyframe1.nodeKeys.at(nodeIndex);

				Node& node = nodes[nodeIndex];

				if (blendRate < 1.0f)
				{
					//���݂̎p���Ǝ��̃L�[�t���[���Ƃ̎p���̕⊮
					// �X�P�[���̕��
					node.scale = DirectX::XMFLOAT3(
						key0.scale.x + (key1.scale.x - key0.scale.x) * blendRate,
						key0.scale.y + (key1.scale.y - key0.scale.y) * blendRate,
						key0.scale.z + (key1.scale.z - key0.scale.z) * blendRate
					);

					// ��]�̕��
					DirectX::XMVECTOR quat0 = DirectX::XMLoadFloat4(&key0.rotate);
					DirectX::XMVECTOR quat1 = DirectX::XMLoadFloat4(&key1.rotate);
					DirectX::XMVECTOR quat = DirectX::XMQuaternionSlerp(quat0, quat1, blendRate);
					DirectX::XMStoreFloat4(&node.rotate, quat);

					// ���s�ړ��̕��
					node.translate = DirectX::XMFLOAT3(
						key0.translate.x + (key1.translate.x - key0.translate.x) * blendRate,
						key0.translate.y + (key1.translate.y - key0.translate.y) * blendRate,
						key0.translate.z + (key1.translate.z - key0.translate.z) * blendRate
					);
				}
				else
				{
					// �X�P�[���̕��
					node.scale = DirectX::XMFLOAT3(
						key0.scale.x + (key1.scale.x - key0.scale.x) * rate,
						key0.scale.y + (key1.scale.y - key0.scale.y) * rate,
						key0.scale.z + (key1.scale.z - key0.scale.z) * rate
					);

					// ��]�̕��
					DirectX::XMVECTOR quat0 = DirectX::XMLoadFloat4(&key0.rotate);
					DirectX::XMVECTOR quat1 = DirectX::XMLoadFloat4(&key1.rotate);
					DirectX::XMVECTOR quat = DirectX::XMQuaternionSlerp(quat0, quat1, rate);
					DirectX::XMStoreFloat4(&node.rotate, quat);

					// ���s�ړ��̕��
					node.translate = DirectX::XMFLOAT3(
						key0.translate.x + (key1.translate.x - key0.translate.x) * rate,
						key0.translate.y + (key1.translate.y - key0.translate.y) * rate,
						key0.translate.z + (key1.translate.z - key0.translate.z) * rate
					);
				}
	
			}
			break;
		}
	}
	// �ŏI�t���[������
	if (animationEndFlag)
	{
		animationLoopFlag = false;
		currentAnimationIndex = -1;
		return;
	}

	//���Ԍo��
	currentAnimationSeconds += elapsedTime;

	//�Đ����Ԃ��o�ߎ��Ԃ𒴂�����
	if (currentAnimationSeconds >= animation.secondsLength)
	{
		if (animationLoopFlag)
		{
			//�Đ����Ԃ������߂�
			currentAnimationSeconds -= animation.secondsLength;
		}
		else
		{
			//�Đ��I�����Ԃɂ���
			currentAnimationSeconds = animation.secondsLength;
			animationEndFlag = true;
		}
	}

}

void Model::RewindAnimation(float rewindTime)
{
	// �A�j���[�V�������Ԃ������߂�
	currentAnimationSeconds -= rewindTime;
	
	if (animationLoopFlag)
	{
		// ���[�v�Đ����̓A�j���[�V�������Ōォ��ĊJ
		currentAnimationSeconds += resource->GetAnimations().at(currentAnimationIndex).secondsLength;
	}
	
}

void Model::PlayAnimation(int index,bool loop, float blendSeconds)
{
	currentAnimationIndex = index;
	currentAnimationSeconds = 0.0f;
	animationLoopFlag = loop;
	animationEndFlag = false;
	animationBlendTime = 0.0f;
	animationBlendSeconds = blendSeconds;
}

bool Model::IsPlayAnimation() const
{
	if (currentAnimationIndex < 0)return false;
	if (currentAnimationIndex >= resource->GetAnimations().size())return false;
	return true;
}

Model::Node* Model::FindNode(const char* name)
{

	// ���ׂẴ��[�g�m�[�h�Ō������J�n
	for (Node& node : nodes) {
		if (strcmp(node.name, name) == 0)
		{
			return &node;
		}
		
	}

	//������Ȃ�����
	return nullptr;
}


