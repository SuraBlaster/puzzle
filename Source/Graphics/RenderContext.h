#pragma once

#include <d3d11.h>
#include <DirectXMath.h>


//���s�������
struct DirectionalLightData
{
	DirectX::XMFLOAT4 direction;//����
	DirectX::XMFLOAT4 color;//�F
};

//�_�������
struct PointLightData
{
	DirectX::XMFLOAT4 position;
	DirectX::XMFLOAT4 color;
	float range;//�͈�
	DirectX::XMFLOAT3 dummy;
};
static constexpr int PointLightMax = 8;

//�X�|�b�g���C�g���
struct SpotLightData
{
	DirectX::XMFLOAT4 position;  //���W
	DirectX::XMFLOAT4 direction;//����
	DirectX::XMFLOAT4 color;    //�F
	float range;     //�͈�
	float innerCorn; //�C���i�[�p�x�͈�
	float outerCorn; //�A�E�^�[�p�x�͈�
	float dummy;
};

//�X�|�b�g���C�g�̍ő吔
static constexpr int SpotLightMax = 8;

// �����_�[�R���e�L�X�g
struct RenderContext
{
	//	�J�������
	DirectX::XMFLOAT4       viewPosition;
	DirectX::XMFLOAT4X4		view;
	DirectX::XMFLOAT4X4		projection;

	//���C�g���
	DirectX::XMFLOAT4    ambientLightColor;
	DirectionalLightData directionalLightData;
	PointLightData       pointLightData[PointLightMax];//�_�������
	SpotLightData        spotLightData[SpotLightMax];//�X�|�b�g���C�g���
	int                  pointLightCount = 0;//�_������
	int                  spotLightCount = 0;        //�X�|�b�g���C�g��


};