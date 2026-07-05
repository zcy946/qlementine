# Qt5 Port Regression Ledger

This ledger tracks Qt6-to-Qt5 replacements that may affect behavior. The goal is to verify each risk deliberately after the Qt 5.15.2 port builds, rather than treating "compiles" and "looks similar" as complete proof.

## Status Legend

- `Not checked`: Needs focused verification.
- `Observed`: Manually inspected during showcase or sandbox use, but not isolated.
- `Verified`: Checked with a focused scenario and accepted.
- `Needs fix`: Behavior differs enough to require code changes.

## Build System

| Area | Change | Risk | Suggested verification | Status |
| --- | --- | --- | --- | --- |
| Root CMake | `find_package(Qt6 ...)` changed to `find_package(Qt5 5.15.2 ...)`; `qt_standard_project_setup()` removed. | Qt5-only configure may miss setup previously supplied by Qt6 helpers. | Configure with the Qt5 x64 debug preset and confirm the `qlementine` target enters the build graph. | Verified: static library builds. |
| Library target | `qt_add_library()` changed to `add_library()` with `AUTOMOC`, `AUTORCC`, and `AUTOUIC` kept as target/global properties. | Qt resource or moc generation could silently differ. | Build the static library from a clean Qt5 build directory and confirm resources/fonts/icons are available at runtime. | Verified: static library builds; runtime resources not checked. |
| Qt link targets | `Qt::Core`, `Qt::Widgets`, and `Qt::Svg` changed to `Qt5::Core`, `Qt5::Widgets`, and `Qt5::Svg`. | Missing Qt module dependencies would surface as link or runtime failures. | Build library, sandbox, and showcase under Qt5. | Observed: user reports all three build. |
| Package config | Exported package dependency changed from Qt6 to Qt5 5.15.2. | Installed package consumers may fail if config dependencies are incomplete. | Install or package the library, then consume it from a small external CMake project. | Not checked. |
| MSVC compile options | Added `/utf-8` for MSVC builds. | Source and execution character set behavior changes; non-ASCII literals become deterministic UTF-8. | Inspect text containing accented characters, ellipsis, and long sample strings in sandbox/showcase. | Observed: user reports visual output is basically consistent. |
| Showcase default | `QLEMENTINE_SHOWCASE` default changed from `ON` to `OFF`. | Default configure no longer builds showcase unless explicitly enabled. | Configure without options and confirm only the library is selected; configure with `QLEMENTINE_SHOWCASE=ON` and confirm showcase is selected. | Not checked. |

## Core Qt API Backports

| Area | Change | Risk | Suggested verification | Status |
| --- | --- | --- | --- | --- |
| Variant color parsing | `QVariant::typeId()` and `QMetaType::Type` checks changed to Qt5 `QVariant::type()` checks. | Theme color parsing from JSON-like variants could reject list or string colors. | Load light and dark themes and compare colors for palette, status colors, and custom color fields. | Observed: user reports visuals are basically consistent. |
| Geometry utilities | Added `<cmath>` for `std::hypot`. | No behavior change expected; compile-only portability fix. | Exercise rounded hit testing through widgets using rounded rect detection. | Not checked. |
| Rect conversion | `QRect::toRectF()` replaced with `QRectF(rect)`. | Sub-pixel border placement could differ by tiny amounts. | Inspect switch groove borders, popover border, and rounded primitive borders at 100%, 150%, and 200% scaling. | Observed: user reports visuals are basically consistent. |
| Enter events | `QEnterEvent*` overrides changed to Qt5 `QEvent*` overrides for `AbstractItemListWidget` and `Switch`. | Hover entry behavior may lose pointer-position detail, though current code only needs entry notification. | Hover navigation bar, segmented control, abstract list items, and switch; verify hover animations start and clear correctly. | Not checked. |
| Mouse local position | `QMouseEvent::position()` changed to `localPos()` in `Popover`. | Hit testing may differ if coordinate spaces are not equivalent. | Click inside and outside popovers, including near rounded corners and borders, and confirm press/release/close behavior. | Not checked. |
| Bitmap conversion | `QBitmap::fromPixmap(mask)` changed to `QBitmap(mask)`. | Popover mask may differ, affecting rounded hit region or clipping. | Open popovers over contrasting backgrounds and inspect rounded mask/clipping edges. | Not checked. |

## High-DPI Rendering

