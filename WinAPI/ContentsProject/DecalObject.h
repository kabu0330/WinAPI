#pragma once
#include "RoomObject.h"

// Ό³Έν :
class ADecalObject : public ARoomObject
{
public:
	// constrcuter destructer
	ADecalObject();
	~ADecalObject();

	// delete Function
	ADecalObject(const ADecalObject& _Other) = delete;
	ADecalObject(ADecalObject&& _Other) noexcept = delete;
	ADecalObject& operator=(const ADecalObject& _Other) = delete;
	ADecalObject& operator=(ADecalObject&& _Other) noexcept = delete;

	void BeginPlay() override;
	void Tick(float _DeltaTime) override;

	void SetSprite(std::string_view _AnimName, FVector2D _Scale) override
	{
		std::string AnimName = _AnimName.data();
		BodyRenderer->ChangeAnimation(AnimName);
		BodyRenderer->SetComponentScale(_Scale);
	}

	void Move(float _DeltaTime);
	void SetMove(AActor* _Actor = nullptr);
	void SetMove(AActor* _Actor, FVector2D _Pos);

	void DestroyCollision() override {};
	void SwitchAnimation() override {};

protected:

private:
	bool IsMove = false;
	FVector2D Force = FVector2D::ZERO;
	FVector2D TargetPos = FVector2D::ZERO;
	float TimeElapsed = 0.0f;


};

