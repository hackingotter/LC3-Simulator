#include "GlobalTypes.h"
#include "qdebug.h"


bool operator ==(mem_loc_t a, mem_loc_t b)
{
    return ((a.addr ==b.addr)&&(a.value==b.value)&&(a.comment == b.comment));
}
bool operator !=(mem_loc_t a, mem_loc_t b)
{
    return !(a==b);
}

//void operator +=(mem_loc_t *a, connector_t *b)
//{

//}

bool operator -=(mem_loc_t a, mem_loc_t b)
{

}
void operator +=(mem_loc_t* loc, connector_t b)
{
    connector_t** cur = &(loc->connectors);
    if((*cur) == nullptr)
    {
        loc->connectors = new connector_t(b.connected);
        return;
    }
    else
    {
        if((*cur)->connected == b.connected)
        {
            return;
        }
        while((*cur)->next != nullptr && (*cur)->connected != b.connected)
        {
            cur = &((*cur)->next);
        }
        if((*cur)->next != nullptr)
        {

            return;
        }
        else
        {
            (*cur)->next = new connector_t(b.connected);
            return;
        }
    }
}

void operator -=(mem_loc_t* loc, connector_t b)
{
    //go along until your next one is the one you are looking for
    connector_t** cur = &(loc->connectors);
    connector_t** past = cur;

    //test if there is not anything attached.
    if((*cur) == nullptr)
    {
        return;
    }
    //if there is
    else
    {
        //is it what is to be removed?
        while((*cur)->connected != b.connected && (*cur)->next != nullptr)
        {
            past = cur;
            cur = &((*cur)->next);
        }
        //case 1, we found it
        if((*cur)->connected == b.connected)
        {
            //case 1a, it was the first thing
//            if((*past)->connected == (*cur)->connected)
//            {
                (*past) = (*cur)->next;
                return;
//            }
            //case 1b,
            //is there anything after it?
            if((*cur)->next != nullptr)
            {
                (**past).next = (*cur)->next;
            }
            else
            {
                if((*past)->connected == (*cur)->connected)
                {

                }
                else
                {
                (**past).next = nullptr;
                }
            }
        }
        else
        {
            return;//it wasn't there.
        }

    }
}
