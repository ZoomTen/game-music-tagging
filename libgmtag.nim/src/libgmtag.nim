import std/strutils
import std/parseutils
import ./datatypes
export datatypes

proc toTime(buffer: string): TimeDef =
  var r = TimeDef()
  type
    tkKind = enum
      Nothing
      Number
      Colon
      Dot

    token = object
      content: string
      kind: tkKind

  # Tokenize the string
  var
    tokens: seq[token] = @[]
    newToken: token
  for i in buffer:
    case i
    of '0' .. '9':
      newToken.kind = Number
      newToken.content.add(i)
    of ':':
      if newToken.kind != Nothing:
        tokens.add(newToken)
      tokens.add(token(content: $i, kind: Colon))
      newToken = token()
    of '.':
      if newToken.kind != Nothing:
        tokens.add(newToken)
      tokens.add(token(content: $i, kind: Dot))
      newToken = token()
    else:
      discard
  tokens.add(newToken)
  # Ad-hoc lexer... :(
  case len(tokens)
  of 1:
    # SS
    if tokens[0].kind == Number:
      r.seconds = tokens[0].content.parseInt().uint64
  of 3:
    case tokens[1].kind
    of Dot:
      # SS.mmm
      r.seconds = tokens[0].content.parseInt().uint64
      r.miliseconds = tokens[2].content.parseInt().uint64
    of Colon:
      # MM:SS
      r.seconds =
        ((tokens[0].content.parseInt() * 60) + (tokens[2].content.parseInt())).uint64
    else:
      discard
  of 5:
    if tokens[1].kind == Colon:
      case tokens[3].kind
      of Dot:
        # MM:SS.mmm
        r.seconds =
          ((tokens[0].content.parseInt() * 60) + (tokens[2].content.parseInt())).uint64
        r.miliseconds = tokens[4].content.parseInt().uint64
      of Colon:
        # HH:MM:SS
        r.seconds = (
          (tokens[0].content.parseInt() * 3600) + (tokens[2].content.parseInt() * 60) +
          (tokens[4].content.parseInt())
        ).uint64
      else:
        discard
  of 7:
    if tokens[1].kind == Colon and tokens[3].kind == Colon and tokens[5].kind == Dot:
      # HH:MM:SS.mmm
      r.seconds = (
        (tokens[0].content.parseInt() * 3600) + (tokens[2].content.parseInt() * 60) +
        (tokens[4].content.parseInt())
      ).uint64
      r.miliseconds = tokens[6].content.parseInt().uint64
  else:
    discard
  return r

proc toDate(buffer: string): DateDef =
  var r = DateDef()
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
    r.year = yyyy.parseInt().uint64
  if len(mm) in 1 .. 2:
    r.month = mm.parseInt().uint8
    if r.month > 12:
      # error here
      discard
  if len(dd) in 1 .. 2:
    r.day = dd.parseInt().uint8
    if r.day > 31:
      # error here
      discard
  return r

proc tags_from_buffer*(buffer: cstring): ptr TagContainer {.cdecl, exportc, dynlib.} =
  let newTags = cast[ptr TagContainer](alloc0Impl(sizeof(TagContainer)))
  var
    curtag: TagData
    trackNum = 1

  for line in ($buffer).splitLines():
    var trimmed = line.strip()

    if len(trimmed) == 0:
      continue # skip

    if trimmed[0] == '#': # a comment, this is what we want
      trimmed = trimmed[1 ..^ 1].strip()
      var command: string
      let
        contentBeginAt = trimmed[1 ..^ 1].parseUntil(command, ' ') + 1
        content = trimmed[contentBeginAt ..^ 1].strip()

      case command.toLowerAscii()
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
      of "length":
        curtag.length = content.toTime()
      of "fade":
        curtag.fade = content.toTime()
      of "comment":
        # TODO
        discard
      else:
        discard
      # may not be the most efficient as it reassigns
      # everytime, but it gets the job done
      if trimmed[0] == '@':
        newTags[][0] = curtag
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
          subtuneNum = numStr.parseInt()
        curtag.track = trackNum
        trackNum += 1
        newTags[][subtuneNum.uint64] = curtag
        # reset to "global" tags
        curtag = newTags[][0]
  return newTags

proc unset_tags*(handle: ptr TagContainer): void {.cdecl, exportc, dynlib.} =
  # Zero out the memory before deallocating it
  handle[].`=destroy`()
  dealloc(handle)

proc get_subtune_count*(handle: ptr TagContainer): uint64 {.cdecl, exportc, dynlib.} =
  let subtuneCount = len(handle[])
  case subtuneCount
  of 0:
    return 0'u64
  else:
    return subtuneCount.uint64 - 1

proc get_subtune_order*(
    handle: ptr TagContainer
): ptr OrderDef {.cdecl, exportc, dynlib.} =
  let p = cast[ptr OrderDef](alloc0Impl(sizeof(OrderDef)))
  let
    container = handle[]
    containerSize = (
      let x = len(container)
      if x < 1:
        0
      else:
        x - 1
    ).uint64
  p[].count = containerSize
  if containerSize > 1:
    let orderArray = cast[ptr UncheckedArray[uint64]](alloc0Impl(
      sizeof(uint64) * (len(container) - 1)
    ))
    for subtuneId in container.keys:
      let trackNum = container[subtuneId].track
      if trackNum > 0:
        orderArray[trackNum - 1] = subtuneId
    p[].playlist = orderArray
  return p

