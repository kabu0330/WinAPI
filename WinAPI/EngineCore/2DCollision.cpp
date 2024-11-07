#include "PreCompile.h"
#include "2DCollision.h"

U2DCollision::U2DCollision()
{
}

U2DCollision::~U2DCollision()
{
}

void U2DCollision::BeginPlay()
{
	Super::BeginPlay();


	AActor* Actor = GetActor();
	ULevel* Level = Actor->GetWorld();

	Level->PushCollision(this);
}

void U2DCollision::ComponentTick(float _DeltaTime)
{
	Super::ComponentTick(_DeltaTime);
}

