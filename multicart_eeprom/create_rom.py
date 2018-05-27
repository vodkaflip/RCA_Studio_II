#!/usr/bin/python
import glob

game_dirs = [r'bank0/*.[bcv][ih][cn8]',r'bank1/*.[vbc][ih][cn8]',r'bank2/*.[bcv][ih][cn8]',r'bank3/*.[bcv][ih][cn8]',r'bank4/*.[vbc][ih][cn8]',r'bank5/*.[vbc][ih][cn8]',r'bank6/*.[vbc][ih][cn8]',r'bank7/*.[vbc][ih][cn8]']

kernel = data = newgame = bytearray()
y = 0
slot = 0
fn = ""
# cycle through 8 banks
for y in range(8):
    game_list = sorted(glob.glob(game_dirs[y]))
    print("--------- Bank: ",y)
    for z in range(16):
    # decide which kernel to use in the first 1k
    # if we want to show an empty slot, use studio2 or victory
    # else use the one defined in kernel_list
        if game_list[z][-3:] == 'bin':
            source = open("studio2.rom","rb")
        if game_list[z][-3:] == 'vic':
            source = open("victory.rom","rb")
        if game_list[z][-3:] == 'ch8':
            source = open("chip8.bin","rb")
        # read the kermel
        kernel = source.read()
        source.close()
        # add the kernel to the buffer
        data += kernel
        # read the game rom
        with open(game_list[z], "rb") as game:
            newgame = game.read()
            game.close()
            # determine total length so far
            l = len(kernel) + len(newgame)
            print(game_list[z])
            # add the game to the buffer
            data += newgame
            # fill 4kb of data
            for dummy in range(0,0x1000-l):
                data += chr(1).encode('ascii')
            #put the game name at 0xXA00, with the correct extension (st2 or ch8)
            namepos = slot * 0x1000 + 0xA00
            if game_list[z][-3:] == 'bin':
                game_list[z] = (game_list[z][:-3] + 'st2')[8:]
            else:
                game_list[z] = game_list[z][8:]
            #data[namepos:(namepos-7+len(game_list[z]))] = game_list[z][8:].encode('ascii')
            #print(game_list[z])
            data[namepos:(namepos+len(game_list[z]))] = game_list[z].encode('ascii')
            slot += 1
f_out = open('39sf040.auto.bin', 'wb+')
f_out.write(data)
f_out.close()