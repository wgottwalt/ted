#include <vector>
#include <nana/gui/filebox.hpp>
#include <nana/gui/drawing.hpp>
#include <nana/gui/msgbox.hpp>
#include "Editor.hxx"

//--- public constructors ---

Editor::Editor(const std::string &name, const std::string &filename)
: form(), _layout(*this), _menu(*this), _linenum(*this), _text(*this)
{
    caption(name);
    setupUi();
    setupMenu();
    setupEditor();

    events().unload([this](const nana::arg_unload &arg)
    {
        if (!saveIfEdited(false))
            arg.cancel = true;
    });

    if (filename != DefFilename)
        _text.load(filename);
}

Editor::~Editor() noexcept
{
}

//--- protected methods ---

void Editor::setupMenu()
{
    _menu.push_back("&File");
    _menu.at(0).append("New", [this](nana::menu::item_proxy &)
    {
        if (saveIfEdited())
            _text.reset();
    });
    _menu.at(0).append_splitter();
    _menu.at(0).append("Open", [this](nana::menu::item_proxy &)
    {
        if (saveIfEdited())
        {
            std::string ifile = fileDialog(true);

            if (!ifile.empty())
                _text.load(ifile.data());
        }
    });
    _menu.at(0).append("Save", [this](nana::menu::item_proxy &)
    {
        std::string ofile = _text.filename();

        if (ofile.empty())
        {
            ofile = fileDialog(false);
            if (ofile.empty())
                return;
        }
        _text.store(ofile);
    });
    _menu.at(0).append("Save as...", [this](nana::menu::item_proxy &)
    {
        std::string ofile = fileDialog(false);

        if (!ofile.empty())
            _text.store(ofile);
    });
    _menu.at(0).append_splitter();
    _menu.at(0).append("Quit", [this](nana::menu::item_proxy &)
    {
        if (saveIfEdited())
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
    _menu.at(2).append("Line Numbers", [this](nana::menu::item_proxy &ip)
    {
    });
    _menu.at(2).check_style(1, nana::menu::checks::highlight);

    _menu.push_back("&About");
    _menu.at(3).append("About Ted...");
    _menu.at(3).append("Anout Nana...");
}

void Editor::setupEditor()
{
    _text.borderless(true);
    _text.enable_dropfiles(true);

    nana::drawing(_linenum).draw([&](nana::paint::graphics &gfx)
    {
        const int32_t pixels_per_line = _text.line_pixels();

        if (pixels_per_line > 0)
        {
            const auto positions = _text.text_position();
            const int32_t right = gfx.width() - 5;
            int32_t top = _text.text_area().y - _text.content_origin().y % pixels_per_line;

            for (auto &pos : positions)
            {
                const auto num = std::to_wstring(pos.y + 1);
                const int32_t pxls = gfx.text_extent_size(num).width;

                if ((pxls + 5) > static_cast<int32_t>(gfx.width()))
                {
                    _layout.modify("ln", ("weight=" + std::to_string(pxls + 10)).c_str());
                    collocate();

                    return;
                }

                gfx.string({right - pxls, top }, num);
                top += pixels_per_line;
            }
        }
    });

    _text.events().mouse_dropfiles([this](const nana::arg_dropfiles &arg)
    {
        if (arg.files.size() && saveIfEdited())
            _text.load(arg.files.at(0));
    });
    _text.events().text_exposed([this]{ nana::API::refresh_window(_linenum); });

    nana::API::effects_edge_nimbus(_text, nana::effects::edge_nimbus::none);
}

void Editor::setupUi()
{
    _layout.div("<><weight=100% vertical <mb weight=26> <gap=1 <ln weight=12> <ed> > ><>");
    _layout.field("mb") << _menu;
    _layout.field("ln") << _linenum;
    _layout.field("ed") << _text;
    _layout.collocate();
}

bool Editor::saveIfEdited(const bool force_requester)
{
    if (force_requester || _text.edited())
    {
        nana::msgbox msg(*this, "Unsaved Changes Dialog", nana::msgbox::button_t::yes_no_cancel);

        msg << "Do you want to save changes?";

        switch (msg.show())
        {
            case nana::msgbox::pick_yes:
            {
                std::string ofile = _text.filename();

                if (ofile.empty())
                {
                    ofile = fileDialog(false);

                    if (ofile.empty())
                        break;
                }
                _text.store(ofile);

                break;
            }
            case nana::msgbox::pick_no:
                break;
            case nana::msgbox::pick_cancel:
            default:
                return false;
        }
    }

    return true;
}

std::string Editor::fileDialog(const bool is_open_dialog) const
{
    nana::filebox dialog(*this, is_open_dialog);

    dialog.add_filter("Text (*.txt)", "*.txt");
    dialog.add_filter("Readme (*.md)", "*.md");
    dialog.add_filter("All Files (*.*)", "*.*");

    if (!is_open_dialog)
        dialog.init_file(DefFilename);

    if (auto files = dialog.show(); !files.empty())
        return files.at(0);

    return std::string();
}
