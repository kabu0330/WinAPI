#include "PreCompile.h"
#include "AttackFly.h"

#include <EngineBase/EngineMath.h>
#include <EngineBase/EngineRandom.h>
#include <EngineCore/SpriteRenderer.h>
#include <EngineCore/2DCollision.h>
#include "PlayGameMode.h"

#include "Global.h"
#include "ContentsEnum.h"

AttackFly::AttackFly()
{

}

AttackFly::~AttackFly()
{
}

void AttackFly::BeginPlay()
{
}

void AttackFly::Tick(float _DeltaTime)
{
}

void AttackFly::Attack(float _DeltaTime)
{
}

void AttackFly::ChaseMove(float _DeltaTime)
{
}

void AttackFly::Move(float _DeltaTime)
{
}

FVector2D AttackFly::GetRandomDir()
{
	return FVector2D();
}

void AttackFly::ClampPositionToRoom()
{
}

bool AttackFly::IsPlayerNearby()
{
	return false;
}

FVector2D AttackFly::GetDirectionToPlayer()
{
	return FVector2D();
}

void AttackFly::ChasePlayer(float _DeltaTime)
{
}

void AttackFly::BodyCollisionCheck(float _DeltaTime)
{
}

void AttackFly::DeathCheck(float _DeltaTime)
{
}

void AttackFly::Death(float _DeltaTime)
{
}
