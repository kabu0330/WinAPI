#include "PreCompile.h"
#include "ContentsCore.h"
#include <EngineCore/EngineAPICore.h>
#include "PlayGameMode.h"
#include "Player.h"

ContentsCore::ContentsCore()
{
}

ContentsCore::~ContentsCore()
{
}

// 엔진이 실행되고 단 한 번만 실행된다.
void ContentsCore::BeginPlay()
{
	// 윈도우 타이틀을 여기서 정한다.
	UEngineAPICore::GetCore()->GetMainWindow().SetWindowTitle("The Binding of Isaac");

	// 윈도우 사이즈를 반드시 정해야 한다.
	UEngineAPICore::GetCore()->GetMainWindow().SetWindowPosAndScale({ 0, 0 }, { 1280, 720 });

	// 레벨을 생성한다.
	//UEngineAPICore::GetCore()->CreateLevel("Title");
	UEngineAPICore::GetCore()->CreateLevel<APlayGameMode, APlayer>("Play");
	//UEngineAPICore::GetCore()->CreateLevel("End");

	// 최초 실행될 레벨을 결정한다.
	UEngineAPICore::GetCore()->OpenLevel("Play");
}

void ContentsCore::Tick()
{
	// 현재 사용 용도가 없음
}

