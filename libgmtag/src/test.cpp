#include <stdlib.h>
#include <stdio.h>
#include "libgmtag.hpp"

int main (void) {
    // let's put the entire thing in the buffer first
    FILE *tags = fopen("test_rip/!tags.m3u", "r");

    fseek(tags, 0, SEEK_END);
    size_t tag_buf_size = ftell(tags);

    char *tag_buf = (char *) malloc(tag_buf_size);
    fseek(tags, 0, SEEK_SET);
    fread(tag_buf, 1, tag_buf_size, tags);

    fclose(tags);

    GmTags tag_obj = GmTags();
    tag_obj.tags_from_buffer(tag_buf);

    printf("test\n");

    free(tag_buf);
    return 0;
}