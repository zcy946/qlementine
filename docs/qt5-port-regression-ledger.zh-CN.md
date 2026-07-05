# Qt5 迁移回归台账

本台账记录 Qt6 到 Qt 5.15.2 迁移过程中可能影响行为的替换、删除和降级实现。目标是把每个风险点后续逐项验证，而不是把“能编译”和“看起来差不多”当作完整证明。

## 状态说明

- `未检查`: 需要后续专项验证。
- `已观察`: 已在 showcase 或 sandbox 使用过程中人工观察过，但还没有隔离验证。
- `已验证`: 已通过明确场景检查并接受。
- `需要修复`: 行为差异足够明显，需要继续改代码。

## 构建系统

| 范围 | 变更 | 风险 | 建议验证 | 状态 |
| --- | --- | --- | --- | --- |
| 根 CMake | `find_package(Qt6 ...)` 改为 `find_package(Qt5 5.15.2 ...)`，删除 `qt_standard_project_setup()`。 | Qt5-only 配置可能缺少原先 Qt6 helper 隐式提供的设置。 | 使用 Qt5 x64 debug preset 配置，确认 `qlementine` target 进入构建图。 | 已验证: 静态库可构建。 |
| 库 target | `qt_add_library()` 改为 `add_library()`，保留 `AUTOMOC`、`AUTORCC`、`AUTOUIC`。 | Qt 资源或 moc 生成行为可能悄悄变化。 | 从干净 Qt5 build 目录构建静态库，并确认运行时字体、图标、资源可用。 | 已验证: 静态库可构建；运行时资源未专项检查。 |
| Qt 链接目标 | `Qt::Core/Widgets/Svg` 改为 `Qt5::Core/Widgets/Svg`。 | Qt 模块依赖缺失会表现为链接失败或运行时失败。 | 在 Qt5 下构建 library、sandbox、showcase。 | 已观察: 用户报告三者均可构建。 |
| 包配置 | 导出包依赖从 Qt6 改为 Qt5 5.15.2。 | install 后的外部 CMake 消费者可能因为依赖配置不完整而失败。 | install 或 package 后，用一个小型外部 CMake 项目消费该库。 | 未检查。 |
| MSVC 编译选项 | MSVC 增加 `/utf-8`。 | 源码字符集和执行字符集行为改变；非 ASCII 字面量会稳定按 UTF-8 处理。 | 检查 sandbox/showcase 中包含重音字符、省略号、长文本的界面。 | 已观察: 用户报告视觉效果基本一致。 |
| showcase 默认值 | `QLEMENTINE_SHOWCASE` 默认从 `ON` 改为 `OFF`。 | 默认 configure 不再构建 showcase，除非显式开启。 | 不传额外选项配置时确认只选择库；传 `QLEMENTINE_SHOWCASE=ON` 时确认 showcase 进入构建图。 | 未检查。 |

## 核心 Qt API 回退

| 范围 | 变更 | 风险 | 建议验证 | 状态 |
| --- | --- | --- | --- | --- |
| Variant 颜色解析 | `QVariant::typeId()` 与 `QMetaType::Type` 判断改为 Qt5 的 `QVariant::type()` 判断。 | 从 JSON-like variant 解析主题颜色时，list 或 string 颜色可能被错误拒绝。 | 加载浅色/深色主题，对比 palette、状态色、自定义颜色字段。 | 已观察: 用户报告视觉效果基本一致。 |
| 几何工具 | 为 `std::hypot` 补 `<cmath>`。 | 预期无行为变化，只是编译可移植性修复。 | 通过使用圆角矩形命中检测的控件触发相关路径。 | 未检查。 |
| 矩形转换 | `QRect::toRectF()` 改为 `QRectF(rect)`。 | 亚像素边框位置可能出现极小差异。 | 在 100%、150%、200% 缩放下检查 switch groove、popover 边框、圆角 primitive 边框。 | 已观察: 用户报告视觉效果基本一致。 |
| 鼠标进入事件 | `AbstractItemListWidget` 和 `Switch` 的 `QEnterEvent*` override 回退为 Qt5 的 `QEvent*`。 | hover 进入行为可能丢失指针位置信息；当前代码只需要进入通知。 | hover navigation bar、segmented control、abstract list items、switch，确认 hover 动画正常开始和清理。 | 未检查。 |
| 鼠标局部坐标 | `Popover` 中 `QMouseEvent::position()` 改为 `localPos()`。 | 如果坐标空间不等价，popover 命中检测可能不同。 | 点击 popover 内部、外部、圆角边缘和边框附近，确认 pressed/released/close 行为。 | 未检查。 |
| Bitmap 转换 | `QBitmap::fromPixmap(mask)` 改为 `QBitmap(mask)`。 | popover mask 可能变化，影响圆角命中区域或裁剪边缘。 | 在对比明显的背景上打开 popover，检查圆角 mask 和裁剪边缘。 | 未检查。 |

