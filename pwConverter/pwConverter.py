#!/usr/bin/python

import sys
import struct
import os
from tempfile import mkstemp
from getpass import getpass

import argparse


key_hex_codes = {
    "NONE":     0x00,
    "ERR_OVF":  0x01,
    "A":    0x04,
    "B":    0x05,
    "C":    0x06,
    "D":    0x07,
    "E":    0x08,
    "F":    0x09,
    "G":    0x0a,
    "H":    0x0b,
    "I":    0x0c,
    "J":    0x0d,
    "K":    0x0e,
    "L":    0x0f,
    "M":    0x10,
    "N":    0x11,
    "O":    0x12,
    "P":    0x13,
    "Q":    0x14,
    "R":    0x15,
    "S":    0x16,
    "T":    0x17,
    "U":    0x18,
    "V":    0x19,
    "W":    0x1a,
    "X":    0x1b,
    "Y":    0x1c,
    "Z":    0x1d,
    "1":    0x1e,
    "2":    0x1f,
    "3":    0x20,
    "4":    0x21,
    "5":    0x22,
    "6":    0x23,
    "7":    0x24,
    "8":    0x25,
    "9":    0x26,
    "0":    0x27,
    "ENTER":    0x28,
    "ESC":  0x29,
    "BACKSPACE":    0x2a,
    "TAB":  0x2b,
    "SPACE":    0x2c,
    "MINUS":    0x2d,
    "EQUAL":    0x2e,
    "LEFTBRACE":    0x2f,
    "RIGHTBRACE":   0x30,
    "BACKSLASH":    0x31,
    "HASHTILDE":    0x32,
    "SEMICOLON":    0x33,
    "APOSTROPHE":   0x34,
    "GRAVE":    0x35,
    "COMMA":    0x36,
    "DOT":  0x37,
    "SLASH":    0x38,
    "CAPSLOCK":     0x39,
    "F1":   0x3a,
    "F2":   0x3b,
    "F3":   0x3c,
    "F4":   0x3d,
    "F5":   0x3e,
    "F6":   0x3f,
    "F7":   0x40,
    "F8":   0x41,
    "F9":   0x42,
    "F10":  0x43,
    "F11":  0x44,
    "F12":  0x45,
    "SYSRQ":    0x46,
    "SCROLLLOCK":   0x47,
    "PAUSE":    0x48,
    "INSERT":   0x49,
    "HOME":     0x4a,
    "PAGEUP":   0x4b,
    "DELETE":   0x4c,
    "END":  0x4d,
    "PAGEDOWN":     0x4e,
    "RIGHT":    0x4f,
    "LEFT":     0x50,
    "DOWN":     0x51,
    "UP":   0x52,
    "NUMLOCK":  0x53,
    "KPSLASH":  0x54,
    "KPASTERISK":   0x55,
    "KPMINUS":  0x56,
    "KPPLUS":   0x57,
    "KPENTER":  0x58,
    "KP1":  0x59,
    "KP2":  0x5a,
    "KP3":  0x5b,
    "KP4":  0x5c,
    "KP5":  0x5d,
    "KP6":  0x5e,
    "KP7":  0x5f,
    "KP8":  0x60,
    "KP9":  0x61,
    "KP0":  0x62,
    "KPDOT":    0x63,
    "102ND":    0x64,
    "COMPOSE":  0x65,
    "POWER":    0x66,
    "KPEQUAL":  0x67,
    "F13":  0x68,
    "F14":  0x69,
    "F15":  0x6a,
    "F16":  0x6b,
    "F17":  0x6c,
    "F18":  0x6d,
    "F19":  0x6e,
    "F20":  0x6f,
    "F21":  0x70,
    "F22":  0x71,
    "F23":  0x72,
    "F24":  0x73,
    "OPEN":     0x74,
    "HELP":     0x75,
    "PROPS":    0x76,
    "FRONT":    0x77,
    "STOP":     0x78,
    "AGAIN":    0x79,
    "UNDO":     0x7a,
    "CUT":  0x7b,
    "COPY":     0x7c,
    "PASTE":    0x7d,
    "FIND":     0x7e,
    "MUTE":     0x7f,
    "VOLUMEUP":     0x80,
    "VOLUMEDOWN":   0x81,
    "KPCOMMA":  0x85,
    "RO":   0x87,
    "KATAKANAHIRAGANA":     0x88,
    "YEN":  0x89,
    "HENKAN":   0x8a,
    "MUHENKAN":     0x8b,
    "KPJPCOMMA":    0x8c,
    "HANGEUL":  0x90,
    "HANJA":    0x91,
    "KATAKANA":     0x92,
    "HIRAGANA":     0x93,
    "ZENKAKUHANKAKU":   0x94,
    "KPLEFTPAREN":  0xb6,
    "KPRIGHTPAREN":     0xb7,
    "LEFTCTRL":     0xe0,
    "LEFTSHIFT":    0xe1,
    "LEFTALT":  0xe2,
    "LEFTMETA":     0xe3,
    "RIGHTCTRL":    0xe4,
    "RIGHTSHIFT":   0xe5,
    "RIGHTALT":     0xe6,
    "RIGHTMETA":    0xe7,
    "MEDIA_PLAYPAUSE":  0xe8,
    "MEDIA_STOPCD":     0xe9,
    "MEDIA_PREVIOUSSONG":   0xea,
    "MEDIA_NEXTSONG":   0xeb,
    "MEDIA_EJECTCD":    0xec,
    "MEDIA_VOLUMEUP":   0xed,
    "MEDIA_VOLUMEDOWN":     0xee,
    "MEDIA_MUTE":   0xef,
    "MEDIA_WWW":    0xf0,
    "MEDIA_BACK":   0xf1,
    "MEDIA_FORWARD":    0xf2,
    "MEDIA_STOP":   0xf3,
    "MEDIA_FIND":   0xf4,
    "MEDIA_SCROLLUP":   0xf5,
    "MEDIA_SCROLLDOWN":     0xf6,
    "MEDIA_EDIT":   0xf7,
    "MEDIA_SLEEP":  0xf8,
    "MEDIA_COFFEE":     0xf9,
    "MEDIA_REFRESH":    0xfa,
    "MEDIA_CALC":   0xfb,
}

