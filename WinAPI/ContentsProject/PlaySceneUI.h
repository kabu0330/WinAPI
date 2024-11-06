#pragma once
#include <string>
#include <vector>

#include <EngineCore/Actor.h>
#include <EngineCore/SpriteRenderer.h>
#include <EngineBase/EngineMath.h>


// ����  PlayGameMode���� �����ϴ� UI���� �����ϴ� Ŭ����
class APlaySceneUI : public AActor
{
public:
	// constrcuter destructer
	APlaySceneUI();
	~APlaySceneUI();

	// delete Function
	APlaySceneUI(const APlaySceneUI& _Other) = delete;
	APlaySceneUI(APlaySceneUI&& _Other) noexcept = delete;
	APlaySceneUI& operator=(const APlaySceneUI& _Other) = delete;
	APlaySceneUI& operator=(APlaySceneUI&& _Other) noexcept = delete;

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
	std::vector<USpriteRenderer*> Renders;
};

