
#ifndef UNDOSTACKMASKER_H
#define UNDOSTACKMASKER_H

#include <QWidget>
#include <HistoryHandler.h>
#include <QUndoView>
class UndoStackMasker : public QWidget
{
    Q_OBJECT

    HistoryHandler* Historian;
    QUndoView* QSV;
    QWidget* Middleman;
    const QSize constructSize();
public:
    explicit UndoStackMasker(HistoryHandler* historian, QWidget *parent = nullptr);

    /** Works to keep the hidden shield over the QSV properly sized
     * @param event
     */
    void resizeEvent(QResizeEvent *event);
signals:

    void signalFlare();
public slots:

    void updateSize();
    void finishedDoing();
protected slots:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
};

#endif // UNDOSTACKMASKER_H
