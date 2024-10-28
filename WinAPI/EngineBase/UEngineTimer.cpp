#include "PreCompile.h"
#include "UEngineTimer.h"

UEngineTimer::UEngineTimer()
{
    // 1�ʿ� �󸶸� �� �� �ִ��� ī��Ʈ
    // �鸸 �̻��� �ſ� ū ���� ��ȯ�ȴ�.
    QueryPerformanceFrequency(&Count);

    // ������ ������ �ϱ� ���ؼ� TimeCheck�� �ƴ� ���⼭ �ʱ�ȭ
    TimeCounter = static_cast<double>(Count.QuadPart);

    // ���α׷� ���ۺ��� �ð��� �󸶳� ��������?
    // ���� ī��Ʈ ���� ��ȯ�Ѵ�.
    QueryPerformanceCounter(&PrevTime);
    QueryPerformanceCounter(&CurTime);
}

UEngineTimer::~UEngineTimer()
{
}

// Tick ���, DeltaTime ��ȯ
void UEngineTimer::TimeCheck()
{
    QueryPerformanceCounter(&CurTime);

    // Tick : ���� QuadPart - ���� QuadPart = ƽ(Tick)�� �� �� ���� ������ ��
    double Tick = static_cast<double>(CurTime.QuadPart - PrevTime.QuadPart);
    DeltaTime = Tick / TimeCounter;

    // float���� ����ȯ. ��κ��� ������ ��ŸŸ���� float���� ���
    fDeltaTime = static_cast<float>(DeltaTime);

    // ���� QuadPart�� PrevTime�� ����
    PrevTime.QuadPart = CurTime.QuadPart;
}

void UEngineTimer::TimeStart()
{
    QueryPerformanceCounter(&PrevTime);
}

float UEngineTimer::End()
{
    TimeCheck();
    return GetDeltaTime();
}

double UEngineTimer::DEnd()
{
    TimeCheck();
    return GetDeltaTime();
}

