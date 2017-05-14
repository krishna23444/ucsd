VERSION 6
BEGIN SCHEMATIC
    BEGIN ATTR DeviceFamilyName "virtex2"
        DELETE all:0
        EDITNAME all:0
        EDITTRAIT all:0
    END ATTR
    BEGIN NETLIST
        SIGNAL D(3)
        SIGNAL D(0)
        SIGNAL D(1)
        SIGNAL D(2)
        SIGNAL XLXN_32
        SIGNAL XLXN_38
        SIGNAL XLXN_41
        SIGNAL XLXN_37
        SIGNAL XLXN_39
        SIGNAL XLXN_33
        SIGNAL CLR
        SIGNAL CLK_CE
        SIGNAL CLK
        SIGNAL D(3:0)
        SIGNAL XLXN_43
        SIGNAL XLXN_46
        SIGNAL XLXN_47
        SIGNAL XLXN_48
        SIGNAL XLXN_49
        SIGNAL XLXN_50
        SIGNAL XLXN_51
        PORT Input CLR
        PORT Input CLK_CE
        PORT Input CLK
        PORT Output D(3:0)
        BEGIN BLOCKDEF ftce
            TIMESTAMP 2001 2 2 12 37 14
            LINE N 192 -32 64 -32 
            LINE N 192 -64 192 -32 
            LINE N 80 -128 64 -144 
            LINE N 64 -112 80 -128 
            LINE N 384 -256 320 -256 
            LINE N 0 -256 64 -256 
            LINE N 0 -32 64 -32 
            LINE N 0 -192 64 -192 
            LINE N 0 -128 64 -128 
            RECTANGLE N 64 -320 320 -64 
        END BLOCKDEF
        BEGIN BLOCKDEF xnor4
            TIMESTAMP 2002 3 20 17 9 8
            LINE N 0 -64 48 -64 
            LINE N 0 -128 64 -128 
            LINE N 0 -192 60 -192 
            LINE N 0 -256 48 -256 
            ARC N -24 -216 88 -104 64 -112 64 -208 
            LINE N 128 -208 64 -208 
            LINE N 128 -112 64 -112 
            LINE N 48 -64 48 -112 
            LINE N 48 -256 48 -208 
            ARC N 44 -208 220 -32 208 -160 128 -208 
            ARC N -40 -216 72 -104 48 -112 48 -208 
            ARC N 44 -288 220 -112 128 -112 208 -160 
            CIRCLE N 212 -168 228 -152 
            LINE N 228 -160 256 -160 
        END BLOCKDEF
        BEGIN BLOCKDEF xor2
            TIMESTAMP 2001 2 2 12 38 38
            LINE N 0 -64 64 -64 
            LINE N 0 -128 60 -128 
            LINE N 256 -96 208 -96 
            ARC N -40 -152 72 -40 48 -48 44 -144 
            ARC N -24 -152 88 -40 64 -48 64 -144 
            LINE N 128 -144 64 -144 
            LINE N 128 -48 64 -48 
            ARC N 44 -144 220 32 208 -96 128 -144 
            ARC N 44 -224 220 -48 128 -48 208 -96 
        END BLOCKDEF
        BEGIN BLOCKDEF and3b2
            TIMESTAMP 2001 2 2 12 38 38
            LINE N 0 -64 40 -64 
            CIRCLE N 40 -76 64 -52 
            LINE N 0 -128 40 -128 
            CIRCLE N 40 -140 64 -116 
            LINE N 0 -192 64 -192 
            LINE N 256 -128 192 -128 
            LINE N 64 -64 64 -192 
            ARC N 96 -176 192 -80 144 -80 144 -176 
            LINE N 144 -80 64 -80 
            LINE N 64 -176 144 -176 
        END BLOCKDEF
        BEGIN BLOCKDEF and3
            TIMESTAMP 2001 2 2 12 38 38
            LINE N 0 -64 64 -64 
            LINE N 0 -128 64 -128 
            LINE N 0 -192 64 -192 
            LINE N 256 -128 192 -128 
            LINE N 64 -176 144 -176 
            LINE N 144 -80 64 -80 
            ARC N 96 -176 192 -80 144 -80 144 -176 
            LINE N 64 -64 64 -192 
        END BLOCKDEF
        BEGIN BLOCKDEF or2
            TIMESTAMP 2001 2 2 12 38 38
            LINE N 0 -64 64 -64 
            LINE N 0 -128 64 -128 
            LINE N 256 -96 192 -96 
            ARC N 28 -224 204 -48 112 -48 192 -96 
            ARC N -40 -152 72 -40 48 -48 48 -144 
            LINE N 112 -144 48 -144 
            ARC N 28 -144 204 32 192 -96 112 -144 
            LINE N 112 -48 48 -48 
        END BLOCKDEF
        BEGIN BLOCK XLXI_30 xor2
            PIN I0 D(3)
            PIN I1 D(2)
            PIN O XLXN_37
        END BLOCK
        BEGIN BLOCK XLXI_31 and3b2
            PIN I0 XLXN_37
            PIN I1 D(0)
            PIN I2 D(1)
            PIN O XLXN_38
        END BLOCK
        BEGIN BLOCK XLXI_32 xor2
            PIN I0 D(3)
            PIN I1 D(2)
            PIN O XLXN_39
        END BLOCK
        BEGIN BLOCK XLXI_33 and3b2
            PIN I0 D(1)
            PIN I1 D(0)
            PIN I2 XLXN_39
            PIN O XLXN_41
        END BLOCK
        BEGIN BLOCK XLXI_4 ftce
            PIN C CLK
            PIN CE CLK_CE
            PIN CLR CLR
            PIN T XLXN_41
            PIN Q D(3)
        END BLOCK
        BEGIN BLOCK XLXI_2 ftce
            PIN C CLK
            PIN CE CLK_CE
            PIN CLR CLR
            PIN T XLXN_32
            PIN Q D(1)
        END BLOCK
        BEGIN BLOCK XLXI_27 xnor4
            PIN I0 D(0)
            PIN I1 D(1)
            PIN I2 D(2)
            PIN I3 D(3)
            PIN O XLXN_33
        END BLOCK
        BEGIN BLOCK XLXI_1 ftce
            PIN C CLK
            PIN CE CLK_CE
            PIN CLR CLR
            PIN T XLXN_33
            PIN Q D(0)
        END BLOCK
        BEGIN BLOCK XLXI_3 ftce
            PIN C CLK
            PIN CE CLK_CE
            PIN CLR CLR
            PIN T XLXN_38
            PIN Q D(2)
        END BLOCK
        BEGIN BLOCK XLXI_41 xor2
            PIN I0 D(1)
            PIN I1 D(2)
            PIN O XLXN_51
        END BLOCK
        BEGIN BLOCK XLXI_43 and3b2
            PIN I0 XLXN_51
            PIN I1 D(3)
            PIN I2 D(0)
            PIN O XLXN_46
        END BLOCK
        BEGIN BLOCK XLXI_44 and3
            PIN I0 XLXN_51
            PIN I1 D(0)
            PIN I2 D(3)
            PIN O XLXN_43
        END BLOCK
        BEGIN BLOCK XLXI_45 or2
            PIN I0 XLXN_43
            PIN I1 XLXN_46
            PIN O XLXN_32
        END BLOCK
    END NETLIST
    BEGIN SHEET 1 3520 2720
        BEGIN BRANCH D(3)
            WIRE 704 384 784 384
            WIRE 784 384 912 384
            WIRE 912 384 912 2240
            WIRE 912 2240 1376 2240
            WIRE 800 2240 912 2240
            WIRE 1376 160 2720 160
            WIRE 2720 160 2896 160
            WIRE 2896 160 3264 160
            WIRE 2896 160 2896 544
            WIRE 2896 544 2896 816
            WIRE 2896 816 2896 1312
            WIRE 2896 1312 2896 2064
            WIRE 1376 160 1376 2240
            WIRE 2080 816 2896 816
            WIRE 2096 544 2896 544
            WIRE 2224 1312 2896 1312
            WIRE 2224 2064 2896 2064
            BEGIN DISPLAY 784 384 ATTR Name
                ALIGNMENT SOFT-BCENTER
            END DISPLAY
            BEGIN DISPLAY 2720 160 ATTR Name
                ALIGNMENT SOFT-BCENTER
            END DISPLAY
        END BRANCH
        BEGIN BRANCH D(0)
            WIRE 704 192 784 192
            WIRE 784 192 1008 192
            WIRE 1008 192 1008 528
            WIRE 1008 528 1248 528
            WIRE 784 528 1008 528
            WIRE 1248 64 1248 528
            WIRE 1248 64 2960 64
            WIRE 2960 64 3088 64
            WIRE 3088 64 3264 64
            WIRE 3088 64 3088 512
            WIRE 3088 512 3088 880
            WIRE 3088 880 3088 1456
            WIRE 3088 1456 3088 2000
            WIRE 1808 1456 3088 1456
            WIRE 1808 2000 3088 2000
            WIRE 2080 880 3088 880
            WIRE 2096 480 2160 480
            WIRE 2160 480 2160 512
            WIRE 2160 512 3088 512
            BEGIN DISPLAY 784 192 ATTR Name
                ALIGNMENT SOFT-BCENTER
            END DISPLAY
            BEGIN DISPLAY 2960 64 ATTR Name
                ALIGNMENT SOFT-BCENTER
            END DISPLAY
        END BRANCH
        BEGIN BRANCH D(1)
            WIRE 704 256 784 256
            WIRE 784 256 976 256
            WIRE 976 256 976 1136
            WIRE 976 1136 1296 1136
            WIRE 784 1136 976 1136
            WIRE 1296 96 1296 1136
            WIRE 1296 96 2848 96
            WIRE 2848 96 3024 96
            WIRE 3024 96 3264 96
            WIRE 3024 96 3024 416
            WIRE 3024 416 3024 1520
            WIRE 3024 1520 3024 1936
            WIRE 1808 1520 3024 1520
            WIRE 1808 1936 3024 1936
            WIRE 2400 384 2704 384
            WIRE 2704 384 2704 416
            WIRE 2704 416 3024 416
            BEGIN DISPLAY 784 256 ATTR Name
                ALIGNMENT SOFT-BCENTER
            END DISPLAY
            BEGIN DISPLAY 2848 96 ATTR Name
                ALIGNMENT SOFT-BCENTER
            END DISPLAY
        END BRANCH
        BEGIN BRANCH D(2)
            WIRE 704 320 784 320
            WIRE 784 320 944 320
            WIRE 944 320 944 1712
            WIRE 944 1712 1328 1712
            WIRE 800 1712 944 1712
            WIRE 1328 128 2784 128
            WIRE 2784 128 2960 128
            WIRE 2960 128 3264 128
            WIRE 2960 128 2960 480
            WIRE 2960 480 2960 1376
            WIRE 2960 1376 2960 2128
            WIRE 1328 128 1328 1712
            WIRE 2224 1376 2960 1376
            WIRE 2224 2128 2960 2128
            WIRE 2400 448 2672 448
            WIRE 2672 448 2672 480
            WIRE 2672 480 2960 480
            BEGIN DISPLAY 784 320 ATTR Name
                ALIGNMENT SOFT-BCENTER
            END DISPLAY
            BEGIN DISPLAY 2784 128 ATTR Name
                ALIGNMENT SOFT-BCENTER
            END DISPLAY
        END BRANCH
        BEGIN BRANCH XLXN_32
            WIRE 384 1008 384 1136
            WIRE 384 1136 400 1136
            WIRE 384 1008 832 1008
            WIRE 832 656 832 1008
            WIRE 832 656 1472 656
        END BRANCH
        INSTANCE XLXI_30 2224 1248 R180
        BEGIN BRANCH XLXN_37
            WIRE 1808 1392 1952 1392
            WIRE 1952 1344 1968 1344
            WIRE 1952 1344 1952 1392
        END BRANCH
        INSTANCE XLXI_31 1808 1328 R180
        INSTANCE XLXI_32 2224 2000 R180
        BEGIN BRANCH XLXN_39
            WIRE 1808 2064 1952 2064
            WIRE 1952 2064 1952 2096
            WIRE 1952 2096 1968 2096
        END BRANCH
        INSTANCE XLXI_33 1808 1872 R180
        BEGIN BRANCH XLXN_38
            WIRE 400 1456 400 1712
            WIRE 400 1712 416 1712
            WIRE 400 1456 1552 1456
        END BRANCH
        BEGIN BRANCH XLXN_41
            WIRE 400 2000 400 2240
            WIRE 400 2240 416 2240
            WIRE 400 2000 1552 2000
        END BRANCH
        INSTANCE XLXI_4 416 2496 R0
        INSTANCE XLXI_2 400 1392 R0
        BEGIN BRANCH XLXN_33
            WIRE 368 288 448 288
            WIRE 368 288 368 528
            WIRE 368 528 400 528
        END BRANCH
        INSTANCE XLXI_27 704 128 R180
        INSTANCE XLXI_1 400 784 R0
        INSTANCE XLXI_3 416 1968 R0
        BEGIN BRANCH CLR
            WIRE 208 2464 336 2464
            WIRE 336 2464 416 2464
            WIRE 336 752 400 752
            WIRE 336 752 336 1360
            WIRE 336 1360 400 1360
            WIRE 336 1360 336 1936
            WIRE 336 1936 336 2464
            WIRE 336 1936 416 1936
        END BRANCH
        IOMARKER 208 2464 CLR R180 28
        BEGIN BRANCH CLK_CE
            WIRE 192 288 288 288
            WIRE 288 288 288 592
            WIRE 288 592 400 592
            WIRE 288 592 288 1200
            WIRE 288 1200 288 1776
            WIRE 288 1776 288 2304
            WIRE 288 2304 416 2304
            WIRE 288 1776 416 1776
            WIRE 288 1200 400 1200
        END BRANCH
        BEGIN BRANCH CLK
            WIRE 192 336 240 336
            WIRE 240 336 240 656
            WIRE 240 656 400 656
            WIRE 240 656 240 1264
            WIRE 240 1264 400 1264
            WIRE 240 1264 240 1840
            WIRE 240 1840 240 2368
            WIRE 240 2368 416 2368
            WIRE 240 1840 416 1840
        END BRANCH
        IOMARKER 192 288 CLK_CE R180 28
        IOMARKER 192 336 CLK R180 28
        BUSTAP 3360 64 3264 64
        BUSTAP 3360 96 3264 96
        BUSTAP 3360 128 3264 128
        BUSTAP 3360 160 3264 160
        BEGIN BRANCH D(3:0)
            WIRE 3360 32 3360 64
            WIRE 3360 64 3360 96
            WIRE 3360 96 3360 128
            WIRE 3360 128 3360 160
            WIRE 3360 160 3360 208
        END BRANCH
        IOMARKER 3360 208 D(3:0) R90 28
        BEGIN BRANCH XLXN_43
            WIRE 1728 624 1824 624
            WIRE 1824 480 1840 480
            WIRE 1824 480 1824 624
        END BRANCH
        BEGIN BRANCH XLXN_46
            WIRE 1728 688 1776 688
            WIRE 1776 688 1776 816
            WIRE 1776 816 1824 816
            WIRE 1824 816 1840 816
        END BRANCH
        INSTANCE XLXI_41 2400 320 R180
        INSTANCE XLXI_43 2080 688 R180
        BEGIN BRANCH XLXN_51
            WIRE 2080 752 2128 752
            WIRE 2096 416 2144 416
            WIRE 2096 416 2128 416
            WIRE 2128 416 2128 752
        END BRANCH
        INSTANCE XLXI_44 2096 352 R180
        INSTANCE XLXI_45 1728 560 R180
    END SHEET
END SCHEMATIC
