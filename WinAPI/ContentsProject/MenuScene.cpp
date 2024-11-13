#include "PreCompile.h"
#include "MenuScene.h"

#include <EngineCore/EngineAPICore.h>
#include <EngineCore/SpriteRenderer.h>
#include "ContentsEnum.h"
#include "Global.h"
#include "Fade.h"

AMenuScene* AMenuScene::Menu = nullptr;

AMenuScene::AMenuScene()
{
	Renderer = CreateDefaultSubObject<USpriteRenderer>();
	Renderer->SetOrder(ERenderOrder::Menu);
	Renderer->SetSprite("Menu_Paused.bmp");
	Renderer->SetSpriteScale(2.0f);
	Renderer->SetComponentLocation({590, 300});
	Renderer->SetActive(false);
	Renderer->SetCameraEffect(false);

	PaperRenderer = CreateDefaultSubObject<USpriteRenderer>();
	PaperRenderer->SetOrder(ERenderOrder::Menu);
	PaperRenderer->SetSprite("Menu_Paper.bmp");
	PaperRenderer->SetSpriteScale(2.0f);
	PaperRenderer->SetComponentLocation({ 220, 150 });
	PaperRenderer->SetActive(false);
	PaperRenderer->SetCameraEffect(false);

	MyStuffRenderer = CreateDefaultSubObject<USpriteRenderer>();
	MyStuffRenderer->SetOrder(ERenderOrder::Menu);
	MyStuffRenderer->SetSprite("Menu_Mystuff.bmp");
	MyStuffRenderer->SetSpriteScale(2.0f);
	MyStuffRenderer->SetComponentLocation({ 250, 330 });
	MyStuffRenderer->SetActive(false);
	MyStuffRenderer->SetCameraEffect(false);
}

AMenuScene::~AMenuScene()
{
}

void AMenuScene::BeginPlay()
{
	Super::BeginPlay();
}

void AMenuScene::Tick(float _DeltaTime)
{
	Super::Tick(_DeltaTime);
}

void AMenuScene::SlideInUI()
{
	AFade::MainFade->GetRenderer()->SetAlphaFloat(0.8f);

}

void AMenuScene::ShowMenu()
{
	Renderer->SetActive(true);
	PaperRenderer->SetActive(true);
	MyStuffRenderer->SetActive(true);
	AFade::MainFade->GetRenderer()->SetAlphaFloat(0.8f);
	AFade::MainFade->GetRenderer()->SetActive(true);
}

void AMenuScene::CloseMenu()
{
	Renderer->SetActive(false);
	PaperRenderer->SetActive(false);
	MyStuffRenderer->SetActive(false);
	AFade::MainFade->GetRenderer()->SetActive(false);
}

