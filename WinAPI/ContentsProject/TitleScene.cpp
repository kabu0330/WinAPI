#include "PreCompile.h"
#include "TitleScene.h"
#include <EngineCore/SpriteRenderer.h>
#include "Global.h"

ATitleScene::ATitleScene()
{
	SetActorLocation(Global::WindowSize.Half());
}

ATitleScene::~ATitleScene()
{
}

void ATitleScene::BeginPlay()
{
	Super::BeginPlay();
	TitleRenderer = CreateDefaultSubObject<USpriteRenderer>();
	TitleRenderer->CreateAnimation("Background", "Title", 0, 6, 0.25f);
	TitleRenderer->SetComponentScale(Global::WindowSize);
	TitleRenderer->ChangeAnimation("Background");

}

void ATitleScene::Tick(float _DeltaTime)
{
	Super::Tick(_DeltaTime);
}

