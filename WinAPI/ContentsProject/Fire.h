#pragma once
#include "RoomObject.h"

// Ό³Έν :
class AFire : public ARoomObject
{
public:
	// constrcuter destructer
	AFire();
	~AFire();

	// delete Function
	AFire(const AFire& _Other) = delete;
	AFire(AFire&& _Other) noexcept = delete;
	AFire& operator=(const AFire& _Other) = delete;
	AFire& operator=(AFire&& _Other) noexcept = delete;

	void BeginPlay();
	void Tick(float _DeltaTime);

	void SwitchAnimation();

protected:

private:
	USpriteRenderer* FlameBaseRenderer = nullptr;
	FVector2D BodyCollisionScale = FVector2D::ZERO;
	FVector2D BodyCollisionLocation = FVector2D::ZERO;
};

