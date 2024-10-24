#include "PreCompile.h"
#include <EngineCore/EngineAPICore.h>
#include <ContentsProject/ContentsCore.h>

// �� �̻� ������ ������� �ʰ� pragma comment�� ���̺귯���� �������Ͽ� ����
#pragma comment (lib, "EngineCore.lib")
#pragma comment (lib, "ContentsProject.lib")


int APIENTRY wWinMain(_In_ HINSTANCE hInstance, // �� ���α׷� �ּ�
	_In_opt_ HINSTANCE hPrevInstance,			// �� ���α׷��� �����Ų ���α׷� �ּ�(ex. ������ó, �����, etc) // �� ��
	_In_ LPWSTR    lpCmdLine,					// ���� �Ķ����  // ����� �� ��
	_In_ int       nCmdShow)					// â�� ȭ�鿡 ǥ�õ� ���
{
	ContentsCore User;
	return UEngineAPICore::EngineStart(hInstance, &User);
}