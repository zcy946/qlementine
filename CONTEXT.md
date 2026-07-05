# Qlementine Fork

This context describes the language for the forked Qt widget style library and its planned redevelopment boundaries.

## Language

**Hard Fork**:
A redevelopment path where this repository becomes an independent library basis, allowing project renaming, namespace changes, CMake target changes, and loss of long-term upstream synchronization.
_Avoid_: Wrapper, upstream-compatible fork

**Upstream Qlementine**:
The original open-source project this repository was forked from.
_Avoid_: Original project, source project

**Vendor Prefix Removal**:
The first renaming step that removes the `oclero` namespace or supplier prefix while leaving Qlementine-specific names in place for now. During this step, `oclero::qlementine` becomes `qlementine`, public include paths move from `oclero/qlementine/...` to `qlementine/...`, the aggregate header moves from `oclero/qlementine.hpp` to `qlementine.hpp`, no compatibility aliases or forwarding headers are kept, and the existing `qlementine` CMake target remains unchanged.
_Avoid_: Full rename, widget removal

**Qt 5.15.2 Port**:
The later migration step that adapts the fork from its current Qt 6 build setup and APIs to a Qt5-only build that explicitly requires Qt 5.15.2 after Vendor Prefix Removal is complete.
_Avoid_: Vendor prefix removal

**Qt5 Library Build Verification**:
The first acceptance check for the Qt 5.15.2 Port, requiring only the `qlementine` static library target to configure and compile with Qt 5.15.2. Showcase, sandbox, and documentation are not hard acceptance gates for this step, and showcase should be disabled by default during this migration.
_Avoid_: Full showcase verification, Qt6 compatibility

**Qt5 Example App Configure Verification**:
The follow-up acceptance check for the Qt 5.15.2 Port where sandbox and showcase are enabled and expected to configure with Qt5. This check only establishes that the example application targets enter the build graph; source-level example app compile errors are handled separately.
_Avoid_: Full showcase verification, library verification

**Minimal Qt API Backport**:
A Qt 5.15.2 Port tactic where source-level Qt API changes are made only in response to actual Qt5 compile errors. It avoids broad preemptive rewrites of Qt enum spelling or application-layer code.
_Avoid_: Qt API cleanup pass, scoped enum rewrite

**Qt5 Port Regression Ledger**:
A verification ledger that records each Qt5-only replacement or deletion that could affect behavior, visual output, high-DPI rendering, input handling, or build semantics. It exists to guide later manual verification and prevent quiet feature loss during the Qt 5.15.2 Port.
_Avoid_: ADR, changelog, implementation spec

**Windows Native Title Bar Dark Frame**:
A Windows-only behavior where the native title bar and window frame follow the active Qlementine light or dark theme instead of staying on the system default frame color.
_Avoid_: Custom title bar, frameless window

**Library Build Verification**:
The first acceptance check for Vendor Prefix Removal, requiring the `qlementine` library target to compile after namespace and include path changes. Showcase and sandbox source references may be mechanically updated for consistency, but showcase, sandbox, and documentation are not hard acceptance gates for this step.
_Avoid_: Full showcase verification, documentation completion

**Mechanical Rename**:
A constrained batch edit used for Vendor Prefix Removal, limited to moving public include paths and replacing the agreed `oclero` namespace and include references. It must not include opportunistic style changes, refactors, redesign work, or broad documentation and README brand cleanup.
_Avoid_: Refactor, cleanup pass
