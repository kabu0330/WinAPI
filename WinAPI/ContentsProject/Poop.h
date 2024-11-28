#pragma once
#include "RoomObject.h"

// Ό³Έν :
class APoop : public ARoomObject
{
public:
	// constrcuter destructer
	APoop();
	~APoop();

	// delete Function
	APoop(const APoop& _Other) = delete;
	APoop(APoop&& _Other) noexcept = delete;
	APoop& operator=(const APoop& _Other) = delete;
	APoop& operator=(APoop&& _Other) noexcept = delete;

	void BeginPlay();
	void Tick(float _DeltaTime);
	void CreateGib();
protected:

private:

};

