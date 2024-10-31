#include "Mathf.h"
#include <random>

float Mathf::Lerp(float a, float b, float t)
{
    return a * (1.0f - t) + (b * t);
}

//�w��͈͓��̃����_���l���v�Z����
float Mathf::RandomRange(float min, float max)
{
    static std::random_device rd;                  // ���������̎�
    static std::mt19937 gen(rd());                 // �����Z���k�E�c�C�X�^�[�v���W�F�N�^�[
    std::uniform_real_distribution<float> dis(min, max); // �w��͈̗͂������z

    return dis(gen);
}
