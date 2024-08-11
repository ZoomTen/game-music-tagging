import std/tables
export tables

type
  # mandated by C API
  TimeDef* = object
    seconds*, miliseconds*: uint64

  DateDef* = object
    year*: uint64
    month*, day*: uint8
    pad: array[0 .. 5, uint8]

  OrderDef* = object
    count*: uint64
    playlist*: ptr UncheckedArray[uint64]

  # implementation defined
  TagData* = object
    track*: int
    album*, company*, publisher*, artist*, composer*, arranger*, sequencer*, engineer*,
      ripper*, tagger*, title*, comments*, copyright*: string
    date*: DateDef
    length*, fade*: TimeDef

  TagContainer* = Table[uint64, TagData]
