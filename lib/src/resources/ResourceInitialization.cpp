// SPDX-FileCopyrightText: Olivier Cléro <oclero@hotmail.com>
// SPDX-License-Identifier: MIT

#include <qlementine/resources/ResourceInitialization.hpp>

#include <qglobal.h>

// This must be done outside of any namespace.
void qlementineResourceInitialization() {
  // Loads the QRC content.
  Q_INIT_RESOURCE(qlementine);
  Q_INIT_RESOURCE(qlementine_font_inter);
  Q_INIT_RESOURCE(qlementine_font_roboto);
}

namespace qlementine::resources {
void initializeResources() {
  qlementineResourceInitialization();
}
} // namespace qlementine::resources
