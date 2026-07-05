// SPDX-FileCopyrightText: Olivier Cléro <oclero@hotmail.com>
// SPDX-License-Identifier: MIT

#include <QApplication>

#include <qlementine/style/QlementineStyle.hpp>
#include <qlementine/style/ThemeManager.hpp>

#include "SandboxWindow.hpp"

#define USE_CUSTOM_STYLE 1

int main(int argc, char* argv[]) {
  // Must be set before creating a QApplication.
  QApplication::setHighDpiScaleFactorRoundingPolicy(Qt::HighDpiScaleFactorRoundingPolicy::PassThrough);

  QApplication qApplication(argc, argv);

  // Must be set after creating a QApplication.
  QGuiApplication::setApplicationDisplayName("Sandbox");
  QCoreApplication::setApplicationName("Sandbox");
  QGuiApplication::setDesktopFileName("Sandbox");
  QCoreApplication::setOrganizationName("oclero");
  QCoreApplication::setOrganizationDomain("olivierclero.com");
  QCoreApplication::setApplicationVersion("1.0.0");
  QApplication::setWindowIcon(QIcon(":/sandbox/qlementine_icon.ico"));

#if USE_CUSTOM_STYLE
  // Set custom QStyle.
  auto* style = new qlementine::QlementineStyle(&qApplication);
  style->setAnimationsEnabled(true);
  style->setAutoIconColor(qlementine::AutoIconColor::TextColor);
  qApplication.setStyle(style);

  // Theme manager.
  auto* themeManager = new qlementine::ThemeManager(style);
  themeManager->loadDirectory(":/sandbox/themes");

  // Define theme on QStyle.
  themeManager->setCurrentTheme("Light");
#endif

  auto window = std::make_unique<qlementine::sandbox::SandboxWindow>(themeManager);
  window->show();

  return qApplication.exec();
}
