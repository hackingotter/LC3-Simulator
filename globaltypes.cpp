#include "GlobalTypes.h"
bool operator ==(mem_loc_t a, mem_loc_t b)
{
    return ((a.addr ==b.addr)&&(a.value==b.value)&&(a.comment == b.comment));
}
bool operator !=(mem_loc_t a, mem_loc_t b)
{
    return !(a==b);
}
