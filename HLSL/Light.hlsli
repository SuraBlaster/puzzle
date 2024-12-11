// ���s�������
struct DirectionalLightData
{
	float4	direction;	// ����
	float4	color;		// �F
};

//�_�������
struct PointLightData
{
	float4	position;	// ���W
	float4	color;		// �F
	float	range;		// �͈�
	float3	dummy;

};

//�_�����̍ő吔
static const int PointLightMax = 8;

//�X�|�b�g���C�g���
struct SpotLightData
{
	float4 position;//���W
	float4 direction;//����
	float4 color;//�͈�
	float range; //�͈�
	float innerCorn;
	float outerCorn;
	float dummy;
};
//�X�|�b�g���C�g�̍ő吔
static const int SpotLightMax = 8;


//--------------------------------------------
//	�����o�[�g�g�U���ˌv�Z�֐�
//--------------------------------------------
// normal		: �@��(���K���ς�)
// lightVector	: ���˃x�N�g��(���K���ς�)
// lightColor	: ���ˌ��F
// kd			: ���˗�(���˂̋���)
// �Ԃ��l		: �g�U���ːF
float3	CalcLambertDiffuse(float3 normal, float3 lightVector, float3 lightColor, float3 kd)
{
	// �@���ƃ��C�g�����ւ̃x�N�g���ł̓��� 
	// ���ς������ʁA���C�g���猩�ė������̖ʂ��ƃ}�C�i�X�̌��ʂ��o�Ă��܂�����
	// ��������̂܂܏�Z����ƐF�����������Ȃ�܂��B
	// 0�ȉ��ł����0�ɂȂ�悤�ɐ��������܂��傤
	float b = max(dot(normal, -lightVector), 0.0);

	// ���ˌ��F�Ɠ��ς̌��ʁA�y�є��˗������ׂď�Z���ĕԋp���܂��傤�B
	return b * lightColor * kd;
}
//--------------------------------------------
//	�n�[�t�����o�[�g�g�U���ˌv�Z�֐�
//--------------------------------------------
// normal		: �@��(���K���ς�)
// lightVector	: ���˃x�N�g��(���K���ς�)
// lightColor	: ���ˌ��F
// kd			: ���˗�(���˂̋���)
float3 ClacHalfLambert(float3 normal, float3 lightVector, float3 lightColor, float3 kd)
{
	float d = dot(normal, lightVector) * 0.5 + 0.5;
	return lightColor * d * kd;
}


//--------------------------------------------
//	�t�H���̋��ʔ��ˌv�Z�֐�
//--------------------------------------------
// normal		: �@��(���K���ς�)
// lightVector	: ���˃x�N�g��(���K���ς�)
// lightColor	: ���ˌ��F
// eyeVector		: �����x�N�g��(���K���ς�)
// shininess		: ����x(���������l)
// ks			: ���˗�(���˂̋���)
// �Ԃ��l		: ���ʔ��ːF
float3 CalcPhongSpecular(float3 normal, float3 lightVector, float3 lightColor, float3 eyeVector, float shininess, float3 ks)
{
	// ���˃x�N�g����@�������ɑ΂��Ĕ��˂��������˃x�N�g�����Z�o
	float3 R = reflect(-lightVector, normal);

	// ���˃x�N�g���Ǝ����x�N�g���Ƃœ���
	// ���ς������ʂ�0�ȉ��ł����0�ɂȂ�悤�ɐ���
	float d = max(dot(R, eyeVector), 0.0);

	// ���򏈗�
	// ���ˌ��F�Ɠ��ς̌��ʂ��w���I�ɕω�������
	d = pow(d, shininess);

	// ���ˌ��F�Ɠ��ς̌��ʁA�y�є��˗������ׂď�Z���ĕԋp
	return d * lightColor * ks;
}


//--------------------------------------------
// �������C�g
//--------------------------------------------
// normal		: �@��(���K���ς�)
// eyeVector	: �����x�N�g��(���K���ς�)
// lightVector	: ���˃x�N�g��(���K���ς�)
// lightColor	: ���ˌ��F
// rimPower		: �������C�g�̋���(�����l�̓e�L�g�[�Ȃ̂Ŏ����Őݒ肷�邪�g)
float3 CalcRimLight(float3 normal, float3 eyeVector, float3 lightVector, float3 lightColor, float rimPower = 3.0f)
{
	float rim = 1.0f - saturate(dot(normal, -eyeVector));

	return  lightColor * pow(rim, rimPower) * saturate(dot(lightVector, -eyeVector));
}

//--------------------------------------------
//	�g�D�[���g�U���ˌv�Z�֐�
//--------------------------------------------
// toonTexture		: �g�D�[���pU���������v�e�N�X�`��
// toonSamplerState	: �g�D�[���p�T���v���[�X�e�[�g
// normal			: �@��(���K���ς�)
// lightVector		: ���˃x�N�g��(���K���ς�)
// lightColor		: ���ˌ��F
// kd				: ���˗�(���˂̋���)
// �Ԃ��l			: �g�U���ːF
float3	CalcToonDiffuse(Texture2D toonTexture, SamplerState toonSamplerState, float3 normal, float3 lightVector, float3 lightColor, float3 kd)
{
	//	�@���Ɠ��˃x�N�g������U���W�����߂�
	float u = saturate(dot(normal, -lightVector) * 0.5f + 0.5f);

	// �g�D�[���e�N�X�`������F���擾����
	float3 c = toonTexture.Sample(toonSamplerState, float2(u, 0.1f));

	return lightColor * c * kd;
}