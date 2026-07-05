// SPDX-FileCopyrightText: Olivier Cléro <oclero@hotmail.com>
// SPDX-License-Identifier: MIT

#pragma once

class QPalette;
class QWidget;

namespace qlementine {
void updateWindowsNativeTitleBar(QWidget* widget, const QPalette& palette);
} // namespace qlementine
