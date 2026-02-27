[root@sjac pcie]# cat check_aer_read_only.sh
#!/usr/bin/bash

# trap ctrl-c and call ctrl_c()
trap ctrl_c INT

function ctrl_c() {
        printf "\n\nInterrupted at iteration: %d\n" $cnt
        printf "Total EP AER error count: %d\n" $ep_aer_err_cnt
        printf "Total EP UAER error count: %d\n" $ep_uaer_err_cnt
        printf "Total RC AER error count: %d\n" $rc_aer_err_cnt
        printf "Total RC UAER error count: %d\n" $rc_uaer_err_cnt
        printf "Total EP lane error count: %d\n" $lane_error_cnt
        printf "Total EP link down error count: %d\n" $link_down_err_cnt
        return 0
}

rate_decode () {
        if [ $1 == "gen5" ]; then
                return_val=4
        elif [ $1 == "gen4" ]; then
                return_val=3
        elif [ $1 == "gen3" ]; then
                return_val=2
        elif [ $1 == "gen2" ]; then
                return_val=1
        elif [ $1 == "gen1" ]; then
                return_val=0
        else
                return_val=10
                return
        fi
}

if [ "$#" -ne 2 ]; then
    echo "Usage: $0 BDF rate_string count. Ex: $0 01:00.0 10000"
    echo "Usage: Where BDF is in the form of xx:xx.x "
    return
fi

98:00.0 Non-VGA unclassified device: Cadence Design Systems, Inc. Device 0100
00: cd 17 00 01 00 00 10 00 00 00 00 00 00 00 00 00
10: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
20: 00 00 00 00 00 00 00 00 00 00 00 00 cd 17 00 00
30: 02 00 00 00 80 00 00 00 00 00 00 00 ff 01 00 00
40: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
50: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
60: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
70: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
80: 01 90 03 5a 08 00 00 00 03 90 00 00 00 00 00 00
90: 05 b0 8a 01 00 00 00 00 00 00 00 00 00 00 00 00
a0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
b0: 11 c0 01 00 40 00 00 00 40 00 00 00 00 00 00 00
c0: 10 00 02 80 22 81 02 10 10 29 01 00 46 a8 41 00
d0: 00 00 45 00 00 00 00 00 00 00 00 00 00 00 00 00
e0: 00 00 00 00 1f 18 75 00 00 00 00 00 7e 00 80 81
f0: 06 00 1e 00 00 00 00 00 00 00 00 00 00 00 00 00
100: 01 00 02 15 00 00 00 00 00 00 18 00 10 30 46 00
110: 01 00 00 00 00 00 00 00 a1 00 00 00 00 00 00 00
120: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
130: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
140: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
150: 03 00 01 16 00 00 00 00 00 00 00 00 00 00 00 00
160: 04 00 81 1b 00 00 00 00 00 00 00 00 00 00 00 00
170: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
180: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
190: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
1a0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
1b0: 00 00 00 00 00 00 00 00 18 00 41 27 00 00 00 00
1c0: 16 00 41 27 07 00 10 08 00 00 00 00 00 01 00 00
1d0: 01 01 01 01 01 01 01 01 00 00 00 00 00 00 00 00
1e0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
1f0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
200: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
210: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
220: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
230: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
240: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
250: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
260: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
270: 00 00 00 00 17 00 01 30 03 02 00 00 00 00 00 00
280: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
290: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
2a0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
2b0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
2c0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
2d0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
2e0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
2f0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
300: 19 00 01 40 00 00 00 00 00 00 00 00 00 75 00 75
310: 00 75 00 75 00 00 00 00 00 00 00 00 00 00 00 00
320: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
330: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
340: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
350: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
360: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
370: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
380: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
390: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
3a0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
3b0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
3c0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
3d0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
3e0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
3f0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
400: 0b 00 01 44 01 00 01 01 00 00 00 00 00 00 00 00
410: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
420: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
430: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
440: 1b 00 01 4c 06 14 00 00 00 00 00 00 00 00 00 00
450: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
460: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
470: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
480: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
490: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
4a0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
4b0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
4c0: 02 00 01 90 00 00 00 00 00 00 00 00 00 00 00 00
4d0: 00 00 00 00 ff 00 00 80 00 00 00 00 00 00 00 00
4e0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
4f0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
500: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
510: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
520: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
530: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
540: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
550: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
560: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
570: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
580: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
590: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
5a0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
5b0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
5c0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
5d0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
5e0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
5f0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
600: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
610: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
620: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
630: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
640: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
650: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
660: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
670: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
680: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
690: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
6a0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
6b0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
6c0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
6d0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
6e0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
6f0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
700: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
710: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
720: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
730: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
740: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
750: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
760: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
770: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
780: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
790: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
7a0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
7b0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
7c0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
7d0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
7e0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
7f0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
800: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
810: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
820: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
830: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
840: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
850: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
860: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
870: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
880: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
890: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
8a0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
8b0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
8c0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
8d0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
8e0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
8f0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
900: 1e 00 01 91 1f 00 28 00 00 00 00 00 28 00 00 00
910: 25 00 01 92 01 00 00 80 01 00 00 80 00 00 00 00
920: 27 00 01 9c 01 00 03 00 38 9c 00 00 38 9c 00 00
930: 38 9c 00 00 38 9c 00 00 00 00 00 00 00 00 00 00
940: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
950: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
960: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
970: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
980: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
990: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
9a0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
9b0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
9c0: 26 00 01 a4 00 00 00 00 00 00 00 00 0f 00 00 00
9d0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
9e0: 70 70 70 70 00 00 00 00 00 00 00 00 00 00 00 00
9f0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
a00: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
a10: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
a20: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
a30: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
a40: 2a 00 01 aa 01 01 00 00 03 00 00 00 0f 02 00 00
a50: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
a60: 90 90 90 90 00 00 00 00 00 00 00 00 00 00 00 00
a70: 2b 00 01 a9 02 00 00 00 00 00 00 00 00 00 ff ff
a80: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
a90: 2f 00 01 e0 0a 00 00 00 78 00 00 00 02 00 00 00
aa0: 31 00 01 ad 00 00 00 00 00 00 00 00 40 00 00 00
ab0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
ac0: f0 f0 f0 f0 00 00 00 00 00 00 00 00 00 00 00 00
ad0: 32 00 01 b1 05 9f 00 00 00 00 00 00 00 00 00 00
ae0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
af0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
b00: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
b10: 33 00 01 b4 00 00 00 00 00 00 00 00 00 00 00 00
b20: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
b30: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
b40: 34 00 01 a7 00 00 00 00 00 00 00 00 00 00 00 00
b50: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
b60: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
b70: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
b80: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
b90: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
ba0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
bb0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
bc0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
bd0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
be0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
bf0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
c00: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
c10: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
c20: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
c30: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
c40: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
c50: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
c60: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
c70: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
c80: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
c90: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
ca0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
cb0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
cc0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
cd0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
ce0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
cf0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
d00: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
d10: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
d20: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
d30: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
d40: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
d50: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
d60: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
d70: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
d80: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
d90: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
da0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
db0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
dc0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
dd0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
de0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
df0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
e00: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
e10: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
e20: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
e30: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
e40: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
e50: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
e60: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
e70: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
e80: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
e90: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
ea0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
eb0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
ec0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
ed0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
ee0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
ef0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
f00: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
f10: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
f20: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
f30: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
f40: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
f50: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
f60: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
f70: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
f80: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
f90: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
fa0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
fb0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
fc0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
fd0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
fe0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
ff0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00





