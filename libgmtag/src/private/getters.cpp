#include "./gmtag.hpp"
#include "./getters.hpp"
#include <cstring>

/**
 * @brief Get the tags for a rip's subtune.
 *
 * If subtune is 0 or a subtune in which tags do not exist for
 * it, it will return the default tags (if any), defined as
 * global tags at the beginning of the file.
 *
 * This requires tags to be init'd with tags_from_buffer().
 *
 * @param subtune
 * @return GmTagDef
 */
GmTagDef get_tags_for_subtune (unsigned long subtune) {
    if (tags.count(subtune))
        return tags[subtune];
    return default_tags;
}

uint64_t get_subtune_count () { return tags.size(); }

int64_t get_length_of_subtune (unsigned long subtune) {
    if (tags.count(subtune)) {
        time_t s =
        ((tags[subtune].length.seconds * 1000) +
        (tags[subtune].length.miliseconds));
        return s;
    }
    return -1;
}

int64_t get_fade_length_of_subtune (unsigned long subtune) {
    if (tags.count(subtune)) {
        time_t s =
        ((tags[subtune].fade.seconds * 1000) +
        (tags[subtune].fade.miliseconds));
        return s;
    }
    return -1;
}

int64_t get_duration_of_subtune (unsigned long subtune) {
    if (!tags.count(subtune)) {
        return -1;
    }
    return get_length_of_subtune(subtune) +
    get_fade_length_of_subtune(subtune);
}

GmTagOrderDef *get_subtune_order () {
    GmTagOrderDef *orders =
    static_cast<GmTagOrderDef *>(malloc(sizeof(GmTagOrderDef))
    );

    orders->order = static_cast<uint64_t *>(
        calloc(tags.size(), sizeof(uint64_t))
    );
    orders->how_many = tags.size();

    size_t i = 0;
    for (auto tag : tags) {
        orders->order[i++] = tag.first;
    }

    return orders;
}

char *get_album (unsigned long subtune) {
    GmTagDef tag = get_tags_for_subtune(subtune);
    char *i = strdup(tag.album.c_str());
    return i;
}

char *get_company (unsigned long subtune) {
    GmTagDef tag = get_tags_for_subtune(subtune);
    char *i = strdup(tag.company.c_str());
    return i;
}

char *get_publisher (unsigned long subtune) {
    GmTagDef tag = get_tags_for_subtune(subtune);
    char *i = strdup(tag.publisher.c_str());
    return i;
}

char *get_artist (unsigned long subtune) {
    GmTagDef tag = get_tags_for_subtune(subtune);
    char *i = strdup(tag.artist.c_str());
    return i;
}

char *get_composer (unsigned long subtune) {
    GmTagDef tag = get_tags_for_subtune(subtune);
    char *i = strdup(tag.composer.c_str());
    return i;
}

char *get_sequencer (unsigned long subtune) {
    GmTagDef tag = get_tags_for_subtune(subtune);
    char *i = strdup(tag.sequencer.c_str());
    return i;
}

char *get_engineer (unsigned long subtune) {
    GmTagDef tag = get_tags_for_subtune(subtune);
    char *i = strdup(tag.engineer.c_str());
    return i;
}

char *get_ripper (unsigned long subtune) {
    GmTagDef tag = get_tags_for_subtune(subtune);
    char *i = strdup(tag.ripper.c_str());
    return i;
}

char *get_tagger (unsigned long subtune) {
    GmTagDef tag = get_tags_for_subtune(subtune);
    char *i = strdup(tag.tagger.c_str());
    return i;
}

char *get_title (unsigned long subtune) {
    GmTagDef tag = get_tags_for_subtune(subtune);
    char *i = strdup(tag.title.c_str());
    return i;
}

char *get_comment (unsigned long subtune) {
    GmTagDef tag = get_tags_for_subtune(subtune);
    char *i = strdup(tag.comments.c_str());
    return i;
}

char *get_copyright (unsigned long subtune) {
    GmTagDef tag = get_tags_for_subtune(subtune);
    char *i = strdup(tag.copyright.c_str());
    return i;
}

GmTagDateDef get_date (unsigned long subtune) {
    GmTagDef tag = get_tags_for_subtune(subtune);
    return tag.date;
}

GmTagTimeDef get_length (unsigned long subtune) {
    GmTagDef tag = get_tags_for_subtune(subtune);
    return tag.length;
}

GmTagTimeDef get_fade (unsigned long subtune) {
    GmTagDef tag = get_tags_for_subtune(subtune);
    return tag.fade;
}

uint64_t get_track_num (unsigned long subtune) {
    GmTagDef tag = get_tags_for_subtune(subtune);
    return tag.track;
}
