import unittest2
import libgmtag

proc timeToMs(h, m, s, ms: int): int64 =
  return (((s + (m * 60) + (h * 3600)) * 1000) + ms).int64

suite "Katakis 3D m3u":
  setup:
    const m3uStr = staticRead("samples/katakis_3d.m3u")
    var tags = tagsFromBuffer(m3uStr)

  test "album":
    for i in 0 .. 18:
      check tags.getAlbum(i.uint64) == cstring"Katakis 3D"

  test "company":
    for i in 0 .. 18:
      check tags.getCompany(i.uint64) == cstring"Similis"

  test "ripper":
    for i in 0 .. 18:
      check tags.getRipper(i.uint64) == cstring"zlago"

  test "tagger":
    for i in 0 .. 18:
      check tags.getTagger(i.uint64) == cstring"Zumi, zlago"

  test "year":
    for i in 0 .. 18:
      check tags.getDate(i.uint64).year == 2001'u64

  test "artist":
    for i in 0 .. 18:
      case i
      of 1:
        check tags.getArtist(i.uint64) == cstring"Chris Huelsbeck, Tufan Uysal"
      else:
        check tags.getArtist(i.uint64) == cstring"Tufan Uysal"

  test "title and other metadata":
    let albumData =
      @[
        (1, cstring"Katakis (Remix)", timeToMs(0, 3, 17, 0)),
        (2, cstring"Loud 'n Proud", timeToMs(0, 0, 53, 0)),
        (3, cstring"Flight to Hell", timeToMs(0, 1, 18, 0)),
        (4, cstring"The Big Thing", timeToMs(0, 0, 49, 0)),
        (5, cstring"Electrical Motions", timeToMs(0, 1, 31, 0)),
        (6, cstring"Protected Beat", timeToMs(0, 0, 24, 0)),
        (7, cstring"Secret Cycles", timeToMs(0, 2, 2, 0)),
        (8, cstring"Radioactive Attack", timeToMs(0, 0, 42, 0)),
        (9, cstring"Enforcer", timeToMs(0, 1, 30, 0)),
        (10, cstring"Someone Wanna Party", timeToMs(0, 0, 50, 0)),
        (11, cstring"Rasit's Spiritual Dreams", timeToMs(0, 1, 18, 0)),
        (12, cstring"Oriental Danger", timeToMs(0, 0, 50, 0)),
        (13, cstring"Boomin' Back Katakis", timeToMs(0, 4, 1, 0)),
        (14, cstring"Master of Universe", timeToMs(0, 1, 14, 0)),
        (15, cstring"The Impregnable", timeToMs(0, 1, 1, 0)),
        (16, cstring"30 Seconds to Go...", timeToMs(0, 0, 45, 0)),
        (17, cstring"Beyond the Stars", timeToMs(0, 4, 27, 0)),
        (18, cstring"Crush Boom Bang", timeToMs(0, 0, 7, 0)),
      ]
    for i in albumData:
      check tags.getTitle(i[0].uint64) == i[1]
      check tags.getDurationOfSubtune(i[0].uint64) == i[2]
      # special check for "Secret Cycles"
      if i[0] == 7:
        check tags.getLengthOfSubtune(7'u64) == timeToMs(0, 1, 52, 0)
        check tags.getFadeLengthOfSubtune(7'u64) == timeToMs(0, 0, 10, 0)

  teardown:
    tags.unsetTags()

suite "Badly-formatted m3u":
  setup:
    const m3uStr = staticRead("samples/weird.m3u")

  test "parse and read":
    skip()
    #debugEcho m3uStr

suite "Spec check":
  setup:
    const m3uStr = staticRead("samples/spectest.m3u")

  test "parse and read":
    skip()
    #debugEcho m3uStr
