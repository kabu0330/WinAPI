#pragma once
#include <EngineCore/Actor.h>

// ���� :
class ABossIntroScene : public AActor
{
public:
	// constrcuter destructer
	ABossIntroScene();
	~ABossIntroScene();

	// delete Function
	ABossIntroScene(const ABossIntroScene& _Other) = delete;
	ABossIntroScene(ABossIntroScene&& _Other) noexcept = delete;
	ABossIntroScene& operator=(const ABossIntroScene& _Other) = delete;
	ABossIntroScene& operator=(ABossIntroScene&& _Other) noexcept = delete;

	void BeginPlay() override;
	void Tick(float _DeltaTime) override;

	void ShowScene();
	void CloseScene();

	void Move(float _DeltaTime);

	void FadeOut();
	void FadeIn();


protected:

private:
	void FadeChange();
	void VsRendererFadeChange();

	class USpriteRenderer* PlayertitleRenderer;
	class USpriteRenderer* BosstitleRenderer;
	class USpriteRenderer* PlayerRenderer;
	class USpriteRenderer* BossRenderer;
	class USpriteRenderer* PlayerSpotRenderer;
	class USpriteRenderer* BossSpotRenderer;
	class USpriteRenderer* VsRenderer;

	float FadeValue = 0.0f;
	float FadeDir = 1.0f;

	FVector2D PlayerDir = FVector2D::ZERO;
	FVector2D BossDir = FVector2D::ZERO;
	FVector2D VsScale = FVector2D::ZERO;
	FVector2D VsScaleOffset = FVector2D::ZERO;


	float IntroTime = 0.0f;
	float FadeTime = 0.0f;
};
