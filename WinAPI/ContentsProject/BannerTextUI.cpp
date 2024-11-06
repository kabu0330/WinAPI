#include "PreCompile.h"
#include "BannerTextUI.h"
#include <EngineCore/SpriteRenderer.h>
#include <EngineBase/EngineString.h>

ABannerTextUI::ABannerTextUI()
{
	for (int i = 0; i < 26; i++)
	{
		USpriteRenderer* Sprite = CreateDefaultSubObject<USpriteRenderer>();
		Sprite->SetCameraEffect(false); // 카메라 이동에 영향을 받지 않음. 윈도우 위치에 고정
		Renders.push_back(Sprite);
	}
	int a = 0;
}

void ABannerTextUI::SetTextSpriteName(const std::string _Text)
{
	TextSpriteName = _Text;
	for (int i = 0; i < Renders.size(); i++)
	{
		Renders[i]->SetSprite(TextSpriteName);
	}
}

void ABannerTextUI::SetOrder(int _Order)
{
	for (int i = 0; i < Renders.size(); i++)
	{
		Renders[i]->SetOrder(_Order);
	}
}

void ABannerTextUI::SetTextOut(std::string_view _Text)
{
	
	std::string Text = UEngineString::ToUpper(_Text);
	if (Renders.size() <= Text.size())
	{
		MSGASSERT("Banner UI의 RenderSize를 초과했습니다.");
		return;
	}
	

	FVector2D Pos = FVector2D::ZERO;
	for (int i = 0; i < Text.size(); i++)
	{
		char Value = Text[i] - 'A';
		Renders[i]->SetSprite(TextSpriteName, Value);
		Renders[i]->SetComponentScale(UIScale);
		Renders[i]->SetComponentLocation(Pos);
		Pos.X += UIScale.X;
		Renders[i]->SetActive(true);
	}

	for (int i = Text.size(); i < Renders.size(); i++)
	{
		Renders[i]->SetActive(false);
	}

}

void ABannerTextUI::BeginPlay()
{
	Super::BeginPlay();
}

void ABannerTextUI::Tick(float _DeltaTime)
{
	Super::Tick(_DeltaTime);
}

ABannerTextUI::~ABannerTextUI()
{
}