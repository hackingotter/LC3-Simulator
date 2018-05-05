//#ifndef HISTORYMODEL_H
//#define HISTORYMODEL_H

//#include <QObject>
//#include <HistoryHandler.h>
//#include <QUndoView>
//#define QED(em) Q_D(em)

////class HistoryModel : public UndoModel
////{
////    Q_OBJECT
////public:
////    HistoryModel();
////};
//class UndoView: public QListView
//{
//public:
//    UndoView(QWidget *parent);
//    UndoView(HistoryHandler *stack, QWidget *parent);
//    #ifndef QT_NO_UNDOGROUP
//    UndoView(QUndoGroup *group, QWidget *parent);
//    #endif // QT_NO_UNDOGROUP
//    HistoryHandler* stack() const;
//    void setStack(HistoryHandler *stack);
//     void setGroup(QUndoGroup *group);
//     void setEmptyLabel(const QString &label);
//     QString emptyLabel() const;
//     void setCleanIcon(const QIcon &icon);
//     QIcon cleanIcon() const;

//};
//class UndoModel : public QAbstractItemModel
//{
//      Q_OBJECT
//  public:
//      UndoModel(QObject *parent = 0);
//        QUndoStack *stack() const;
//        virtual QModelIndex index(int row, int column,
//                  const QModelIndex &parent = QModelIndex()) const;
//      virtual QModelIndex parent(const QModelIndex &child) const;
//      virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
//      virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;
//      virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
//        QModelIndex selectedIndex() const;
//      QItemSelectionModel *selectionModel() const;
//        QString emptyLabel() const;
//      void setEmptyLabel(const QString &label);
//        void setCleanIcon(const QIcon &icon);
//      QIcon cleanIcon() const;
//    public slots:
//      void setStack(HistoryHandler *stack);
//    private slots:
//      void stackChanged();
//      void stackDestroyed(QObject *obj);
//      void setStackCurrentIndex(const QModelIndex &index);
//    private:
//      HistoryHandler *m_stack;
//      QItemSelectionModel *m_sel_model;
//      QString m_emty_label;
//      QIcon m_clean_icon;
//  };

//#endif // HISTORYMODEL_H
