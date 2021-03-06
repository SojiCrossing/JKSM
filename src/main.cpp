#include <3ds.h>

#include "gfx.h"
#include "fs.h"
#include "ui.h"
#include "util.h"
#include "data.h"
#include "sys.h"

extern int state;

int main(int argc, const char *argv[])
{
    sys::init();
    gfx::init();
    fs::init();

    if(envIsHomebrew())
    {
        std::string path = argv[0];
        size_t lastSlash = path.find_last_of('/');
        path.erase(lastSlash + 1, path.length());
        if(util::fexists(path + "JKSM.xml"))
        {
            data::haxDataInit();
            data::haxMode = true;
            fs::fsStartSession();
            state = HAX_MENU;
        }
        //I'll fix this later
        else
        {
            data::loadTitles();
            data::loadNand();
        }
    }
    else
    {
        data::loadTitles();
        data::loadNand();
    }
    ui::prepMenus();

    while(aptMainLoop() && sys::run)
    {
        hidScanInput();

        data::cartCheck();

        uint32_t down = hidKeysDown();
        uint32_t held = hidKeysHeld();

        if(down & KEY_START)
            break;

        ui::runApp(down, held);
    }

    sys::exit();
    gfx::exit();
    fs::exit();
}
