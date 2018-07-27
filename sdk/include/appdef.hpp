#pragma once

// volatile so the compiler doesn't optimise the section out
#define HOLLYHOCK_SECTION_STRING(name, str) \
    __attribute__ ((section(".hollyhock_" #name))) \
        volatile char hollyhock_ ## name [] = str;

#define APP_NAME(app_name) \
    HOLLYHOCK_SECTION_STRING(name, app_name)
#define APP_DESCRIPTION(app_description) \
    HOLLYHOCK_SECTION_STRING(description, app_description)
#define APP_AUTHOR(app_author) \
    HOLLYHOCK_SECTION_STRING(author, app_author)
#define APP_VERSION(app_version) \
    HOLLYHOCK_SECTION_STRING(version, app_version)
