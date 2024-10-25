#include "PreCompile.h"
#include "UEngineTimer.h"

UEngineTimer::UEngineTimer()
{
    // 1�ʿ� �󸶸� �� �� �ִ��� ī��Ʈ
    QueryPerformanceFrequency(&Count);

    // ������ ������ �ϱ� ���ؼ� TimeCheck�� �ƴ� ���⼭ �ʱ�ȭ
    TimeCounter = static_cast<double>(Count.QuadPart);

    // ���α׷� ���ۺ��� �ð��� �󸶳� ��������?
    QueryPerformanceCounter(&PrevTime);
    QueryPerformanceCounter(&CurTime);
}

UEngineTimer::~UEngineTimer()
{
}

void UEngineTimer::TimeCheck()
{
    QueryPerformanceCounter(&CurTime);

    // Tick : ���� QuadPart - ���� QuadPart = ƽ�� �� �� ���� ������ ��
    double Tick = static_cast<double>(CurTime.QuadPart - PrevTime.QuadPart);
    DeltaTime = Tick / TimeCounter;
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