## 高 DPI 渲染

| 范围 | 变更 | 风险 | 建议验证 | 状态 |
| --- | --- | --- | --- | --- |
| 示例应用 high-DPI attributes | 通过在创建 `QApplication` 前启用 `Qt::AA_EnableHighDpiScaling` 和 `Qt::AA_UseHighDpiPixmaps`，为 Qt5 showcase 和 sandbox 补回 Qt6 默认 high-DPI 行为。 | 如果缺少这些 Qt5 application attributes，showcase 和 sandbox 会忽略高 DPI 缩放，即使 style 代码仍然处理 device pixel ratio。 | 在 100%、150%、200% 缩放下运行 showcase 和 sandbox，确认窗口几何、图标尺寸/清晰度、popover 和文字指标都按预期缩放。 | 已验证: 用户确认 Qt5 attributes 恢复后，showcase 的高 DPI 适配已正常工作。 |
| 图标 pixmap 缓存 | Qt6 `QIcon::pixmap(size, dpr, mode, state)` 改为尽量使用 Qt5 window-aware `pixmap(window, size, mode, state)`，调用方传逻辑图标尺寸，由 Qt 选择匹配的高 DPI pixmap。 | 图标可能模糊、过大、过小，或在不同缩放/状态下缓存错误。 | 在 100%、150%、200% 缩放下比较 normal、disabled、checked、selected、hovered 图标。 | 已验证: 用户确认避免整数 widget DPR 和手动 double scaling 后，图标模糊和图标过大回归均已完全解决。 |
| Header 图标 | header 绘制中应用同样的高 DPI pixmap 回退。 | tree/table header 图标可能尺寸或自动染色不正确。 | 检查带图标的 table/tree header，包括 disabled 状态和 auto icon color。 | 未检查。 |
| Popover 阴影尺寸 | `QPixmap::deviceIndependentSize()` 改为用尺寸除以 `devicePixelRatio()`。 | 高 DPI 下 drop shadow 位置可能偏移。 | 在多个缩放比例下打开 popover，对比阴影居中和 offset。 | 未检查。 |
| DPR 变化事件 | 删除 Qt6-only `QEvent::DevicePixelRatioChange` popover frame 处理。 | 在不同 DPI 显示器之间移动打开的 popover 时，可能不会立即重新计算尺寸/阴影。 | 将打开的 popover 在不同 DPI 显示器间移动，或修改系统缩放，确认 frame/shadow 对齐。 | 未检查。 |

## 样式指标和布局

| 范围 | 变更 | 风险 | 建议验证 | 状态 |
| --- | --- | --- | --- | --- |
| Tab index | 删除 Qt6 `QStyleOptionTab::tabIndex`，改为依赖 `QTabBar::tabAt()`。 | 如果几何查找和 style option 元数据不一致，tab hover/current 计算可能错误。 | hover tab、关闭 tab、滚动 tab bar，确认 selected/hovered/pressed 状态。 | 未检查。 |
| 菜单快捷键宽度 | 删除 Qt6 `QStyleOptionMenuItem::reservedShortcutWidth`，只使用实际 shortcut 文本宽度。 | 带 shortcut 和不带 shortcut 的菜单项宽度可能更窄或对齐不一致。 | 打开含不同长度快捷键的菜单，检查 label、shortcut、arrow、icon、check mark 对齐。 | 未检查。 |
| Line edit 图标指标 | 删除 Qt6-only `PM_LineEditIconMargin` 和 `PM_LineEditIconSize` case。 | 若代码向 style 查询这些 Qt6 metric，在 Qt5 下不会再获得 theme 值。 | 检查带前置/后置 action、clear button、校验状态和文本边距的 line edit。 | 未检查。 |

