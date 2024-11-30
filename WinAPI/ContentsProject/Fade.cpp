#include "PreCompile.h"
#include "Fade.h"
#include <EngineCore/SpriteRenderer.h>
#include <EngineCore/EngineAPICore.h>
#include "ContentsEnum.h"

AFade* AFade::MainFade = nullptr;

AFade::AFade()
{
	Renderer = CreateDefaultSubObject<USpriteRenderer>();
	Renderer->SetOrder(ERenderOrder::Fade);
	Renderer->SetSprite("Fade.bmp");
	FVector2D MapScale = Renderer->SetSpriteScale(1.0f);
	Renderer->SetComponentLocation(MapScale.Half());
	Renderer->SetActive(false);
	Renderer->SetCameraEffect(false);
	Renderer->SetActive(false);
}

AFade::~AFade()
{
}

void AFade::FadeChange()
{
	float DeltaTime = UEngineAPICore::GetCore()->GetDeltaTime();
	FadeValue += DeltaTime * 0.5f * FadeDir;
	Renderer->SetAlphaFloat(FadeValue);
}

void AFade::FadeIn()
{
	FadeValue = 0.0f;
	FadeDir = 1.0f;
	TimeEventer.PushEvent(2.0f, std::bind_front(&AFade::FadeChange, this), true, false);
}

void AFade::FadeOut()
{
	FadeValue = 1.0f;
	FadeDir = -1.0f;
	TimeEventer.PushEvent(2.0f, std::bind_front(&AFade::FadeChange, this), true, false);
}

void AFade::BeginPlay()
{
	Super::BeginPlay();
}

void AFade::Tick(float _DeltaTime)
{
	Super::Tick(_DeltaTime);
}

void AFade::SceneDimming(float _Alpha)
{
	Renderer->SetAlphaFloat(_Alpha);
}

void AFade::LevelChangeStart()
{
	MainFade = this;
}



