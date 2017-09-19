//
//  Util.h
//  LC3-Sim-Core
//
//  Created by Lars Ott on 3/29/17.
//  Copyright © 2017 EIU. All rights reserved.
//

#ifndef Util_h
#define Util_h

#include "GlobalTypes.h"
#include <QString>

// TODO write stuff
QString getHexString(uint16_t val);

QString getBinString(uint16_t val);

/**
 * Flips the bytes in a 16-bit value
 * \brief flipBytes
 * \param value
 */
void flipBytes(uint16_t *value);


#endif /* Util_h */
