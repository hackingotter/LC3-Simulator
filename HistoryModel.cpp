//#include "HistoryModel.h"
//#include "QListView"


//UndoModel::UndoModel(QObject *parent)
//    : QAbstractItemModel(parent)
//{
//    m_stack = 0;
//    m_sel_model = new QItemSelectionModel(this, this);
//    connect(m_sel_model, SIGNAL(currentChanged(QModelIndex,QModelIndex)),
//            this, SLOT(setStackCurrentIndex(QModelIndex)));
//    m_emty_label = tr("<empty>");
//}
//QItemSelectionModel *UndoModel::selectionModel() const
//{
//    return m_sel_model;
//}
//QUndoStack *UndoModel::stack() const
//{
//    return m_stack;
//}
//void UndoModel::setStack(HistoryHandler *stack)
//{
//    if (m_stack == stack)
//        return;
//    if (m_stack != 0) {
//        disconnect(m_stack, SIGNAL(cleanChanged(bool)), this, SLOT(stackChanged()));
//        disconnect(m_stack, SIGNAL(indexChanged(int)), this, SLOT(stackChanged()));
//        disconnect(m_stack, SIGNAL(destroyed(QObject*)), this, SLOT(stackDestroyed(QObject*)));
//    }
//    m_stack = stack;
//    if (m_stack != 0) {
//        connect(m_stack, SIGNAL(cleanChanged(bool)), this, SLOT(stackChanged()));
//        connect(m_stack, SIGNAL(indexChanged(int)), this, SLOT(stackChanged()));
//        connect(m_stack, SIGNAL(destroyed(QObject*)), this, SLOT(stackDestroyed(QObject*)));
//    }
//    stackChanged();
//}
//void UndoModel::stackDestroyed(QObject *obj)
//{
//    if (obj != m_stack)
//        return;
//    m_stack = 0;
//    stackChanged();
//}
//void UndoModel::stackChanged()
//{
////    reset();
//    m_sel_model->setCurrentIndex(selectedIndex(), QItemSelectionModel::ClearAndSelect);
//}
//void UndoModel::setStackCurrentIndex(const QModelIndex &index)
//{
//    if (m_stack == 0)
//        return;
//    if (index == selectedIndex())
//        return;
//    if (index.column() != 0)
//        return;
//    m_stack->setIndex(index.row());
//}
//QModelIndex UndoModel::selectedIndex() const
//{
//    return m_stack == 0 ? QModelIndex() : createIndex(m_stack->index(), 0);
//}
//QModelIndex UndoModel::index(int row, int column, const QModelIndex &parent) const
//{
//    if (m_stack == 0)
//        return QModelIndex();
//    if (parent.isValid())
//        return QModelIndex();
//    if (column != 0)
//        return QModelIndex();
//    if (row < 0 || row > m_stack->count())
//        return QModelIndex();
//    return createIndex(row, column);
//}
//QModelIndex UndoModel::parent(const QModelIndex&) const
//{
//    return QModelIndex();
//}
//int UndoModel::rowCount(const QModelIndex &parent) const
//{
//    if (m_stack == 0)
//        return 0;
//    if (parent.isValid())
//        return 0;
//    return m_stack->count() + 1;
//}
//int UndoModel::columnCount(const QModelIndex&) const
//{
//    return 1;
//}
//QVariant UndoModel::data(const QModelIndex &index, int role) const
//{
//    if (m_stack == 0)
//        return QVariant();
//    if (index.column() != 0)
//        return QVariant();
//    if (index.row() < 0 || index.row() > m_stack->count())
//        return QVariant();
//    if (role == Qt::DisplayRole) {
//        if (index.row() == 0)
//            return m_emty_label;
//        return m_stack->text(index.row() - 1);
//    } else if (role == Qt::DecorationRole) {
//        if (index.row() == m_stack->cleanIndex() && !m_clean_icon.isNull())
//            return m_clean_icon;
//        return QVariant();
//    }
//    return QVariant();
//}
//QString UndoModel::emptyLabel() const
//{
//    return m_emty_label;
//}
//void UndoModel::setEmptyLabel(const QString &label)
//{
//    m_emty_label = label;
//    stackChanged();
//}
//void UndoModel::setCleanIcon(const QIcon &icon)
//{
//    m_clean_icon = icon;
//    stackChanged();
//}
//QIcon UndoModel::cleanIcon() const
//{
//    return m_clean_icon;
//}
//class UndoViewPrivate : public QListViewPrivate
// {
//     Q_DECLARE_PUBLIC(UndoView)
// public:
//     UndoViewPrivate() :
// #ifndef QT_NO_UNDOGROUP
//         group(0),
// #endif
//         model(0) {}

