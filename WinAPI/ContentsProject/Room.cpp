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

	// BaseRoom���� �����ǵ��� �ڵ带 �����Ͽ��� ��.
	//ControlsRenderer = CreateDefaultSubObject<USpriteRenderer>();
	//ControlsRenderer->SetSprite("controls.png");
	//ControlsRenderer->SetComponentLocation(GetActorLocation());
	//ControlsRenderer->SetComponentScale({ 325, 85 });
	//ControlsRenderer->SetOrder(ERenderOrder::CONTROLS);
	
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
	AddDoor(CurLocation, _Dir, this);

	// this�� _Room�� �� ��ġ�� ���� ���ݴ밡 �Ǿ�� �Ѵ�.
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

	// Room ��� ��ġ ����
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

	// ������ �ٲ� ��ġ�� ���� ��ġ�� ������ �� �� �߰�
	_Room->AddDoor(_Room->GetActorLocation(), _Dir, _Room);

	return true;
}

ARoom* ARoom::LinkRoom(ARoom* _Room, RoomDir _Dir)
{
	if (this == _Room)
	{
		MSGASSERT("�ڱ� �ڽ��� ������ �� �����ϴ�.");
		return nullptr;
	}

	if (true == IsLinking(_Room))
	{
		MSGASSERT("�̹� ����� ���Դϴ�.");
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
	FVector2D DoorPos = _RoomLocation;
	FVector2D DoorOffestX = FVector2D(RoomRenderer->GetComponentScale().Half().iX(), 0);
	FVector2D DoorOffestY = FVector2D(0, RoomRenderer->GetComponentScale().Half().iY());
	FVector2D OffestX = { 40, 0 };
	FVector2D OffestY = { 0, 40 };

	switch (_Dir)
	{
	case RoomDir::NONE:
		break;
	case RoomDir::LEFT:
	{
		//DoorPos = _RoomLocation + FVector2D(-_RoomLocation.iX() / 2, GetLeftPos().iY());
		
		DoorPos = _RoomLocation - DoorOffestX + OffestX;
		break;
	}
	case RoomDir::RIGHT:
	{
		DoorPos = _RoomLocation + DoorOffestX - OffestX;
		break;
	}
	case RoomDir::UP:
	{
		DoorPos = _RoomLocation - DoorOffestY + OffestY;
		break;
	}
	case RoomDir::DOWN:
	{
		DoorPos = _RoomLocation + DoorOffestY - OffestY;
		break;
	}
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
	// map���� insert�� �ڷ���� ��� delete�ϱ� ������ ���� �Ҹ��ڸ� ȣ������ �ʿ䰡 ����.
}

void ARoom::BeginPlay()
{
	Super::BeginPlay();

}

void ARoom::Tick(float _DeltaTime)
{
	Super::Tick(_DeltaTime);
}




