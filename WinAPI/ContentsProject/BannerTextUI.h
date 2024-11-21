#pragma once
#include <EngineCore/Actor.h>

// Ό³Έν :
class ABannerTextUI : public AActor
{
public:
	// constrcuter destructer
	ABannerTextUI();
	~ABannerTextUI();

	// delete Function
	ABannerTextUI(const ABannerTextUI& _Other) = delete;
	ABannerTextUI(ABannerTextUI&& _Other) noexcept = delete;
	ABannerTextUI& operator=(const ABannerTextUI& _Other) = delete;
	ABannerTextUI& operator=(ABannerTextUI&& _Other) noexcept = delete;

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

	void SetTextOut(std::string_view _Text);

protected:
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;

private:
	std::string TextSpriteName = "";
	FVector2D UIScale = FVector2D::ZERO;
	std::vector<class USpriteRenderer*> Renders;
};

