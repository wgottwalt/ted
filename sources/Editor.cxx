#include <nana/gui/filebox.hpp>
#include "Editor.hxx"

//--- public constructors ---

Editor::Editor(const std::string &name, const std::string &filename)
: form(), _widget(*this), _menu(*this), _text(*this), _filename(filename)
{
    caption(name);
    setupMenu();
    setupEditor();
    setupUi();

    events().unload([this](const nana::arg_unload &arg)
    {
        if (!isSaveNeeded(_filename))
            arg.cancel = true;
    });
}

Editor::~Editor() noexcept
{
}

//--- public methods ---

bool Editor::save(const std::string &filename)
{
}

bool Editor::load(const std::string &filename)
{
}

//--- protected methods ---

void Editor::setupMenu()
{
    _menu.push_back("&File");
    _menu.at(0).append("New", [this](nana::menu::item_proxy &)
    {
        if (isSaveNeeded(_filename))
            _text.reset();
    });
    _menu.at(0).append_splitter();
    _menu.at(0).append("Open", [this](nana::menu::item_proxy &)
    {
        if (isSaveNeeded(_filename))
        {
            auto ifile = fileDialog(true);

            if (ifile.size())
                _text.load(ifile.data());
        }
    });
    _menu.at(0).append("Save", [this](nana::menu::item_proxy &)
    {
        auto ofile = _text.filename();

        if (ofile.empty())
        {
            ofile = fileDialog(false);
            if (ofile.empty())
                return;
        }
        _text.store(ofile);
    });
    _menu.at(0).append("Save as...");
    _menu.at(0).append_splitter();
    _menu.at(0).append("Quit", [this](nana::menu::item_proxy &)
    {
        nana::API::exit();
    });

    _menu.push_back("&Edit");
    _menu.at(1).append("Undo");
    _menu.at(1).append("Redo");
    _menu.at(1).append_splitter();
    _menu.at(1).append("Cut");
    _menu.at(1).append("Copy");
    _menu.at(1).append("Paste");
    _menu.at(1).append("Delete");
    _menu.at(1).append_splitter();
    _menu.at(1).append("Select all");

    _menu.push_back("&Configuration");
    _menu.at(2).append("Line Wrap", [this](nana::menu::item_proxy &ip)
    {
        _text.line_wrapped(ip.checked());
    });
    _menu.at(2).check_style(0, nana::menu::checks::highlight);

    _menu.push_back("&About");
    _menu.at(3).append("About Ted...");
    _menu.at(3).append("Anout Nana...");
}

void Editor::setupEditor()
{
    _text.borderless(true);
    _text.enable_dropfiles(true);
    _text.events().mouse_dropfiles([this](const nana::arg_dropfiles &arg)
    {
        if (arg.files.size() && isSaveNeeded(_filename))
            _text.load(arg.files.at(0));
    });

    nana::API::effects_edge_nimbus(_text, nana::effects::edge_nimbus::none);
}

void Editor::setupUi()
{
    _widget.div("vert<menubar weight=26><textbox>");
    _widget.field("menubar") << _menu;
    _widget.field("textbox") << _text;
    _widget.collocate();
}

bool Editor::isSaveNeeded(const std::string &filename)
{
    if (!_text.saved())
        return save(filename);

    return _text.saved();
}

std::string Editor::fileDialog(const bool is_open) const
{
    nana::filebox dialog(*this, is_open);

    dialog.add_filter("Readme", "*.md");
    dialog.add_filter("Text", "*.txt");
    dialog.add_filter("All Files", "*.*");

    dialog.show();

    return dialog.path();
}
