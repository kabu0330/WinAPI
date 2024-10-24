#include "PreCompile.h"
#include <EngineCore/EngineAPICore.h>
#include <ContentsProject/ContentsCore.h>

// 더 이상 참조를 사용하지 않고 pragma comment로 라이브러리를 실행파일에 포함
#pragma comment (lib, "EngineCore.lib")
#pragma comment (lib, "ContentsProject.lib")


int APIENTRY wWinMain(_In_ HINSTANCE hInstance, // 내 프로그램 주소
	_In_opt_ HINSTANCE hPrevInstance,			// 내 프로그램을 실행시킨 프로그램 주소(ex. 스팀런처, 스토브, etc) // 안 씀
	_In_ LPWSTR    lpCmdLine,					// 메인 파라미터  // 요새는 안 씀
	_In_ int       nCmdShow)					// 창이 화면에 표시될 방식
{
	ContentsCore User;
	return UEngineAPICore::EngineStart(hInstance, &User);
}