proc get_length_of_subtune*(
    handle: ptr TagContainer, subtune: uint64
): int64 {.cdecl, exportc, dynlib.} =
  if subtune > handle.get_subtune_count() or handle == nil:
    return -1
  let tagInfo = handle[][subtune]
  return ((tagInfo.length.seconds * 1000) + tagInfo.length.miliseconds).int64

proc get_fade_length_of_subtune*(
    handle: ptr TagContainer, subtune: uint64
): int64 {.cdecl, exportc, dynlib.} =
  if subtune > handle.get_subtune_count() or handle == nil:
    return -1
  let tagInfo = handle[][subtune]
  return ((tagInfo.fade.seconds * 1000) + tagInfo.fade.miliseconds).int64

proc get_duration_of_subtune*(
    handle: ptr TagContainer, subtune: uint64
): int64 {.cdecl, exportc, dynlib.} =
  let
    stLen = handle.get_length_of_subtune(subtune)
    stFadeLen = handle.get_fade_length_of_subtune(subtune)
  if (stLen < 0) or (stFadeLen < 0):
    return -1
  return stLen + stFadeLen

proc makeCopyOf(s: string): cstring =
  ## The original C++ -> C API uses strdup() to hand out
  ## copies of the string so that whoever calls it can keep it
  ## when the tag handler is destroyed, I might as well
  ## emulate that here.
  let n: cstring = cast[cstring](alloc0Impl(len(s) + 1))
  cast[pointer](n).copyMem(s[0].addr, len(s))
  return n

proc get_album*(
    handle: ptr TagContainer, subtune: uint64
): cstring {.cdecl, exportc, dynlib.} =
  return (if handle == nil: nil else: handle[][subtune].album.makeCopyOf())

proc get_company*(
    handle: ptr TagContainer, subtune: uint64
): cstring {.cdecl, exportc, dynlib.} =
  return (if handle == nil: nil else: handle[][subtune].company.makeCopyOf())

proc get_publisher*(
    handle: ptr TagContainer, subtune: uint64
): cstring {.cdecl, exportc, dynlib.} =
  return (if handle == nil: nil
  else: handle[][subtune].publisher.makeCopyOf()
  )

proc get_artist*(
    handle: ptr TagContainer, subtune: uint64
): cstring {.cdecl, exportc, dynlib.} =
  return (if handle == nil: nil else: handle[][subtune].artist.makeCopyOf())

proc get_composer*(
    handle: ptr TagContainer, subtune: uint64
): cstring {.cdecl, exportc, dynlib.} =
  return (if handle == nil: nil
  else: handle[][subtune].composer.makeCopyOf()
  )

proc get_sequencer*(
    handle: ptr TagContainer, subtune: uint64
): cstring {.cdecl, exportc, dynlib.} =
  return (if handle == nil: nil
  else: handle[][subtune].sequencer.makeCopyOf()
  )

proc get_arranger*(
    handle: ptr TagContainer, subtune: uint64
): cstring {.cdecl, exportc, dynlib.} =
  return (if handle == nil: nil
  else: handle[][subtune].arranger.makeCopyOf()
  )

proc get_engineer*(
    handle: ptr TagContainer, subtune: uint64
): cstring {.cdecl, exportc, dynlib.} =
  return (if handle == nil: nil
  else: handle[][subtune].engineer.makeCopyOf()
  )

proc get_ripper*(
    handle: ptr TagContainer, subtune: uint64
): cstring {.cdecl, exportc, dynlib.} =
  return (if handle == nil: nil else: handle[][subtune].ripper.makeCopyOf())

proc get_tagger*(
    handle: ptr TagContainer, subtune: uint64
): cstring {.cdecl, exportc, dynlib.} =
  return (if handle == nil: nil else: handle[][subtune].tagger.makeCopyOf())

proc get_title*(
    handle: ptr TagContainer, subtune: uint64
): cstring {.cdecl, exportc, dynlib.} =
  return (if handle == nil: nil else: handle[][subtune].title.makeCopyOf())

proc get_comment*(
    handle: ptr TagContainer, subtune: uint64
): cstring {.cdecl, exportc, dynlib.} =
  # TODO
  return cstring""

proc get_copyright*(
    handle: ptr TagContainer, subtune: uint64
): cstring {.cdecl, exportc, dynlib.} =
  return (if handle == nil: nil
  else: handle[][subtune].copyright.makeCopyOf()
  )

proc get_track_num*(
    handle: ptr TagContainer, subtune: uint64
): uint64 {.cdecl, exportc, dynlib.} =
  return (if handle == nil: 0 else: handle[][subtune].track.uint64)

proc get_date*(
    handle: ptr TagContainer, subtune: uint64
): DateDef {.cdecl, exportc, dynlib.} =
  return (if handle == nil: DateDef() else: handle[][subtune].date)

proc get_length*(
    handle: ptr TagContainer, subtune: uint64
): TimeDef {.cdecl, exportc, dynlib.} =
  return (if handle == nil: TimeDef() else: handle[][subtune].length)

proc get_fade*(
    handle: ptr TagContainer, subtune: uint64
): TimeDef {.cdecl, exportc, dynlib.} =
  return (if handle == nil: TimeDef() else: handle[][subtune].fade)
