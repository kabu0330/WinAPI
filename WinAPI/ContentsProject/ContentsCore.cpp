#include "PreCompile.h"
#include "Player.h"

#include <EngineBase/EngineDebug.h>
#include <EngineBase/EngineDirectory.h>
#include <EngineBase/EngineFile.h>
#include <EngineCore/EngineAPICore.h>
#include <EngineCore/ImageManager.h>

#include "ContentsCore.h"
#include "PlayGameMode.h"
#include "Global.h"
#include "Room.h"

ContentsCore::ContentsCore()
{
}

ContentsCore::~ContentsCore()
{
}

// 엔진이 실행되고 단 한 번만 실행된다.
void ContentsCore::BeginPlay()
{
	// 진입 전까지 과정
	// 1. 게임 엔진 Core 전역에 메모리 주소 전달(GetCore)
	// 2. 창 클래스 설정 및 윈도우 창 생성, 메인 HDC 생성
	
	// 1. 윈도우 타이틀을 여기서 정한다.
	UEngineAPICore::GetCore()->GetMainWindow().SetWindowTitle("The Binding of Isaac");


	// 2. 윈도우 사이즈를 반드시 정해야 한다.
	Global::WindowSize = { 1280, 720 };

	// 3. 윈도우 사이즈를 재설정하는 과정에서 메인 HDC가 아닌 뒤에서 그림을 그릴 BackBuffer를 생성한다.
	UEngineAPICore::GetCore()->GetMainWindow().SetWindowPosAndScale({ 0, 0 }, Global::WindowSize);


	// 4. 파일 리소스 가져온다.
	// Directory 클래스를 생성자를 통해 현재 실행 파일 경로를 EnginePath의 Path 멤버에 저장한다.
	UEngineDirectory Dir;

	// 상대경로 : 디버깅을 할 때와 릴리즈하여 배포할 때 내 실행 파일의 경로가 다르다.
	// 이 때, 언제 어떤 상황에서도 내 리소스 파일을 찾아갈 수 있도록 방법을 만들어야 한다.
	// 현재 리소스 파일을 보관하고 있는 폴더명을 적는다.
	if (false == Dir.MoveParentToDirectory("Resources"))
	{
		MSGASSERT("리소스 폴더를 찾지 못했습니다.");
		return;
	}

	std::vector<UEngineFile> ImageFiles = Dir.GetAllFile(/*true*/); 
	// 디폴트 매개변수는 true이며, 하위 디렉토리의 리소스도 가져온다.

	for (size_t i = 0; i < ImageFiles.size(); i++)
	{
		std::string FilePath = ImageFiles[i].GetPathToString();

		// Load 함수 : 이미지의 파일명을 Key / HDC와 위치, 크기를 값으로 저장해주는 함수
		UImageManager::GetInst().Load(FilePath);
	}
	
	// 이미지 한 장에서 스프라이트로 만들 크기가 몇인지
	//UImageManager::GetInst().CuttingSprite("icon.png", { 1024, 1024 });
	//UImageManager::GetInst().CuttingSprite("SampleMap(848,536).png", { 848, 536 });
	UImageManager::GetInst().CuttingSprite("Body.png", 5, 6);
	UImageManager::GetInst().CuttingSprite("Head.png", 5, 2);


	//{

	//	UEngineDirectory BombDir;
	//	BombDir.MoveParentToDirectory("Resources");
	//	BombDir.Append("bomb");

	//	UImageManager::GetInst().LoadFolder(BombDir.GetPathToString());

	//}



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

