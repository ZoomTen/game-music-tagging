#pragma once

#include <map>
#include <stdint.h>
#include <time.h>

typedef struct time_def {
    time_t seconds;
    time_t miliseconds;
} GmTagTime;

typedef struct gmtag_def {
    const char* album = "";
    const char* company = "";
    const char* publisher = "";
    const char* artist = "";
    const char* composer = "";
    const char* sequencer = "";
    const char* engineer = "";
    const char* date = "";
    const char* ripper = "";
    const char* tagger = "";
    const char* source = "";
    const char* title = "";
    const char* comments = "";
    const char* copyright = "";
    GmTagTime length;
    GmTagTime fade;
} GmTagDef;

class GmTags {
    public:
        GmTags();
        ~GmTags();
        void tags_from_buffer (const char *buff);
        GmTagDef get_tags_for_subtune(int subtune);
    private:
        std::map<uint64_t, GmTagDef> tags;
        GmTagDef default_tags;
};