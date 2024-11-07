#include "PreCompile.h"
#include "2DCollision.h"
#include <EngineCore/EngineCoreDebug.h>

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

	if (0 > CollisionGroup)
	{
		MSGASSERT("충돌 그룹은 음수가 될수 없습니다.");
		return;
	}

	Level->PushCollision(this);
}

void U2DCollision::ComponentTick(float _DeltaTime)
{
	Super::ComponentTick(_DeltaTime);


	if (true == IsDebug() || true == GetActor()->IsDebug())
	{

		FTransform ActorTransform = GetActorTransform();
		FVector2D CameraPos = GetActor()->GetWorld()->GetCameraPos();

		ActorTransform.Location -= CameraPos;

		UEngineDebug::CoreDebugRender(ActorTransform, UEngineDebug::EDebugPosType::Rect);
	}
}

