#include <sdk/os/debug.hpp>
#include <sdk/os/gui.hpp>
#include <sdk/os/lcd.hpp>
#include <sdk/os/mem.hpp>
#include <sdk/os/string.hpp>
#include "apps.hpp"

class Launcher : public GUIDialog {
public:
    int m_selectedApp;

    Launcher() : GUIDialog(
        GUIDialog::Height95, GUIDialog::AlignTop,
        "Hollyhock Launcher",
        GUIDialog::KeyboardStateNone
    ), m_appNames(
        GetLeftX() + 10, GetTopY() + 10, GetRightX() - 10, GetBottomY() - 10,
        APP_NAMES_EVENT_ID
    ), m_appInfo(
        GetLeftX() + 10, GetTopY() + 90, GetRightX() - 10, GetBottomY() - 10,
        "Path: \\fls0\\myApp.hhk\n\nName: This is the app name\n\nDescription: It does some fancy stuff for you. You can make it bleep and bloop.\n\nAuthor: The6P4C\n\nVersion: 1.0.0"
    ), m_run(
        GetLeftX() + 10, GetTopY() + 45, GetLeftX() + 10 + 100, GetTopY() + 45 + 35,
        "Run", RUN_EVENT_ID
    ), m_close(
        GetRightX() - 10 - 100, GetTopY() + 45, GetRightX() - 10, GetTopY() + 45 + 35,
        "Close", CLOSE_EVENT_ID
    ) {
        m_selectedApp = 0;

        Apps::LoadAppInfo();

        for (int i = 0; i < Apps::g_numApps; ++i) {
            struct Apps::AppInfo *app = &Apps::g_apps[i];

            const char *name = app->path;

            // first char of the name will not be \0 if a name was included.
            if (app->name[0] != '\0') {  
                name = app->name;
            }

            m_appNames.AddMenuItem(*(
                new GUIDropDownMenuItem(
                    name, i + 1,
                    GUIDropDownMenuItem::FlagEnabled |
                    GUIDropDownMenuItem::FlagTextAlignLeft
                )
            ));
        }

        m_appNames.SetScrollBarVisibility(
            GUIDropDownMenu::ScrollBarVisibleWhenRequired
        );
        AddElement(m_appNames);

        AddElement(m_appInfo);        
        AddElement(m_run);
        AddElement(m_close);

        UpdateAppInfo();
    }

    virtual int OnEvent(GUIDialog_Wrapped *dialog, GUIDialog_OnEvent_Data *event) {
        if (event->GetEventID() == APP_NAMES_EVENT_ID && (event->type & 0xF) == 0xD) {
            m_selectedApp = event->data - 1;

            UpdateAppInfo();

            return 0;
        }

        return GUIDialog::OnEvent(dialog, event);
    }

    void UpdateAppInfo() {
        struct Apps::AppInfo *app = &Apps::g_apps[m_selectedApp];
        bool hasName = app->name[0] != '\0';
        bool hasDescription = app->description[0] != '\0';
        bool hasAuthor = app->author[0] != '\0';
        bool hasVersion = app->version[0] != '\0';

        memset(m_appInfoString, 0, sizeof(m_appInfoString));

        if (hasName) {
            strcat(m_appInfoString, app->name);
        } else {
            strcat(m_appInfoString, app->path);
        }

        if (hasAuthor || hasVersion) {
            strcat(m_appInfoString, "\n(");

            if (hasVersion) {
                strcat(m_appInfoString, "version ");
                strcat(m_appInfoString, app->version);
            }

            if (hasAuthor) {
                if (hasVersion) {
                    strcat(m_appInfoString, " by ");
                } else {
                    strcat(m_appInfoString, "by ");
                }

                strcat(m_appInfoString, app->author);
            }

            strcat(m_appInfoString, ")");
        }

        if (hasName) {
            strcat(m_appInfoString, "\n(from ");
            strcat(m_appInfoString, app->path);
            strcat(m_appInfoString, ")");
        }

        if (hasDescription) {
            strcat(m_appInfoString, "\n\n");
            strcat(m_appInfoString, app->description);
        }

        // App Name (version 1.0.0 by Meme King)
        // \fls0\meme.hhk

        // Meme to your heart's content.

        m_appInfo.SetText(m_appInfoString);
        m_appInfo.Refresh();
        Refresh();
    }

private:
    const uint16_t APP_NAMES_EVENT_ID = 1;
    GUIDropDownMenu m_appNames;

    GUILongLabel m_appInfo;

    // While GUILongLabel seems to copy the string into it's own memory when
    // calling SetText, when it refreshes sometimes it tries to read the old
    // memory. Not sure why, but that's why this is here - it'll never move
    // because GUIDialog has the copy/move ctor deleted. This should therefore
    // be a safe solution.
    char m_appInfoString[500];

    const uint16_t RUN_EVENT_ID = GUIDialog::DialogResultOK;
    GUIButton m_run;

    const uint16_t CLOSE_EVENT_ID = GUIDialog::DialogResultCancel;
    GUIButton m_close;
};

void main() {
    Launcher launcher;
    if (launcher.ShowDialog() == GUIDialog::DialogResultOK) {
        Apps::EntryPoint ep = Apps::RunApp(launcher.m_selectedApp);
        if (ep != nullptr) {
            ep();
        }
    }
}
