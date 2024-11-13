#include "PreCompile.h"
#include "LoadingScene.h"

#include <EngineBase/EngineRandom.h>
#include <EngineCore/EngineAPICore.h>
#include <EngineCore/SpriteRenderer.h>
#include "Global.h"
#include "ContentsEnum.h"

ALoadingScene::ALoadingScene()
{
	Renderer = CreateDefaultSubObject<USpriteRenderer>();
	Renderer->SetOrder(ERenderOrder::Loading);
	Renderer->SetSprite("Fade.bmp");
	Renderer->SetComponentScale({ 157, 193 });
	Renderer->SetComponentLocation(Global::WindowHalfScale);
	Renderer->SetActive(false);
	Renderer->SetCameraEffect(false);
}

void ALoadingScene::RandomImage()
{
	UEngineRandom Random;
	Random.SetSeed(time(NULL));
	
	int Result = Random.RandomInt(0, 6);
	std::string NumberString = std::to_string(Result);
	Renderer->SetSprite("LoadingImage" + NumberString + ".png");
}

ALoadingScene::~ALoadingScene()
{
}

void ALoadingScene::BeginPlay()
{
	Super::BeginPlay();
	RandomImage();
}

void ALoadingScene::Tick(float _DeltaTime)
{
	Super::Tick(_DeltaTime);
}

void ALoadingScene::FadeOut()
{
	FadeValue = 1.0f;
	FadeDir = -1.0f;
	TimeEventer.PushEvent(2.0f, std::bind_front(&ALoadingScene::FadeChange, this), true, false);
}

void ALoadingScene::FadeChange()
{
	float DeltaTime = UEngineAPICore::GetCore()->GetDeltaTime();
	FadeValue += DeltaTime * 0.5f * FadeDir;
	Renderer->SetAlphaFloat(FadeValue);
}

