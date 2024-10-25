#pragma once
#include <Windows.h>

// 설명 :
class UEngineTimer
{
public:
	// constrcuter destructer
	UEngineTimer();
	~UEngineTimer();

	// delete Function
	UEngineTimer(const UEngineTimer& _Other) = delete;
	UEngineTimer(UEngineTimer&& _Other) noexcept = delete;
	UEngineTimer& operator=(const UEngineTimer& _Other) = delete;
	UEngineTimer& operator=(UEngineTimer&& _Other) noexcept = delete;

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

	LARGE_INTEGER Count = LARGE_INTEGER(); // QuadPart => 1 => 나는 1초에 1을 셀 수 있어요.
	LARGE_INTEGER PrevTime = LARGE_INTEGER(); // LONGLONG QuadPart => 10000 / 1(Count)
	LARGE_INTEGER CurTime = LARGE_INTEGER(); // LONGLONG QuadPart => 10002(Cur) - 10000(Prev) = 2(Tick)

	
	double TimeCounter = 0.0;
	double DeltaTime = 0.0;
	float fDeltaTime = 0.0f;


};