ep_bdf=$1
iterations=$2
RATE=("Invalid" "gen1" "gen2" "gen3" "gen4" "gen5")
return_val=0
ep_aer_err_cnt=0
ep_uaer_err_cnt=0
rc_aer_err_cnt=0
rc_uaer_err_cnt=0
link_down_err_cnt=0
rate_change_err_cnt=0
lane_error_cnt=0
width_error_cnt=0
error_detected=0
how_often=${3:-1000}

if [ -z "$ep_bdf" ]; then
        echo "Error: no device specified"
        return 1
fi

if [ ! -e "/sys/bus/pci/devices/$ep_bdf" ]; then
        ep_bdf="0000:$ep_bdf"
fi

if [ ! -e "/sys/bus/pci/devices/$ep_bdf" ]; then
        echo "Error: device $ep_bdf not found"
        return 1
fi

echo "$ep_bdf=/sys/bus/pci/devices/$ep_bdf"
rc_bdf=$(basename $(dirname $(readlink "/sys/bus/pci/devices/$ep_bdf")))
echo "rc_bdf=$rc_bdf ep_bdf=$ep_bdf"

link_status=$(setpci -s $ep_bdf CAP_EXP+12.W)
initial_rate=$(("0x$link_status" & 0xF))
initial_width=$((("0x$link_status" & 0xF0) >> 4))

printf "Running with initial state of ${RATE[initial_rate]}x${initial_width}\n"

# Clear
aer=$(setpci -s $ep_bdf ECAP_AER+10.L)
aer=$(("0x$aer" & 0xFFFF))
if [[ $aer -ne 0 ]]; then
        #setpci -s $ep_bdf ECAP_AER+10.L=$aer
        printf "Clearing initial EP AER error: aer=%s\n" $aer
fi

