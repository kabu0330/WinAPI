#pragma once
#include "RoomObject.h"

// Ό³Έν :
class AItemAlter : public ARoomObject
{
public:
	// constrcuter destructer
	AItemAlter();
	~AItemAlter();

	// delete Function
	AItemAlter(const AItemAlter& _Other) = delete;
	AItemAlter(AItemAlter&& _Other) noexcept = delete;
	AItemAlter& operator=(const AItemAlter& _Other) = delete;
	AItemAlter& operator=(AItemAlter&& _Other) noexcept = delete;

	void BeginPlay();
	void Tick(float _DeltaTime);

	void BlockPlayerCollision(class APlayer* _Player, FVector2D _Pos) override;

	void BounceBack(float _DeltaTime);
	void InitDir();

protected:

private:
	bool IsCollision = false;
	FVector2D InitPos = FVector2D::ZERO;
	FVector2D Dir = FVector2D::ZERO;

	float TimeElapsed = 0.0f;
	float Duration = 0.4f;
};

