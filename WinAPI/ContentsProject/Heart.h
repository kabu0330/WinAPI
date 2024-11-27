#pragma once
#include "Item.h"

// Ό³Έν :
class AHeart : public AItem
{
public:
	// constrcuter destructer
	AHeart();
	~AHeart();

	// delete Function
	AHeart(const AHeart& _Other) = delete;
	AHeart(AHeart&& _Other) noexcept = delete;
	AHeart& operator=(const AHeart& _Other) = delete;
	AHeart& operator=(AHeart&& _Other) noexcept = delete;

	void BeginPlay() override;
	void Tick(float _DeltaTime);

	bool EatFunction(APlayer* _Player) override
	{
		int CurHp = _Player->GetHp();
		int MaxHp = _Player->GetHptMax();
		if (CurHp == MaxHp)
		{
			return false;
		}

		Sound = UEngineSound::Play("large_chew.wav");

		IsPushback = false;
		_Player->AddHp(ItemCount);
		

		IsUseEnd = true;

		return true;
	}
	void SetHalfHeart() override
	{
		DropRenderer->ChangeAnimation("HalfHeart");
		ItemCount = 1;
	}

protected:

private:
};

