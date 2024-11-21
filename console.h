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
        this->oldin =  GetConsoleCP();
        this->oldout = GetConsoleOutputCP();

        SetConsoleCP(cp);
        SetConsoleOutputCP(cp);

        this->hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);
        GetConsoleScreenBufferInfo(this->hConsoleOutput, &this->csb);
    }

    ~console()
    {
        SetConsoleTextAttribute(this->hConsoleOutput, this->csb.wAttributes);

        SetConsoleCP(this->oldin);
        SetConsoleOutputCP(this->oldout);
    }

    HANDLE output()
    {
        return this->hConsoleOutput;
    }
};

#endif /* CONSOLE_H */
