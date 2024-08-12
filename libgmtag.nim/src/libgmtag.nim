import std/strutils
import std/parseutils
import ./datatypes

# All of these are TODO
when defined(test):
  import pretty
  proc toDate(buffer: string): DateDef =
    result = DateDef()
    type which = enum
      Year
      Month
      Day

    var
      atMode: which = Year
      yyyy: string
      mm: string
      dd: string
      dumpTo = yyyy.addr

    for i in buffer:
      case i
      of '0' .. '9':
        dumpTo[].add(i)
      of '-':
        case atMode
        of Year:
          dumpTo = mm.addr
          atMode = Month
        of Month:
          dumpTo = dd.addr
          atMode = Day
        else:
          discard
      else:
        discard
    if len(yyyy) > 0:
      result.year = yyyy.parseInt().uint64
    if len(mm) in 1 .. 2:
      result.month = mm.parseInt().uint8
      if result.month > 12:
        # error here
        discard
    if len(dd) in 1 .. 2:
      result.day = dd.parseInt().uint8
      if result.day > 31:
        # error here
        discard
    return result

  proc testParse*(buffer: string): void =
    var
      container: TagContainer
      curtag: TagData
      curtagNum = 0

    for line in ($buffer).splitLines():
      var trimmed = line.strip()

      if len(trimmed) == 0:
        continue # skip

      debugEcho trimmed

      if trimmed[0] == '#': # a comment, this is what we want
        trimmed = trimmed[1 ..^ 1].strip()
        var command: string
        let
          contentBeginAt = trimmed[1 ..^ 1].parseUntil(command, ' ') + 1
          content = trimmed[contentBeginAt ..^ 1].strip()

        case command
        of "album":
          curtag.album = content
        of "artist":
          curtag.artist = content
        of "title":
          curtag.title = content
        of "company":
          curtag.company = content
        of "publisher":
          curtag.publisher = content
        of "composer":
          curtag.composer = content
        of "arranger":
          curtag.arranger = content
        of "sequencer":
          curtag.sequencer = content
        of "engineer":
          curtag.engineer = content
        of "ripper":
          curtag.ripper = content
        of "tagger":
          curtag.tagger = content
        of "copyright":
          curtag.copyright = content
        of "date":
          curtag.date = content.toDate()
        else:
          discard
        # may not be the most efficient as it reassigns
        # everytime, but it gets the job done
        if trimmed[0] == '@':
          container[0] = curtag
      else: # a m3u entry
        # Assuming the subtune number is always at the end,
        # we can work backwards
        var
          qmarkNumberBeginsAt = -1
          numberEncountered = false
        for i in countdown(len(trimmed) - 1, 0):
          case trimmed[i]
          of '0' .. '9':
            if not numberEncountered:
              numberEncountered = true
          of '?':
            if numberEncountered:
              qmarkNumberBeginsAt = i
          else:
            discard
          if qmarkNumberBeginsAt > 0:
            # this is a "valid" line, so don't process further
            break
        if qmarkNumberBeginsAt < 0:
          # error here
          discard
        else:
          let
            numStr = trimmed[qmarkNumberBeginsAt + 1 ..^ 1]
            trkNum = numStr.parseInt()
          curtag.track = trkNum
          container[trkNum.uint64] = curtag
          # reset to "global" tags
          curtag = container[0]

    for i in 0 .. 5:
      print(container[i.uint64])

proc tags_from_buffer(buffer: cstring): ptr TagContainer {.cdecl, exportc, dynlib.} =
  let newTags = cast[ptr TagContainer](alloc0Impl(sizeof(TagContainer)))
  return newTags

proc unset_tags(handle: ptr TagContainer): void {.cdecl, exportc, dynlib.} =
  # Zero out the memory before deallocating it
  handle[] = TagContainer()
  dealloc(handle)

