#include "FollowButton.h"
#include "Utility.h"
FollowButton::FollowButton():QPushButton()
{
setText("PC");
}
void FollowButton::mouseDoubleClickEvent(QMouseEvent *event)
{
qDebug("FOUBL");
emit doubleClicked();
}
