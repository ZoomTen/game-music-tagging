#include <string>
#include <map>
#include "libgmtag.h"

typedef struct gmtag_def {
    uint64_t track;
    std::string album;
    std::string company;
    std::string publisher;
    std::string artist;
    std::string composer;
    std::string sequencer;
    std::string engineer;
    std::string ripper;
    std::string tagger;
    std::string title;
    std::string comments;
    std::string copyright;
    GmTagDateDef date;
    GmTagTimeDef length;
    GmTagTimeDef fade;
} GmTagDef;

// every field has a maximum length of:
#define MAX_FIELD_LENGTH 256

extern std::map<uint64_t, GmTagDef> tags;
extern GmTagDef default_tags;
