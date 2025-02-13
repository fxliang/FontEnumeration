#include "utils.h"
#include <dwrite.h>
#include <iostream>
#include <memory>
#include <wrl.h>

using namespace Microsoft::WRL;
#define WHITE (FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE)
#define RED (FOREGROUND_RED)
#define BLUE (FOREGROUND_BLUE)
#define OUTPUT(color)                                                          \
  SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),                     \
                          FOREGROUND_INTENSITY | color);

int wmain(int argc, wchar_t *argv[]) {
  wchar_t locale[LOCALE_NAME_MAX_LENGTH] = {0};
  if (!GetUserDefaultLocaleName(locale, LOCALE_NAME_MAX_LENGTH)) {
    DWORD errorCode = GetLastError();
    std::wcout << L"Error Code: " << std::hex << errorCode << ", Error Message"
               << StrzHr(errorCode) << std::endl;
    return errorCode;
  }
  ComPtr<IDWriteFactory> pDWriteFactory;
  HR(DWriteCreateFactory(
      DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory),
      reinterpret_cast<IUnknown **>(pDWriteFactory.ReleaseAndGetAddressOf())));
  ComPtr<IDWriteFontCollection> pFontCollection;
  // Get the system font collection.
  HR(pDWriteFactory->GetSystemFontCollection(
      pFontCollection.ReleaseAndGetAddressOf()));
  UINT32 familyCount = 0;
  // Get the number of font families in the collection.
  familyCount = pFontCollection->GetFontFamilyCount();
  ComPtr<IDWriteFontFamily> pFontFamily;
  ComPtr<IDWriteLocalizedStrings> pFamilyNames;
  const auto func = [&](const wchar_t *localeName, WORD color) -> void {
    UINT32 index = 0;
    BOOL exists = false;
    HR(pFamilyNames->FindLocaleName(localeName, &index, &exists));
    // If the specified locale doesn't exist, select the first on the list.
    index = !exists ? 0 : index;
    UINT32 length = 0;
    HR(pFamilyNames->GetStringLength(index, &length));
    std::unique_ptr<wchar_t[]> name(new wchar_t[length + 1]);
    HR(!name ? E_OUTOFMEMORY : S_OK);
    HR(pFamilyNames->GetString(index, name.get(), length + 1));
    std::wcout << localeName << L" family name:\t";
    OUTPUT(color);
    WriteConsole(GetStdHandle(STD_OUTPUT_HANDLE), name.get(), length + 1, 0, 0);
    OUTPUT(WHITE);
    std::cout << std::endl;
  };
  std::cout << "\n-------------------------------------------------------\n";
  for (UINT32 i = 0; i < familyCount; ++i) {
    HR(pFontCollection->GetFontFamily(i, pFontFamily.ReleaseAndGetAddressOf()));
    HR(pFontFamily->GetFamilyNames(pFamilyNames.ReleaseAndGetAddressOf()));
    func(locale, RED);
    func(L"en-US", BLUE);
    std::cout << "-------------------------------------------------------\n";
  }
  system("pause");
  return 0;
}
