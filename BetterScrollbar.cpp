#include "BetterScrollbar.h"
#include <QWidget>
#include <QHash>
#include <QStyleOptionSlider>
#include <QEvent>
#include <QTimer>
#include <QPainter>
#include "computer.h"
#include "Utility.h"
#include <math.h>

//template <class T>
//constexpr std::add_const_t<T> &asConst(T &t) noexcept
//{
//    return t;
//}

template <class T>
void asConst(const T &&) = delete;
class HighlightScrollBarOverlay : public QWidget
{
public:
    HighlightScrollBarOverlay(HighlightScrollBar *scrollBar)
        : QWidget(scrollBar)
        , m_visibleRange(0.0)
        , m_offset(0.0)
        , m_cacheUpdateScheduled(false)
        , m_scrollBar(scrollBar)
    {}

    void scheduleUpdate();
    void updateCache();
    void adjustPosition();

    float m_visibleRange;
    float m_offset;
    QHash<Highlight::Id, QVector<Highlight> > m_highlights;

    bool m_cacheUpdateScheduled;
    QMap<int, Highlight> m_cache;

protected:
    void paintEvent(QPaintEvent *paintEvent) override;

private:
    HighlightScrollBar *m_scrollBar;
};

HighlightScrollBar::HighlightScrollBar(Qt::Orientation orientation, QWidget *parent)
    : QScrollBar(orientation, parent)
    , m_widget(parent)
    , m_overlay(new HighlightScrollBarOverlay(this))
{
    QObject::connect(m_overlay, &HighlightScrollBarOverlay::destroyed,
            this, &HighlightScrollBar::overlayDestroyed);
    // valueChanged(0) flashes transient scroll bars, which is needed
    // for a correct initialization.
    emit valueChanged(0);
}

HighlightScrollBar::~HighlightScrollBar()
{
    if (!m_overlay || m_overlay->parent() == this)
        return;

    delete m_overlay;
}

void HighlightScrollBar::setVisibleRange(float visibleRange)
{
    if (!m_overlay)
        return;
    m_overlay->m_visibleRange = visibleRange;
}

void HighlightScrollBar::setRangeOffset(float offset)
{
    if (!m_overlay)
        return;
    m_overlay->m_offset = offset;
}

QRect HighlightScrollBar::overlayRect()
{
    QStyleOptionSlider opt;
    initStyleOption(&opt);
    return style()->subControlRect(QStyle::CC_ScrollBar, &opt, QStyle::SC_ScrollBarGroove, this);
}

void HighlightScrollBar::overlayDestroyed()
{
    m_overlay = 0;
}


void HighlightScrollBar::addHighlight(Highlight highlight)
{
    if (!m_overlay)
        return;
    m_overlay->m_highlights[highlight.category] << highlight;
    m_overlay->scheduleUpdate();
}

void HighlightScrollBar::removeHighlights(Highlight::Id category)
{
    if (!m_overlay)
        return;
    m_overlay->m_highlights.remove(category);
    m_overlay->scheduleUpdate();
}

void HighlightScrollBar::removeAllHighlights()
{
    if (!m_overlay)
        return;
    m_overlay->m_highlights.clear();
    m_overlay->scheduleUpdate();
}

bool HighlightScrollBar::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == m_widget && m_overlay && m_widget == m_overlay->parent() &&
            (event->type() == QEvent::Resize || event->type() == QEvent::Move)) {
        QStyleOptionSlider opt;
        initStyleOption(&opt);
        const int width = style()->pixelMetric(QStyle::PM_ScrollBarExtent, &opt, this);
        m_overlay->move(m_widget->width() - width, 0);
        resize(width, m_widget->height());
    }
    return false;
}

void HighlightScrollBar::resizeEvent(QResizeEvent *event)
{
    if (!m_overlay)
        return;
    QScrollBar::resizeEvent(event);
    m_overlay->resize(size());
}

void HighlightScrollBar::moveEvent(QMoveEvent *event)
{
    if (!m_overlay)
        return;
    QScrollBar::moveEvent(event);
    m_overlay->adjustPosition();
}

void HighlightScrollBar::showEvent(QShowEvent *event)
{
    if (!m_overlay)
        return;
    QScrollBar::showEvent(event);
    if (parentWidget() != this) {
        m_widget->removeEventFilter(this);
        m_overlay->setParent(this);
        m_overlay->adjustPosition();
        m_overlay->show();
    }
}

