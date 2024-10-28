#include "PreCompile.h"
#include "Player.h"
#include <EnginePlatform/EngineInput.h>
#include "Tear.h"

APlayer::APlayer()
{
	SetActorLocation({ 300, 300 });
	SetActorScale({ 100, 100 });
}

APlayer::~APlayer()
{
}

void APlayer::BeginPlay()
{
	// �Է� ��� 2 : �̺�Ʈ ������� ó��
	UEngineInput::GetInst().BindAction('A', KeyEvent::PRESS, std::bind(&APlayer::LeftMove , this, std::placeholders::_1));
	UEngineInput::GetInst().BindAction('D', KeyEvent::PRESS, std::bind(&APlayer::RightMove, this, std::placeholders::_1));
	UEngineInput::GetInst().BindAction('W', KeyEvent::PRESS, std::bind(&APlayer::UpMove   , this, std::placeholders::_1));
	UEngineInput::GetInst().BindAction('S', KeyEvent::PRESS, std::bind(&APlayer::DownMove , this, std::placeholders::_1));

}

void APlayer::Tick(float _DeltaTime)
{
	// �⺻������ � ����� ���� ����
	// ���� ������ �ִٸ� ������ ������ �����ϴ� ����� ��� �Ѵ�.
	// 1. �ٸ� ������������ ���� ����ϰ� �ִ� ����� ���ɼ��� ����.
	// 2. ���� �������� �Ǹ� ���� ���� ����� ������ �� �ִ�.
	// 
	// ���� ���� ���� ������ �ȴٸ�, 
	// 1. OS���� �����޾ƾ� �ϴ���, std���� ���� �޾ƾ��ϴ��� �˾ƾ� �Ѵ�.
	// 2. OS  : �ϵ���� ���� -> �Է� ����, CPU, �޸�
	// 3. std : ������ ���鿡�� �����ϴ� ������ -> std::vector 

	// �Է� ��� 1
	//if (true == UEngineInput::GetInst().IsPress('A'))
	//{
	//	AddActorLocation(FVector2D::LEFT * _DeltaTime * Speed);
	//}
	//if (true == UEngineInput::GetInst().IsPress('D'))
	//{
	//	AddActorLocation(FVector2D::RIGHT * _DeltaTime * Speed);
	//}
	//if (true == UEngineInput::GetInst().IsPress('W'))
	//{
	//	AddActorLocation(FVector2D::UP * _DeltaTime * Speed);
	//}
	//if (true == UEngineInput::GetInst().IsPress('S'))
	//{
	//	AddActorLocation(FVector2D::DOWN * _DeltaTime * Speed);
	//}

	if (1.0f < UEngineInput::GetInst().IsPressTime(VK_SPACE))
	{
		// �÷��̾ ���� ������ Bullet�� �����Ѵ�.
        ATear* Ptr = GetWorld()->SpawnActor<ATear>();
		Ptr->SetActorLocation(GetActorLocation());
		return;
	}
}

// �Է� ��� 2 : �̺�Ʈ ������� ó��
void APlayer::LeftMove(float _DeltaTime)
{
	AddActorLocation(FVector2D::LEFT * _DeltaTime * Speed);
}

void APlayer::RightMove(float _DeltaTime)
{
	AddActorLocation(FVector2D::RIGHT * _DeltaTime * Speed);
}

void APlayer::UpMove(float _DeltaTime)
{
	AddActorLocation(FVector2D::UP * _DeltaTime * Speed);
}

void APlayer::DownMove(float _DeltaTime)
{
	AddActorLocation(FVector2D::DOWN * _DeltaTime * Speed);
}

