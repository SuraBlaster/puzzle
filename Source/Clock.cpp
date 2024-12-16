#include "Clock.h"
#include "Input/Input.h"
#include "ClockManager.h"
#include "random"

Clock::Clock()
{
    //�����ݒ�
    clockModel = std::make_unique<Model>("Data/Model/Clock/clock.mdl");
    scale.x = scale.y = scale.z = 0.005f;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<>dist(0, 5);

    ShortHandRotate = dist(gen) * 90.0f;
    longHandRotate = ShortHandRotate * 24.0f;

}
void Clock::Update(float elapsedTime)
{
    //��]�p�x
    UpdateRotationHand();

    // �Z�j�ƒ��j�̉�]�����ۂ̃I�u�W�F�N�g�ɔ��f
    m_shortHand->SetAngle(DirectX::XMFLOAT3(0.0f, 0.0f, DirectX::XMConvertToRadians(ShortHandRotate)));
    if (m_longhand) {
        m_longhand->SetAngle(DirectX::XMFLOAT3(0.0f, 0.0f, DirectX::XMConvertToRadians(longHandRotate)));
    }

    // �ʏ�̎��v�X�V����
    UpdateTransform();

    // ���v���f���̍s����X�V
    clockModel->UpdateTransform(transform);
}

void Clock::Render(ID3D11DeviceContext* dc, Shader* shader)
{
    // ���v�Ɛj�̕`�揈��
    shader->Draw(dc, clockModel.get());//���v�{�̂̕`��
}

void Clock::UpdateRotationHand()
{
    // �Q�[���p�b�h�̓��͂��擾
    GamePad& gamePad = Input::Instance().GetGamePad();

    // A�{�^���������ꂽ���`�F�b�N
    if (gamePad.GetButtonDown() & GamePad::BTN_1)
    {
        //�Z�j�̉�]
        ClockManager::Instance().GetClock(1)->SetShortHandAngle(
            ClockManager::Instance().GetClock(1)->GetShortHandRotation() + 1.5f);

        ClockManager::Instance().GetClock(2)->SetShortHandAngle(
            ClockManager::Instance().GetClock(2)->GetShortHandRotation() + 1.5f);

        ClockManager::Instance().GetClock(3)->SetShortHandAngle(
            ClockManager::Instance().GetClock(3)->GetShortHandRotation() + 6.0f);

        //���j�̉�]
        ClockManager::Instance().GetClock(1)->SetLongHandAngle(
            ClockManager::Instance().GetClock(1)->GetLongHandRotation() + 18.0f);

        ClockManager::Instance().GetClock(2)->SetLongHandAngle(
            ClockManager::Instance().GetClock(2)->GetLongHandRotation() + 18.0f);

        ClockManager::Instance().GetClock(3)->SetLongHandAngle(
            ClockManager::Instance().GetClock(3)->GetLongHandRotation() + 72.0f);
    }

    if (gamePad.GetButtonDown() & GamePad::BTN_2)
    {
        //�Z�j�̉�]
        ClockManager::Instance().GetClock(0)->SetShortHandAngle(
            ClockManager::Instance().GetClock(0)->GetShortHandRotation() - 1.5f);

        ClockManager::Instance().GetClock(2)->SetShortHandAngle(
            ClockManager::Instance().GetClock(2)->GetShortHandRotation() - 3.0f);

        ClockManager::Instance().GetClock(3)->SetShortHandAngle(
            ClockManager::Instance().GetClock(3)->GetShortHandRotation() + 3.0f);

        ClockManager::Instance().GetClock(4)->SetShortHandAngle(
            ClockManager::Instance().GetClock(4)->GetShortHandRotation() + 6.0f);

        //���j�̉�]
        ClockManager::Instance().GetClock(0)->SetLongHandAngle(
            ClockManager::Instance().GetClock(0)->GetLongHandRotation() - 18.0f);

        ClockManager::Instance().GetClock(2)->SetLongHandAngle(
            ClockManager::Instance().GetClock(2)->GetLongHandRotation() - 36.0f);

        ClockManager::Instance().GetClock(3)->SetLongHandAngle(
            ClockManager::Instance().GetClock(3)->GetLongHandRotation() + 36.0f);

        ClockManager::Instance().GetClock(4)->SetLongHandAngle(
            ClockManager::Instance().GetClock(4)->GetLongHandRotation() + 72.0f);
    }

    if (gamePad.GetButtonDown() & GamePad::BTN_3)
    {
        //�Z�j�̉�]
        ClockManager::Instance().GetClock(0)->SetShortHandAngle(
            ClockManager::Instance().GetClock(0)->GetShortHandRotation() + 6.0f);

        ClockManager::Instance().GetClock(1)->SetShortHandAngle(
            ClockManager::Instance().GetClock(1)->GetShortHandRotation() - 1.5f);

        ClockManager::Instance().GetClock(2)->SetShortHandAngle(
            ClockManager::Instance().GetClock(2)->GetShortHandRotation() + 1.5f);

        ClockManager::Instance().GetClock(3)->SetShortHandAngle(
            ClockManager::Instance().GetClock(3)->GetShortHandRotation() - 3.0f);

        ClockManager::Instance().GetClock(4)->SetShortHandAngle(
            ClockManager::Instance().GetClock(4)->GetShortHandRotation() - 1.5f);

        //���j�̉�]
        ClockManager::Instance().GetClock(0)->SetLongHandAngle(
            ClockManager::Instance().GetClock(0)->GetLongHandRotation() + 72.0f);

        ClockManager::Instance().GetClock(1)->SetLongHandAngle(
            ClockManager::Instance().GetClock(1)->GetLongHandRotation() - 18.0f);

        ClockManager::Instance().GetClock(2)->SetLongHandAngle(
            ClockManager::Instance().GetClock(2)->GetLongHandRotation() + 18.0f);

        ClockManager::Instance().GetClock(3)->SetLongHandAngle(
            ClockManager::Instance().GetClock(3)->GetLongHandRotation() - 36.0f);

        ClockManager::Instance().GetClock(4)->SetLongHandAngle(
            ClockManager::Instance().GetClock(4)->GetLongHandRotation() - 18.0f);
    }

}


