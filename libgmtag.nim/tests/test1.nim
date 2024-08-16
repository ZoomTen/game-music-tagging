import unittest2
import libgmtag

when defined(showStruct):
  import pretty

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
      check:
        tags.getTitle(i[0].uint64) == i[1]
        tags.getDurationOfSubtune(i[0].uint64) == i[2]
      # special check for "Secret Cycles"
      if i[0] == 7:
        check:
          tags.getLengthOfSubtune(7'u64) == timeToMs(0, 1, 52, 0)
          tags.getFadeLengthOfSubtune(7'u64) == timeToMs(0, 0, 10, 0)

  test "playlist order":
    let order = tags.getSubtuneOrder()
    for i in 0 .. order[].count:
      check order[].playlist[][i] == i

  teardown:
    tags.unsetTags()

suite "Badly-formatted m3u":
  setup:
    const m3uStr = staticRead("samples/weird.m3u")
    var tags = tagsFromBuffer(m3uStr)

  test "album":
    for i in [0, 1, 2, 3, 6]:
      check tags.getAlbum(i.uint64) == cstring"Zumi's GB Music Engine v1"

  test "company":
    for i in [0, 1, 2, 3, 6]:
      check tags.getCompany(i.uint64) == cstring"Zumi"

  test "playlist order and track number":
    let
      order = tags.getSubtuneOrder()
      count = tags.getSubtuneCount()
      expected =
        @[
          (0'u64, 0'u64),
          (1'u64, 1'u64),
          (2'u64, 12'u64),
          (3'u64, 2'u64),
          (6'u64, 3'u64),
        ]
    for i in 0 .. count:
      check:
        order[].playlist[][i] == expected[i][0]
        tags.getTrackNum(expected[i][0]) == expected[i][1]

  teardown:
    tags.unsetTags()

suite "Spec check":
  setup:
    const m3uStr = staticRead("samples/spectest.m3u")
    var tags = tagsFromBuffer(m3uStr)

  test "global tag":
    check:
      tags.getAlbum(0'u64) == cstring"Something"
      tags.getArranger(0'u64) == cstring"C"

  test "subtune 1":
    check:
      tags.getArtist(1'u64) == cstring"Why would you do this?"
      tags.getTitle(1'u64) == cstring "Music A"
      tags.getAlbum(1'u64) == cstring"Something"
      tags.getArranger(1'u64) == cstring"C"
      tags.getTrackNum(1'u64) == 1

  test "subtune 2":
    check:
      tags.getArtist(2'u64) == cstring""
      tags.getTitle(2'u64) == cstring "Music B"
      tags.getAlbum(2'u64) == cstring"Something"
      tags.getArranger(2'u64) == cstring"C"
      tags.getTrackNum(2'u64) == 2

  test "subtune 5":
    check:
      tags.getTitle(5'u64) == cstring "B"
      tags.getAlbum(5'u64) == cstring"B"
      tags.getCompany(5'u64) == cstring"B"
      tags.getPublisher(5'u64) == cstring"B"
      tags.getArtist(5'u64) == cstring"B"
      tags.getComposer(5'u64) == cstring"B"
      tags.getArranger(5'u64) == cstring"A"
      tags.getSequencer(5'u64) == cstring"B"
      tags.getEngineer(5'u64) == cstring"B"
      tags.getRipper(5'u64) == cstring"B"
      tags.getTagger(5'u64) == cstring"B"
      tags.getCopyright(5'u64) == cstring"B"
      tags.getTrackNum(5'u64) == 0

  when defined(showStruct):
    test "show structure":
      for i in tags[].keys:
        print(i)
        print(tags[][i])
      skip()

  teardown:
    tags.unsetTags()
