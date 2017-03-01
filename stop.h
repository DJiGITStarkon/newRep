#ifndef STOP_H_INCLUDED
#define STOP_H_INCLUDED

#include <string.h>

struct Stop
{
    char name[];
    unsigned char id;
    Stop();
    Stop(char newName[], unsigned char newId);
};
Stop::Stop(){};
Stop::Stop(char newName[], unsigned char newId)
{
    memcpy(name, newName, sizeof(name));
    id = newId;
}
#endif // STOP_H_INCLUDED