| Area | Change | Risk | Suggested verification | Status |
| --- | --- | --- | --- | --- |
| Example app high-DPI attributes | Qt6 default high-DPI behavior is restored for Qt5 showcase and sandbox by enabling `Qt::AA_EnableHighDpiScaling` and `Qt::AA_UseHighDpiPixmaps` before `QApplication` creation. | Without these Qt5 application attributes, showcase and sandbox ignore high-DPI scaling even though the style code still handles device pixel ratios. | Run showcase and sandbox at 100%, 150%, and 200% scaling and confirm window geometry, icon size/sharpness, popovers, and text metrics scale correctly. | Verified: user confirmed high-DPI adaptation works in showcase after the Qt5 attributes were restored. |
| Icon pixmap cache | Qt6 `QIcon::pixmap(size, dpr, mode, state)` replaced with Qt5 window-aware `pixmap(window, size, mode, state)` where possible, so callers pass logical icon size and Qt selects the matching high-DPI pixmap. | Icons could render blurry, oversized, undersized, or cache incorrectly across scale factors and states. | Compare normal, disabled, checked, selected, and hovered icons at 100%, 150%, and 200% scaling. | Verified: user confirmed the icon blur and oversized-icon regressions are fully resolved after avoiding integer widget DPR and manual double scaling. |
| Header icons | Same high-DPI pixmap replacement applied in header drawing. | Tree/table header icons could have incorrect size or colorization. | Inspect table/tree headers with icons, disabled state, and auto icon color enabled. | Not checked. |
| Popover shadow size | `QPixmap::deviceIndependentSize()` replaced with size divided by `devicePixelRatio()`. | Drop shadow placement could shift under high-DPI scaling. | Open popovers at multiple scale factors and compare shadow centering and offset. | Not checked. |
| Device-pixel-ratio event | Removed Qt6-only `QEvent::DevicePixelRatioChange` handling from popover frame. | Popover may not resize/recompute immediately when moving between monitors with different scale factors. | Move an open popover between monitors with different DPI or change scale, then verify frame/shadow alignment. | Not checked. |

## Style Metrics And Layout

| Area | Change | Risk | Suggested verification | Status |
| --- | --- | --- | --- | --- |
| Tab index | Removed Qt6 `QStyleOptionTab::tabIndex` usage and rely on `QTabBar::tabAt()`. | Tab hover/current calculations may be wrong if geometry lookup differs from style option metadata. | Hover tabs, close tabs, scroll tab bars, and verify selected/hovered/pressed states. | Not checked. |
| Menu shortcut width | Removed Qt6 `QStyleOptionMenuItem::reservedShortcutWidth`; shortcut width now uses actual shortcut text width. | Menu item widths may be narrower or inconsistent across items with and without shortcuts. | Open menus with mixed shortcut lengths and compare alignment of labels, shortcuts, arrows, icons, and check marks. | Not checked. |
| Line edit icon pixel metrics | Removed Qt6-only `PM_LineEditIconMargin` and `PM_LineEditIconSize` cases. | Code asking the style for these Qt6 metrics will no longer get theme values under Qt5. | Inspect line edits with leading/trailing actions, clear buttons, validation states, and text margins. | Not checked. |

## Include Completeness

| Area | Change | Risk | Suggested verification | Status |
| --- | --- | --- | --- | --- |
| About dialog includes | Added full `QIcon` and `QUrl` includes instead of relying on transitive Qt6 includes. | No behavior change expected; prevents incomplete-type failures in Qt5. | Open About dialog, verify icon, website link, and social media buttons. | Not checked. |

## Example Applications

| Area | Change | Risk | Suggested verification | Status |
| --- | --- | --- | --- | --- |
| Sandbox target | `qt_add_executable()` changed to `add_executable()` and MSVC `/utf-8` added. | Resource or moc behavior could differ; non-ASCII text should remain correct. | Build and run sandbox; inspect text-heavy pages and resources. | Observed: user reports sandbox visual output is basically consistent. |
| Showcase target | `qt_add_executable()` changed to `add_executable()`. | Resource or moc behavior could differ. | Build and run showcase; inspect icons, resources, theme switching, and main widgets. | Observed: user reports showcase visual output is basically consistent. |
| Showcase icons dependency | `FetchContent_MakeAvailable(qlementine-icons)` replaced by manual static target construction from the fetched sources/resources. | Icon resources, include paths, or target properties may diverge from upstream qlementine-icons behavior. | Check all showcase icon categories, icon theme initialization, and themed icon colorization. | Observed: user reports showcase visual output is basically consistent. |
| Menu action creation | Qt6 `QMenu::addAction` overloads with shortcut/callback replaced by explicit `QAction` creation, shortcut assignment, and signal connection. | Shortcuts or callbacks may not fire, and action ownership/lifetime should remain correct. | Exercise File/Edit/View/Help menus, shortcuts, theme switching, and About dialog actions. | Not checked. |
| Sandbox menu actions | Qt6 `QMenu::addAction` shortcut/callback overloads replaced by explicit actions and shortcuts. | Context menu shortcuts and callbacks may differ. | Open sandbox custom context menus and trigger generated actions/shortcuts. | Not checked. |
| Example long strings | Some `QStringLiteral` raw strings changed to `QString::fromUtf8`. | Text encoding and line breaks may differ. | Inspect text edit, plain text edit, and message box sample text including ellipsis and links. | Observed: user reports visuals are basically consistent. |

## Open Verification Order

1. Verify package consumption from an external CMake project.
2. Verify high-DPI icon and popover behavior at multiple scale factors.
3. Verify menus with shortcuts, check marks, icons, and submenus.
4. Verify line edits with embedded actions and validation states.
5. Verify hover/pressed/selected animations for tabs, switch, navigation bar, and segmented control.
6. Verify About dialog links and icon behavior.
