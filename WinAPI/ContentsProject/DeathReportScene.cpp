#include "PreCompile.h"
#include "DeathReportScene.h"

#include <EngineCore/EngineAPICore.h>
#include <EngineCore/SpriteRenderer.h>
#include "ContentsEnum.h"
#include "Global.h"
#include "Fade.h"

ADeathReportScene* ADeathReportScene::DeathReport = nullptr;

ADeathReportScene::ADeathReportScene()
{
	Renderer = CreateDefaultSubObject<USpriteRenderer>();
	Renderer->SetOrder(ERenderOrder::Menu);
	Renderer->SetSprite("DeathReport.bmp");
	Renderer->SetSpriteScale(2.0f);
	Renderer->SetComponentLocation(Global::WindowHalfScale);
	Renderer->SetActive(false);
	Renderer->SetCameraEffect(false);

	ExitRenderer = CreateDefaultSubObject<USpriteRenderer>();
	ExitRenderer->SetOrder(ERenderOrder::Menu);
	ExitRenderer->SetSprite("Exit.bmp");
	ExitRenderer->SetSpriteScale(2.0f);
	ExitRenderer->SetComponentLocation({ 95, 485 + 60});
	ExitRenderer->SetActive(false);
	ExitRenderer->SetCameraEffect(false);

	RestartRenderer = CreateDefaultSubObject<USpriteRenderer>();
	RestartRenderer->SetOrder(ERenderOrder::Menu);
	RestartRenderer->SetSprite("Restart.bmp");
	RestartRenderer->SetSpriteScale(2.0f);
	RestartRenderer->SetComponentLocation({ 880, 480 + 60 });
	RestartRenderer->SetActive(false);
	RestartRenderer->SetCameraEffect(false);
}

ADeathReportScene::~ADeathReportScene()
{
}

void ADeathReportScene::BeginPlay()
{
	Super::BeginPlay();
	
}

void ADeathReportScene::Tick(float _DeltaTime)
{
	Super::Tick(_DeltaTime);
}

void ADeathReportScene::ShowDeathReport()
{
	Renderer->SetActive(true);
	ExitRenderer->SetActive(true);
	RestartRenderer->SetActive(true);

	AFade::MainFade->GetRenderer()->SetAlphaFloat(0.8f);
	AFade::MainFade->GetRenderer()->SetActive(true);
}

void ADeathReportScene::CloseDeathReport()
{
	Renderer->SetActive(false);

	ExitRenderer->SetActive(false);
	RestartRenderer->SetActive(false);

	AFade::MainFade->GetRenderer()->SetActive(false);
}

