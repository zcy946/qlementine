// SPDX-FileCopyrightText: Olivier Cléro <oclero@hotmail.com>
// SPDX-License-Identifier: MIT

#include <qlementine/widgets/Menu.hpp>

namespace qlementine {

void Menu::setEnabledPredicate(const std::function<bool()>& cb) {
  _updateEnabledCb = cb;
  updateEnabled();
}

void Menu::setVisiblePredicate(const std::function<bool()>& cb) {
  _updateVisibleCb = cb;
  updateVisible();
}

void Menu::updateEnabled() {
  if (_updateEnabledCb) {
    setEnabled(_updateEnabledCb());
  }
}

void Menu::updateVisible() {
  if (_updateVisibleCb) {
    setVisible(_updateVisibleCb());
  }
}

void Menu::updateProps() {
  updateEnabled();
  updateVisible();
}
} // namespace qlementine