german_keymap = {
    "^" : ["GRAVE"],
    "°" : ["LEFTSHIFT", "GRAVE"],
    "1" : ["1"],
    "!" : ["LEFTSHIFT", "1"],
    "2" : ["2"],
    '"' : ["LEFTSHIFT", "2"],
    "²" : ["RIGHTALT", "2"],
    "3" : ["3"],
    "§" : ["LEFTSHIFT", "3"],
    "³" : ["RIGHTALT", "3"],
    "4" : ["4"],
    "$" : ["LEFTSHIFT","4"],
    "5" : ["5"],
    "%" : ["LEFTSHIFT", "5"],
    "6" : ["6"],
    "&" : ["LEFTSHIFT","6"],
    "7" : ["7"],
    "/" : ["LEFTSHIFT","7"],
    "{" : ["RIGHTALT","7"],
    "8" : ["8"],
    "(" : ["LEFTSHIFT","8"],
    "[" : ["RIGHTALT","8"],
    "9" : ["9"],
    ")" : ["LEFTSHIFT","9"],
    "]" : ["RIGHTALT","9"],
    "0" : ["0"],
    "=" : ["LEFTSHIFT","0"],
    "}" : ["RIGHTALT","0"],
    "ß" : ["MINUS"],
    "?" : ["LEFTSHIFT", "MINUS"],
    "\\": ["RIGHTALT", "MINUS"],
    "\t": ["TAB"],
    "\n": ["ENTER"],
    "q" : ["Q"],
    "Q" : ["LEFTSHIFT", "Q"],
    "@" : ["RIGHTALT", "Q"],
    "w" : ["W"],
    "W" : ["LEFTSHIFT","W"],
    "e" : ["E"],
    "E" : ["LEFTSHIFT", "E"],
    "€" : ["RIGHTALT", "E"],
    "r" : ["R"],
    "R" : ["LEFTSHIFT","R"],
    "t" : ["T"],
    "T" : ["LEFTSHIFT","T"],
    "z" : ["Y"],
    "Z" : ["LEFTSHIFT","Y"],
    "u" : ["U"],
    "U" : ["LEFTSHIFT","U"],
    "i" : ["I"],
    "I" : ["LEFTSHIFT","I"],
    "o" : ["O"],
    "O" : ["LEFTSHIFT","O"],
    "p" : ["P"],
    "P" : ["LEFTSHIFT","P"],
    "ü" : ["LEFTBRACE"],
    "Ü" : ["LEFTSHIFT","LEFTBRACE"],
    "+" : ["RIGHTBRACE"],
    "*" : ["LEFTSHIFT","RIGHTBRACE"],
    "~" : ["LEFTALT", "RIGHTBRACE"],
    "a" : ["A"],
    "A" : ["LEFTSHIFT","A"],
    "s" : ["S"],
    "S" : ["LEFTSHIFT","S"],
    "d" : ["D"],
    "D" : ["LEFTSHIFT","D"],
    "f" : ["F"],
    "F" : ["LEFTSHIFT","F"],
    "g" : ["G"],
    "G" : ["LEFTSHIFT","G"],
    "h" : ["H"],
    "H" : ["LEFTSHIFT","H"],
    "j" : ["J"],
    "J" : ["LEFTSHIFT","J"],
    "k" : ["K"],
    "K" : ["LEFTSHIFT","K"],
    "l" : ["L"],
    "L" : ["LEFTSHIFT","L"],
    "ö" : ["SEMICOLON"],
    "Ö" : ["SEMICOLON"],
    "ä" : ["APOSTROPHE"],
    "Ä" : ["LEFTSHIFT","APOSTROPHE"],
    "#" : ["HASHTILDE"],
    "'" : ["LEFTSHIFT","HASHTILDE"],
    "<" : ["BACKSLASH"],
    ">" : ["LEFTSHIFT","BACKSLASH"],
    "|" : ["RIGHTALT","BACKSLASH"],
    "y" : ["Z"],
    "Y" : ["LEFTSHIFT","Z"],
    "x" : ["X"],
    "X" : ["LEFTSHIFT","X"],
    "c" : ["C"],
    "C" : ["LEFTSHIFT","C"],
    "v" : ["V"],
    "V" : ["LEFTSHIFT","V"],
    "b" : ["B"],
    "B" : ["LEFTSHIFT","B"],
    "n" : ["N"],
    "N" : ["LEFTSHIFT","N"],
    "m" : ["M"],
    "M" : ["LEFTSHIFT","M"],
    "µ" : ["RIGHTALT","µ"],
    "," : ["COMMA"],
    ";" : ["LEFTSHIFT","COMMA"],
    "." : ["DOT"],
    ":" : ["LEFTSHIFT","DOT"],
    "-" : ["SLASH"],
    "_" : ["LEFTSHIFT","SLASH"],
    " " : ["SPACE"]
}