## Include 完整性

| 范围 | 变更 | 风险 | 建议验证 | 状态 |
| --- | --- | --- | --- | --- |
| About dialog include | 增加完整 `QIcon` 和 `QUrl` include，不再依赖 Qt6 传递 include。 | 预期无行为变化；用于避免 Qt5 incomplete type 编译失败。 | 打开 About dialog，检查 icon、website link、social media buttons。 | 未检查。 |

## 示例应用

| 范围 | 变更 | 风险 | 建议验证 | 状态 |
| --- | --- | --- | --- | --- |
| Sandbox target | `qt_add_executable()` 改为 `add_executable()`，并增加 MSVC `/utf-8`。 | 资源或 moc 行为可能变化；非 ASCII 文本应保持正确。 | 构建并运行 sandbox，检查文本密集页面和资源。 | 已观察: 用户报告 sandbox 视觉效果基本一致。 |
| Showcase target | `qt_add_executable()` 改为 `add_executable()`。 | 资源或 moc 行为可能变化。 | 构建并运行 showcase，检查图标、资源、主题切换和主要控件。 | 已观察: 用户报告 showcase 视觉效果基本一致。 |
| Showcase icons 依赖 | `FetchContent_MakeAvailable(qlementine-icons)` 改为从 fetched sources/resources 手动构造静态 target。 | 图标资源、include path 或 target property 可能和上游 qlementine-icons 行为不同。 | 检查所有 showcase 图标分类、icon theme 初始化、themed icon colorization。 | 已观察: 用户报告 showcase 视觉效果基本一致。 |
| Windows 原生标题栏深色 frame | Qt6 build 中，Qlementine 切换主题时会用当前 theme palette 调用 `QApplication::setPalette()`。Qt6 Windows platform window 收到由此产生的 `QEvent::ApplicationPaletteChange` 后调用 `QWindowsWindow::setDarkBorder()`，最终使用 `DwmSetWindowAttribute(DWMWA_USE_IMMERSIVE_DARK_MODE)`。 | Qt5.15.2 port 中，Qlementine 侧的 palette 更新仍然发生，但 Qt6 Windows platform 对 application palette change 的这条响应链不再以同样形式存在，因此 client area 切到深色主题时，原生标题栏可能仍保持浅色。 | 在 Windows 上让 showcase 在 `light.json` 和 `dark.json` 间切换，确认原生标题栏跟随当前 Qlementine theme palette，而不依赖 Windows 系统 app 主题。 | 未检查: 代码路径已恢复，等待 Windows 手动验证。 |
| 菜单 action 创建 | Qt6 带 shortcut/callback 的 `QMenu::addAction` overload 改为显式创建 `QAction`、设置 shortcut、连接 signal。 | shortcut 或 callback 可能不触发，action 所有权和生命周期需要保持正确。 | 操作 File/Edit/View/Help 菜单、快捷键、主题切换和 About dialog action。 | 未检查。 |
| Sandbox 菜单 action | Qt6 `QMenu::addAction` shortcut/callback overload 改为显式 action 和 shortcut。 | context menu shortcut 和 callback 可能不同。 | 打开 sandbox 自定义 context menu，触发生成的 action/shortcut。 | 未检查。 |
| 示例长字符串 | 部分 raw `QStringLiteral` 改为 `QString::fromUtf8`。 | 文本编码和换行可能不同。 | 检查 text edit、plain text edit、message box 示例文本，包括省略号和链接。 | 已观察: 用户报告视觉效果基本一致。 |

## 建议验证顺序

1. 验证 install/package 后能被外部 CMake 项目消费。
2. 验证动态切换 `light.json`/`dark.json` 时 Windows 原生标题栏深色 frame 跟随当前 Qlementine 主题。
3. 在多个缩放比例下验证高 DPI 图标和 popover 行为。
4. 验证菜单 shortcut、check mark、icon、submenu 的布局和触发。
5. 验证带 embedded action 和 validation state 的 line edit。
6. 验证 tab、switch、navigation bar、segmented control 的 hover/pressed/selected 动画。
7. 验证 About dialog 的链接和图标行为。
