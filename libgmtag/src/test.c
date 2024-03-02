#include <stdio.h>
#include <stdlib.h>

#include "libgmtag.h"

int main (void) {
  // let's put the entire thing in the buffer first
  FILE *tags = fopen("test_rip/!tags.m3u", "r");

  fseek(tags, 0, SEEK_END);
  size_t tag_buf_size = (size_t)ftell(tags);

  char *tag_buf = (char *)malloc(tag_buf_size);
  fseek(tags, 0, SEEK_SET);
  fread(tag_buf, 1, tag_buf_size, tags);

  fclose(tags);

  tags_from_buffer(tag_buf);

  for (size_t i = 0; i < 20; i++) {
    GmTagDef tag = get_tags_for_subtune(i);
    printf("\n---- subtune %d ----\n", (int)i);
    printf(
        "Album:     %s\n"
        "Company:   %s\n"
        "Publisher: %s\n"
        "Artist:    %s\n"
        "Composer:  %s\n"
        "Sequencer: %s\n"
        "Engineer:  %s\n"
        "Date:      %04u-%02u-%02u\n"
        "Ripper:    %s\n"
        "Tagger:    %s\n"
        "Title:     %s\n"
        "Comment:   %s\n"
        "Copyright: %s\n"
        "Length:    %03lu.%03lu\n"
        "Fade:      %03lu.%03lu\n",
        tag.album,
        tag.company,
        tag.publisher,
        tag.artist,
        tag.composer,
        tag.sequencer,
        tag.engineer,
        tag.date.year,
        tag.date.month,
        tag.date.day,
        tag.ripper,
        tag.tagger,
        tag.title,
        tag.comments,
        tag.copyright,
        tag.length.seconds,
        tag.length.miliseconds,
        tag.fade.seconds,
        tag.fade.miliseconds
    );
  }

  free(tag_buf);
  return 0;
}
