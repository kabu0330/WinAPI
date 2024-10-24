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

void APlayer::Tick()
{
	AddActorLoaction(FVector2D::RIGHT * 0.0001f);
}

