#pragma once
#include <EngineCore/Actor.h>
#include <EngineCore/SpriteRenderer.h>

// ���� : ���Ͱ� �װ� ����� ���ع�
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

