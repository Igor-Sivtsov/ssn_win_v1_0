#ifndef CONSOLE_H
#define CONSOLE_H

#include <windows.h>

class console
{
    unsigned int                oldin;
    unsigned int                oldout;

    CONSOLE_SCREEN_BUFFER_INFO  csb;
    HANDLE                      hConsoleOutput;

public:
    console(int cp)
    {
        oldin =  GetConsoleCP();
        oldout = GetConsoleOutputCP();

        SetConsoleCP(cp);
        SetConsoleOutputCP(cp);

        hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);
        GetConsoleScreenBufferInfo(hConsoleOutput, &csb);
    }

    ~console()
    {
        SetConsoleTextAttribute(hConsoleOutput, csb.wAttributes);

        SetConsoleCP(oldin);
        SetConsoleOutputCP(oldout);
    }

    HANDLE output()
    {
        return hConsoleOutput;
    }
};

#endif /* CONSOLE_H */
