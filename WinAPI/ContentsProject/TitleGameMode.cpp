#include "PreCompile.h"
#include "TitleGameMode.h"

#include <EnginePlatform/EngineInput.h>
#include <EngineCore/EngineAPICore.h>

#include "TitleScene.h"
#include "Global.h"
#include "TitleScene.h"
#include "Fade.h"

ATitleGameMode::ATitleGameMode()
{
}

ATitleGameMode::~ATitleGameMode()
{
}

void ATitleGameMode::BeginPlay()
{
	Super::BeginPlay();
	ATitleScene* Scene = GetWorld()->SpawnActor<ATitleScene>();
	FadeBackground = GetWorld()->SpawnActor<AFade>();

}

void ATitleGameMode::Tick(float _DeltaTime)
{
	Super::Tick(_DeltaTime);

	if (true == UEngineInput::GetInst().IsDown(VK_SPACE))
	{
		IsChangeGameMode = true;
	}
	EnterPlayGameMode();
}

void ATitleGameMode::EnterPlayGameMode()
{
	if (false == IsChangeGameMode)
	{
		return;
	}
	
	FadeBackground->SetActive(true);

	//float DeltaTime = UEngineAPICore::GetCore()->GetDeltaTime();
	TimeEventer.PushEvent(1.0f, std::bind(&ATitleGameMode::FadeOut, this));

}
 
void ATitleGameMode::FadeOut()
{
	FadeBackground->FadeOut();
	UEngineAPICore::GetCore()->OpenLevel("Play");
}

