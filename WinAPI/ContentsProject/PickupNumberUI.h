#pragma once
#include <string>
#include <vector>

#include <EngineCore/Actor.h>

// 설명  Pickup Item 습득 개수 표기 UI
class APickupNumberUI : public AActor
{
public:
	// constrcuter destructer
	APickupNumberUI();
	~APickupNumberUI();

	// delete Function
	APickupNumberUI(const APickupNumberUI& _Other) = delete;
	APickupNumberUI(APickupNumberUI&& _Other) noexcept = delete;
	APickupNumberUI& operator=(const APickupNumberUI& _Other) = delete;
	APickupNumberUI& operator=(APickupNumberUI&& _Other) noexcept = delete;

	void SetTextSpriteName(const std::string _Text);

	void SetTextScale(FVector2D _TextScale)
	{
		UIScale = _TextScale;
	}

	template<typename EnumType>
	void SetOrder(EnumType _Order)
	{
		SetOrder(static_cast<int>(_Order));
	}

	void SetOrder(int _Order);

	void SetValue(int _Value);

	bool IsSingleDigit(int _Value);


protected:
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;

private:
	std::string TextSpriteName;
	FVector2D UIScale;
	std::vector<class USpriteRenderer*> Renders;
};

