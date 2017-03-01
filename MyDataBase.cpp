#include "MyDataBase.h"
#include <iostream>
#include <stdlib.h>
#include <algorithm>

using namespace std;

static const char* getName (unsigned short val, const char& type){

    char *s = new char[3];
    s[0] = type;
    s[1] = val/10 + '0';
    s[2] = val%10 + '0';
    return s;
}
ifstream::pos_type Main::findType(unsigned short& val, const char& type){

    switch(type)
    {
    case 'S':
        fin.open("stops.bin",ios_base::in | ios_base::out | ios_base::binary);
        break;
    default:
        fin.open("transport.bin",ios_base::in | ios_base::out | ios_base::binary);
        break;
    }
    while(!fin.eof())
    {
        char data[3];
        fin.read(data,sizeof(data));
        if(!fin.eof())
        {
            if(data[0]==type && ((data[1]-'0')*10+(data[2]-'0')) == val){
                std::ifstream::pos_type ps = fin.tellg();
                fin.close();
                return ps;
                }
        }
    }
    fin.close();
    return -1;
}
ifstream::pos_type Main::findConnection(unsigned short& transVal, const char &transType, unsigned short& stopVal){

    fin2.open("connection.bin",ios_base::in | ios_base::out | ios_base::binary);
    while(!fin2.eof())
    {
        char transData[3], stopsData[3];
        fin2.read(transData,sizeof(transData));
        fin2.read(stopsData,sizeof(stopsData));
        if(!fin2.eof())
        {
            if(transData[0]==transType && ((transData[1]-'0')*10+(transData[2]-'0')) == transVal
               && ((stopsData[1]-'0')*10+(stopsData[2]-'0')) == stopVal ){
                ifstream::pos_type ps = fin2.tellg();
                fin2.close();
                return ps;
                }
        }
    }
    fin2.close();
    return -1;
}
bool Main::removeWithData(unsigned short& val, const char& type){
    std::ifstream::pos_type pos = findType(val,type);
    if(pos==-1)
        return false;
    switch(type)
    {
    case 'S':
       fin.open("stops.bin",ios_base::in | ios_base::out | ios_base::binary);
       break;
    default:
        fin.open("transport.bin",ios_base::in | ios_base::out | ios_base::binary);
        break;
    }
    fout.open("temp.bin",ios_base::out  | ios_base :: binary);
    while(!fin.eof())
    {
        char data[3];
        fin.read(data,sizeof(data));
        if(!fin.eof())
        {
            if(fin.tellg() != pos)
                fout.write(data,sizeof(data));
        }
    }
    fout.close();
    fin.close();
    switch(type)
    {
        case 'S':
        {
            remove("stops.bin");
            rename("temp.bin","stops.bin");
            break;
        }
        default:
        {
            remove("transport.bin");
            rename("temp.bin","transport.bin");
            break;
        }
    }
    return true;
}
void Main::updTransNum (){
    /*fout.open("transport.bin",ios_base::out | ios_base::binary);
    fout.close();*/
    fin.open("transport.bin", ios_base::in | ios_base::binary);
    if(fin.tellg() == -1)
    {
        fin.close();
        fout.open("transport.bin",ios_base::out | ios_base::binary);
        fout.close();
        fin.open("transport.bin", ios_base::in | ios_base::binary);
    }
    while(!fin.eof())
    {
        char data[3];
        fin.read(data,sizeof(data));
        if(!fin.eof())
        {
            switch(data[0])
            {
            case 'B':
                busNumber = (data[1]-'0') * 10 + (data[2]-'0');
                break;
            case 'O':
                trolleybusNumber = (data[1]-'0') * 10 + (data[2]-'0');
                break;
            case 'A':
                tramNumber = (data[1]-'0') * 10 + (data[2]-'0');
                break;
            default:
                cout<<"Error : Bad transport data in transport.bin\n";
                break;
            }
        }
    }
    fin.close();
}
void Main::updStopsNum()
{
    fin.open("stops.bin", ios_base::in | ios_base::binary);
    if(fin.tellg() == -1)
    {
        fin.close();
        fout.open("stops.bin",ios_base::out | ios_base::binary);
        fout.close();
        fin.open("stops.bin", ios_base::in | ios_base::binary);
    }
    while(!fin.eof())
    {
        char data[3];
        fin.read(data,sizeof(data));
        if(!fin.eof())
        {
            switch(data[0])
            {
            case 'S':
                stopsNumber = (data[1]-'0') * 10 + (data[2]-'0');
                break;
            default:
                cout<<"Error : Bad stops data in stops.bin\n";
                break;
            }
        }
    }
    fin.close();
}
void Main::load(){
    updTransNum();
    updStopsNum();
    fin.open("connection.bin", ios_base::in | ios_base::binary);
    if(fin.tellg() == -1)
    {
        fin.close();
        fout.open("connection.bin",ios_base::out | ios_base::binary);
        fout.close();
    }
    else
    {
        fin.close();
    }
}
void Main::menu(){
    cout<<"--------------����--------------\n";
    cout<<"1) ��������/������� ���������\n";
    cout<<"2) ��������/������� ���������\n";
    cout<<"3) �������� ���������\n";
    cout<<"4) �������� ���������\n";
    cout<<"5) ������� ��������� � ���������\n";
    cout<<"6) ����������� ��������� � ���������\n";
    cout<<"7) �������� �����\n";
    cout<<"8) ��� �������?\n";
    cout<<"0) �����\n";

    char choice;
    cin>>choice;

    system("CLS");
    switch(choice){
    case '1':
        add_removeTrans();
        break;
    case '2':
        add_removeStops();
        break;
    case '3':
        showTrans();
        break;
    case '4':
        showStops();
        break;
    case '5':
        connect();
        break;
    case '6':
        unconnect();
        break;
    case '7':
        showConnection();
        break;
    case '8':
        howToReach();
    default:
        Exit();
        break;
    }
}

