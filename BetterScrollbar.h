/* Yeah, this is a mostly copy of highlightscrollbar that QtCreator uses.
 *
 * Since they don't have this in Qt on its own and porting it will be unbelievably
 * painful, I have decided to rewrite it
 *
 *
 */


#ifndef BETTERSCROLLBAR_H
#define BETTERSCROLLBAR_H
#include <QObject>
#include <QColor>
#include <QScrollBar>


struct Highlight
{
    enum Priority {
        Invalid = -1,
        LowPriority = 0,
        NormalPriority = 1,
        HighPriority = 2,
        HighestPriority = 3
    };
    enum Id {
        breakpoint = 0,
        regpoint    =1

    };


    Highlight(Id category, int position, int colorNum, Priority priority);
    Highlight() = default;
    Id category = breakpoint;
    int position = -1;
    int colorNum = 0;
    Priority priority = Invalid;
};


class HighlightScrollBarOverlay;

class HighlightScrollBar : public QScrollBar
{
    Q_OBJECT

public:
    HighlightScrollBar(Qt::Orientation orientation, QWidget *parent = 0);
    ~HighlightScrollBar() override;

    void setVisibleRange(float visibleRange);
    void setRangeOffset(float offset);

    void addHighlight(Highlight highlight);

    void removeHighlights(Highlight::Id id);
    void removeAllHighlights();

    bool eventFilter(QObject *, QEvent *event) override;

protected:
    void moveEvent(QMoveEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
    void showEvent(QShowEvent *event) override;
    void hideEvent(QHideEvent *event) override;
    void changeEvent(QEvent *even) override;

private:
    QRect overlayRect();
    void overlayDestroyed();

    QWidget *m_widget;
    HighlightScrollBarOverlay *m_overlay;
    friend class HighlightScrollBarOverlay;

};

#endif // BETTERSCROLLBAR_H
