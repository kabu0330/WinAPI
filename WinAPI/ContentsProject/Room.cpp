#include "PreCompile.h"
#include "Room.h"
#include <EngineBase/EngineDebug.h>
#include <EngineCore/EngineAPICore.h>
#include <EngineCore/SpriteRenderer.h>
#include "ContentsEnum.h"

ARoom::ARoom()
{
	SetActorLocation(Global::WindowSize.Half());
	
 //	SpriteRenderers.push_back(SpriteRenderer);

	//for (int i = 0; i < SpriteRenderers.size(); i++)
	//{
	//	SpriteRenderers[i] = CreateDefaultSubObject<USpriteRenderer>();
	//	SpriteRenderers[i]->SetSprite("SampleMap(848,536).png");
	//	SpriteRenderers[i]->SetComponentScale(Global::WindowSize * 0.5);
	//	SpriteRenderers[i]->SetOrder(ERenderOrder::BACKGROUND);

	//}

	SpriteRenderer = CreateDefaultSubObject<USpriteRenderer>();
	SpriteRenderer->SetSprite("SampleMap(848,536).png");
	SpriteRenderer->SetComponentScale(Global::WindowSize * 0.5);
	SpriteRenderer->SetOrder(ERenderOrder::BACKGROUND);
	 
	//SpriteRenderer->CreateAnimation("BaseRoom", "SampleMap(848,536).png",0, 0);
	//SpriteRenderer->ChangeAnimation("BaseRoom");

	//FVector2D WindowSize =  UEngineAPICore::GetCore()->GetMainWindow().GetWindowSize();
	//SetActorScale(WindowSize.Half());
	//SetActorLocation(WindowSize.Half());

}

ARoom::~ARoom()
{
	// map에서 insert한 자료들을 모두 delete하기 때문에 따로 소멸자를 호출해줄 필요가 없다.
}

void ARoom::BeginPlay()
{
	Super::BeginPlay();

}

void ARoom::Tick(float _DeltaTime)
{
	Super::Tick(_DeltaTime);
}

bool ARoom::IsLinking(ARoom* _Room)
{
	std::map<RoomDir, ARoom*>::iterator StartIter = Rooms.begin();
	std::map<RoomDir, ARoom*>::iterator EndIter = Rooms.end();

	for (; StartIter != EndIter; ++StartIter)
	{
		if (_Room == StartIter->second)
		{
			return true;
		}
	}
	return false;
}

bool ARoom::InterLinkRoom(ARoom* _Room, RoomDir _Dir)
{
	this->LinkRoom(_Room, _Dir);
	FVector2D CurLocation = this->GetActorLocation();

	// _Room과 this의 문 위치는 서로 정반대가 되어야 한다.
	if (RoomDir::LEFT == _Dir)
	{
		_Dir = RoomDir::RIGHT;
		_Room->Directon = _Dir;
	}
	else if (RoomDir::RIGHT == _Dir)
	{
		_Dir = RoomDir::LEFT;
		_Room->Directon = _Dir;
	}
	else if (RoomDir::UP == _Dir)
	{
		_Dir = RoomDir::DOWN;
		_Room->Directon = _Dir;
	}
	else if (RoomDir::DOWN == _Dir)
	{
		_Dir = RoomDir::UP;
		_Room->Directon = _Dir;
	}

	_Room->LinkRoom(this, _Dir);


	// 위치 조정
	// 문제점 1. 방을 연결한 횟수만큼 중복해서 그려지는 문제?
	if (RoomDir::LEFT == _Room->Directon)
	{
		_Room->SetActorLocation({ GetRightPos().X + GetActorScale().Half().X, GetActorLocation().Y });
	}
	else if (RoomDir::RIGHT == _Room->Directon)
	{
		_Room->SetActorLocation({ GetLeftPos().X - GetActorScale().Half().X, GetActorLocation().Y });
	}
	else if (RoomDir::UP == _Room->Directon)
	{
		//_Room->SetActorLocation({ GetActorLocation().X, GetDownPos().Y + GetActorScale().Half().Y });
		_Room->SetActorLocation({ 0 , 0});
	}
	else if (RoomDir::DOWN == _Room->Directon)
	{
		_Room->SetActorLocation({ GetActorLocation().X, GetUpPos().Y - GetActorScale().Half().Y });
	}

	return true;
}

ARoom* ARoom::LinkRoom(ARoom* _Room, RoomDir _Dir)
{
	if (this == _Room)
	{
		MSGASSERT("자기 자신을 연결할 수 없습니다.");
		return nullptr;
	}

	if (true == IsLinking(_Room))
	{
		MSGASSERT("이미 연결된 방입니다.");
		return nullptr;
	}

	switch (_Dir)
	{
	case RoomDir::NONE:
		break;
	case RoomDir::LEFT:
	{
		//Direction = FVector2D::LEFT;
		Rooms.insert({ RoomDir::LEFT, _Room });
		return Rooms.find(RoomDir::LEFT)->second;
		break;
	}
	case RoomDir::RIGHT:
	{
		//Direction = FVector2D::RIGHT;
		Rooms.insert({ RoomDir::RIGHT, _Room });
		return Rooms.find(RoomDir::RIGHT)->second;
		break;
	}
	case RoomDir::UP:
	{
		//Direction = FVector2D::UP;
		Rooms.insert({ RoomDir::UP, _Room });
		return Rooms.find(RoomDir::UP)->second;
		break;
	}
	case RoomDir::DOWN:
	{
		//Direction = FVector2D::DOWN;
		Rooms.insert({ RoomDir::DOWN, _Room });
		return Rooms.find(RoomDir::DOWN)->second;
		break;
	}
	case RoomDir::MAX:
		break;
	default:
		break;
	}

    return nullptr;
}







