// SPDX-FileCopyrightText: Olivier Cléro <oclero@hotmail.com>
// SPDX-License-Identifier: MIT

#include "WindowsTitleBarUtils.hpp"

#include <QPalette>
#include <QWidget>

#ifdef Q_OS_WIN
#include <QtCore/qt_windows.h>

#include <dwmapi.h>
#endif

namespace qlementine {
namespace {
#ifdef Q_OS_WIN
constexpr auto DwmwaUseImmersiveDarkMode = DWORD{ 20 };
constexpr auto DwmwaUseImmersiveDarkModeBefore20h1 = DWORD{ 19 };

bool shouldUseDarkTitleBar(const QPalette& palette) {
  return palette.color(QPalette::WindowText).lightness() > palette.color(QPalette::Window).lightness();
}

void setDarkTitleBar(HWND hwnd, bool dark) {
  const auto value = dark ? TRUE : FALSE;
  if (SUCCEEDED(DwmSetWindowAttribute(hwnd, DwmwaUseImmersiveDarkMode, &value, sizeof(value)))) {
    return;
  }
  DwmSetWindowAttribute(hwnd, DwmwaUseImmersiveDarkModeBefore20h1, &value, sizeof(value));
}
#endif
} // namespace

void updateWindowsNativeTitleBar(QWidget* widget, const QPalette& palette) {
#ifdef Q_OS_WIN
  if (!widget || !widget->isWindow() || widget->windowFlags().testFlag(Qt::FramelessWindowHint)) {
    return;
  }

  const auto winId = widget->internalWinId();
  if (!winId) {
    return;
  }

  const auto hwnd = reinterpret_cast<HWND>(winId);
  setDarkTitleBar(hwnd, shouldUseDarkTitleBar(palette));
#else
  Q_UNUSED(widget)
  Q_UNUSED(palette)
#endif
}
} // namespace qlementine
