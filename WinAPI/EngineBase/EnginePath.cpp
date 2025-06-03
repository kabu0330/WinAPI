#include "PreCompile.h"
#include "EnginePath.h"
#include <EngineBase/EngineDebug.h>

UEnginePath::UEnginePath()
	: Path(std::filesystem::current_path())
{
}

UEnginePath::UEnginePath(std::string_view _Path)
	: Path(_Path)
{
}

UEnginePath::UEnginePath(std::filesystem::path _Path)
	: Path(_Path)
{
}

UEnginePath::~UEnginePath()
{
}

std::string UEnginePath::GetPathToString()
{
	return Path.string();
}

std::string UEnginePath::GetFileName()
{
	if (true == IsDirectory())
	{
		MSGASSERT("���� ����϶��� GetFileName�� ȣ���Ҽ� �ֽ��ϴ�." + Path.string());
		return "";
	}

	return Path.filename().string();
}

std::string UEnginePath::GetDirectoryName()
{
	if (false == IsDirectory())
	{
		MSGASSERT("���丮 ����϶��� GetDirectoryName�� ȣ���Ҽ� �ֽ��ϴ�." + Path.string());
		return "";
	}

	return Path.filename().string();
}

std::string UEnginePath::GetExtension()
{
	return Path.extension().string();
}

void UEnginePath::Append(std::string_view _AppendName)
{
	Path.append(_AppendName);
}

bool UEnginePath::IsExists()
{
	return std::filesystem::exists(Path);
}

bool UEnginePath::IsFile()
{
	return false == IsDirectory();
}

bool UEnginePath::IsDirectory()
{
	return std::filesystem::is_directory(Path);
}

void UEnginePath::MoveParent()
{
	Path = Path.parent_path();
}

bool UEnginePath::MoveParentToDirectory(std::string_view _Path)
{
	UEnginePath DummyPath = UEnginePath(Path);

	if (false == DummyPath.IsDirectory())
	{
		MSGASSERT("���丮 ����϶��� 'Move Parent To Directory' �Լ��� ȣ���� �� �ֽ��ϴ�");
		return false;
	}

	bool Result = false;
	std::filesystem::path CurPath = DummyPath.Path;

	// root_path : ����̺� ��ġ(C:\\ �̳� D:\\�� ���� '�� ��ǻ��'���� ���̴�) 
	std::filesystem::path Root = CurPath.root_path();
	while (true)
	{
		CurPath = DummyPath.Path;

		// ���ѷ��� Ż�� ���� append�� �����ϱ� ���� Ż��
		if (CurPath == Root)
		{
			break;
		}

		CurPath.append(_Path); // ���� ��ο��� _Path(Resource) �������� �߰��Ͽ� �˻�
		if (true == std::filesystem::exists(CurPath))
		{
			Result = true;
			Path = CurPath; // ã�� ��θ� ����� ����
			break;
		}
		DummyPath.MoveParent(); // ã�� ������ ������ ���� ������ �̵�
	}

	return Result; // ã���� �ϴ� ������ ã���� true ��ȯ
}
