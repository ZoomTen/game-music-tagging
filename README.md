# !tags.m3u proposal for retro soundtrack rips

(One-file-to-rule-them-all; if possible)

## Abstract

> Tagging is a long standing issue that has plagued old sequenced rips for over the last 20 years. Typically rips for NSF, GBS, etc. are shared in standalone files. You are forced to cycle through the subtunes to find that one song you want to listen to. Additionally, they have a default time like 5 minutes or 3 minutes. There's a few formats like NSFE that address the time issue (but vary in granularity), but each format requires a custom tool, a new format (some of which die an unceremonious death like GBSX), and still don't tackle the issue of subtune cycling.

https://github.com/libgme/game-music-emu/issues/85

## Existing extended tagging formats

### Embedded

<dl>
  <dt>VGM / VGZ (multi-system, usually Sega)</dt>
  <dd>GD3 format, analogous to MP3’s ID3; see https://vgmrips.net/wiki/GD3_Specification<br>One file each track.</dd>
  
  <dt>SPC / RSN (SNES)</dt>
  <dd>ID666; see https://wiki.superfamicom.org/spc-and-rsn-file-format<br>One file each track.</dd>
  
  <dt>SNSF (SNES)</dt>
  <dd>Implements PSF-style tagging; see http://snsf.caitsith2.net/snsf%20spec.txt and https://gist.githubusercontent.com/SaxxonPike/a0b47f8579aad703b842001b24d40c00/raw/a6fa28b44fb598b8874923dbffe932459f6a61b9/psf_format.txt</dd>
  
  <dt>NSFE (NES)</dt>
  <dd>RIFF with embedded NSF; see https://www.nesdev.org/wiki/NSFe</dd>
  
  <dt>NSF2 (NES)</dt>
  <dd>NSF with embedded RIFF; see https://www.nesdev.org/wiki/NSF2</dd>
  
  <dt>GBSX (Game Boy 8-bit)</dt>
  <dd>Discontinued format, can either be embedded or as sidecar; see https://raw.githubusercontent.com/mmitch/gbsplay/05b68b8ff0b1faa6a2f1c987a34c564b28cde1fe/gbsformat.txt</dd>
</dl>

### Requires sidecar

<dl>
  <dt>GBS (Game Boy 8-bit)</dt>
  <dd>No intended standard (and no room for it, either); existing HCS64 rips use Knurek’s comments format in conjunction with the NEZPlug++ tagging format; see https://forums.bannister.org/ubbthreads.php?ubb=showflat&Number=78196#Post78196 and https://nezplug.sourceforge.net/in_nez.txt<br>One of two formats parsed by Game_Music_Emu.</dd>
  
  <dt>SID (Commodore 64)</dt>
  <dd>ID666; see https://wiki.superfamicom.org/spc-and-rsn-file-format<br>One file each track.</dd>
  
  <dt>SNSF (SNES)</dt>
  <dd>Refers to the STIL (SID Tune Information List): https://www.hvsc.c64.org/download/C64Music/DOCUMENTS/STIL.txt
Refers to Songlength.md5; see https://www.hvsc.c64.org/download/C64Music/DOCUMENTS/Songlength.md5</dd>
</dl>

## M3U sidecar formats supported by Game_Music_Emu

