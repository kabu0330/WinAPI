#pragma once
#include <EngineCore/Actor.h>
#include <EngineCore/SpriteRenderer.h>

// 설명 : 몬스터가 죽고 출력할 잔해물
class DeathDebris : public AActor
{
public:
	// constrcuter destructer
	DeathDebris();
	~DeathDebris();

	// delete Function
	DeathDebris(const DeathDebris& _Other) = delete;
	DeathDebris(DeathDebris&& _Other) noexcept = delete;
	DeathDebris& operator=(const DeathDebris& _Other) = delete;
	DeathDebris& operator=(DeathDebris&& _Other) noexcept = delete;

	void BeginPlay() override;
	void Tick(float _DeltaTime) override;

	void SetDeathEffectScale(FVector2D _Scale)
	{
		BloodEffectScale = _Scale;
	}
	void SetDeathEffectLocation(FVector2D _Location)
	{
		BloodEffectLocation = _Location;
	}
	
	bool IsCurAnimationEnd()
	{
		return BloodEffect->IsCurAnimationEnd();
	}

	USpriteRenderer* GetBloodEffect()
	{
		return BloodEffect;
	}

protected:

private:
	USpriteRenderer* BloodEffect = nullptr;
	FVector2D BloodEffectScale = { 0, -40 };
	FVector2D BloodEffectLocation = FVector2D(256, 256);
};

