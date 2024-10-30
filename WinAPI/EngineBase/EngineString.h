#pragma once
#include <string>

// 설명 : 문자열과 관련된 모든 기능을 static 함수로 제공하는 헬퍼 클래스 
class UEngineString
{
public:
	// constrcuter destructer
	~UEngineString();

	// delete Function
	UEngineString(const UEngineString& _Other) = delete;
	UEngineString(UEngineString&& _Other) noexcept = delete;
	UEngineString& operator=(const UEngineString& _Other) = delete;
	UEngineString& operator=(UEngineString&& _Other) noexcept = delete;

	static std::string ToUpper(std::string_view _string);
	static std::wstring AnsiToUnicode(std::string_view _Name);

protected:

private:
	UEngineString();
};

