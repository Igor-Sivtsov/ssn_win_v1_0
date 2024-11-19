#include "console.h"

#include <conio.h>
#include <cstring>
#include <iostream>
#include <fstream>
#include <regex>

#define SN_LEN      29

char                sn[SN_LEN] =    {0,};
const unsigned int  sn_addr    =    0x1fc00;

const std::string   file_name  =    "Converter_USB-RS485_v1_m1.bin";

const std::regex    pattern(R"(^\d{2}-\d{3}-\d{3}-\d{2}-\d{3})");

enum class text_color
{
    blue    = 1,
    green   = 2,
    red     = 4,
    white   = 15
};

int main()
{
    console cons(CP_UTF8);

    std::fstream fw_file(file_name, std::ios::in | std::ios::out | std::ios::binary);

    const auto color_print = [&](const std::string& text, text_color color) -> void
    {
        SetConsoleTextAttribute(cons.output(), static_cast<unsigned short>(color));
        std::cout << text;
        SetConsoleTextAttribute(cons.output(), static_cast<unsigned short>(text_color::white));
    };

    const auto exit_fun = [&]() -> void
    {
        fw_file.close();

        color_print("\n" "Для завершения нажмите любую клавишу... ", text_color::white);
        char ch = getch();
    };

    const auto is_match = [&](const std::string& str) -> bool
    {
        if(not std::regex_match(str, pattern))
        {
            color_print("Неверный формат серийного номера, попробуйте еще раз.\n", text_color::red);
            return false;
        }
        return true;
    };

    if(not fw_file)
    {
        color_print("Ошибка открытия файла ", text_color::red);
        color_print("\"" + file_name + "\"" + "\n", text_color::white);

        exit_fun();
        return EXIT_FAILURE;
    }

    const auto read_sn = [&]() -> std::string
    {
        try
        {
            fw_file.seekp(sn_addr, std::ios::beg);
            fw_file.read(sn, SN_LEN);

            std::string str(sn);
            return str;
        }
        catch(...)
        {
            return "";
        }
    };

    std::string sn_str = read_sn();

    if(sn_str.find("FORWARD_DYN_") != std::string::npos)
        color_print("Эта программа предназначена для установки серийного \n"
                    "номера в файл прошивки преобразователя USB-RS485." "\n", text_color::blue);
    else
    {
        color_print("Серийный номер не найден в файле прошивки." "\n", text_color::red);

        exit_fun();
        return EXIT_FAILURE;
    }

    std::string number;

    do
    {
        color_print("\n" "Сейчас серийный номер:        ", text_color::green);
        color_print(sn_str + "\n", text_color::white);

        color_print("Введите новый серийный номер: ", text_color::green);
        color_print("FORWARD_DYN_", text_color::white);
        std::cin >> number;
    }
    while(not is_match(number));

    sn_str.clear();
    sn_str = "FORWARD_DYN_" + number;
    std::strncpy(sn, sn_str.c_str(), SN_LEN);

    try
    {
        fw_file.seekp(sn_addr, std::ios::beg);
        fw_file.write(sn, SN_LEN);

        if(read_sn() == sn_str)
            color_print("\n" "Новый серийный номер успешно сохранен.", text_color::blue);
    }
    catch (...)
    {
        color_print("\n" "При записи нового серийного номера произошла ошибка.", text_color::red);

        exit_fun();
        return EXIT_FAILURE;
    }

    exit_fun();

    return EXIT_SUCCESS;
}
