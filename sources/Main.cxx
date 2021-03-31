#include <iostream>
#include <nana/gui.hpp>
#include "Editor.hxx"

int32_t main(int32_t argc, char **argv)
{
    Editor *ed = nullptr;

    if (argc == 2)
        ed = new Editor("TEd", argv[1]);
    else
        ed = new Editor("TEd");

    ed->show();
    nana::exec();

    delete ed;

    return 0;
}
