#pragma once
#include <vector>
#include <EngineCore/Actor.h>

// 설명 : Player의 HP와 연동하여 현재 체력을 표기해주는 UI
class AHeartUI : public AActor
{
public:
	// constrcuter destructer
	AHeartUI();
	~AHeartUI();

	// delete Function
	AHeartUI(const AHeartUI& _Other) = delete;
	AHeartUI(AHeartUI&& _Other) noexcept = delete;
	AHeartUI& operator=(const AHeartUI& _Other) = delete;
	AHeartUI& operator=(AHeartUI&& _Other) noexcept = delete;

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

	void SetPlayerHp(int _PlayerHp);



protected:
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;

private:
	std::string TextSpriteName;
	FVector2D UIScale;
	std::vector<class USpriteRenderer*> Renders;
};