uaer=$(setpci -s $ep_bdf ECAP_AER+4.L)
uaer=$(("0x$uaer" & 0xFFFF))
if [[ $uaer -ne 0 ]]; then
        #setpci -s $ep_bdf ECAP_AER+4.L=$uaer
        printf "Clearing initial EP UAER error: aer=%s\n" $uaer
fi

lane_error=$(setpci -s $ep_bdf ECAP_SECPCI+8.l)
lane_error=$(("0x$lane_error" & 0xFF))
#setpci -s $ep_bdf ECAP_SECPCI+8.w=$lane_error
if [[ $lane_error -ne 0 ]]; then
        printf "Clearing initial EP lane error: lane_error=0x%x\n" $lane_error
fi

aer=$(setpci -s $rc_bdf ECAP_AER+10.L)
aer=$(("0x$aer" & 0xFFFF))
#setpci -s $rc_bdf ECAP_AER+10.L=$aer
if [[ $aer -ne 0 ]]; then
        printf "Clearing initial RC AER error: aer=%s\n" $aer
fi

uaer=$(setpci -s $rc_bdf ECAP_AER+4.L)
uaer=$(("0x$uaer" & 0xFFFF))
if [[ $uaer -ne 0 ]]; then
        setpci -s $rc_bdf ECAP_AER+4.L=$uaer
        printf "Clearing initial RC UAER error: aer=%s\n" $uaer
fi


for (( cnt=1; cnt<=$iterations; cnt++ ))
do
        if [ $(expr $cnt % $how_often) == "0" ]; then
                if [[ $error_detected -eq 0 ]]; then
                        printf "Done %8d iterations of %d so far...\n" $cnt $iterations
                fi
                error_detected=0
        fi

        aer=$(setpci -s $ep_bdf ECAP_AER+10.L)
        aer=$(("0x$aer" & 0xFFFF))
        if [[ $aer -ne 0 ]]; then
                error_detected=1
                ep_aer_err_cnt=$((ep_aer_err_cnt +1))
                #setpci -s $ep_bdf ECAP_AER+10.L=$aer
                printf "ERROR: EP aer=%s iteration=%d\n" $aer $cnt
        fi

        uaer=$(setpci -s $ep_bdf ECAP_AER+4.L)
        uaer=$(("0x$uaer" & 0xFFFF))
        if [[ $uaer -ne 0 ]]; then
                error_detected=1
                ep_uaer_err_cnt=$((ep_uaer_err_cnt +1))
                #setpci -s $ep_bdf ECAP_AER+4.L=$uaer
                printf "ERROR: EP uncorrectable error=%s iteration=%d\n" $uaer $cnt
        fi

        aer=$(setpci -s $rc_bdf ECAP_AER+10.L)
        if [[ $aer -ne 0 ]]; then
                error_detected=1
                rc_aer_err_cnt=$((ep_aer_err_cnt +1))
                #setpci -s $ep_bdf ECAP_AER+10.L=$aer
                printf "ERROR: RC aer=%s iteration=%d\n" $aer $cnt
        fi

        uaer=$(setpci -s $rc_bdf ECAP_AER+4.L)
        if [[ $uaer -ne 0 ]]; then
                error_detected=1
                rc_uaer_err_cnt=$((rc_uaer_err_cnt +1))
                #setpci -s $rc_bdf ECAP_AER+4.L=$uaer
                printf "ERROR: RC uncorrectable error=%s iteration=%d\n" $uaer $cnt
        fi

        # ltssm_org=$(setpci -s $ep_bdf 728.L)
        # ltssm=$(("0x$ltssm_org" & 0x3F))
        # if [[ $ltssm -ne 0x11 ]]; then
        #       link_down_err_cnt=$((link_down_err_cnt + 1))
        #       printf "ERROR: link is down. Wait 2 seconds for recovery\n" $ltssm_org
        #       sleep 2
        #       ltssm_org=$(setpci -s $ep_bdf 728.L)
        #       ltssm=$(("0x$ltssm_org" & 0x3F))
        #       if [[ $ltssm -ne 0x11 ]]; then
        #               printf "ERROR: link is stil down after 2 seconds: %s Stop checking AER" $ltssm_org
        #               return
        #       fi
        # fi

        lane_error=$(setpci -s $ep_bdf ECAP_SECPCI+8.w)
        lane_error=$(("0x$lane_error" & 0xFF))
        if [[ $lane_error -ne 0 ]]; then
                error_detected=1
                lane_error_cnt=$((lane_error_cnt + 1))
                printf "ERROR: EP lane_error=0x%x iteration=%d\n" $lane_error $cnt
                #setpci -s $ep_bdf ECAP_SECPCI+8.w=$lane_error
        fi

        link_status=$(setpci -s $ep_bdf CAP_EXP+12.W)
        rate=$(("0x$link_status" & 0xF))
        width=$((("0x$link_status" & 0xF0) >> 4))
        if [[ $rate -ne $initial_rate ]]; then
                error_detected=1
                printf "ERROR: rate has change at iteration $cnt: Initial rate ${RATE[initial_rate]}, current rate ${RATE[rate]}.\n"
                printf "ERROR: Waiting for 2 seconds to see if rate recover"
        sleep 2
        link_status=$(setpci -s $ep_bdf CAP_EXP+12.W)
        rate=$(("0x$link_status" & 0xF))
                if [[ $rate -ne $initial_rate ]]; then
                        printf "ERROR: rate has not recovered: Initial rate ${RATE[initial_rate]}, current rate ${RATE[rate]}.\n"
                        printf "Stop checking AER\n"
                        return 1
                fi
        fi
        link_status=$(setpci -s $ep_bdf CAP_EXP+12.W)
        width=$((("0x$link_status" & 0xF0) >> 4))
        if [[ $width -ne $initial_width ]]; then
                error_detected=1
                printf "ERROR: width has change at iteration $cnt: Initial width $initial_width, current width $width.\n"
        fi
