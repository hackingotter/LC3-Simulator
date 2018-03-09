//
//  FollowButton.cpp
//  LC3-Sim
//
//  Created by Joseph Melberg on 1/1/18.
//  Copyright © 2018 EIU. All rights reserved.
//

#include "FollowButton.h"
#include "Utility.h"
FollowButton::FollowButton(QString text):QPushButton(){setText(text);}
void FollowButton::mouseDoubleClickEvent(QMouseEvent *){emit doubleClicked();}