bool Clock::IsAlignedTo12Clock() const
{
    auto NormalizeAngle = [](float angle, float mod) {
        float normalized = std::fmod(angle, mod);  // mod�͈̔͂Ő��K��
        return (normalized < 0.0f) ? normalized + mod : normalized;
    };

    return (NormalizeAngle(ShortHandRotate, 72.0f) == 0.0f &&
        NormalizeAngle(longHandRotate, 72.0f) == 0.0f);
}


LongHand::LongHand()
{
    //�����ݒ�
    longHandModel = std::make_unique<Model>("Data/Model/Clock/tyousin.mdl");
    scale.x = scale.y = scale.z = 0.005f;
}

void LongHand::Update(float elapsedTime)
{
    UpdateTransform();

    //�s��X�V����
    longHandModel->UpdateTransform(transform);
}

void LongHand::Render(ID3D11DeviceContext* dc, Shader* shader)
{
    //���j�̕`�� 
    shader->Draw(dc, longHandModel.get());
}

ShortHand::ShortHand()
{
    shortHandModel = std::make_unique<Model>("Data/Model/Clock/tansin.mdl");
    scale.x = scale.y = scale.z = 0.005f;
}

void ShortHand::Update(float elapsedTime)
{
    UpdateTransform();

    //�s��X�V����
    shortHandModel->UpdateTransform(transform);
}

void ShortHand::Render(ID3D11DeviceContext* dc, Shader* shader)
{
    //���j�̕`��
    shader->Draw(dc, shortHandModel.get());

}