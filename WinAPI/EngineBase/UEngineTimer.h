#pragma once
#include <Windows.h>

// 설명 : 시간 측정 및 DeltaTime 반환
class UEngineTimer
{
public:
	UEngineTimer();
	~UEngineTimer();

	void TimeCheck();

	float GetDeltaTime()
	{
		return fDeltaTime;
	}

	double GetDoubleDeltaTime()
	{
		return DeltaTime;
	}

	void TimeStart();

	float End();
	
	double DEnd();


protected:

private:
	// LARGE_INTEGER : 64비트 정수(long long)
	LARGE_INTEGER Count    = LARGE_INTEGER(); // QuadPart => 1 => 나는 1초에 1을 센다.
	LARGE_INTEGER PrevTime = LARGE_INTEGER(); // LONGLONG QuadPart => 10000 / 1(Count)
	LARGE_INTEGER CurTime  = LARGE_INTEGER(); // LONGLONG QuadPart => 10002(Cur) - 10000(Prev) = 2(Tick)

	
	double TimeCounter = 0.0;
	double DeltaTime   = 0.0;
	float  fDeltaTime  = 0.0f;

	// delete Function
	UEngineTimer(const UEngineTimer& _Other) = delete;
	UEngineTimer(UEngineTimer&& _Other) noexcept = delete;
	UEngineTimer& operator=(const UEngineTimer& _Other) = delete;
	UEngineTimer& operator=(UEngineTimer&& _Other) noexcept = delete;

};

