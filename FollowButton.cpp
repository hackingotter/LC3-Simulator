#include "FollowButton.h"
#include "Utility.h"
FollowButton::FollowButton(QString text):QPushButton(){setText(text);}
void FollowButton::mouseDoubleClickEvent(QMouseEvent *event){emit doubleClicked();}
