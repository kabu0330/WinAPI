#include "PreCompile.h"
#include "Room.h"

#include <EngineBase/EngineDebug.h>
#include <EngineCore/EngineAPICore.h>
#include <EngineCore/SpriteRenderer.h>

#include "ContentsEnum.h"
#include "PlayGameMode.h"
#include "Door.h"

ARoom::ARoom()
{
	SetActorLocation(Global::WindowSize.Half());
	SetActorScale(Global::WindowScale);

	float ScaleX = Global::WindowSize.X / 960;
	float ScaleY = Global::WindowSize.Y / 540;
	FVector2D RoomScale = { 848.0f * ScaleX * GetActorScale().X / Global::WindowScale.X , 536 * ScaleY * GetActorScale().Y / Global::WindowScale.Y };

	RoomRenderer = CreateDefaultSubObject<USpriteRenderer>();
	RoomRenderer->SetSprite("SampleMap(848,536).png");
	RoomRenderer->SetComponentScale(RoomScale);
	RoomRenderer->SetOrder(ERenderOrder::BACKGROUND);

	BolderLineRenderer = CreateDefaultSubObject<USpriteRenderer>();
	BolderLineRenderer->SetSprite("BolderLine.png");
	BolderLineRenderer->SetComponentScale(GetActorScale());
	BolderLineRenderer->SetComponentLocation({ RoomRenderer->GetComponentLocation().iX(), RoomRenderer->GetComponentLocation().iY() - 1});
	BolderLineRenderer->SetOrder(ERenderOrder::BOLDERLINE);
	
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
	AddDoor(CurLocation, _Dir, _Room);

	// this와 _Room의 문 위치는 서로 정반대가 되어야 한다.
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

	// Room 상대 위치 조정
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
		_Room->SetActorLocation({ GetActorLocation().X, GetDownPos().Y + GetActorScale().Half().Y });
	}
	else if (RoomDir::DOWN == _Room->Directon)
	{
		_Room->SetActorLocation({ GetActorLocation().X, GetUpPos().Y - GetActorScale().Half().Y });
	}

	_Room->AddDoor(_Room->GetActorLocation(), _Dir, _Room);

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

void ARoom::AddDoor(FVector2D _RoomLocation, RoomDir _Dir, ARoom* ConnectedRoom)
{
	FVector2D DoorPos = FVector2D::ZERO;

	switch (_Dir)
	{
	case RoomDir::NONE:
		break;
	case RoomDir::LEFT:
		DoorPos = _RoomLocation + FVector2D(-_RoomLocation.iX() / 2, GetLeftPos().iY());
		break;
	case RoomDir::RIGHT:
		DoorPos = _RoomLocation + FVector2D(_RoomLocation.iX() / 2, GetRightPos().iY());
		break;
	case RoomDir::UP:
		DoorPos = _RoomLocation + FVector2D(GetUpPos().iX(), -_RoomLocation.iY() / 2);
		break;
	case RoomDir::DOWN:
		DoorPos = _RoomLocation + FVector2D(GetDownPos().iX(), _RoomLocation.iY() / 2);
		break;
	case RoomDir::MAX:
		break;
	default:
		break;
	}

	ADoor* Door = GetWorld()->SpawnActor<ADoor>();
	if (nullptr != Door)
	{
		Door->Initialize(DoorPos, _Dir, ConnectedRoom);
		Doors[_Dir] = Door;
	}
}

ARoom* ARoom::GetConnectedRoom(RoomDir _Direction)
{
	std::map<RoomDir, ADoor*>::iterator FindIter = Doors.find(_Direction);
	std::map<RoomDir, ADoor*>::iterator EndIter = Doors.end();

	if (FindIter != EndIter)
	{
		return Doors[_Direction]->GetConnectedRoom();
	}

	return nullptr;
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