void Main::add_removeTrans(){
    cout<<"--------------(��������/������� ���������)--------------\n";
    cout<<"1) ��������\n";
    cout<<"2) �������\n";
    cout<<"9) ����\n";
    cout<<"0) �����\n";

    char choice;
    cin>>choice;

    system("CLS");
    switch(choice)
    {
    case '1':
        addTrans();
        break;
    case '2':
        removeTrans();
        break;
    case '9':
        menu();
        break;
    default:
        Exit();
        break;
    }
}
void Main::add_removeStops(){
    cout<<"--------------(��������/������� ���������)--------------\n";
    cout<<"1) ��������\n";
    cout<<"2) �������\n";
    cout<<"9) ����\n";
    cout<<"0) �����\n";

    char choice;
    cin>>choice;

    system("cls");
    switch(choice)
    {
    case '1':
        addStops();
        break;
    case '2':
        removeStops();
        break;
    case '9':
        menu();
        break;
    default:
        Exit();
        break;
    }
}
void Main::showTrans(){
    fin.open("transport.bin", ios_base::in | ios_base::binary);
    if(!fin.is_open())
        cout<<"������ ����\n";
    else
    {
        fin.seekg(0,ios_base::end);
        if(!fin.tellg())
            cout<<"������ ����\n";
        else
        {
            fin.seekg(0,ios_base::beg);
            char data[3];
            while(!fin.eof())
            {
                fin.read(data,sizeof(data));
                if(!fin.eof()){
                    for(int i=0;i<3;i++)
                        cout<<data[i];
                    cout<<'\n';
                }
            }
        }
    }
    fin.close();
    cout<<"\n�������� � ���� ������� 9, ����� - 0...\n";
    char choice;
    cin>>choice;
    system("CLS");
    switch(choice)
    {
    case '9':
        menu();
    default:
        Exit();
    }
}
void Main::showStops(){
    fin.open("stops.bin", ios_base::in | ios_base::binary);
    if(!fin.is_open())
        cout<<"������ ����\n";
    else
    {
        fin.seekg(0,ios_base::end);
        if(!fin.tellg())
            cout<<"������ ����\n";
        else
        {
            fin.seekg(0,ios_base::beg);
            char data[3];
            while(!fin.eof())
            {
                fin.read(data,sizeof(data));
                if(!fin.eof()){
                    for(int i=0;i<3;i++)
                        cout<<data[i];
                    cout<<'\n';
                }
            }
        }
    }
    fin.close();
    cout<<"\n�������� � ���� ������� 9, ����� - 0...\n";
    char choice;
    cin>>choice;
    system("CLS");
    switch(choice)
    {
    case '9':
        menu();
    default:
        Exit();
    }
}

