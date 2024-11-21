#include "PreCompile.h"
#include "PickupItemUI.h"
#include <EngineCore/SpriteRenderer.h>

APickupItemUI::APickupItemUI()
{
	Render = CreateDefaultSubObject<USpriteRenderer>();
	Render->SetCameraEffect(false); // ī�޶� �̵��� ������ ���� ����. ������ ��ġ�� ����
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