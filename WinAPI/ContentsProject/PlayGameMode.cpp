#include "PreCompile.h"
#include "PlayGameMode.h"
#include "Room.h"
#include <EngineCore/EngineAPICore.h>

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
	
	ARoom* BaseRoom    = GetWorld()->SpawnActor<ARoom>();
	ARoom* CommonRoom0 = GetWorld()->SpawnActor<ARoom>();
	ARoom* CommonRoom1 = GetWorld()->SpawnActor<ARoom>();
	ARoom* CommonRoom2 = GetWorld()->SpawnActor<ARoom>();
	ARoom* CommonRoom3 = GetWorld()->SpawnActor<ARoom>();
	ARoom* BossRoom    = GetWorld()->SpawnActor<ARoom>();

	   BaseRoom->LinkRoom(CommonRoom0, RoomDir::LEFT );
	   BaseRoom->LinkRoom(CommonRoom1, RoomDir::RIGHT);
	   BaseRoom->LinkRoom(CommonRoom2, RoomDir::UP   );
	   BaseRoom->LinkRoom(CommonRoom3, RoomDir::DOWN );
	CommonRoom3->LinkRoom(BossRoom   , RoomDir::DOWN );
}