proc get_subtune_count(handle: ptr TagContainer): uint64 {.cdecl, exportc, dynlib.} =
  let subtuneCount = len(handle[])
  case subtuneCount
  of 0:
    return 0'u64
  else:
    return subtuneCount.uint64 - 1

proc get_subtune_order(
    handle: ptr TagContainer
): ptr OrderDef {.cdecl, exportc, dynlib.} =
  return nil

proc get_length_of_subtune(
    handle: ptr TagContainer, subtune: uint64
): int64 {.cdecl, exportc, dynlib.} =
  if subtune > handle.get_subtune_count():
    return -1
  let tagInfo = handle[][subtune]
  return ((tagInfo.length.seconds * 1000) + tagInfo.length.miliseconds).int64

proc get_fade_length_of_subtune(
    handle: ptr TagContainer, subtune: uint64
): int64 {.cdecl, exportc, dynlib.} =
  if subtune > handle.get_subtune_count():
    return -1
  let tagInfo = handle[][subtune]
  return ((tagInfo.fade.seconds * 1000) + tagInfo.fade.miliseconds).int64

proc get_duration_of_subtune(
    handle: ptr TagContainer, subtune: uint64
): int64 {.cdecl, exportc, dynlib.} =
  let
    stLen = handle.get_length_of_subtune(subtune)
    stFadeLen = handle.get_fade_length_of_subtune(subtune)
  if (stLen < 0) or (stFadeLen < 0):
    return -1
  return stLen + stFadeLen

proc get_album(
    handle: ptr TagContainer, subtune: uint64
): cstring {.cdecl, exportc, dynlib.} =
  return cstring""

proc get_company(
    handle: ptr TagContainer, subtune: uint64
): cstring {.cdecl, exportc, dynlib.} =
  return cstring""

proc get_publisher(
    handle: ptr TagContainer, subtune: uint64
): cstring {.cdecl, exportc, dynlib.} =
  return cstring""

proc get_artist(
    handle: ptr TagContainer, subtune: uint64
): cstring {.cdecl, exportc, dynlib.} =
  return cstring""

proc get_composer(
    handle: ptr TagContainer, subtune: uint64
): cstring {.cdecl, exportc, dynlib.} =
  return cstring""

proc get_sequencer(
    handle: ptr TagContainer, subtune: uint64
): cstring {.cdecl, exportc, dynlib.} =
  return cstring""

proc get_arranger(
    handle: ptr TagContainer, subtune: uint64
): cstring {.cdecl, exportc, dynlib.} =
  return cstring""

proc get_engineer(
    handle: ptr TagContainer, subtune: uint64
): cstring {.cdecl, exportc, dynlib.} =
  return cstring""

proc get_ripper(
    handle: ptr TagContainer, subtune: uint64
): cstring {.cdecl, exportc, dynlib.} =
  return cstring""

proc get_tagger(
    handle: ptr TagContainer, subtune: uint64
): cstring {.cdecl, exportc, dynlib.} =
  return cstring""

proc get_title(
    handle: ptr TagContainer, subtune: uint64
): cstring {.cdecl, exportc, dynlib.} =
  return cstring""

proc get_comment(
    handle: ptr TagContainer, subtune: uint64
): cstring {.cdecl, exportc, dynlib.} =
  return cstring""

proc get_copyright(
    handle: ptr TagContainer, subtune: uint64
): cstring {.cdecl, exportc, dynlib.} =
  return cstring""

proc get_track_num(
    handle: ptr TagContainer, subtune: uint64
): uint64 {.cdecl, exportc, dynlib.} =
  return 0

proc get_date(
    handle: ptr TagContainer, subtune: uint64
): DateDef {.cdecl, exportc, dynlib.} =
  return DateDef()

proc get_length(
    handle: ptr TagContainer, subtune: uint64
): TimeDef {.cdecl, exportc, dynlib.} =
  return TimeDef()

proc get_fade(
    handle: ptr TagContainer, subtune: uint64
): TimeDef {.cdecl, exportc, dynlib.} =
  return TimeDef()
