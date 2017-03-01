#ifndef TRANSPORT_H_INCLUDED
#define TRANSPORT_H_INCLUDED

#include <string.h>

struct Transport
{
    char type[];
    unsigned char id;
    Transport();
    Transport(char newType[], unsigned char newId);
};
Transport::Transport(){};
Transport::Transport(char newType[], unsigned char newId)
{
    memcpy(type, newType, sizeof type);
    id = newId;
}
#endif // TRANSPORT_H_INCLUDED
