#include "PreCompile.h"
#include "Fade.h"
#include <EngineCore/SpriteRenderer.h>
#include <EngineCore/EngineAPICore.h>
#include "ContentsEnum.h"

AFade* AFade::MainFade = nullptr;

AFade::AFade()
{
	BackSpriteRenderer = CreateDefaultSubObject<USpriteRenderer>();
	BackSpriteRenderer->SetOrder(ERenderOrder::FADE);
	BackSpriteRenderer->SetSprite("Fade.bmp");
	FVector2D MapScale = BackSpriteRenderer->SetSpriteScale(1.0f);
	BackSpriteRenderer->SetComponentLocation(MapScale.Half());
	BackSpriteRenderer->SetActive(false);
	BackSpriteRenderer->SetCameraEffect(false);
}

AFade::~AFade()
{
}

void AFade::FadeChange()
{
	float DeltaTime = UEngineAPICore::GetCore()->GetDeltaTime();
	FadeValue += DeltaTime * 0.5f * FadeDir;
	BackSpriteRenderer->SetAlphaFloat(FadeValue);
}

void AFade::FadeIn()
{
	FadeValue = 0.0f;
	FadeDir = 1.0f;
	TimeEventer.PushEvent(5.0f, std::bind_front(&AFade::FadeChange, this), true, false);
}

void AFade::FadeOut()
{
	FadeValue = 1.0f;
	FadeDir = -1.0f;
	TimeEventer.PushEvent(5.0f, std::bind_front(&AFade::FadeChange, this), true, false);
}

void AFade::LevelChangeStart()
{
	MainFade = this;
}



