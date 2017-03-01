#ifndef MYDATABASE_H_INCLUDED
#define MYDATABASE_H_INCLUDED

#include "transport.h"
#include "stop.h"
#include <fstream>

class Main
{
    public:
    unsigned short busNumber;
    unsigned short trolleybusNumber;
    unsigned short tramNumber;
    unsigned short stopsNumber;

    std::ifstream fin;
    std::ofstream fout;
    std::ifstream fin2;
    std::ofstream fout2;

    public:
        Main():busNumber(0),trolleybusNumber(0),tramNumber(0),stopsNumber(0) {}
    public:
    void load();

    void updTransNum();
    void updStopsNum();

    void menu();

    void add_removeTrans();
    void add_removeStops();
    void showTrans();
    void showStops();

    std::ifstream::pos_type findType(unsigned short& val, const char& type);
    std::ifstream::pos_type findConnection(unsigned short& transVal, const char &type, unsigned short& stopVal);
    bool removeWithData(unsigned short& val, const char& type);
    void unconnectWithData(const unsigned short& transVal, const char &type, const unsigned short& stopVal);
    void addTrans();
    void removeTrans();
    void addStops();
    void removeStops();
    void connect();
    void unconnect();
    void showConnection();
    void howToReach();


    void run();
    void Exit();
};

#endif // MYDATABASE_H_INCLUDED
