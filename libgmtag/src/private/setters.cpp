#include "./utils.hpp"
#include "./gmtag.hpp"
#include "./setters.hpp"
#include <vector>

static inline const char *
set_generic (std::string &target, const char *buffer)
{
  buffer = skip_spaces(buffer);
  char *tmp = static_cast<char *>(malloc(MAX_FIELD_LENGTH));
  buffer =
      into_buffer_until_newline(buffer, tmp, MAX_FIELD_LENGTH);
  target = std::string(tmp);
  free(tmp);
  return buffer;
}

const char *set_album (GmTagDef &tag, const char *buffer)
{
  return set_generic(tag.album, buffer);
}

const char *set_title (GmTagDef &tag, const char *buffer)
{
  return set_generic(tag.title, buffer);
}

const char *set_composer (GmTagDef &tag, const char *buffer)
{
  return set_generic(tag.composer, buffer);
}

const char *set_company (GmTagDef &tag, const char *buffer)
{
  return set_generic(tag.company, buffer);
}

const char *set_publisher (GmTagDef &tag, const char *buffer)
{
  return set_generic(tag.publisher, buffer);
}

const char *set_artist (GmTagDef &tag, const char *buffer)
{
  return set_generic(tag.artist, buffer);
}

const char *set_sequencer (GmTagDef &tag, const char *buffer)
{
  return set_generic(tag.sequencer, buffer);
}

const char *set_arranger (GmTagDef &tag, const char *buffer)
{
  return set_generic(tag.arranger, buffer);
}

const char *set_engineer (GmTagDef &tag, const char *buffer)
{
  return set_generic(tag.engineer, buffer);
}

const char *set_date (GmTagDef &tag, const char *buffer)
{
  buffer = skip_spaces(buffer);
  char cur_char = *buffer++;
  std::string year, month, day;

  // get year
  while (is_number(cur_char))
  {
    year.push_back(cur_char);
    cur_char = *buffer++;
  }

  if (cur_char != '-')
  {
    goto do_parse_date;
  }

  cur_char = *buffer++;

  // get month
  while (is_number(cur_char))
  {
    month.push_back(cur_char);
    cur_char = *buffer++;
  }

  if ((cur_char != '-') || (month.length() < 1) ||
      (month.length() > 2))
  {
    goto do_parse_date;
  }

  cur_char = *buffer++;

  // get date
  while (is_number(cur_char))
  {
    day.push_back(cur_char);
    cur_char = *buffer++;
  }

do_parse_date:
  if (year == "")
  {
    year = "0";
  }
  if (month == "")
  {
    month = "0";
  }
  if (day == "")
  {
    day = "0";
  }

  tag.date.year = std::stoul(year);
  tag.date.month = static_cast<uint8_t>(std::stoul(month));
  tag.date.day = static_cast<uint8_t>(std::stoul(day));
  return --buffer;
}

const char *set_ripper (GmTagDef &tag, const char *buffer)
{
  return set_generic(tag.ripper, buffer);
}

const char *set_tagger (GmTagDef &tag, const char *buffer)
{
  return set_generic(tag.tagger, buffer);
}

const char *str_to_time (const char *buffer, GmTagTimeDef &time)
{
  // in case this was called with uninit'd &time
  time.seconds = 0;
  time.miliseconds = 0;

  std::string tmp = "";
  std::vector<time_t> tmp_stack;

  char cur_char = *buffer++;
  while (is_number(cur_char))
  {
    tmp.push_back(cur_char);
    cur_char = *buffer++;
  }

  if (cur_char == '.')
  {  // must be 420.690
    // so the buffer right now must be seconds
    time.seconds = std::stol(tmp);

    // then process just the miliseconds part
    tmp = "";
    cur_char = *(++buffer);
    while (is_number(cur_char))
    {
      tmp.push_back(cur_char);
      cur_char = *buffer++;
    }
    time.miliseconds = std::stol(tmp);
    return --buffer;
  }
  else if (cur_char != ':')
  {  // must be 420
    time.seconds = std::stol(tmp);
    return --buffer;
  }

  // save the number for now
  // because I don't know if it's 00:07:00.690
  // or if it's 07:00.690 or what…
  tmp_stack.push_back(std::stol(tmp));
  tmp = "";

  cur_char = *buffer++;
  while (is_number(cur_char))
  {
    tmp.push_back(cur_char);
    cur_char = *buffer++;
  }

  if (cur_char != ':')
  {  // must be 07:00.690 or 07:00
    time.seconds = tmp_stack.back() * 60;
    time.seconds += std::stol(tmp);

    if (cur_char != '.')
    {  // 07:00
      return --buffer;
    }

    // 07:00.690
    tmp = "";
    cur_char = *(++buffer);
    while (is_number(cur_char))
    {
      tmp.push_back(cur_char);
      cur_char = *buffer++;
    }
    time.miliseconds = std::stol(tmp);
    return --buffer;
  }

  // this leaves us with 00:07:00 or 00:07:00.690
  tmp_stack.push_back(std::stol(tmp));
  tmp = "";

  cur_char = *buffer++;
  while (is_number(cur_char))
  {
    tmp.push_back(cur_char);
    cur_char = *buffer++;
  }

  // minutes
  time.seconds = tmp_stack.back() * 60;
  tmp_stack.pop_back();

  // hours
  time.seconds = tmp_stack.back() * 60 * 60;
  time.seconds += std::stol(tmp);

  if (cur_char != '.')
  {  // 00:07:00
    return --buffer;
  }

  // 00:07:00.690
  tmp = "";
  cur_char = *(++buffer);
  while (is_number(cur_char))
  {
    tmp.push_back(cur_char);
    cur_char = *buffer++;
  }
  time.miliseconds = std::stol(tmp);

  return --buffer;
}

const char *set_length (GmTagDef &tag, const char *buffer)
{
  buffer = skip_spaces(buffer);
  GmTagTimeDef time = GmTagTimeDef{};
  buffer = str_to_time(buffer, time);
  tag.length = time;
  return buffer;
}

const char *set_fade (GmTagDef &tag, const char *buffer)
{
  buffer = skip_spaces(buffer);
  GmTagTimeDef time = GmTagTimeDef{};
  buffer = str_to_time(buffer, time);
  tag.fade = time;
  return buffer;
}

const char *set_comment (GmTagDef &tag, const char *buffer)
{
  buffer = skip_spaces(buffer);
  char *tmp = static_cast<char *>(malloc(MAX_FIELD_LENGTH));
  buffer =
      into_buffer_until_newline(buffer, tmp, MAX_FIELD_LENGTH);

  if (tag.comments.empty())
  {
    tag.comments = std::string(tmp);
  }
  else
  {
    tag.comments.push_back('\n');
    tag.comments.append(tmp);
  }

  free(tmp);
  return buffer;
}

const char *set_copyright (GmTagDef &tag, const char *buffer)
{
  return set_generic(tag.copyright, buffer);
}

const char *set_track (GmTagDef &tag, const char *buffer)
{
  buffer = skip_spaces(buffer);
  std::string tmp;
  char cur_char = *buffer++;
  while (is_number(cur_char))
  {
    tmp.push_back(cur_char);
    cur_char = *buffer++;
  }
  if (tmp.length() > 0)
  {
    tag.track = std::stoul(tmp);
  }
  return --buffer;
}
