#include "PreCompile.h"
#include "Door.h"
#include <EngineCore/SpriteRenderer.h>
#include "Global.h"
#include "ContentsEnum.h"

ADoor::ADoor()
{

}


void ADoor::Initialize(const FVector2D& _Location, RoomDir _Direction, ARoom* _ConnectedRoom)
{
	//SpriteSetting();
	//DoorRendererX->SetComponentLocation(Global::WindowSize.Half());
	//DoorRendererX->SetComponentScale({ 250, 200 });
	//DoorRendererX->SetOrder(ERenderOrder::DOOR);
	//DoorRendererY->SetComponentLocation(Global::WindowSize.Half());
	//DoorRendererY->SetComponentScale({ 250, 200 });
	//DoorRendererY->SetOrder(ERenderOrder::DOOR);
	//switch (_Direction)
	//{

	//case RoomDir::LEFT:
	//case RoomDir::RIGHT:
	//	DoorRendererX->SetComponentLocation(Global::WindowSize.Half());
	//	DoorRendererX->SetComponentScale({ 250, 200 });
	//	DoorRendererX->SetOrder(ERenderOrder::DOOR);
	//	break;
	//case RoomDir::UP:
	//case RoomDir::DOWN:
	//	DoorRendererY->SetComponentLocation(Global::WindowSize.Half());
	//	DoorRendererY->SetComponentScale({ 250, 200 });
	//	DoorRendererY->SetOrder(ERenderOrder::DOOR);
	//	break;

	//default:
	//	break;
	//}


	DoorRendererX->SetComponentLocation(_Location);
	DoorRendererY->SetComponentLocation(_Location);
	this->Direction = _Direction;
	this->ConnectedRoom = _ConnectedRoom;
	
	//switch (_Direction)
	//{
	//case RoomDir::NONE:
	//	break;
	//case RoomDir::LEFT:
	//	DoorRendererX->ChangeAnimation("Door_Left_Open");
	//	break;
	//case RoomDir::RIGHT:
	//	DoorRendererX->ChangeAnimation("Door_Right_Open");
	//	break;
	//case RoomDir::UP:
	//	DoorRendererY->ChangeAnimation("Door_Up_Open");
	//	break;
	//case RoomDir::DOWN:
	//	DoorRendererY->ChangeAnimation("Door_Down_Open");
	//	break;
	//case RoomDir::MAX:
	//	break;
	//default:
	//	break;
	//}

	return;
}

void ADoor::SpriteSetting()
{
	//DoorRendererX = CreateDefaultSubObject<USpriteRenderer>();
	//DoorRendererY = CreateDefaultSubObject<USpriteRenderer>();
	//DoorRendererX->CreateAnimation("Door_Left_Open", "NormalRoomDoor.png", 0, 0, 0.1f, false);
	//DoorRendererX->CreateAnimation("Door_Left_Lock", "NormalRoomDoor.png", 4, 4, 0.1f, false);

	//DoorRendererX->CreateAnimation("Door_Right_Open", "NormalRoomDoor.png", 1, 1, 0.1f, false);
	//DoorRendererX->CreateAnimation("Door_Right_Lock", "NormalRoomDoor.png", 5, 5, 0.1f, false);

	//DoorRendererY->CreateAnimation("Door_Up_Open", "NormalRoomDoor.png", 2, 2, 0.1f, false);
	//DoorRendererY->CreateAnimation("Door_Up_Lock", "NormalRoomDoor.png", 6, 6, 0.1f, false);

	//DoorRendererY->CreateAnimation("Door_Down_Open", "NormalRoomDoor.png", 3, 3, 0.1f, false);
	//DoorRendererY->CreateAnimation("Door_Down_Lock", "NormalRoomDoor.png", 7, 7, 0.1f, false);

	//DoorRendererX->ChangeAnimation("Door_Left_Open");
	//DoorRendererY->ChangeAnimation("Door_Down_Open");
}

ADoor::~ADoor()
{
}

void ADoor::BeginPlay()
{
	Super::BeginPlay();
}

void ADoor::Tick(float _DeltaTime)
{
	Super::Tick(_DeltaTime);
}