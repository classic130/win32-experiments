#pragma once
#include <string>
#include <memory>
#include <comdef.h>
#include <Wbemidl.h>
#include <wrl/client.h>
#include <vector>
#include <functional>

namespace Util
{
	std::string ConvertWStringToString(const std::wstring& wstr);
	std::wstring ConvertStringToWString(const std::string& str);
	void PrintLastError(wchar_t* lpszFunction);
	std::vector<std::wstring> TokeniseString(const std::wstring& stringToTokenise, const std::wstring& delimiter);
	std::wstring Replace(std::wstring stringToWorkOn, const std::wstring& whatToReplace, const std::wstring& whatToReplaceWith);
	
	using ErrorCallback = std::function<std::string()>;
	void CheckHr(HRESULT hr, const std::string& msg);
	void CheckHr(HRESULT hr, ErrorCallback lambda);
}

int MsftDiskInfo();
int Win32DiskInfo();
int Win32LogicalDisk();
int Win32RamInfo();
int Win32ProcessorInfo();
int Win32ComputerSystem();
int Win32LoggedOnUser();
int Win32LogonSession();

class WmiClassObject
{
	public:
		WmiClassObject(IWbemClassObject* clsObj);
		WmiClassObject(const WmiClassObject&) = delete;
		~WmiClassObject();

		void operator=(const WmiClassObject&) = delete;

		[[nodiscard]] const short Short(const wchar_t* name);
		[[nodiscard]] const int Int32(const wchar_t* name);
		[[nodiscard]] const long Long(const wchar_t* name);
		[[nodiscard]] const long long Int64(const wchar_t* name);

		[[nodiscard]] const unsigned short UShort(const wchar_t* name);
		[[nodiscard]] const unsigned int UInt32(const wchar_t* name);
		[[nodiscard]] const unsigned long ULong(const wchar_t* name);
		[[nodiscard]] const unsigned long long UInt64(const wchar_t* name);

		[[nodiscard]] const std::wstring String(const wchar_t* name);
		[[nodiscard]] const uint64_t StringAsUInt64(const wchar_t* name);

		[[nodiscard]] const void* ObjectRef(const wchar_t* name);

	private:
		IWbemClassObject* m_clsObj;
};

class WmiObjectEnumerator
{
	public:
		WmiObjectEnumerator(IEnumWbemClassObject* wbemEnum);
		WmiObjectEnumerator(const WmiObjectEnumerator&) = delete;
		~WmiObjectEnumerator();

		void operator=(const WmiObjectEnumerator&) = delete;

		[[nodiscard]]
		IWbemClassObject* Next();

	private:
		IEnumWbemClassObject* m_wbemEnum;
};

class WmiServer final
{
	public:
		WmiServer(IWbemServices* pSvc);
		~WmiServer();

		WmiServer(const WmiServer&) = delete;
		void operator=(const WmiServer&) = delete;

		[[nodiscard]]
		WmiObjectEnumerator Query(const std::wstring& string);
		[[nodiscard]]
		WmiClassObject GetClassObject(const wchar_t* objectPath);

	private:
		IWbemServices* m_wbemService;
};

class WmiProxy final
{
	public:
		WmiProxy();
		WmiProxy(const WmiProxy&) = delete;
		~WmiProxy();

		void operator=(const WmiProxy&) = delete;

		[[nodiscard]]
		WmiServer ConnectServer(const std::wstring& server);

	private:
		IWbemLocator* m_wbemLocator;
};

class ComInitialiser final
{
	public:
		ComInitialiser(COINIT aparthmentThreadingMode = COINIT_MULTITHREADED);
		~ComInitialiser();
};
