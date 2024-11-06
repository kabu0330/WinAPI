#include "PreCompile.h"
#include "PickupNumberUI.h"
#include <EngineBase/EngineDebug.h>
#include <EngineCore/SpriteRenderer.h>

APickupNumberUI::APickupNumberUI()
{
	for (int i = 0; i < 10; i++)
	{
		USpriteRenderer* Sprite = CreateDefaultSubObject<USpriteRenderer>();
		Sprite->SetCameraEffect(false); // 카메라 이동에 영향을 받지 않음. 윈도우 위치에 고정
		Renders.push_back(Sprite);
	}
	int a = 0;
}

void APickupNumberUI::SetTextSpriteName(const std::string _Text)
{
	TextSpriteName = _Text;
	for (int i = 0; i < Renders.size(); i++)
	{
		Renders[i]->SetSprite(TextSpriteName);
	}
}

void APickupNumberUI::SetOrder(int _Order)
{
	for (int i = 0; i < Renders.size(); i++)
	{
		Renders[i]->SetOrder(_Order);
	}
}

void APickupNumberUI::SetValue(int _Value)
{
	std::string Number = std::to_string(_Value);
	if (Renders.size() <= Number.size())
	{
		MSGASSERT("UI의 RenderSize를 초과했습니다.");
		return;
	}

	FVector2D Pos = FVector2D::ZERO;
	for (int i = 0; i < Number.size(); i++)
	{
		char Value = Number[i] - '0';
		if (true == IsSingleDigit(_Value))
		{
			return;
		}
		Renders[i]->SetSprite(TextSpriteName, Value);
		Renders[i]->SetComponentScale(UIScale);
		Renders[i]->SetComponentLocation(Pos);
		Pos.X += UIScale.X - 5;
		Renders[i]->SetActive(true);
	}

	for (int i = Number.size(); i < Renders.size() ; i++)
	{
		Renders[i]->SetActive(false);
	}
	
}

bool APickupNumberUI::IsSingleDigit(int _Value)
{
	if (10 > _Value)
	{
		FVector2D Pos = FVector2D::ZERO;

		Renders[0]->SetSprite(TextSpriteName, 0);
		Renders[0]->SetComponentScale(UIScale);
		Renders[0]->SetComponentLocation(Pos);
		Renders[0]->SetActive(true);

		std::string Number = std::to_string(_Value);
		char Value = Number[0] - '0';
		Renders[1]->SetSprite(TextSpriteName, Value);
		Renders[1]->SetComponentScale(UIScale);
		Pos.X += UIScale.X - 7;
		Renders[1]->SetComponentLocation(Pos);
		Renders[1]->SetActive(true);
		return true;
	}
	
	return false;
}

void APickupNumberUI::BeginPlay()
{
	Super::BeginPlay();
}

void APickupNumberUI::Tick(float _DeltaTime)
{
	Super::Tick(_DeltaTime);
}

APickupNumberUI::~APickupNumberUI()
{
}