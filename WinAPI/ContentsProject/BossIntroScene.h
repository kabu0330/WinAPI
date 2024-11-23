#pragma once
#include <EngineCore/Actor.h>

// Ό³Έν :
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
	void AnimateScale(float _DeltaTime);
	void Oscillation(float _DeltaTime);

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

	float IntroTime = 0.0f;
	float FadeTime = 0.0f;

	FVector2D PlayerDir = FVector2D::ZERO;
	FVector2D BossDir = FVector2D::ZERO;
	bool ReverseMove = false;

	FVector2D VsScale = FVector2D::ZERO;
	FVector2D VsScaleOffset = FVector2D::ZERO;
	bool IsShrinking = true;
	const FVector2D	MinScale = { 324.0f, 162.0f };
	const FVector2D MaxScale = MinScale * 1.1f;
};