void Main::unconnectWithData(const unsigned short& transVal, const char &type, const unsigned short& stopVal)
{
    fin.open("connection.bin",ios_base::in | ios_base::binary);
    fout.open("newConnection.bin",ios_base::out | ios_base::binary);
    while(!fin.eof())
    {
        char data[6];
        fin.read(data,sizeof(data));
        if(!fin.eof())
        {
            if((transVal == 0 || (type == data[0] && (char)(transVal/10+48) == data[1] && (char)(transVal%10+48) == data[2]) )
               && (stopVal == 0 || ('S' == data[3] && (char)(stopVal/10+48) == data[4] && (char)(stopVal%10+48) == data[5]) )
               )
            {
                fout.write(data,sizeof(data));
            }
        }
    }
    fout.close();
    fin.close();
    remove("connection.bin");
    rename("newConnection.bin","connection.bin");
}
void Main::addTrans(){

    cout<<"--------------(�������� ���������)--------------\n";
    cout<<"�������� ��� ����������: \n";
    cout<<"1) �������\n";
    cout<<"2) ���������\n";
    cout<<"3) �������\n";
    cout<<"9) ����\n";
    cout<<"0) �����\n";

    char choice;
    cin>>choice;

    system("cls");
    switch(choice)
    {
    case '1':
    {
        cout<<"������� ��������.\n";
        fout.open("transport.bin", ios_base::out | ios_base::in | ios_base::ate | ios_base::binary);
        const char *data = getName(++busNumber,'B');
        fout.write(data,3);
        fout.close();
        system("pause");
        system("cls");
        menu();
        break;
    }
    case '2':
    {
        cout<<"��������� ��������.\n";
        fout.open("transport.bin", ios_base::out | ios_base::in | ios_base::ate | ios_base::binary);
        const char *data = getName(++trolleybusNumber,'O');
        fout.write(data,3);
        fout.close();
        system("pause");
        system("cls");
        menu();
        break;
    }
    case '3':
    {
        cout<<"������� ��������.\n";
        fout.open("transport.bin", ios_base::out | ios_base::in | ios_base::ate | ios_base::binary);
        const char *data = getName(++tramNumber,'A');
        fout.write(data,3);
        fout.close();
        system("pause");
        system("cls");
        menu();
        break;
    }
    case '9':
        menu();
        break;
    default:
        Exit();
        break;
    }

}
void Main::removeTrans(){
    cout<<"--------------(������� ���������)--------------\n";
    cout<<"�������� ��� ����������: \n";
    cout<<"1) �������\n";
    cout<<"2) ���������\n";
    cout<<"3) �������\n";
    cout<<"9) ����\n";
    cout<<"0) �����\n";

    char choice;
    cin>>choice;

    system("CLS");
    switch(choice)
    {
    case '1':
    {
        unsigned short val;
        cout<<"������� ����� �������� : ";
        cin>>val;
        if(removeWithData(val,'B'))
        {
            unconnectWithData(val,'B',0);
            cout<<"�������"<<val<<" ������� ������!\n";
        }
        else
        {
            cout<<"�������"<<val<<" �� ������.\n";
        }
        system("pause");
        system("cls");
        menu();
        break;
    }
    case '2':
    {
        unsigned short val;
        cout<<"������� ����� ���������� : ";
        cin>>val;
        if(removeWithData(val,'O'))
        {
            unconnectWithData(val,'O',0);
            cout<<"���������"<<val<<" ������� ������!\n";
        }
        else
        {
            cout<<"���������"<<val<<" �� ������.\n";
        }
        system("pause");
        system("cls");
        menu();
        break;
    }
    case '3':
    {
        unsigned short val;
        cout<<"������� ����� ������� : ";
        cin>>val;
        if(removeWithData(val,'A'))
        {
            unconnectWithData(val,'A',0);
            cout<<"�������"<<val<<" ������� ������!\n";
        }
        else
        {
            cout<<"�������"<<val<<" �� ������.\n";
        }
        system("pause");
        system("cls");
        menu();
        break;
    }
    case '9':
        menu();
        break;
    default:
        Exit();
        break;
    }
}
void Main::addStops(){

    system("CLS");
    cout<<"��������� ���������.\n";
    fout.open("stops.bin", ios_base::out | ios_base::in | ios_base::ate | ios_base::binary);
    const char *data = getName(++stopsNumber,'S');
    fout.write(data,3);
    fout.close();
    system("pause");
    system("cls");
    menu();
}
void Main::removeStops(){
    system("cls");
    unsigned short val;
    cout<<"������� ����� ��������� : ";
    cin>>val;
    if(removeWithData(val,'S'))
    {
        unconnectWithData(0,'A',val);
        cout<<"���������"<<val<<" ������� �������!\n";
    }
    else
    {
        cout<<"���������"<<val<<" �� �������.\n";
    }
    system("pause");
    system("cls");
    menu();
}

