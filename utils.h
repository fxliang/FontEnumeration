#pragma once
#include <sstream>
#include <tchar.h>
#include <windows.h>
inline std::basic_string<TCHAR> StrzHr(HRESULT hr) {
  TCHAR buffer[1024] = {0};
  if (FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                    NULL, hr, 0, buffer, sizeof(buffer) / sizeof(TCHAR),
                    NULL)) {
    return std::basic_string<TCHAR>(buffer);
  } else {
    std::basic_ostringstream<TCHAR> oss;
    oss << _T("Unknown error: 0x") << std::hex << hr;
    return oss.str();
  }
}
class DebugStream {
public:
  DebugStream() = default;
  ~DebugStream() {
    if (!wss.str().empty())
      OutputDebugString(wss.str().c_str());
    if (!ss.str().empty())
      OutputDebugStringA(ss.str().c_str());
  }
  template <typename T> DebugStream &operator<<(const T &value) {
    ss << value;
    return *this;
  }
  DebugStream &operator<<(const char *value) {
    if (value) {
      std::string svalue((value));
      ss << svalue;
    }
    return *this;
  }
  DebugStream &operator<<(const std::string value) {
    std::string svalue(value);
    ss << svalue;
    return *this;
  }
  DebugStream &operator<<(const std::wstring value) {
    std::wstring wvalue(value);
    wss << wvalue;
    return *this;
  }
private:
  std::wstringstream wss;
  std::stringstream ss;
};
struct ComException {
  HRESULT result;
  ComException(HRESULT const value) : result(value) {}
};
inline void HR_Impl(HRESULT const result, const char *file, int line) {
  if (S_OK != result) {
    DebugStream() << "[" << file << ":" << line << "] " << StrzHr(result);
    throw ComException(result);
  }
}
#define HR(result) HR_Impl(result, __FILE__, __LINE__)
