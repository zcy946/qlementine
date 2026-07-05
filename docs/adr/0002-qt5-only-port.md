# Qt5-only port

The Qt 5.15.2 migration will make this fork target Qt5 only instead of maintaining Qt5 and Qt6 compatibility. This keeps the hard fork focused on the intended runtime, avoids `QT_VERSION_MAJOR` compatibility branches, and allows the CMake build to replace Qt6-specific commands with the simpler Qt5 build shape.