void Main::connect(){
    cout<<"--------------(�������)--------------\n";
    cout<<"�������� ��������� :\n";
    cout<<"1) �������\n";
    cout<<"2) ���������\n";
    cout<<"3) �������\n";
    cout<<"9) ����\n";
    cout<<"0) �����\n";

    char transChoiceId;
    cin>>transChoiceId;
    system("cls");
    if(transChoiceId!='1' && transChoiceId!='2' && transChoiceId!='3' && transChoiceId!='9')
        Exit();
    if(transChoiceId=='9')
    {
        menu();
        return;
    }
    cout<<"--------------(�������)--------------\n";
    cout<<"������� ����� "<<(transChoiceId == '1' ? "��������": (transChoiceId == '2' ? "����������" : "�������"))<<" :\n";
    unsigned short transChoiceNum;
    cin>>transChoiceNum;

    system("cls");
    cout<<"--------------(�������)--------------\n";
    cout<<"������� ����� ��������� :\n";
    unsigned short stopChoiceNum;
    cin>>stopChoiceNum;

    system("cls");
    if(findType(transChoiceNum,transChoiceId == '1' ? 'B' : (transChoiceId == '2' ? 'O' : 'A')) == -1 ||
       findType(stopChoiceNum,'S') == -1 )
    {
        cout<<"��������� ��� �������� �� �������!\n";
        system("pause");
        system("cls");
        menu();
    }
    else
    {
        if(findConnection(transChoiceNum,(transChoiceId == '1' ? 'B': (transChoiceId == '2' ? 'O' : 'A')),stopChoiceNum) != -1)
        {
            cout<<"���������� ����������!\n";
            system("pause");
            system("cls");
            menu();
        }
        else
        {
            char transData[] = {(transChoiceId == '1' ? 'B': (transChoiceId == '2' ? 'O' : 'A')),(char)(transChoiceNum/10 + 48),
                              (char)(transChoiceNum % 10 + 48)};
            char stopData[] = {'S', (char)(stopChoiceNum / 10 + 48), (char)(stopChoiceNum % 10 + 48)};

            fout.open("connection.bin",ios_base::out | ios_base::in | ios_base::ate | ios_base::binary);
            fout.write(transData,sizeof(transData));
            fout.write(stopData,sizeof(stopData));
            fout.close();
            cout<<"���������� �������!\n";
            system("pause");
            system("cls");
            menu();
        }
    }
}
void Main::unconnect(){
    cout<<"--------------(�����������)--------------\n";
    cout<<"�������� ��������� :\n";
    cout<<"1) �������\n";
    cout<<"2) ���������\n";
    cout<<"3) �������\n";
    cout<<"9) ����\n";
    cout<<"0) �����\n";

    char transChoiceId;
    cin>>transChoiceId;
    system("cls");
    if(transChoiceId!='1' && transChoiceId!='2' && transChoiceId!='3' && transChoiceId!='9')
        Exit();
    if(transChoiceId=='9')
    {
        menu();
        return;
    }
    cout<<"--------------(�����������)--------------\n";
    cout<<"������� ����� "<<(transChoiceId == '1' ? "��������": (transChoiceId == '2' ? "����������" : "�������"))<<" :\n";
    unsigned short transChoiceNum;
    cin>>transChoiceNum;

    system("cls");
    cout<<"--------------(�����������)--------------\n";
    cout<<"������� ����� ��������� :\n";
    unsigned short stopChoiceNum;
    cin>>stopChoiceNum;

    system("cls");
    ifstream::pos_type ps = findConnection(transChoiceNum,transChoiceId == '1' ? 'B' : (transChoiceId == '2' ? 'O' : 'A'),stopChoiceNum);
    if(ps == -1)
    {
        cout<<"����� �� �������!\n";
    }
    else
    {
        fin.open("connection.bin",ios_base::in | ios_base::binary);
        fout.open("newConnection.bin",ios_base::out | ios_base::binary);
        while(!fin.eof())
        {
            char data[6];
            fin.read(data,sizeof(data));
            if(!fin.eof())
            {
                if(fin.tellg()!=ps)
                {
                    fout.write(data,sizeof(data));
                }
            }
        }
        fout.close();
        fin.close();
        remove("connection.bin");
        rename("newConnection.bin","connection.bin");
        cout<<"������������ �������!\n";
    }
    system("pause");
    system("cls");
    menu();
}
void Main::showConnection(){
    fin.open("connection.bin",ios_base::in | ios_base::out | ios_base::binary);
    fin.seekg(0,ios_base::end);
    if(!fin.tellg())
        cout<<"��� ��������� ������!\n";
    else
    {
        fin.seekg(0,ios_base::beg);
        while(!fin.eof())
        {
            char data[6];
            fin.read(data,sizeof(data));
            if(!fin.eof())
            {
                for(int i=0;i<3;i++)
                    cout<<data[i];
                cout<<'\t';
                for(int i=3;i<6;i++)
                    cout<<data[i];
                cout<<'\n';
            }
        }
    }
    fin.close();
    system("pause");
    system("cls");
    menu();
}
void Main::howToReach(){
    cout<<"--------------(��� �������?)--------------\n";
    cout<<"�����������. ������� ����� ��������� : ";
    unsigned short stop1;
    cin>>stop1;
    system("cls");
    cout<<"--------------(��� �������?)--------------\n";
    cout<<"��������. ������� ����� ��������� : ";
    unsigned short stop2;
    cin>>stop2;
    system("cls");
    fin.open("transport.bin",ios_base::in | ios_base::out | ios_base::binary);
    bool f = false;
    while(!fin.eof())
    {
        char data[3];
        fin.read(data,sizeof(data));
        if(!fin.eof())
        {
            unsigned short transNum = ( (data[1]-'0')*10 + (data[2]-'0'));
            if(findConnection(transNum, data[0],stop1) != -1 &&
               findConnection(transNum, data[0],stop2) != -1
               )
            {
                f = true;
                switch(data[0])
                {
                case 'B':
                    cout<<"������� �"<<data[1]<<data[2]<<'\n';
                    break;
                case 'O':
                    cout<<"��������� �"<<data[1]<<data[2]<<'\n';
                    break;
                case 'A':
                    cout<<"������� �"<<data[1]<<data[2]<<'\n';
                    break;
                }
            }
        }
    }
    fin.close();
    if(!f)
        cout<<"�������� �� �������!\n";
    system("pause");
    system("cls");
    menu();
}

void Main::run(){
    setlocale(LC_ALL,"russian");
    load();
    menu();
}
void Main::Exit(){
    cout<<"�� ��������\n";
    exit(0);
}

