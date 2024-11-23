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
	// �̹� �ı��Ǿ����� ����
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

	// Host ���� ����
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

	// �浹ü�� ������ ��� Actor�� �Ѱ���� �Ѵ�.
	ECollisionGroup CollisionType = static_cast<ECollisionGroup>(CollisionMonster->GetBodyCollision()->GetGroup());
	CollisionActor = TearCollision->CollisionOnce(CollisionType);
	if (nullptr == CollisionActor)
	{
		return;
	}

	//Explosion();

	// ���� ���� ó��
	// �浹ü�� ������ ��� Actor�� �Ѱ���� �Ѵ�.
	CollisionMonster->ApplyDamaged(CollisionActor, ActorAtt, KnockbackDir); // �� �Լ��� ü�µ� ��� �ǰ� �ִϸ��̼ǵ� ����Ѵ�.

	UEngineDebug::OutPutString(CollisionMonster->GetName() + "���� " + std::to_string(ActorAtt) + " �� �������� �־����ϴ�. // ���� ü�� : " + std::to_string(CollisionMonster->GetHp()));
}

