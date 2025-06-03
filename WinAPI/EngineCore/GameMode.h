#pragma once
#include "Actor.h"

// 설명 : 게임에서 가장 먼저 만들어지고 게임의 규칙을 정하는 클래스
class AGameMode : public AActor
{
public:
	AGameMode();
	~AGameMode();

protected:

private:
	// delete Function
	AGameMode(const AGameMode& _Other) = delete;
	AGameMode(AGameMode&& _Other) noexcept = delete;
	AGameMode& operator=(const AGameMode& _Other) = delete;
	AGameMode& operator=(AGameMode&& _Other) noexcept = delete;
};

