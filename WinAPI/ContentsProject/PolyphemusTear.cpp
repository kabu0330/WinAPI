#include "PreCompile.h"
#include "PolyphemusTear.h"

#include <EngineCore/2DCollision.h>
#include <EngineCore/SpriteRenderer.h>
#include "ContentsEnum.h"
#include <EngineCore/Actor.h>
#include "Player.h"
#include "Monster.h"
#include "PlayGameMode.h"
#include "RoomObject.h"
#include "Item.h"

APolyphemusTear::APolyphemusTear()
{
}

APolyphemusTear::~APolyphemusTear()
{
}

void APolyphemusTear::BeginPlay()
{
	Super::BeginPlay();
	ATear::BeginPlay();
}

void APolyphemusTear::Tick(float _DeltaTime)
{
	Super::Tick(_DeltaTime);
	ATear::Tick(_DeltaTime);
}

void APolyphemusTear::HandleMonsterCollision(AActor* _Other)
{
	// 이미 파괴되었으면 리턴
	if (nullptr == TearCollision)
	{
		return;
	}
	if (true == TearCollision->IsDestroy())
	{
		return;
	}

	AMonster* CollisionMonster = dynamic_cast<AMonster*>(_Other);
	if (nullptr == CollisionMonster)
	{
		return;
	}
	if (nullptr == CollisionMonster->GetBodyCollision())
	{
		return;
	}

	// Host 전용 로직
	if (nullptr != CollisionMonster->GetHeadCollision())
	{
		if (true == CollisionMonster->GetHeadCollision()->IsActive())
		{
			ECollisionGroup CollisionHeadType = static_cast<ECollisionGroup>(CollisionMonster->GetHeadCollision()->GetGroup());

			if (ECollisionGroup::Monster_Barrier == CollisionHeadType)
			{
				//Explosion();
				return;
			}
		}
	}

	// 충돌체의 정보는 모두 Actor로 넘겨줘야 한다.
	ECollisionGroup CollisionType = static_cast<ECollisionGroup>(CollisionMonster->GetBodyCollision()->GetGroup());
	CollisionActor = TearCollision->CollisionOnce(CollisionType);
	if (nullptr == CollisionActor)
	{
		return;
	}

	//Explosion();

	// 피해 로직 처리
	// 충돌체의 정보는 모두 Actor로 넘겨줘야 한다.
	CollisionMonster->ApplyDamaged(CollisionActor, ActorAtt, KnockbackDir); // 이 함수로 체력도 깎고 피격 애니메이션도 출력한다.

	UEngineDebug::OutPutString(CollisionMonster->GetName() + "에게 " + std::to_string(ActorAtt) + " 의 데미지를 주었습니다. // 현재 체력 : " + std::to_string(CollisionMonster->GetHp()));
}

