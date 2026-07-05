// SPDX-FileCopyrightText: Olivier Cléro <oclero@hotmail.com>
// SPDX-License-Identifier: MIT

#pragma once

#include <functional>

class QMenu;
class QAction;

namespace qlementine {
QMenu* getTopLevelMenu(QMenu* menu);

void flashAction(QAction* action, QMenu* menu, const std::function<void()>& onAnimationFinished);
} // namespace qlementine
