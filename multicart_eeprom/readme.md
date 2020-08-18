The python 3 script *create_rom.py* compiles an image for the eeprom on the multicart. When using a 512k eeprom like the 39sf040, there’s 8 banks of 16 slots available.

```
Bank0
   ——— 0-game0_1.bin
   ——— 1-game0_2.bin
   ——— …
   ——— e-game0_14.bin
   ——— f-empty.bin
Bank1
   ——— 0-game1_1.bin
   ———…
…
Bank7
   ——— 0-game1_1.bin
   ——— 1-game0_2.bin
   ——— …
   ——— e-game0_14.bin
   ——— f-empty.bin
```

The script will add the appropriate kernel before the game rom. Each 4k slot will have the following structure:
```
$000 - $3ff = kernel
$400 - $7ff = game rom
$800 - $bff = dummy value - this area is not mapped to the cartridge
$c00 - $fff = game rom (only for larger games)
```

The script will decide which kernel to use based on the extension of the game rom:
```
.bin --> standard RCA Studio II kernel
.vic --> Victory/MPA-02/PAL clone kernel
.ch8 --> chip8 kernel by Marcel
```

Note that not all chip8 game will work and even if they do, the keyboard layout may not make much sense.

The script will insert the ROM filename at $a00 - this enables the Emma 02 emulator to display the name of the ROM when using the multicart binary image.
