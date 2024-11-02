#include "PreCompile.h"
#include "Door.h"
#include <EngineCore/SpriteRenderer.h>
#include "Global.h"
#include "ContentsEnum.h"

ADoor::ADoor()
{
	DoorRenderer = CreateDefaultSubObject<USpriteRenderer>();
	DoorRenderer->SetComponentLocation(Global::WindowSize.Half());
	DoorRenderer->SetComponentScale({300, 150});
	DoorRenderer->SetOrder(ERenderOrder::DOOR);

	NormalDoorSetting();

}


bool ADoor::Initialize(const FVector2D& _Location, RoomDir _Direction, ARoom* _ConnectedRoom)
{
	SetActorLocation(_Location);
	this->Direction = _Direction;
	this->ConnectedRoom = _ConnectedRoom;

	return true;
}

void ADoor::NormalDoorSetting()
{
	DoorRenderer->CreateAnimation("Door_Left_Open", "NormalRoomDoor.png", 0, 0, 0.1f, false);
	DoorRenderer->CreateAnimation("Door_Left_Lock", "NormalRoomDoor.png", 4, 4, 0.1f, false);

	DoorRenderer->CreateAnimation("Door_Right_Open", "NormalRoomDoor.png", 1, 1, 0.1f, false);
	DoorRenderer->CreateAnimation("Door_Right_Lock", "NormalRoomDoor.png", 5, 5, 0.1f, false);

	DoorRenderer->CreateAnimation("Door_Up_Open", "NormalRoomDoor.png", 2, 2, 0.1f, false);
	DoorRenderer->CreateAnimation("Door_Up_Lock", "NormalRoomDoor.png", 6, 6, 0.1f, false);

	DoorRenderer->CreateAnimation("Door_Down_Open", "NormalRoomDoor.png", 3, 3, 0.1f, false);
	DoorRenderer->CreateAnimation("Door_Down_Lock", "NormalRoomDoor.png", 7, 7, 0.1f, false);

	DoorRenderer->ChangeAnimation("Door_Up_Lock");
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