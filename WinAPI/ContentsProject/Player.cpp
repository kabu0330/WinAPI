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
	// 입력 방법 2 : 이벤트 방식으로 처리
	UEngineInput::GetInst().BindAction('A', KeyEvent::PRESS, std::bind(&APlayer::LeftMove , this, std::placeholders::_1));
	UEngineInput::GetInst().BindAction('D', KeyEvent::PRESS, std::bind(&APlayer::RightMove, this, std::placeholders::_1));
	UEngineInput::GetInst().BindAction('W', KeyEvent::PRESS, std::bind(&APlayer::UpMove   , this, std::placeholders::_1));
	UEngineInput::GetInst().BindAction('S', KeyEvent::PRESS, std::bind(&APlayer::DownMove , this, std::placeholders::_1));

}

void APlayer::Tick(float _DeltaTime)
{
	// 기본적으로 어떤 기능을 만들 때는
	// 게임 엔진이 있다면 무조건 엔진이 제공하는 기능을 써야 한다.
	// 1. 다른 컨텐츠에서도 많이 사용하고 있는 기능일 가능성이 높다.
	// 2. 엔진 버전업이 되면 내가 만든 기능이 무너질 수 있다.
	// 
	// 만약 내가 직접 만들어야 된다면, 
	// 1. OS에서 제공받아야 하는지, std에서 제공 받아야하는지 알아야 한다.
	// 2. OS  : 하드웨어 관련 -> 입력 버퍼, CPU, 메모리
	// 3. std : 문법적 측면에서 접근하는 문제들 -> std::vector 

	// 입력 방법 1
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
		// 플레이어가 속한 레벨에 Bullet을 생성한다.
        ATear* Ptr = GetWorld()->SpawnActor<ATear>();
		Ptr->SetActorLocation(GetActorLocation());
		return;
	}
}

// 입력 방법 2 : 이벤트 방식으로 처리
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

