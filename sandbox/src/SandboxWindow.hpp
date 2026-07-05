// SPDX-FileCopyrightText: Olivier Cléro <oclero@hotmail.com>
// SPDX-License-Identifier: MIT

#pragma once

#include <QMainWindow>

namespace qlementine {
class QlementineStyle;
class ThemeManager;
} // namespace qlementine

namespace qlementine::sandbox {
class SandboxWindow : public QMainWindow {
public:
  SandboxWindow(ThemeManager* themeManager = nullptr, QWidget* parent = nullptr);
  ~SandboxWindow();

  bool eventFilter(QObject* watched, QEvent* event) override;

private:
  struct Impl;
  std::unique_ptr<Impl> _impl{};
};
} // namespace qlementine::sandbox
