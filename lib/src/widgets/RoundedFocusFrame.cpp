// SPDX-FileCopyrightText: Olivier Cléro <oclero@hotmail.com>
// SPDX-License-Identifier: MIT

#include <qlementine/widgets/RoundedFocusFrame.hpp>

namespace qlementine {
const RadiusesF& RoundedFocusFrame::radiuses() const {
  return _radiuses;
}

void RoundedFocusFrame::setRadiuses(const RadiusesF& radiuses) {
  if (radiuses != _radiuses) {
    _radiuses = radiuses;
    Q_EMIT radiusesChanged();
    update();
  }
}
} // namespace qlementine
