//#ifndef BORROWED_H
//#define BORROWED_H
//#include "qobject.h"
//#include "QModelIndex"
//#include "QAbstractItemModel"
//class QUndoModel : public QAbstractItemModel
//{
//      Q_OBJECT
//  public:
//      QUndoModel(QObject *parent = 0);
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
//      void setStack(QUndoStack *stack);
//    private slots:
//      void stackChanged();
//      void stackDestroyed(QObject *obj);
//      void setStackCurrentIndex(const QModelIndex &index);
//    private:
//      QUndoStack *m_stack;
//      QItemSelectionModel *m_sel_model;
//      QString m_emty_label;
//      QIcon m_clean_icon;
//  };
//    QUndoModel::QUndoModel(QObject *parent)
//      : QAbstractItemModel(parent)
//  {
//      m_stack = 0;
//      m_sel_model = new QItemSelectionModel(this, this);
//      connect(m_sel_model, SIGNAL(currentChanged(QModelIndex,QModelIndex)),
//             this, SLOT(setStackCurrentIndex(QModelIndex)));
//     m_emty_label = tr("<empty>");
// }
//  QItemSelectionModel *QUndoModel::selectionModel() const
// {
//     return m_sel_model;
// }
//  QUndoStack *QUndoModel::stack() const
// {
//     return m_stack;
// }
//  void QUndoModel::setStack(QUndoStack *stack)
// {
//     if (m_stack == stack)
//         return;
//      if (m_stack != 0) {
//         disconnect(m_stack, SIGNAL(cleanChanged(bool)), this, SLOT(stackChanged()));
//         disconnect(m_stack, SIGNAL(indexChanged(int)), this, SLOT(stackChanged()));
//         disconnect(m_stack, SIGNAL(destroyed(QObject*)), this, SLOT(stackDestroyed(QObject*)));
//     }
//     m_stack = stack;
//     if (m_stack != 0) {
//         connect(m_stack, SIGNAL(cleanChanged(bool)), this, SLOT(stackChanged()));
//         connect(m_stack, SIGNAL(indexChanged(int)), this, SLOT(stackChanged()));
//         connect(m_stack, SIGNAL(destroyed(QObject*)), this, SLOT(stackDestroyed(QObject*)));
//     }
//      stackChanged();
// }
//  void QUndoModel::stackDestroyed(QObject *obj)
// {
//     if (obj != m_stack)
//         return;
//     m_stack = 0;
//      stackChanged();
// }
//  void QUndoModel::stackChanged()
// {
//     reset();
//     m_sel_model->setCurrentIndex(selectedIndex(), QItemSelectionModel::ClearAndSelect);
// }
//  void QUndoModel::setStackCurrentIndex(const QModelIndex &index)
// {
//     if (m_stack == 0)
//         return;
//      if (index == selectedIndex())
//         return;
//      if (index.column() != 0)
//         return;
//      m_stack->setIndex(index.row());
// }
//  QModelIndex QUndoModel::selectedIndex() const
// {
//     return m_stack == 0 ? QModelIndex() : createIndex(m_stack->index(), 0);
// }
//  QModelIndex QUndoModel::index(int row, int column, const QModelIndex &parent) const
// {
//     if (m_stack == 0)
//         return QModelIndex();
//      if (parent.isValid())
//         return QModelIndex();
//      if (column != 0)
//         return QModelIndex();
//      if (row < 0 || row > m_stack->count())
//         return QModelIndex();
//      return createIndex(row, column);
// }
//  QModelIndex QUndoModel::parent(const QModelIndex&) const
// {
//     return QModelIndex();
// }
//  int QUndoModel::rowCount(const QModelIndex &parent) const
// {
//     if (m_stack == 0)
//         return 0;
//      if (parent.isValid())
//         return 0;
//      return m_stack->count() + 1;
// }
//  int QUndoModel::columnCount(const QModelIndex&) const
// {
//     return 1;
// }
//  QVariant QUndoModel::data(const QModelIndex &index, int role) const
// {
//     if (m_stack == 0)
//         return QVariant();
//      if (index.column() != 0)
//         return QVariant();
//      if (index.row() < 0 || index.row() > m_stack->count())
//         return QVariant();
//      if (role == Qt::DisplayRole) {
//         if (index.row() == 0)
//             return m_emty_label;
//         return m_stack->text(index.row() - 1);
//     } else if (role == Qt::DecorationRole) {
//         if (index.row() == m_stack->cleanIndex() && !m_clean_icon.isNull())
//             return m_clean_icon;
//         return QVariant();
//     }
//      return QVariant();
// }
//  QString QUndoModel::emptyLabel() const
// {
//     return m_emty_label;
// }
//  void QUndoModel::setEmptyLabel(const QString &label)
// {
//     m_emty_label = label;
//     stackChanged();
// }
//  void QUndoModel::setCleanIcon(const QIcon &icon)
// {
//     m_clean_icon = icon;
//     stackChanged();
// }
//  QIcon QUndoModel::cleanIcon() const
// {
//     return m_clean_icon;
// }
//#endif // BORROWED_H