void HighlightScrollBar::hideEvent(QHideEvent *event)
{
    if (!m_overlay)
        return;
    QScrollBar::hideEvent(event);
    if (parentWidget() != m_widget) {
        m_widget->installEventFilter(this);
        m_overlay->setParent(m_widget);
        m_overlay->adjustPosition();
        m_overlay->show();
    }
}

void HighlightScrollBar::changeEvent(QEvent *event)
{
    // Workaround for QTBUG-45579
    if (event->type() == QEvent::ParentChange)
        setStyle(style());
}

void HighlightScrollBarOverlay::scheduleUpdate()
{
    if (m_cacheUpdateScheduled)
        return;

    m_cacheUpdateScheduled = true;
    QTimer::singleShot(0, this, static_cast<void (QWidget::*)()>(&QWidget::update));
}

void HighlightScrollBarOverlay::updateCache()
{
    if (!m_cacheUpdateScheduled)
        return;

    m_cache.clear();
    const QList<Highlight::Id> &categories = m_highlights.keys();
    for (const Highlight::Id &category : categories) {
        for (const Highlight &highlight : (m_highlights[category])) {
            Highlight oldHighlight = m_cache[highlight.position];
            if (highlight.priority > oldHighlight.priority)
                m_cache[highlight.position] = highlight;
        }
    }
    m_cacheUpdateScheduled = false;
}

void HighlightScrollBarOverlay::adjustPosition()
{
    move(parentWidget()->mapFromGlobal(m_scrollBar->mapToGlobal(m_scrollBar->pos())));
}

void HighlightScrollBarOverlay::paintEvent(QPaintEvent *paintEvent)
{
    QWidget::paintEvent(paintEvent);

    updateCache();

    if (m_cache.isEmpty())
        return;

    const QRect &rect = m_scrollBar->overlayRect();
    if(rect.height()<=0)return;

    int previousColor = 0;
    Highlight::Priority previousPriority = Highlight::LowPriority;
    QRect *previousRect = 0;

    const int scrollbarRange = m_scrollBar->maximum() + m_scrollBar->pageStep();
    const int range = qMax(m_visibleRange, float(scrollbarRange));
    const int horizontalMargin = 3;
    const int resultWidth = rect.width() - 2 * horizontalMargin + 1;
    const int resultHeight = qMin(int(rect.height() / range) + 1, 4);
    const int offset = rect.height() / range * m_offset;
    const int verticalMargin = ((rect.height() / range) - resultHeight) / 2;
    int previousBottom = -1;

    QHash<int, QVector<QRect> > highlights;
    for (Highlight currentHighlight : (m_cache)) {
        // Calculate top and bottom
        int top = rect.top() + offset + verticalMargin
                + float(currentHighlight.position) / range * rect.height();
        const int bottom = top + resultHeight;

        if (previousRect && previousColor == currentHighlight.colorNum && previousBottom + 1 >= top) {
            // If the previous highlight has the same color and is directly prior to this highlight
            // we just extend the previous highlight.
            previousRect->setBottom(bottom - 1);

        } else { // create a new highlight
            if (previousRect && previousBottom >= top) {
                // make sure that highlights with higher priority are drawn on top of other highlights
                // when rectangles are overlapping

                if (previousPriority > currentHighlight.priority) {
                    // Moving the top of the current highlight when the previous
                    // highlight has a higher priority
                    top = previousBottom + 1;
                    if (top == bottom) // if this would result in an empty highlight just skip
                        continue;
                } else {
                    previousRect->setBottom(top - 1); // move the end of the last highlight
                    if (previousRect->height() == 0) // if the result is an empty rect, remove it.
                        highlights[previousColor].removeLast();
                }
            }
            highlights[currentHighlight.colorNum] << QRect(rect.left() + horizontalMargin, top,
                                                        resultWidth, bottom - top);
            previousRect = &highlights[currentHighlight.colorNum].last();
            previousColor = currentHighlight.colorNum;
            previousPriority = currentHighlight.priority;
        }
        previousBottom = previousRect->bottom();
    }

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, false);
    QColor colors[] = {Qt::red,Qt::yellow,Qt::green,Qt::blue,Qt::black,Qt::gray,Qt::darkBlue};
    foreach (int themeColor, highlights.keys()) {
        const QColor &color = (colors[themeColor]);
        for (int i = 0, total = highlights[themeColor].size(); i < total; ++i) {
            const QRect rect = highlights[themeColor][i];
            painter.fillRect(rect, color);
        }
    }
}

Highlight::Highlight(Id category_, int position_,
                     int colornum_, Highlight::Priority priority_)
    : category(category_)
    , position(position_)
    , colorNum(colornum_)
    , priority(priority_)
{
}
