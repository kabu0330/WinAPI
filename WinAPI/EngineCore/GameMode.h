#pragma once
#include "Actor.h"

// ���� : ���ӿ��� ���� ���� ��������� ������ ��Ģ�� ���ϴ� Ŭ����
class AGameMode : public AActor
{
public:
	AGameMode();
	~AGameMode();

protected:

private:
	// delete Function
	AGameMode(const AGameMode& _Other) = delete;
	AGameMode(AGameMode&& _Other) noexcept = delete;
	AGameMode& operator=(const AGameMode& _Other) = delete;
	AGameMode& operator=(AGameMode&& _Other) noexcept = delete;
};

