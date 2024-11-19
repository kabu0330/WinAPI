#pragma once
#include "RoomObject.h"

// Ό³Έν :
class ABloodPool : public ARoomObject
{
public:
	// constrcuter destructer
	ABloodPool();
	~ABloodPool();

	// delete Function
	ABloodPool(const ABloodPool& _Other) = delete;
	ABloodPool(ABloodPool&& _Other) noexcept = delete;
	ABloodPool& operator=(const ABloodPool& _Other) = delete;
	ABloodPool& operator=(ABloodPool&& _Other) noexcept = delete;

	void BeginPlay() override;
	void Tick(float _DeltaTime) override;

	void SetSprite(std::string_view _StrNum, FVector2D _Scale) override
	{
		std::string Num = _StrNum.data();
		std::string Name = "bloodpool" + Num;
		BodyRenderer->ChangeAnimation(Name);
		BodyRenderer->SetComponentScale(_Scale);
	}

protected:

private:

};

