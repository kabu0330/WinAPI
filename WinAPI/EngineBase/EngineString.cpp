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

// ��Ƽ����Ʈ(char)�� ���̵����Ʈ(wchar_t)�� �ٲ��ִ� �Լ�
std::wstring UEngineString::AnsiToUnicode(std::string_view _Name)
{
    // ��Ƽ����Ʈ : 1 ~ 3����Ʈ (����)
    // ���̵����Ʈ : 2����Ʈ(����)
    // ��Ƽ����Ʈ�� ���̵����Ʈ�� �ٲ� �� ����Ʈ�� �󸶳� �ʿ����� ��Ȯ�� �����ϱⰡ ��ƴ�.(����, ���� ����)
    // �׷��� ���ڿ��� �����ϱ� ���� �� ����Ʈ�� �ʿ�����, �׸��� ���̵����Ʈ�� ���ڵ��� ���ִ� �Լ��� �����쿡�� �����Ѵ�.
    // �� ���� ����� ��� �ϳ��� �Լ��� �����Ѵ�.

    int Size = MultiByteToWideChar(CP_ACP, 0, _Name.data(), static_cast<int>(_Name.size()), nullptr, 0);
    // CP_ACP : ���� ���� �ڵ带 ������� ����
    // ���ڿ��� ������ �ּҸ� nullptr�� �Է��ϸ� ���ڿ��� ������ ����Ʈ ũ�⸦ ��ȯ�Ѵ�.
    
    // ���ڿ��� ���ٸ� ����
    if (0 == Size)
    {
        MSGASSERT("Multibyte To WideChar �Լ��� ���ڿ� ��ȯ�� �����߽��ϴ�. ���� : ���ڿ� ũ�� 0  " + std::string(_Name));
        return L"";
    }
    
    std::wstring Result; // ���̵����Ʈ ���ڿ��� ������ �ּ� ����
    Result.resize(Size); // std::vector<wchar_t>�� push_back�ϱ� ���� �̸� vector�� ũ�⸦ �����Ѵ�.

    Size = MultiByteToWideChar(CP_ACP, 0, _Name.data(), static_cast<int>(_Name.size()), &Result[0], Size);
    
    // ���ڿ��� ������ �� ���ٸ�
    if (0 == Size)
    {
        MSGASSERT("MultyByte To WideChar �Լ��� ������ ���ڿ��� �����ϴ�. " + std::string(_Name));
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

