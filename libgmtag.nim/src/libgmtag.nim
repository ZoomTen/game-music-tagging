import std/strutils
import std/parseutils
import ./datatypes

# All of these are TODO
when defined(test):
  import pretty
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

      if trimmed[0] == '#': # is comment, try and process it
        trimmed = trimmed[1 ..^ 1].strip()
        var command: string
        let x = trimmed[1 ..^ 1].parseUntil(command, ' ') + 1

        case command
        of "album":
          curtag.album = trimmed[x ..^ 1].strip()
        of "artist":
          curtag.artist = trimmed[x ..^ 1].strip()
        of "title":
          curtag.title = trimmed[x ..^ 1].strip()
        of "company":
          curtag.company = trimmed[x ..^ 1].strip()
        of "publisher":
          curtag.publisher = trimmed[x ..^ 1].strip()
        of "composer":
          curtag.composer = trimmed[x ..^ 1].strip()
        of "arranger":
          curtag.arranger = trimmed[x ..^ 1].strip()
        of "sequencer":
          curtag.sequencer = trimmed[x ..^ 1].strip()
        of "engineer":
          curtag.engineer = trimmed[x ..^ 1].strip()
        of "ripper":
          curtag.ripper = trimmed[x ..^ 1].strip()
        of "tagger":
          curtag.tagger = trimmed[x ..^ 1].strip()
        of "copyright":
          curtag.copyright = trimmed[x ..^ 1].strip()
        else:
          discard

        if trimmed[0] == '@':
          container[0] = curtag
      else:
        # this must be a m3u entry. Assuming the subtune number
        # is always at the end, we can work backwards
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
            break
        debugEcho "subtune id begins at ", $qmarkNumberBeginsAt
        if qmarkNumberBeginsAt > 0:
          let
            numStr = trimmed[qmarkNumberBeginsAt + 1 ..^ 1]
            trkNum = numStr.parseInt()
          print(trkNum)
          curtag.track = trkNum
          container[trkNum.uint64] = curtag
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
