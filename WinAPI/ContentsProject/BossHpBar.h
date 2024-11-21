#pragma once
#include <EngineCore/Actor.h>

// Ό³Έν :
class ABossHpBar : public AActor
{
public:
	// constrcuter destructer
	ABossHpBar();
	~ABossHpBar();

	// delete Function
	ABossHpBar(const ABossHpBar& _Other) = delete;
	ABossHpBar(ABossHpBar&& _Other) noexcept = delete;
	ABossHpBar& operator=(const ABossHpBar& _Other) = delete;
	ABossHpBar& operator=(ABossHpBar&& _Other) noexcept = delete;

	template<typename EnumType>
	void SetOrder(EnumType _Order)
	{
		SetOrder(static_cast<int>(_Order));
	}

	void SetHpBar(class AMonster* _Monster);
	void Update();


protected:
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;

private:
	std::string TextSpriteName = "";
	class USpriteRenderer* BackRenderer = nullptr;
	class USpriteRenderer* FrontRenderer = nullptr;
	class AMonster* Monster = nullptr;
	bool IsGetHp = false;
	int HpMax = 1;
	float HpRatio = 0.0f;
	FVector2D Scale = FVector2D::ZERO;
};

