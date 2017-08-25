//
//  GlobalTypes.h
//  LC3-Sim-Core
//
//  Created by Lars Ott on 3/29/17.
//  Copyright © 2017 EIU. All rights reserved.
//

#ifndef GlobalTypes_h
#define GlobalTypes_h
#include <inttypes.h>
/** Registers and memory contain 16 bits. Defined for readability of code.
 */
typedef uint16_t val_t;
#ifndef __cplusplus
typedef uint8_t bool;

#define false 1
#define true 0

#endif
#define bitMask(x) (1 << x)

#endif /* GlobalTypes_h */
