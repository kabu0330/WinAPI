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

	if (0 > CollisionGroup)
	{
		MSGASSERT("충돌 그룹은 음수가 될수 없습니다.");
		return;
	}

	Level->PushCollision(this);

	if (nullptr != Enter || nullptr != Stay || nullptr != End)
	{
		Level->PushCheckCollision(this);
	}
}

void U2DCollision::ComponentTick(float _DeltaTime)
{
	Super::ComponentTick(_DeltaTime);

	if (true == IsDebug() || true == GetActor()->IsDebug())
	{

		FTransform ActorTransform = GetActorTransform();
		FVector2D CameraPos = GetActor()->GetWorld()->GetCameraPos();

		ActorTransform.Location -= CameraPos;

		switch (CollisionType)
		{
		case ECollisionType::Rect:
			UEngineDebug::CoreDebugRender(ActorTransform, UEngineDebug::EDebugPosType::Rect);
			break;
		case ECollisionType::Circle:
			UEngineDebug::CoreDebugRender(ActorTransform, UEngineDebug::EDebugPosType::Circle);
			break;
		default:
			break;
		}
	}
}

bool U2DCollision::Collision(int _OtherCollisionGroup, std::vector<AActor*>& _Result, FVector2D _NextPos, unsigned int  _Limite)
{
	// 내가 _OtherCollisionGroup 그룹이랑 충돌
	// 모든 충돌체를 한곳에 모아놓는게 Level
	U2DCollision* ThisCollision = this;

	if (false == ThisCollision->IsActive())
	{
		return false;
	}

	std::list<class U2DCollision*>& OtherCollisions = GetActor()->GetWorld()->Collisions[_OtherCollisionGroup];

	std::list<class U2DCollision*>::iterator StartIter = OtherCollisions.begin();
	std::list<class U2DCollision*>::iterator EndIter = OtherCollisions.end();

	for (; StartIter != EndIter; ++StartIter)
	{
		U2DCollision* DestCollision = *StartIter;

		if (ThisCollision == DestCollision)
		{
			continue;
		}

		if (false == DestCollision->IsActive())
		{
			continue;
		}
		
		FTransform ThisTrans = ThisCollision->GetActorTransform();
		FTransform DestTrans = DestCollision->GetActorTransform();

		ThisTrans.Location += _NextPos;

		ECollisionType ThisType = ThisCollision->CollisionType;
		ECollisionType DestType = DestCollision->CollisionType;

		bool Result = FTransform::Collision(ThisType, ThisTrans, DestType, DestTrans);

		// 충돌 true
		if (true == Result)
		{
			_Result.push_back(DestCollision->GetActor());
			--_Limite;

			if (0 == _Limite)
			{
				return 0 != _Result.size();
			}
		}
	}

	return 0 != _Result.size();
}

// 이벤트 방식
void U2DCollision::SetCollisionEnter(std::function<void(AActor*)> _Function)
{
	Enter = _Function;

	ULevel* Level = GetActor()->GetWorld();

	if (nullptr != GetActor()->GetWorld())
	{
		Level->PushCheckCollision(this);
	}
}

void U2DCollision::SetCollisionStay(std::function<void(AActor*)> _Function)
{
	Stay = _Function;

	ULevel* Level = GetActor()->GetWorld();

	if (nullptr != GetActor()->GetWorld())
	{
		Level->PushCheckCollision(this);
	}
}

void U2DCollision::SetCollisionEnd(std::function<void(AActor*)> _Function)
{
	End = _Function;

	ULevel* Level = GetActor()->GetWorld();

	if (nullptr != GetActor()->GetWorld())
	{
		Level->PushCheckCollision(this);
	}
}

void U2DCollision::CollisionEventCheck(class U2DCollision* _Other)
{
	// 최초 충돌
	// 충돌 중
	// 충돌 끝
	U2DCollision* ThisCollision = this;
	U2DCollision* DestCollision = _Other;
	// 
	FTransform ThisTrans = ThisCollision->GetActorTransform();
	FTransform DestTrans = DestCollision->GetActorTransform();

	ECollisionType ThisType = ThisCollision->GetCollisionType();
	ECollisionType DestType = DestCollision->GetCollisionType();

	bool Result = FTransform::Collision(ThisType, ThisTrans, DestType, DestTrans);

	// 충돌 true
	if (true == Result)
	{
		if (false == CollisionCheckSet.contains(DestCollision))
		{
			if (nullptr != Enter)
			{
				Enter(DestCollision->GetActor());
			}

			CollisionCheckSet.insert(DestCollision);
		}
		else
		{
			if (nullptr != Stay)
			{
				Stay(DestCollision->GetActor());
			}
		}
	}
	else
	{
		// 충돌은 안했는데 예전에 충돌한 기록은 가지고 있어.
		if (true == CollisionCheckSet.contains(DestCollision))
		{
			if (nullptr != End)
			{
				End(DestCollision->GetActor());
			}

			CollisionCheckSet.erase(DestCollision);
		}
	}
}