done

printf "Total EP AER error count: %d\n" $ep_aer_err_cnt
printf "Total EP UAER error count: %d\n" $ep_uaer_err_cnt
printf "Total RC AER error count: %d\n" $rc_aer_err_cnt
printf "Total RC UAER error count: %d\n" $rc_uaer_err_cnt
printf "Total EP lane error count: %d\n" $lane_error_cnt
printf "Total EP link down error count: %d\n" $link_down_err_cnt

2026-02-25 17:00:20,152 | [hotreset_test] | BROADCAST | INFO | ========Cycle | Total Cycle: 306 | 1000========
2026-02-25 17:00:20,153 | [hotreset_test] | 0x0:98:00.00 | INFO | RC Secondary Bus Reset bit was set.
2026-02-25 17:00:20,156 | [hotreset_test] | 0x0:98:00.00 | DEBUG | Clear RC Slot Status Register by writing 0x0108.
2026-02-25 17:00:20,156 | [hotreset_test] | 0x0:98:00.00 | INFO | RC Secondary Bus Reset bit was reset.
2026-02-25 17:00:20,156 | [hotreset_test] | 0x0:98:00.00 | DEBUG | PCIe Layer Checking.
2026-02-25 17:00:20,268 | [hotreset_test] | 0x0:98:00.00 | INFO | PCIe Link Ready Time(from reset to DL_Active=1b) is 112.35 ms.
2026-02-25 17:00:20,269 | [hotreset_test] | 0x0:98:00.00 | INFO | Will wait 100ms after Data Link Layer Link is active.
2026-02-25 17:00:20,669 | [hotreset_test] | 0x0:98:00.00 | INFO | Wait 0.3 second for PCIe Checking delay.
2026-02-25 17:00:20,669 | [hotreset_test] | 0x0:98:00.00 | INFO | PCIe Configuration Ready Time is less than 513.03 ms.(from reset to the First Successful CfgRd)
2026-02-25 17:00:20,669 | [hotreset_test] | 0x0:98:00.00 | DEBUG | Endpoint Target Link Speed is 6.
2026-02-25 17:00:20,669 | [hotreset_test] | 0x0:98:00.00 | DEBUG | Root Complex Target Link Speed is 5.
2026-02-25 17:00:20,669 | [hotreset_test] | 0x0:98:00.00 | INFO | The device is able to reach Gen5x4.(EP Link Capability is Gen6x4; RC link Capability is Gen5x16)
2026-02-25 17:00:20,669 | [hotreset_test] | 0x0:98:00.00 | ERROR | Endpoint Correctable Error is 0x1.
2026-02-25 17:00:20,670 | [hotreset_test] | WARNING | [0x0:98: 0.0]PCIe Failures detected.
2026-02-25 17:00:20,670 | [hotreset_test] | 0x0:98:00.00 | INFO | Software did Config Write(0x0)=0x88888888 to trigger the failure.
2026-02-25 17:00:20,670 | [hotreset_test] | 0x0:98:00.00 | ERROR | PCIe Link Error is detected.
2026-02-25 17:00:20,670 | [hotreset_test] | BROADCAST | INFO | Save dmesg "DMESG_cycle_307_2026-02-25_17-00-20.log"...
2026-02-25 17:00:20,685 | [hotreset_test] | 0x0:98:00.00 | INFO | Save PCIe Configuration Space.
2026-02-25 17:00:20,686 | [hotreset_test] | 0x0:98:00.00 | INFO | File (/root/DVT/Log/PCIeConfigSpace_cycle_307_0x0-97-1-0_February_25_2026-17_00_20_686015.log) will be created.
2026-02-25 17:00:20,695 | [hotreset_test] | 0x0:98:00.00 | INFO | File (/root/DVT/Log/PCIeConfigSpace_cycle_307_0x0-98-0-0_February_25_2026-17_00_20_695748.log) will be created.
2026-02-25 17:00:20,705 | [hotreset_test] | 0x0:98:00.00 | DEBUG | SKIP NVMe Level Check, since no NVMe Error bit was set.
2026-02-25 17:00:20,706 | [hotreset_test] | WARNING | The test was paused because of ERROR.

