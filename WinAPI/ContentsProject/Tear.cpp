#include "PreCompile.h"
#include "Tear.h"

ATear::ATear()
{
	SetActorScale({ 50, 50 });
}

ATear::~ATear()
{
}

void ATear::Tick(float _DeltaTime)
{
	AddActorLocation(FVector2D::RIGHT * _DeltaTime * 300.0f);
}

