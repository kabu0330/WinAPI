#include "PreCompile.h"
#include "Player.h"

APlayer::APlayer()
{
	SetActorLoaction({ 300, 300 });
	SetActorScale({ 100, 100 });
}

APlayer::~APlayer()
{
}

void APlayer::BeginPlay()
{
}

void APlayer::Tick(float _DeltaTime)
{
	AddActorLoaction(FVector2D::RIGHT * _DeltaTime * Speed);
}