Click Enter to continue...
2026-02-25 17:02:20,029 | [hotreset_test] | BROADCAST | INFO | Wait 1.000 sec before reach next cycle.
2026-02-25 17:02:21,030 | [hotreset_test] | BROADCAST | WARNING | The test has been terminated.
2026-02-25 17:02:21,030 | [hotreset_test] | BROADCAST | INFO | The test is done.
         Test Name  Number of Drives                                           DUT Info  ... End of The Test Start Timestamp Test Duration
0  [hotreset_test]                 1  [{'EP Address': '0x0:98:00.00', 'RC Address': ...  ...             TBD    1.772056e+09           TBD

[1 rows x 14 columns]
2026-02-25 17:02:21,158 | [hotreset_test] | INFO    | Generate HTML Test Report.
2026-02-25 17:02:21,160 | [hotreset_test] | 0x0:98:00.00 | ERROR | count_ep_correctable_error_failure: 1
2026-02-25 17:02:21,165 | [hotreset_test] | ERROR   | DB ERROR: upload_to_db diabled
2026-02-25 17:02:21,165 | [hotreset_test] | BROADCAST | INFO | ============================================================
2026-02-25 17:02:21,165 | [hotreset_test] | BROADCAST | INFO | FAILURE SUMMARY
2026-02-25 17:02:21,165 | [hotreset_test] | BROADCAST | INFO | Test: [hotreset_test]  |  Total Cycles Run: 306
2026-02-25 17:02:21,165 | [hotreset_test] | BROADCAST | INFO | ============================================================
2026-02-25 17:02:21,165 | [hotreset_test] | BROADCAST | INFO | DUT: 0:98:00.00  |  S/N: N/A
2026-02-25 17:02:21,165 | [hotreset_test] | BROADCAST | INFO |   PCIe Failures: 1
2026-02-25 17:02:21,165 | [hotreset_test] | BROADCAST | ERROR |     [PCIe] Endpoint Correctable Error is 0x1.
2026-02-25 17:02:21,165 | [hotreset_test] | BROADCAST | INFO |   NVMe Failures: 0
2026-02-25 17:02:21,165 | [hotreset_test] | BROADCAST | INFO | ============================================================
2026-02-25 17:02:21,165 | [hotreset_test] | BROADCAST | INFO | Save DVT Logs into a Zip file.
2026-02-25 17:02:21,175 | [hotreset_test] | BROADCAST | DEBUG | 10 Files Transfer from "/root/DVT/Log" to "/root/DVT/Backup/hotreset_test_2026-02-25_17-02-21.zip".
2026-02-25 17:02:21,175 | [hotreset_test] | BROADCAST | DEBUG | DVT Test Running Status Has Been Changed To "DONE"...
2026-02-25 17:02:21,176 | [hotreset_test] | DEBUG   | Cannot Find File '/root/DVT/Config/wd_dvt.cfg'.
2026-02-25 17:02:21,176 | [hotreset_test] | DEBUG   | Delete 'testname.ini' FIle.
2026-02-25 17:02:21,176 | unknown | DEBUG   | Delete 'DVT.temp' FIle.
2026-02-25 17:02:21,176 | unknown | DEBUG   | Delete 'status.json' FIle.
2026-02-25 17:02:21,176 | unknown | DEBUG   | Delete 'host.json' FIle.
2026-02-25 17:02:21,176 | unknown | DEBUG   | Delete 'dvt_running_status.json' FIle.
2026-02-25 17:02:21,176 | unknown | DEBUG   | Delete 'Device_0000_98_00_0_INFO.json' FIle.
2026-02-25 17:02:21,176 | unknown | DEBUG   | Delete 'log_path_book.json' FIle.
2026-02-25 17:02:21,176 | unknown | INFO    | Test[0][hotreset_test] has been done.
[root@sjac DVT]#

Here is the script I have to read the errors ( you can find the script in /root/hoang.latest/pcie)

[root@sjac pcie]# source ./check_aer_read_only.sh 98:00.0 2
0000:98:00.0=/sys/bus/pci/devices/0000:98:00.0
rc_bdf=0000:97:01.0 ep_bdf=0000:98:00.0
Running with initial state of gen5x4
Clearing initial EP AER error: aer=1
Clearing initial EP lane error: lane_error=0x4
ERROR: EP aer=1 iteration=1
ERROR: EP lane_error=0x4 iteration=1
ERROR: EP aer=1 iteration=2
ERROR: EP lane_error=0x4 iteration=2
Total EP AER error count: 2
Total EP UAER error count: 0
Total RC AER error count: 0
Total RC UAER error count: 0
Total EP lane error count: 2
Total EP link down error count: 0

现在我有足够的信息来解释问题了。

  ---
  根本原因分析

  DVT 报告 1 个错误（AER CE），Shell 脚本报告 2 个（AER CE + Lane Error）

  问题所在

  关键在于 DVT 读取 Lane Error 的前提条件（PCIeTest.py:308-312）：

  if dut.SecondaryPCIeExtCapAdr != 0xFFFF:    # ← 必须找到 Cap ID=0x19
      ep_LaneErrorStatus = dut.PCIeConfigRead32(
          dut.SecondaryPCIeExtCapAdr + 0x8)
  else:
      ep_LaneErrorStatus = 0x0                  # ← 找不到就置0，跳过检查

  SecondaryPCIeExtCapAdr 在设备初始化时通过 findExtCapAdr(0x19) 查找（Device.py:77）。

  两种可能原因：

  ---
  可能原因 1：Cap 0x19 未找到（最可能）

  ┌────────────────────┬────────────────────────────────────────────┐
  │        工具        │                  查找方式                  │
  ├────────────────────┼────────────────────────────────────────────┤
  │ DVT                │ 启动时扫描一次 ExtCap 链表，找到后保存地址 │
  ├────────────────────┼────────────────────────────────────────────┤
  │ setpci ECAP_SECPCI │ 每次调用时实时扫描 ExtCap 链表             │
  └────────────────────┴────────────────────────────────────────────┘

  如果 DVT 初始化时（第 1 次 cycle 之前）设备还没 expose 出 Cap 0x19，SecondaryPCIeExtCapAdr = 0xFFFF，之后所有 cycle
  都跳过 Lane Error 检查。

  验证方法——在目标机器上运行：

  # 检查该设备是否有 Secondary PCIe Extended Capability (Cap ID=0x19)
  setpci -s 98:00.0 ECAP_SECPCI+0.L
  # 若返回 00000019，则 cap 存在；若报错则不存在

  ---
  可能原因 2：时序问题

  DVT 执行 Hot Reset
       │
       ├── DVT 读 AER CE = 0x1  ✓ 检测到
       ├── DVT 读 Lane Error = 0x0  ← 此时还未置位
       │
       └── 之后 Lane Error 才被 EP 置为 0x4
             │
             └── Shell 脚本读到 Lane Error = 0x4

  DVT 的读取时机可能早于 Lane Error 被设备置位。

  ---
  如何确认

  在 DVT 日志中搜索设备初始化时是否打印了 ExtCap 相关信息，或临时添加一行 debug：

  # Device.py:77 附近
  self.SecondaryPCIeExtCapAdr = self.findExtCapAdr(0x19)
  # 查看实际值：
  print(f"SecondaryPCIeExtCapAdr = 0x{self.SecondaryPCIeExtCapAdr:X}")

  如果输出是 0xFFFF，就是原因 1；如果有合法地址（如 0x1A0），就是原因 2（时序问题）。

✻ Baked for 2m 6s

❯   setpci -s 98:00.0 ECAP_SECPCI+0.L 输出40010019， 如果我想让dvt 也检测lane error该怎么做

● 40010019 说明 Cap 0x19 确实存在。让我找出 DVT 在检查前是否提前清除了 Lane Error。

● Searched for 4 patterns, read 5 files (ctrl+o to expand)

● 找到关键了！让我看 capabilities.json。

● Searched for 1 pattern, read 1 file (ctrl+o to expand)

● 找到真正的根本原因了。

  根本原因

  init_extend_cap_adr_book() 在扫描 Extended Cap 链表时，遇到 capabilities.json 里没有的 Cap ID 就会
  break（Device_base.py:541）：

  else:
      if self.is_ep:
          self.dut_logger.error('...Ext Cap({:04X}) in offset 0x{:03X} was invalid.')
          break   # ← 遇到未知 Cap 就停止扫描！

  如果设备在 Cap 链表中，0x19 之前有一个不在 capabilities.json 里的 Cap，DVT 就永远扫不到 0x19，导致
  SecondaryPCIeExtCapAdr = 0xFFFF，Lane Error 被跳过。

  ---
  第一步：确认原因

  在测试机上查找 DVT 初始化日志：

  grep -i "secondary\|broken.*cap\|invalid\|does not support" /root/DVT/Log/*.log | head -30

  - 如果看到 does not support Secondary PCIe Ext Cap → 确认是这个问题
  - 如果看到 Endpoint PCIe Configuration Space was broken. Ext Cap(XXXX) → 可以看到是哪个 Cap 导致中断

  同时运行以下命令，找出设备实际有哪些 Extended Caps：

  lspci -vvv -s 0000:98:00.0 | grep "Extended Capabilities"

  ---
  第二步：修复（将缺失的 Cap 加入 capabilities.json）

  假设 lspci 输出显示某个未知 Cap（例如 0x000C = MFVC），在 Config/capabilities.json 的 "extend" 数组中加入对应条目：

  {
      "CapId": 12,
      "Name": "MFVC Extended Cap",
      "Required": false,
      "Ext": true
  },

  常见容易缺失的 Cap ID（十进制）：

  ┌─────┬────────────────────────────────────┐
  │ ID  │                名称                │
  ├─────┼────────────────────────────────────┤
  │ 12  │ MFVC (Multi-Function VC)           │
  ├─────┼────────────────────────────────────┤
  │ 15  │ ATS (Address Translation Services) │
  ├─────┼────────────────────────────────────┤
  │ 16  │ SR-IOV                             │
  ├─────┼────────────────────────────────────┤
  │ 19  │ Page Request Interface             │
  ├─────┼────────────────────────────────────┤
  │ 26  │ REBAR                              │
  └─────┴────────────────────────────────────┘

  ---
  第三步：验证 Cap 是否被找到

  修改后重新运行 DVT，在 cycle 1 的日志里应该能看到：

  Port(0x0:98:00.0) Supports SECONDARY PCI EXPRESS EXTENDED CAPABILITY(0019h).

  以及 clear_lane_error() 打印的：
  DUT(0:98:0.0) Lane Error Status is 0x0 at the beginning.

  之后若 Lane Error 发生，DVT 会正常报告：
  ERROR | Endpoint Lane Error Status is 0x4.

✻ Cooked for 4m 2s

❯ 我现在缺失的哪个cap？ 怎么添加

● 我没有办法直接看到你的设备，需要你在测试机上跑一个命令来找出答案。

● Read 1 file (ctrl+o to expand)

● 第一步：在测试机上扫描设备的 Extended Cap 链表

  在 CentOS 机器上运行这个 Python 脚本：

  python3 -c "
  import subprocess

  addr = 0x100
  known = {1,2,3,4,5,6,7,8,9,10,11,13,14,18,20,21,22,23,24,25,27,28,29,30,31,33,34,35,37,38,39,40,41,42,43,44,47,48,49,5
  0,51,52,53,54}

  print('--- Extended Cap Chain for 98:00.0 ---')
  while addr != 0:
      out = subprocess.check_output(['setpci','-s','98:00.0', hex(addr)+'.L'], text=True).strip()
      val = int(out, 16)
      capid = val & 0xFFFF
      next_addr = (val >> 20) & 0xFFF
      status = 'OK' if capid in known else '<<< MISSING IN capabilities.json'
      print(f'  offset=0x{addr:03X}  CapID=0x{capid:04X} ({capid:3d})  next=0x{next_addr:03X}  {status}')
      if next_addr == 0:
          break
      addr = next_addr
  "

  把输出结果贴给我，我告诉你要加哪几行。

  ---
  如果你已经看到 DVT 日志里有类似这行

  ERROR | Endpoint PCIe Configuration Space was broken. Ext Cap(XXXX) in offset 0xYYY was invalid.

  直接把 XXXX（十六进制 Cap ID）告诉我，也可以直接定位。

✻ Cogitated for 31s

────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────
❯ 
────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────


[root@sjac DVT]# python3 test_cap.py 
--- Extended Cap Chain for 98:00.0 ---
  offset=0x100  CapID=0x0001 (  1)  next=0x150  OK
  offset=0x150  CapID=0x0003 (  3)  next=0x160  OK
  offset=0x160  CapID=0x0004 (  4)  next=0x1B8  OK
  offset=0x1B8  CapID=0x0018 ( 24)  next=0x274  OK
  offset=0x274  CapID=0x0017 ( 23)  next=0x300  OK
  offset=0x300  CapID=0x0019 ( 25)  next=0x400  OK
  offset=0x400  CapID=0x000B ( 11)  next=0x440  OK
  offset=0x440  CapID=0x001B ( 27)  next=0x4C0  OK
  offset=0x4C0  CapID=0x0002 (  2)  next=0x900  OK
  offset=0x900  CapID=0x001E ( 30)  next=0x910  OK
  offset=0x910  CapID=0x0025 ( 37)  next=0x920  OK
  offset=0x920  CapID=0x0027 ( 39)  next=0x9C0  OK
  offset=0x9C0  CapID=0x0026 ( 38)  next=0xA40  OK
  offset=0xA40  CapID=0x002A ( 42)  next=0xAA0  OK
  offset=0xAA0  CapID=0x0031 ( 49)  next=0xAD0  OK
  offset=0xAD0  CapID=0x0032 ( 50)  next=0xB10  OK
  offset=0xB10  CapID=0x0033 ( 51)  next=0xB40  OK
  offset=0xB40  CapID=0x0034 ( 52)  next=0xA70  OK
  offset=0xA70  CapID=0x002B ( 43)  next=0xA90  OK
  offset=0xA90  CapID=0x002F ( 47)  next=0xE00  OK
  offset=0xE00  CapID=0x0000 (  0)  next=0x000  <<< MISSING IN capabilities.json


Hi,

  I'd like to share some findings on how DVT handles PCIe error detection and a few configuration options that may help
  focus the test on the errors you care about.

  ---
  DVT Error Detection Logic

  After each reset event (e.g., Hot Reset), DVT performs a single snapshot read of all relevant PCIe registers at the
  same point in time, then checks each error type independently using the pcie_error_bits bitmask. Because all checks
  are sequential if statements with no early exit, multiple errors can be reported within the same cycle as long as they
   are all present at the moment DVT reads the registers.

  The timing of this read is controlled by PCIe_checking_delay in DVT.cfg (default: 0.3s). This delay is applied after
  the PCIe link is re-established following a reset.

  ---
  Option 1 — Increase PCIe_checking_delay to Catch Multiple Errors

  Some error bits (e.g., Lane Error Status, bit 9) are latched by the hardware slightly later than others (e.g., AER
  Correctable Error, bit 4). If DVT reads too early, it may miss the Lane Error even though it appears shortly after.

  Increasing the delay gives the hardware more time to latch all error bits before DVT reads:

  PCIe_checking_delay = 1.0    # increased from default 0.3s

  With this change, DVT should be able to catch both errors simultaneously in the same cycle:
  ERROR | Endpoint Correctable Error is 0x1.
  ERROR | Endpoint Lane Error Status is 0x4.

  ---
  Option 2 — Mask Bits 3 & 4 to Focus on Lane Error (Bit 9)

  If you are not interested in Receiver Errors (AER Correctable, bit 4) or In-band Reset (bit 3), you can disable them
  and focus solely on Lane Error detection (bit 9):

  [Hotreset_test]
  PCIe_Error_Bits = 0xFFFE7    # disable bit3 (In-band Reset) + bit4 (AER CE)

  However, from my experiments, this approach frequently triggers Config Space Mismatch errors (bit 12) as a side
  effect, because increasing PCIe_checking_delay causes DVT to read the config space at a later point when certain
  vendor-specific and status registers have already been updated by firmware. Examples observed:

  - Offset=0xE9Fh, Init Value=0x00, Current Value=0x70
  - Offset=0x0E5h, Init Value=0x08, Current Value=0x18
  - A third offset mismatch appearing at the same time

  These three mismatches tend to appear together in the same cycle, as they are all status/initialization registers that
   settle after link re-training completes.

  ---
  Option 3 — Also Mask Bit 12 if Config Space Errors Are Not Your Target

  If the config space mismatches above are not the failure mode you are investigating, you can additionally disable bit
  12 (Config Space Compare) to suppress these false positives:

  [Hotreset_test]
  PCIe_Error_Bits = 0xFEFE7    # disable bit3 + bit4 + bit12, retain bit9

  This keeps Lane Error detection (bit 9) fully active while eliminating the noise from receiver errors and config space
   drift caused by the longer checking delay.

  ---
  Summary

  ┌──────────────────────────────────────────────────┬───────────────────────────┐
  │                       Goal                       │    Recommended Setting    │
  ├──────────────────────────────────────────────────┼───────────────────────────┤
  │ Catch both AER CE and Lane Error                 │ PCIe_checking_delay = 1.0 │
  ├──────────────────────────────────────────────────┼───────────────────────────┤
  │ Focus on Lane Error only                         │ PCIe_Error_Bits = 0xFFFE7 │
  ├──────────────────────────────────────────────────┼───────────────────────────┤
  │ Focus on Lane Error, suppress config space noise │ PCIe_Error_Bits = 0xFEFE7 │
  └──────────────────────────────────────────────────┴───────────────────────────┘

  Please let me know if you have any questions.

  Best regards
