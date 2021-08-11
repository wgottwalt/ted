#include <iostream>
#include <nana/gui.hpp>
#include "Editor.hxx"

int32_t main(int32_t argc, char **argv) noexcept
{
    Editor *ed = nullptr;

    try
    {
        if (argc == 2)
            ed = new Editor("TEd", argv[1]);
        else
            ed = new Editor("TEd");

        ed->show();
        nana::exec();

        delete ed;
    }
    catch (std::exception &e)
    {
        std::cerr << "error: " << e.what() << std::endl;

        if (ed)
            delete ed;

        return 1;
    }

    return 0;
}
