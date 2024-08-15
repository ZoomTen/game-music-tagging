# SPC

[Original link](https://wiki.superfamicom.org/spc-and-rsn-file-format)

SPC is an audio format for playing native SNES audio.

## SPC File Format

|                           |   Offset |  Size | Description                                                                |
|:--------------------------|:--------:|------:|:---------------------------------------------------------------------------|
|                           | `00000h` |    33 | File header "SNES-SPC700 Sound File Data v0.30"                            |
|                           | `00021h` |     2 | `26`, `26`                                                             |
|                           | `00023h` |     1 | `26` = Header Has ID666 Information<br/>`27` = Header Has No ID666 Tag |
|                           | `00024h` |     1 | Version Minor (i.e. 30)                                                    |
| SPC700 Registers          | `00025h` |     2 | PC                                                                         |
| SPC700 Registers          | `00027h` |     1 | A                                                                          |
| SPC700 Registers          | `00028h` |     1 | X                                                                          |
| SPC700 Registers          | `00029h` |     1 | Y                                                                          |
| SPC700 Registers          | `0002Ah` |     1 | PSW                                                                        |
| SPC700 Registers          | `0002Bh` |     1 | SP (Lower Byte)                                                            |
| SPC700 Registers          | `0002Ch` |     2 | Reserved                                                                   |
| ID666 Tag (Text Format)   | `0002Eh` |    32 | Song Title                                                                 |
| ID666 Tag (Text Format)   | `0004Eh` |    32 | Game Title                                                                 |
| ID666 Tag (Text Format)   | `0006Eh` |    16 | Name of Dumper                                                             |
| ID666 Tag (Text Format)   | `0007Eh` |    32 | Comments                                                                   |
| ID666 Tag (Text Format)   | `0009Eh` |    11 | Date SPC was Dumped (MM/DD/YYYY)                                           |
| ID666 Tag (Text Format)   | `000A9h` |     3 | Number of Seconds to Play Song before Fading Out                           |
| ID666 Tag (Text Format)   | `000ACh` |     5 | Length of Fade in Milliseconds                                             |
| ID666 Tag (Text Format)   | `000B1h` |    32 | Artist of Song                                                             |
| ID666 Tag (Text Format)   | `000D1h` |     1 | Default Channel Disables<br/>0 = Enable<br/>1 = Disable                    |
| ID666 Tag (Text Format)   | `000D2h` |     1 | Emulator used to dump SPC<br/>0 = Unknown<br/>1 = ZSNES<br/>2 = Snes9x     |
| ID666 Tag (Text Format)   | `000D3h` |    45 | Reserved (Set to `0x00`)                                                   |
| ID666 Tag (Binary Format) | `0002Eh` |    32 | Song Title                                                                 |
| ID666 Tag (Binary Format) | `0004Eh` |    32 | Game Title                                                                 |
| ID666 Tag (Binary Format) | `0006Eh` |    16 | Name of Dumper                                                             |
| ID666 Tag (Binary Format) | `0007Eh` |    32 | Comments                                                                   |
| ID666 Tag (Binary Format) | `0009Eh` |     4 | Date SPC was Dumped (YYYYMMDD)                                             |
| ID666 Tag (Binary Format) | `000A2h` |     7 | Unused                                                                     |
| ID666 Tag (Binary Format) | `000A9h` |     3 | Number of Seconds to Play Song before Fading Out (Numeric string)          |
| ID666 Tag (Binary Format) | `000ACh` |     4 | Length of Fade in Milliseconds (Numeric string)                            |
| ID666 Tag (Binary Format) | `000B0h` |    32 | Artist of Song                                                             |
| ID666 Tag (Binary Format) | `000D0h` |     1 | Default Channel Disables<br/>0 = Enable<br/>1 = Disable                    |
| ID666 Tag (Binary Format) | `000D1h` |     1 | Emulator used to dump SPC<br/>0 = Unknown<br/>1 = ZSNES<br/>2 = Snes9x     |
| ID666 Tag (Binary Format) | `000D2h` |    46 | Reserved (Set to `0x00`)                                                   |
|                           | `00100h` | 65536 | 64KB RAM                                                                   |
|                           | `10100h` |   128 | DSP Registers                                                              |
|                           | `10180h` |    64 | Unused                                                                     |
|                           | `101C0h` |    64 | Extra RAM (Memory Region used when the IPL ROM region is set to read-only) |
|                           | `10200h` |       | [Extended ID666](#extended-id666)                                          |

## Extended ID666 Format

Extended information is stored at the end of the SPC file as a RIFF chunk with an ID of `xid6`.  Items that can be stored in the ID666 tag without any loss of data should not be stored in the extended area.

| Offset | Size | Description                      |
|:------:|:----:|:---------------------------------|
| 0      | 4    | Chunk type, ie `xid6`            |
| 4      | 4    | Chunk size, not including header |


### Sub-Chunk Header

Inside the chunk are sub-chunks.  Each sub-chunk consists of a 4-byte header, and possibly data.  All data is 32-bit aligned.  If the data stored doesn't reach a 32-bit boundary, it will be padded with 0's.

| Offset | Size | Description                      
|:------:|:----:|:----------------------------|
| 0      |    1 | ID: Song Name, Length, etc. |
| 1      |    1 | Type<br/>0 = Data is stored in the header<br/>Non-Zero = data is stored after header |
| 2      |    2 | Data<br/>If 'Type' = Zero, this contains the data<br/>If 'Type' is non-zero, this contains the length of the data following the header |


### Extended ID666 Items

    ID:   00-0F - Items from original ID666 tag
          10-1F - Extended items
          30-3F - Items related to playback
    
    Type: 00 Data    - Data is saved in the 'data' item of the sub-chunk header.
          01 String  - Data is stored as a null terminated string (max 256 characters including null). 
                       Strings are saved using 8-bit character encoding.
                       Support for unicode may be added in the future.
          04 Integer - Data is stored as a 32-bit integer.

Size: The minimum and maximum sizes of an item.

|  ID |    Type |  Size |  Description                                                                         |
|:----|:--------|------:|:-------------------------------------------------------------------------------------|
| 01h | String  | 4-256 | Song Name                                                                            |
| 02h | String  | 4-256 | Game Name                                                                            |
| 03h | String  | 4-256 | Artist's Name                                                                        |
| 04h | String  | 4-256 | Dumper's Name                                                                        |
| 05h | Integer | 4     | Date Song was Dumped (stored as YYYYMMDD)                                            |
| 06h | Data    | 1     | Emulator Used                                                                        |
| 07h | String  | 4-256 | Comments                                                                             |
| 10h | String  | 4-256 | Official Soundtrack Title                                                            |
| 11h | Data    | 1     | OST Disc                                                                             |
| 12h | Data    | 2     | OST Track (upper byte is the number 0-99, lower byte is an optional ASCII character) |
| 13h | String  | 4-256 | Publisher's Name                                                                     |
| 14h | Data    | 2     | Copyright Year                                                                       |
| 30h | Integer | 4     | Introduction Length (lengths are stored in 1/64000th seconds)                        |
| 31h | Integer | 4     | Loop Length                                                                          |
| 32h | Integer | 4     | End Length                                                                           |
| 33h | Integer | 4     | Fade Length                                                                          |
| 34h | Data    | 1     | Muted Voices (a bit is set for each voice that's muted)                              |
| 35h | Data    | 1     | Number of Times to Loop                                                              |
| 36h | Data    | 1     | Mixing (Preamp) Level                                                                |

Lengths are stored in ticks.  A tick is 1/64000th of a second. The maximum length is 383999999 ticks. The End can contain a negative value.

This may seem like a messy way to implement a format, but I wanted to assure something that would be easily expandable. The source code to [SNESAmp](http://www.alpha-ii.com) contains a C++ class for reading and writing ID666 and xid6 tags.

### Addendum
* Fixed the number of reserved bytes at offset `0002Ch`
* Changed the version number to v0.31
* Detecting the format of the SPC file (binary or text) can be a bit tricky, since fields might contain ambiguous values and there's no format indicator. Some fields have different offsets depending of the format, which might help with the detection. The current Win32 version of ZSNES saves binary SPC files; SNES9x saves in text format.

# RSN
RSN is a packed/grouped format for SPC files.

## Solid Archives
Often, SPC files will only differ by a few KB, with the rest bit-identical. For example, several songs might all use the same playback code and set of samples, with only the module data (or even just the playback pointer) differing between songs. But because PKZIP always compresses each file as a separate object, it can't take advantage of similarities between files. Even tar.gz, which combines the files before compressing them into a "solid archive", could only reference data from the past 32768 bytes. RAR was the first widely used general data compression program that used a window bigger than a single SPC file, so SPC sets were often distributed as RAR files. 

## Why Specifically RAR
The one disadvantage of RAR is that it is not free software due to a license term prohibiting reverse-engineering the format to create a description of the compression method:
> The UnRAR sources may be used in any software to handle RAR
> archives without limitations free of charge, but cannot be used
> to re-create the RAR compression algorithm, which is proprietary.
> Distribution of modified UnRAR sources in separate form or as a
> part of other software is permitted, provided that it is clearly
> stated in the documentation and source comments that the code may
> not be used to develop a RAR (WinRAR) compatible archiver.

7-Zip and tar.bz2 are free alternatives to RAR, but RAR had the first mover advantage, and music archives that already deal in non-free music have seen no need to repack in a free format. 

## Why the Rename to RSN
Windows Explorer (the file manager in Windows) uses the part of the filename after the last period character ("the extension") to determine file type. That's why various file formats based on PKZIP archives have different extensions: .zip for general data, .jar for Java applications, .smzip for StepMania, .cbz for sequences of JPEG or PNG images ("comic book zip"), .odt for OASIS text documents, .ods for OASIS spreadsheets, etc. Likewise, RAR has .rar for general data, .cbr for image sequences, and .rsn (RAR Super Nintendo) for sets of SPC files. 

The first player to support RSN files was Winamp, which does not support playing files from archives in general. Input plug-ins for archives exist; they unpack an archive's contents to a temporary folder and then open the resulting files with the correct input plug-in.

Players such as foobar2000 do not require the RSN hack since the player itself can support RAR files directly. Of course, you probably don't want to associate RAR files with the audio player, but you can still drag the archives onto the player, as well as open them within the player, and they can contain more than just SPC files. Although most general purpose audio formats are already compressed, so they don't really benefit from being stuffed into other archive formats.

_RSN Descriptions written by Tepples and kode54_

_SPC File Format v0.31 by creaothceann, 2006-02-27 21:48_