// SPDX-FileCopyrightText: Olivier Cléro <oclero@hotmail.com>
// SPDX-License-Identifier: MIT

#pragma once

#include <QMainWindow>

#include <memory>

namespace qlementine {
class QlementineStyle;
class ThemeManager;
} // namespace qlementine

namespace qlementine::showcase {
class ShowcaseWindow : public QWidget {
public:
  explicit ShowcaseWindow(ThemeManager* themeManager = nullptr, QWidget* parent = nullptr);
  ~ShowcaseWindow();

private:
  struct Impl;
  std::unique_ptr<Impl> _impl{};
};
} // namespace qlementine::showcase
