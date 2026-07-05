// SPDX-FileCopyrightText: Olivier Cléro <oclero@hotmail.com>
// SPDX-License-Identifier: MIT

#pragma once

#include <QPushButton>

namespace qlementine {

class ActionButton : public QPushButton {
  Q_OBJECT

public:
  explicit ActionButton(QWidget* parent = nullptr);
  ~ActionButton() override;

public:
  void setAction(QAction* action);
  void updateFromAction();

private:
  QAction* _action{ nullptr };
};
} // namespace qlementine
