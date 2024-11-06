#pragma once
#include <string>
#include <vector>

#include <EngineCore/Actor.h>

// Ό³Έν :
class APickupItemUI : public AActor
{
public:
	// constrcuter destructer
	APickupItemUI();
	~APickupItemUI();

	// delete Function
	APickupItemUI(const APickupItemUI& _Other) = delete;
	APickupItemUI(APickupItemUI&& _Other) noexcept = delete;
	APickupItemUI& operator=(const APickupItemUI& _Other) = delete;
	APickupItemUI& operator=(APickupItemUI&& _Other) noexcept = delete;

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



protected:
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;

private:
	std::string TextSpriteName;
	FVector2D UIScale;
	class USpriteRenderer* Render;
};

