#include "PreCompile.h"
#include "TitleGameMode.h"

#include <EnginePlatform/EngineInput.h>
#include <EngineCore/EngineAPICore.h>
#include <EngineCore/EngineCoreDebug.h>
#include <EngineCore/SpriteRenderer.h>

#include "PlayGameMode.h"
#include "Player.h"
#include "TitleScene.h"
#include "Global.h"
#include "TitleScene.h"

ATitleGameMode::ATitleGameMode()
{

}

ATitleGameMode::~ATitleGameMode()
{
}

void ATitleGameMode::BeginPlay()
{
	Super::BeginPlay();

	GetWorld()->SetCameraPos({ 0, 0 });
	GetWorld()->SetCameraToMainPawn(false);

	SetupScene();
}

void ATitleGameMode::Tick(float _DeltaTime)
{
	Super::Tick(_DeltaTime);

}

void ATitleGameMode::SetupScene()
{
	ATitleScene* MainTitleScene = GetWorld()->SpawnActor<ATitleScene>();
	ATitleScene* SaveFileScene = GetWorld()->SpawnActor<ATitleScene>();
	ATitleScene* SelectScene = GetWorld()->SpawnActor<ATitleScene>();
	ATitleScene* CharacterSelectScene = GetWorld()->SpawnActor<ATitleScene>();

	MainTitleScene->SetName("MainTitleScene");
	SaveFileScene->SetName("SaveFileScene");
	SelectScene->SetName("SelectScene");
	CharacterSelectScene->SetName("CharacterSelectScene");

	MainTitleScene->InterLinkScene(SaveFileScene, TitleSceneDir::DOWN);
	SaveFileScene->InterLinkScene(SelectScene, TitleSceneDir::DOWN);
	SelectScene->InterLinkScene(CharacterSelectScene, TitleSceneDir::RIGHT);
}