def getKeySequence(inputKey):
    sequence = german_keymap[inputKey]
    keys = [key_hex_codes[x] for x in sequence]
    return keys

def main(arguments):
    needDeleteTmp = False
    if arguments.input is None:
        needDeleteTmp = True
        _, arguments.input = mkstemp()
        with open(arguments.input, "w") as input:
            input.write(
                getpass("Enter key sequence: "))
        compare_sequence = getpass("Enter sequence again to confirm: ")
        with open(arguments.input, "r") as input:
            if compare_sequence != input.read():
                print("Error: sequences do not match", file=sys.stderr)
                os.remove(arguments.input)
                return -1

    try: # This makes sure we delete the temporary file in case anything goes wrong
        with open(arguments.input,"r") as input:
            with open(arguments.output,"wb") as output:
                for line in input:
                    for char in line:
                        keyCodes = getKeySequence(char)
                        # Press the keys
                        for i in keyCodes:
                            output.write(struct.pack("@BB",0x80,i))

                        output.write(struct.pack("@BB", 0x40, 0))
                        # Release the keys
                        for i in keyCodes:
                            output.write(struct.pack("@BB",0x00,i))

                        output.write(struct.pack("@BB", 0x40, 0))

        if needDeleteTmp:
            os.remove(arguments.input)

    except BaseException as e:
        if needDeleteTmp:
            os.remove(arguments.input)
        raise(e)

    return 0


if __name__=="__main__":
    parser = argparse.ArgumentParser(
        prog="PasswordConverter",
        description="Converts a character sequence to a keyfile"
    )
    parser.add_argument("--input", "-i", metavar="FILE",
                        help="file to be converted to keystroke (if left blank, you will be prompted for a key sequence)")
    parser.add_argument("--output", "-o", metavar="FILE", default="out.key",
                        help="keyfile to be generated (default: %(default)s)")
    exit(main(parser.parse_args()))
