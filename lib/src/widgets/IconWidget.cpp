// SPDX-FileCopyrightText: Olivier Cléro <oclero@hotmail.com>
// SPDX-License-Identifier: MIT

#include <qlementine/widgets/IconWidget.hpp>

#include <qlementine/style/QlementineStyle.hpp>
#include <qlementine/utils/PrimitiveUtils.hpp>

#include <QPainter>

namespace qlementine {
IconWidget::IconWidget(QWidget* parent)
  : QWidget(parent) {
  setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
  setFocusPolicy(Qt::NoFocus);
  const auto* style = this->style();
  const auto extent = style ? style->pixelMetric(QStyle::PM_ButtonIconSize, nullptr, this) : 16;
  _iconSize = QSize{ extent, extent };
}

IconWidget::IconWidget(const QIcon& icon, QWidget* parent)
  : QWidget(parent)
  , _icon{ icon } {
  setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
  setFocusPolicy(Qt::NoFocus);
  const auto* style = this->style();
  const auto extent = style ? style->pixelMetric(QStyle::PM_ButtonIconSize, nullptr, this) : 16;
  _iconSize = QSize{ extent, extent };
}

IconWidget::IconWidget(const QIcon& icon, const QSize& size, QWidget* parent)
  : QWidget(parent)
  , _iconSize{ size }
  , _icon{ icon } {
  setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
  setFocusPolicy(Qt::NoFocus);
}

const QIcon& IconWidget::icon() const {
  return _icon;
}

void IconWidget::setIcon(const QIcon& icon) {
  _icon = icon;
  Q_EMIT iconChanged();
  update();
}

const QSize& IconWidget::iconSize() const {
  return _iconSize;
}

void IconWidget::setIconSize(const QSize& iconSize) {
  if (iconSize != _iconSize) {
    _iconSize = iconSize;
    Q_EMIT iconSizeChanged();
    updateGeometry();
    update();
  }
}

QSize IconWidget::sizeHint() const {
  const auto padding = contentsMargins();
  const auto w = _iconSize.width() + padding.left() + padding.right();
  const auto h = _iconSize.height() + padding.top() + padding.bottom();
  return { w, h };
}

void IconWidget::paintEvent(QPaintEvent*) {
  const auto* qlementineStyle = qobject_cast<QlementineStyle*>(style());
  const auto autoIconColor = qlementineStyle ? qlementineStyle->autoIconColor(this) : AutoIconColor::None;
  const auto mouse = isEnabled() || autoIconColor != AutoIconColor::None ? MouseState::Normal : MouseState::Disabled;
  const auto& color = palette().color(isEnabled() ? QPalette::Normal : QPalette::Disabled, QPalette::Text);

  QPainter p(this);
  p.setRenderHint(QPainter::Antialiasing, true);
  const auto x = (width() - _iconSize.width()) / 2;
  const auto y = (height() - _iconSize.height()) / 2;
  const auto iconRect = QRect{ x, y, _iconSize.width(), _iconSize.height() };
  drawIcon(iconRect, &p, _icon, mouse, CheckState::NotChecked, this, autoIconColor != AutoIconColor::None, color);
}
} // namespace qlementine
