The python 3 script create_rom.py compiles an image for the eeprom on the multicart. When using a 29sf040 eeprom, there’s 8 banks of 16 slots available.

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
$800 - $bff = dummy value
$c00 - $fff = game rom (only for larger games)
```

The script will decide which kernel to use based on the extension of the game rom:
```
.bin --> standard RCA Studio II kernel
.vic --> Victory/MPA-02/PAL clone kernel
.ch8 --> chip8 kernel by Marcel
```

Note that not all chip8 game will work and even if they do, the keyboard layout may not make much sense.
