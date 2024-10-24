#include "PreCompile.h"
#include "ContentsCore.h"
#include <EngineCore/EngineAPICore.h>

ContentsCore::ContentsCore()
{
}

ContentsCore::~ContentsCore()
{
}

void ContentsCore::BeginPlay()
{
	UEngineAPICore::GetCore()->CreateLevel("Title");
	UEngineAPICore::GetCore()->CreateLevel("Play");
	UEngineAPICore::GetCore()->CreateLevel("End");
}

void ContentsCore::Tick()
{
}

