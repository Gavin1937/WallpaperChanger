#include <QApplication>
#include <QSystemTrayIcon>
#include <QtPlugin>

#include "resources.h"
#include "mainwindow.h"

#include <windows.h>
//#include <shellapi.h>
//#include <processenv.h>
//#include <locale>
//#include <codecvt>

//int main(int argc, char *argv[])
int APIENTRY WinMain(
  HINSTANCE hInstance,
  HINSTANCE hPrevInstance,
  LPSTR     lpCmdLine,
  int       nShowCmd
)
{
  Q_INIT_RESOURCE(resources);

//  int argc = 0;
//  auto argvw = CommandLineToArgvW(GetCommandLine(), &argc);
  
//  std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
//  char** argv = new char*[wcslen((wchar_t*)argvw)];
//  for (int i = 0; i < wcslen((wchar_t*)argvw); ++i)
//	argv[i] = converter.to_bytes(argvw[i]).data();
//  QApplication a(argc, argv);

  int argc = 0;
  char** argv = nullptr;

  QApplication a(argc, argv);

  MainWindow w;
  w.show();

  return a.exec();
}
