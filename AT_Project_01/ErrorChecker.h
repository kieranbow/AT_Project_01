#pragma once
#include "WinHeader.h"
#include <string>
#include <comdef.h>

class ErrorChecker
{
	public:
		static std::wstring StringToWideString(std::string input_str)
		{
			std::wstring w_string(input_str.begin(), input_str.end());
			return w_string;
		}

		static void LogError(std::string message)
		{
			std::string error_msg = "Error: " + message;
			MessageBoxA(NULL, error_msg.c_str(), "Error", MB_ICONERROR);
		}

		static void LogError(HRESULT hResult, std::string message)
		{
			_com_error error(hResult);
			std::wstring error_msg = L"Error: " + StringToWideString(message) + L"\n" + error.ErrorMessage();
			MessageBoxW(NULL, error_msg.c_str(), L"Error", MB_ICONERROR);
		}

		static void LogError(HRESULT hResult, std::wstring message)
		{
			_com_error error(hResult);
			std::wstring error_msg = L"Error: " + message + L"\n" + error.ErrorMessage();
			MessageBoxW(NULL, error_msg.c_str(), L"Error", MB_ICONERROR);
		}

		static void ThrowIf(HRESULT hr, std::string msg)
		{
			if (FAILED(hr))
			{
				LogError(hr, msg);
			}
			// if (SUCCEEDED(hr))
			// {
				// OutputDebugStringA("Success\n");
			// }
		}

	private:
};

