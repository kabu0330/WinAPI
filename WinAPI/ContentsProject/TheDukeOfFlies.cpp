#include "PreCompile.h"
#include "TheDukeOfFlies.h"

ATheDukeOfFlies::ATheDukeOfFlies()
{
}

ATheDukeOfFlies::~ATheDukeOfFlies()
{
}

void ATheDukeOfFlies::BeginPlay()
{
	Super::BeginPlay();
	AMonster::BeginPlay();
}

void ATheDukeOfFlies::Tick(float _DeltaTime)
{
	Super::Tick(_DeltaTime);
	AMonster::Tick(_DeltaTime);
}

