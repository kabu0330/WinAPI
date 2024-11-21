#include "PreCompile.h"
#include "PickupItemUI.h"
#include <EngineCore/SpriteRenderer.h>

APickupItemUI::APickupItemUI()
{
	Render = CreateDefaultSubObject<USpriteRenderer>();
	Render->SetCameraEffect(false); // 카메라 이동에 영향을 받지 않음. 윈도우 위치에 고정
}

void APickupItemUI::SetTextSpriteName(const std::string _Text)
{
	TextSpriteName = _Text;
	Render->SetSprite(TextSpriteName);

}

void APickupItemUI::SetOrder(int _Order)
{
	Render->SetOrder(_Order);
}

void APickupItemUI::SetValue(int _Value)
{
	FVector2D Pos = FVector2D::ZERO;
	Render->SetSprite(TextSpriteName, _Value);
	Render->SetComponentScale(UIScale);
	Render->SetComponentLocation(Pos);
	Render->SetActive(true);
	
	//for (int i = Number.size(); i < Renders.size(); i++)
	//{
	//	Renders[i]->SetActive(false);
	//}

}

void APickupItemUI::BeginPlay()
{
	Super::BeginPlay();
}

void APickupItemUI::Tick(float _DeltaTime)
{
	Super::Tick(_DeltaTime);
}

APickupItemUI::~APickupItemUI()
{
}