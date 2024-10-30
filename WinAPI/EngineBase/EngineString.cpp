#include "PreCompile.h"
#include "EngineString.h"
#include "EngineDebug.h"

std::string UEngineString::ToUpper(std::string_view _string)
{
    std::string Result = _string.data();
    Result.resize(_string.size());

    for (size_t i = 0; i < _string.size(); i++)
    {
        Result[i] = std::toupper(_string[i]);
    }

    return Result;
}

// 멀티바이트(char)를 와이드바이트(wchar_t)로 바꿔주는 함수
std::wstring UEngineString::AnsiToUnicode(std::string_view _Name)
{
    // 멀티바이트 : 1 ~ 3바이트 (가변)
    // 와이드바이트 : 2바이트(고정)
    // 멀티바이트를 와이드바이트로 바꿀 때 바이트가 얼마나 필요한지 정확히 예측하기가 어렵다.(영어, 숫자 제외)
    // 그래서 문자열을 저장하기 위해 몇 바이트가 필요한지, 그리고 와이드바이트로 인코딩을 해주는 함수를 윈도우에서 지원한다.
    // 두 가지 기능을 모두 하나의 함수가 수행한다.

    int Size = MultiByteToWideChar(CP_ACP, 0, _Name.data(), static_cast<int>(_Name.size()), nullptr, 0);
    // CP_ACP : 현재 국가 코드를 기반으로 변경
    // 문자열을 저장할 주소를 nullptr로 입력하면 문자열을 저장할 바이트 크기를 반환한다.
    
    // 문자열이 없다면 에러
    if (0 == Size)
    {
        MSGASSERT("Multibyte To WideChar 함수로 문자열 변환에 실패했습니다. 사유 : 문자열 크기 0  " + std::string(_Name));
        return L"";
    }
    
    std::wstring Result; // 와이드바이트 문자열을 저장할 주소 생성
    Result.resize(Size); // std::vector<wchar_t>를 push_back하기 전에 미리 vector의 크기를 설정한다.

    Size = MultiByteToWideChar(CP_ACP, 0, _Name.data(), static_cast<int>(_Name.size()), &Result[0], Size);
    
    // 문자열을 저장할 게 없다면
    if (0 == Size)
    {
        MSGASSERT("MultyByte To WideChar 함수로 저장할 문자열이 없습니다. " + std::string(_Name));
        return L"";
    }

    return Result;
}

UEngineString::UEngineString()
{
}

UEngineString::~UEngineString()
{
}