// #ifndef QT_NO_UNDOGROUP
//     QPointer<QUndoGroup> *group;
// #endif
//     UndoModel *model;

//     void init();
// };

// void UndoViewPrivate::init()
// {
//     Q_Q(UndoView);

//     model = new UndoModel(q);
//     q->setModel(model);
//     q->setSelectionModel(model->selectionModel());
// }

// UndoView::UndoView(QWidget *parent)
//     : QListView(*new UndoViewPrivate(), parent)
// {
//     Q_D(UndoView);
//     d->init();
// }

// UndoView::UndoView(QUndoStack *stack, QWidget *parent)
//     : QListView(*new UndoViewPrivate(), parent)
// {
//     Q_D(UndoView);
//     d->init();
//     setStack(stack);
// }

// #ifndef QT_NO_UNDOGROUP

// UndoView::UndoView(QUndoGroup *group, QWidget *parent)
//     : QListView(*new UndoViewPrivate(), parent)
// {
//     Q_D(UndoView);
//     d->init();
//     setGroup(group);
// }

// #endif // QT_NO_UNDOGROUP

// UndoView::~UndoView()
// {
// }

// HistoryHandler *UndoView::stack() const
// {
//     Q_D(const UndoView);
//     return d->model->stack();
// }

// void UndoView::setStack(HistoryHandler *stack)
// {
//     Q_D(UndoView);
// #ifndef QT_NO_UNDOGROUP
//     setGroup(0);
// #endif
//     d->model->setStack(stack);
// }

// #ifndef QT_NO_UNDOGROUP

// void UndoView::setGroup(QUndoGroup *group)
// {
//     Q_D(UndoView);

//     if (d->group == group)
//         return;

//     if (d->group != 0) {
//         disconnect(d->group, SIGNAL(activeStackChanged(HistoryHandler*)),
//                 d->model, SLOT(setStack(HistoryHandler*)));
//     }

//     d->group = group;

//     if (d->group != 0) {
//         connect(d->group, SIGNAL(activeStackChanged(HistoryHandler*)),
//                 d->model, SLOT(setStack(HistoryHandler*)));
//         d->model->setStack(d->group->activeStack());
//     } else {
//         d->model->setStack(0);
//     }
// }

// QUndoGroup *UndoView::group() const
// {
//     Q_D(const UndoView);
//     return d->group;
// }

// #endif // QT_NO_UNDOGROUP

// void UndoView::setEmptyLabel(const QString &label)
// {
//     Q_D(UndoView);
//     d->model->setEmptyLabel(label);
// }

// QString UndoView::emptyLabel() const
// {
//     Q_D(const UndoView);
//     return d->model->emptyLabel();
// }

// void UndoView::setCleanIcon(const QIcon &icon)
// {
//     Q_D(const UndoView);
//     d->model->setCleanIcon(icon);

// }

// QIcon UndoView::cleanIcon() const
// {
//     Q_D(const UndoView);
//     return d->model->cleanIcon();
// }

// QT_END_NAMESPACE

//// #include "qundoview.moc"

//// #endif // QT_NO_UNDOVIEW
