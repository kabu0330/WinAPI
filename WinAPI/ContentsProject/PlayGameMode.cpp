#include "PreCompile.h"
#include "PlayGameMode.h"

#include <EngineCore/EngineAPICore.h>

#include "Global.h"
#include "Room.h"
#include "Door.h"

APlayGameMode::APlayGameMode()
{
}

APlayGameMode::~APlayGameMode()
{
}

void APlayGameMode::BeginPlay()
{
	// ������ ��������� ���� 1ȸ ������ ���⼭ �Ѵ�.
	// ���� ���� �������� �Ѵ�. 
	// ATitleGameMode, EndingGameMode, BossIntroGameMode
	// ������ ������ ���ÿ� ��������� �ϴ� �͵��� ���⼭ �����.
	// ex. ���
	
	// Player�� BeginPlay �Լ������� ���� ������ ȿ���� �� �� ������
	// Player�μ��� ������ �������� �����ֱ� ���ؼ� Player�� ����� �ƴϸ� ��� ���⼭ �����.
	GetWorld()->SetCameraPos({0, 0});
	GetWorld()->SetCameraToMainPawn(false);

	ARoom* BaseRoom    = GetWorld()->SpawnActor<ARoom>();
	ARoom* MinionRoom0 = GetWorld()->SpawnActor<ARoom>();
	ARoom* MinionRoom1 = GetWorld()->SpawnActor<ARoom>();
	ARoom* MinionRoom2 = GetWorld()->SpawnActor<ARoom>();
	ARoom* MinionRoom3 = GetWorld()->SpawnActor<ARoom>();
	ARoom* BossRoom    = GetWorld()->SpawnActor<ARoom>();

	   BaseRoom->InterLinkRoom(MinionRoom0, RoomDir::LEFT );
	   BaseRoom->InterLinkRoom(MinionRoom1, RoomDir::RIGHT);
	   BaseRoom->InterLinkRoom(MinionRoom2, RoomDir::UP   );
	   BaseRoom->InterLinkRoom(MinionRoom3, RoomDir::DOWN );
	MinionRoom3->InterLinkRoom(BossRoom   , RoomDir::DOWN );

	//ADoor* Door = GetWorld()->SpawnActor<ADoor>();

	int a = 0;
}

