#include "PreCompile.h"
#include "UEngineTimer.h"

UEngineTimer::UEngineTimer()
{
    // 1초에 얼마를 셀 수 있는지 카운트
    // 백만 이상의 매우 큰 값이 반환된다.
    QueryPerformanceFrequency(&Count);

    // 연산을 빠르게 하기 위해서 TimeCheck가 아닌 여기서 초기화
    TimeCounter = static_cast<double>(Count.QuadPart);

    // 프로그램 시작부터 시간을 얼마나 세었는지?
    // 현재 카운트 값을 반환한다.
    QueryPerformanceCounter(&PrevTime);
    QueryPerformanceCounter(&CurTime);
}

UEngineTimer::~UEngineTimer()
{
}

// Tick 계산, DeltaTime 반환
void UEngineTimer::TimeCheck()
{
    QueryPerformanceCounter(&CurTime);

    // Tick : 현재 QuadPart - 직전 QuadPart = 틱(Tick)이 한 번 도는 동안의 값
    double Tick = static_cast<double>(CurTime.QuadPart - PrevTime.QuadPart);
    DeltaTime = Tick / TimeCounter;

    // float으로 형변환. 대부분의 게임이 델타타임을 float으로 사용
    fDeltaTime = static_cast<float>(DeltaTime);

    // 현재 QuadPart를 PrevTime에 저장
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

