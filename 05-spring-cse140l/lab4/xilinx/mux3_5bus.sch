VERSION 6
BEGIN SCHEMATIC
    BEGIN ATTR DeviceFamilyName "virtex2"
        DELETE all:0
        EDITNAME all:0
        EDITTRAIT all:0
    END ATTR
    BEGIN NETLIST
        SIGNAL B(4:0)
        SIGNAL O(4:0)
        SIGNAL A(0)
        SIGNAL B(0)
        SIGNAL A(4:0)
        SIGNAL C(4:0)
        SIGNAL C(0)
        SIGNAL S(1:0)
        SIGNAL S(0)
        SIGNAL S(1)
        SIGNAL XLXN_30
        SIGNAL O(0)
        SIGNAL A(1)
        SIGNAL B(1)
        SIGNAL C(1)
        SIGNAL XLXN_51
        SIGNAL O(1)
        SIGNAL A(2)
        SIGNAL B(2)
        SIGNAL C(2)
        SIGNAL XLXN_155
        SIGNAL O(2)
        SIGNAL A(3)
        SIGNAL B(3)
        SIGNAL C(3)
        SIGNAL XLXN_162
        SIGNAL O(3)
        SIGNAL A(4)
        SIGNAL B(4)
        SIGNAL C(4)
        SIGNAL XLXN_169
        SIGNAL O(4)
        PORT Input B(4:0)
        PORT Output O(4:0)
        PORT Input A(4:0)
        PORT Input C(4:0)
        PORT Input S(1:0)
        BEGIN BLOCKDEF m4_1e
            TIMESTAMP 2001 2 2 12 39 29
            LINE N 0 -416 96 -416 
            LINE N 0 -352 96 -352 
            LINE N 0 -288 96 -288 
            LINE N 0 -224 96 -224 
            LINE N 0 -32 96 -32 
            LINE N 320 -320 256 -320 
            LINE N 0 -160 96 -160 
            LINE N 0 -96 96 -96 
            LINE N 176 -96 96 -96 
            LINE N 176 -208 176 -96 
            LINE N 224 -32 96 -32 
            LINE N 224 -216 224 -32 
            LINE N 256 -224 96 -192 
            LINE N 256 -416 256 -224 
            LINE N 96 -448 256 -416 
            LINE N 96 -192 96 -448 
            LINE N 128 -160 96 -160 
            LINE N 128 -200 128 -160 
        END BLOCKDEF
        BEGIN BLOCKDEF gnd
            TIMESTAMP 2001 2 2 12 37 29
            LINE N 64 -64 64 -96 
            LINE N 76 -48 52 -48 
            LINE N 68 -32 60 -32 
            LINE N 88 -64 40 -64 
            LINE N 64 -64 64 -80 
            LINE N 64 -128 64 -96 
        END BLOCKDEF
        BEGIN BLOCK XLXI_9 m4_1e
            PIN D0 A(0)
            PIN D1 B(0)
            PIN D2 C(0)
            PIN D3 XLXN_30
            PIN E XLXN_30
            PIN S0 S(0)
            PIN S1 S(1)
            PIN O O(0)
        END BLOCK
        BEGIN BLOCK XLXI_10 gnd
            PIN G XLXN_30
        END BLOCK
        BEGIN BLOCK XLXI_14 m4_1e
            PIN D0 A(1)
            PIN D1 B(1)
            PIN D2 C(1)
            PIN D3 XLXN_51
            PIN E XLXN_51
            PIN S0 S(0)
            PIN S1 S(1)
            PIN O O(1)
        END BLOCK
        BEGIN BLOCK XLXI_15 gnd
            PIN G XLXN_51
        END BLOCK
        BEGIN BLOCK XLXI_38 m4_1e
            PIN D0 A(2)
            PIN D1 B(2)
            PIN D2 C(2)
            PIN D3 XLXN_155
            PIN E XLXN_155
            PIN S0 S(0)
            PIN S1 S(1)
            PIN O O(2)
        END BLOCK
        BEGIN BLOCK XLXI_39 gnd
            PIN G XLXN_155
        END BLOCK
        BEGIN BLOCK XLXI_40 m4_1e
            PIN D0 A(3)
            PIN D1 B(3)
            PIN D2 C(3)
            PIN D3 XLXN_162
            PIN E XLXN_162
            PIN S0 S(0)
            PIN S1 S(1)
            PIN O O(3)
        END BLOCK
        BEGIN BLOCK XLXI_41 gnd
            PIN G XLXN_162
        END BLOCK
        BEGIN BLOCK XLXI_42 m4_1e
            PIN D0 A(4)
            PIN D1 B(4)
            PIN D2 C(4)
            PIN D3 XLXN_169
            PIN E XLXN_169
            PIN S0 S(0)
            PIN S1 S(1)
            PIN O O(4)
        END BLOCK
        BEGIN BLOCK XLXI_43 gnd
            PIN G XLXN_169
        END BLOCK
    END NETLIST
    BEGIN SHEET 1 3520 2720
        BEGIN BRANCH B(4:0)
            WIRE 144 176 224 176
            WIRE 224 176 256 176
            WIRE 256 176 256 304
            WIRE 256 304 256 784
            WIRE 256 784 256 1264
            WIRE 256 1264 256 1744
            WIRE 256 1744 256 2224
            WIRE 256 2224 256 2688
            BEGIN DISPLAY 224 176 ATTR Name
                ALIGNMENT SOFT-BCENTER
            END DISPLAY
        END BRANCH
        BEGIN BRANCH O(4:0)
            WIRE 864 112 912 112
            WIRE 864 112 864 224
            WIRE 864 224 864 336
            WIRE 864 336 864 816
            WIRE 864 816 864 1296
            WIRE 864 1296 864 1776
            WIRE 864 1776 864 2256
            WIRE 864 2256 864 2688
            BEGIN DISPLAY 864 224 ATTR Name
                ALIGNMENT SOFT-TVCENTER
            END DISPLAY
        END BRANCH
        BEGIN BRANCH A(0)
            WIRE 384 240 400 240
            WIRE 400 240 416 240
            BEGIN DISPLAY 400 240 ATTR Name
                ALIGNMENT SOFT-BCENTER
            END DISPLAY
        END BRANCH
        BUSTAP 288 240 384 240
        BUSTAP 256 304 352 304
        BEGIN BRANCH B(0)
            WIRE 352 304 400 304
            WIRE 400 304 416 304
            BEGIN DISPLAY 400 304 ATTR Name
                ALIGNMENT SOFT-BCENTER
            END DISPLAY
        END BRANCH
        BEGIN BRANCH A(4:0)
            WIRE 144 112 224 112
            WIRE 224 112 288 112
            WIRE 288 112 288 240
            WIRE 288 240 288 720
            WIRE 288 720 288 1200
            WIRE 288 1200 288 1680
            WIRE 288 1680 288 2160
            WIRE 288 2160 288 2688
            BEGIN DISPLAY 224 112 ATTR Name
                ALIGNMENT SOFT-BCENTER
            END DISPLAY
        END BRANCH
        BEGIN BRANCH C(4:0)
            WIRE 144 240 224 240
            WIRE 224 240 224 368
            WIRE 224 368 224 848
            WIRE 224 848 224 1328
            WIRE 224 1328 224 1808
            WIRE 224 1808 224 2288
            WIRE 224 2288 224 2688
            BEGIN DISPLAY 224 240 ATTR Name
                ALIGNMENT SOFT-BCENTER
            END DISPLAY
        END BRANCH
        INSTANCE XLXI_9 416 656 R0
        BUSTAP 224 368 320 368
        BEGIN BRANCH C(0)
            WIRE 320 368 400 368
            WIRE 400 368 416 368
            BEGIN DISPLAY 400 368 ATTR Name
                ALIGNMENT SOFT-BCENTER
            END DISPLAY
        END BRANCH
        BUSTAP 176 496 272 496
        BUSTAP 176 560 272 560
        BEGIN BRANCH S(1:0)
            WIRE 112 48 176 48
            WIRE 176 48 176 976
            WIRE 176 976 176 1040
            WIRE 176 1040 176 1456
            WIRE 176 1456 176 1520
            WIRE 176 1520 176 1936
            WIRE 176 1936 176 2000
            WIRE 176 2000 176 2416
            WIRE 176 2416 176 2480
            WIRE 176 2480 176 2688
            BEGIN DISPLAY 176 48 ATTR Name
                ALIGNMENT SOFT-BCENTER
            END DISPLAY
        END BRANCH
        BEGIN BRANCH S(0)
            WIRE 272 496 320 496
            WIRE 320 496 416 496
            BEGIN DISPLAY 320 496 ATTR Name
                ALIGNMENT SOFT-BCENTER
            END DISPLAY
        END BRANCH
        BEGIN BRANCH S(1)
            WIRE 272 560 320 560
            WIRE 320 560 416 560
            BEGIN DISPLAY 320 560 ATTR Name
                ALIGNMENT SOFT-BCENTER
            END DISPLAY
        END BRANCH
        BEGIN BRANCH XLXN_30
            WIRE 352 432 352 528
            WIRE 352 528 352 544
            WIRE 352 528 400 528
            WIRE 400 528 400 624
            WIRE 400 624 416 624
            WIRE 352 432 416 432
        END BRANCH
        INSTANCE XLXI_10 288 672 R0
        BUSTAP 864 336 768 336
        BEGIN BRANCH O(0)
            WIRE 736 336 752 336
            WIRE 752 336 768 336
            BEGIN DISPLAY 752 336 ATTR Name
                ALIGNMENT SOFT-BCENTER
            END DISPLAY
        END BRANCH
        IOMARKER 144 240 C(4:0) R180 28
        IOMARKER 144 176 B(4:0) R180 28
        IOMARKER 144 112 A(4:0) R180 28
        IOMARKER 112 48 S(1:0) R180 28
        BEGIN BRANCH A(1)
            WIRE 384 720 400 720
            WIRE 400 720 416 720
            BEGIN DISPLAY 400 720 ATTR Name
                ALIGNMENT SOFT-BCENTER
            END DISPLAY
        END BRANCH
        BUSTAP 288 720 384 720
        BUSTAP 256 784 352 784
        BEGIN BRANCH B(1)
            WIRE 352 784 400 784
            WIRE 400 784 416 784
            BEGIN DISPLAY 400 784 ATTR Name
                ALIGNMENT SOFT-BCENTER
            END DISPLAY
        END BRANCH
        INSTANCE XLXI_14 416 1136 R0
        BUSTAP 224 848 320 848
        BEGIN BRANCH C(1)
            WIRE 320 848 400 848
            WIRE 400 848 416 848
            BEGIN DISPLAY 400 848 ATTR Name
                ALIGNMENT SOFT-BCENTER
            END DISPLAY
        END BRANCH
        BEGIN BRANCH S(0)
            WIRE 272 976 320 976
            WIRE 320 976 416 976
            BEGIN DISPLAY 320 976 ATTR Name
                ALIGNMENT SOFT-BCENTER
            END DISPLAY
        END BRANCH
        BEGIN BRANCH S(1)
            WIRE 272 1040 320 1040
            WIRE 320 1040 416 1040
            BEGIN DISPLAY 320 1040 ATTR Name
                ALIGNMENT SOFT-BCENTER
            END DISPLAY
        END BRANCH
        BEGIN BRANCH XLXN_51
            WIRE 352 912 352 1008
            WIRE 352 1008 352 1024
            WIRE 352 1008 400 1008
            WIRE 400 1008 400 1104
            WIRE 400 1104 416 1104
            WIRE 352 912 416 912
        END BRANCH
        INSTANCE XLXI_15 288 1152 R0
        BUSTAP 864 816 768 816
        BEGIN BRANCH O(1)
            WIRE 736 816 752 816
            WIRE 752 816 768 816
            BEGIN DISPLAY 752 816 ATTR Name
                ALIGNMENT SOFT-BCENTER
            END DISPLAY
        END BRANCH
        BUSTAP 176 976 272 976
        BUSTAP 176 1040 272 1040
        IOMARKER 912 112 O(4:0) R0 28
        BEGIN BRANCH A(2)
            WIRE 384 1200 400 1200
            WIRE 400 1200 416 1200
            BEGIN DISPLAY 400 1200 ATTR Name
                ALIGNMENT SOFT-BCENTER
            END DISPLAY
        END BRANCH
        BUSTAP 288 1200 384 1200
        BUSTAP 256 1264 352 1264
        BEGIN BRANCH B(2)
            WIRE 352 1264 400 1264
            WIRE 400 1264 416 1264
            BEGIN DISPLAY 400 1264 ATTR Name
                ALIGNMENT SOFT-BCENTER
            END DISPLAY
        END BRANCH
        INSTANCE XLXI_38 416 1616 R0
        BUSTAP 224 1328 320 1328
        BEGIN BRANCH C(2)
            WIRE 320 1328 400 1328
            WIRE 400 1328 416 1328
            BEGIN DISPLAY 400 1328 ATTR Name
                ALIGNMENT SOFT-BCENTER
            END DISPLAY
        END BRANCH
        BEGIN BRANCH S(0)
            WIRE 272 1456 320 1456
            WIRE 320 1456 416 1456
            BEGIN DISPLAY 320 1456 ATTR Name
                ALIGNMENT SOFT-BCENTER
            END DISPLAY
        END BRANCH
        BEGIN BRANCH S(1)
            WIRE 272 1520 320 1520
            WIRE 320 1520 416 1520
            BEGIN DISPLAY 320 1520 ATTR Name
                ALIGNMENT SOFT-BCENTER
            END DISPLAY
        END BRANCH
        BEGIN BRANCH XLXN_155
            WIRE 352 1392 416 1392
            WIRE 352 1392 352 1488
            WIRE 352 1488 352 1504
            WIRE 352 1488 400 1488
            WIRE 400 1488 400 1584
            WIRE 400 1584 416 1584
        END BRANCH
        INSTANCE XLXI_39 288 1632 R0
        BUSTAP 864 1296 768 1296
        BEGIN BRANCH O(2)
            WIRE 736 1296 752 1296
            WIRE 752 1296 768 1296
            BEGIN DISPLAY 752 1296 ATTR Name
                ALIGNMENT SOFT-BCENTER
            END DISPLAY
        END BRANCH
        BUSTAP 176 1456 272 1456
        BUSTAP 176 1520 272 1520
        BEGIN BRANCH A(3)
            WIRE 384 1680 400 1680
            WIRE 400 1680 416 1680
            BEGIN DISPLAY 400 1680 ATTR Name
                ALIGNMENT SOFT-BCENTER
            END DISPLAY
        END BRANCH
        BUSTAP 288 1680 384 1680
        BUSTAP 256 1744 352 1744
        BEGIN BRANCH B(3)
            WIRE 352 1744 400 1744
            WIRE 400 1744 416 1744
            BEGIN DISPLAY 400 1744 ATTR Name
                ALIGNMENT SOFT-BCENTER
            END DISPLAY
        END BRANCH
        INSTANCE XLXI_40 416 2096 R0
        BUSTAP 224 1808 320 1808
        BEGIN BRANCH C(3)
            WIRE 320 1808 400 1808
            WIRE 400 1808 416 1808
            BEGIN DISPLAY 400 1808 ATTR Name
                ALIGNMENT SOFT-BCENTER
            END DISPLAY
        END BRANCH
        BEGIN BRANCH S(0)
            WIRE 272 1936 320 1936
            WIRE 320 1936 416 1936
            BEGIN DISPLAY 320 1936 ATTR Name
                ALIGNMENT SOFT-BCENTER
            END DISPLAY
        END BRANCH
        BEGIN BRANCH S(1)
            WIRE 272 2000 320 2000
            WIRE 320 2000 416 2000
            BEGIN DISPLAY 320 2000 ATTR Name
                ALIGNMENT SOFT-BCENTER
            END DISPLAY
        END BRANCH
        BEGIN BRANCH XLXN_162
            WIRE 352 1872 416 1872
            WIRE 352 1872 352 1968
            WIRE 352 1968 352 1984
            WIRE 352 1968 400 1968
            WIRE 400 1968 400 2064
            WIRE 400 2064 416 2064
        END BRANCH
        INSTANCE XLXI_41 288 2112 R0
        BUSTAP 864 1776 768 1776
        BEGIN BRANCH O(3)
            WIRE 736 1776 752 1776
            WIRE 752 1776 768 1776
            BEGIN DISPLAY 752 1776 ATTR Name
                ALIGNMENT SOFT-BCENTER
            END DISPLAY
        END BRANCH
        BUSTAP 176 1936 272 1936
        BUSTAP 176 2000 272 2000
        BEGIN BRANCH A(4)
            WIRE 384 2160 400 2160
            WIRE 400 2160 416 2160
            BEGIN DISPLAY 400 2160 ATTR Name
                ALIGNMENT SOFT-BCENTER
            END DISPLAY
        END BRANCH
        BUSTAP 288 2160 384 2160
        BUSTAP 256 2224 352 2224
        BEGIN BRANCH B(4)
            WIRE 352 2224 400 2224
            WIRE 400 2224 416 2224
            BEGIN DISPLAY 400 2224 ATTR Name
                ALIGNMENT SOFT-BCENTER
            END DISPLAY
        END BRANCH
        BUSTAP 224 2288 320 2288
        BEGIN BRANCH C(4)
            WIRE 320 2288 400 2288
            WIRE 400 2288 416 2288
            BEGIN DISPLAY 400 2288 ATTR Name
                ALIGNMENT SOFT-BCENTER
            END DISPLAY
        END BRANCH
        BEGIN BRANCH S(0)
            WIRE 272 2416 320 2416
            WIRE 320 2416 416 2416
            BEGIN DISPLAY 320 2416 ATTR Name
                ALIGNMENT SOFT-BCENTER
            END DISPLAY
        END BRANCH
        BEGIN BRANCH S(1)
            WIRE 272 2480 320 2480
            WIRE 320 2480 416 2480
            BEGIN DISPLAY 320 2480 ATTR Name
                ALIGNMENT SOFT-BCENTER
            END DISPLAY
        END BRANCH
        BEGIN BRANCH XLXN_169
            WIRE 352 2352 352 2448
            WIRE 352 2448 352 2464
            WIRE 352 2448 384 2448
            WIRE 384 2448 384 2544
            WIRE 384 2544 416 2544
            WIRE 352 2352 416 2352
        END BRANCH
        INSTANCE XLXI_43 288 2592 R0
        BUSTAP 864 2256 768 2256
        BEGIN BRANCH O(4)
            WIRE 736 2256 752 2256
            WIRE 752 2256 768 2256
            BEGIN DISPLAY 752 2256 ATTR Name
                ALIGNMENT SOFT-BCENTER
            END DISPLAY
        END BRANCH
        BUSTAP 176 2416 272 2416
        BUSTAP 176 2480 272 2480
        INSTANCE XLXI_42 416 2576 R0
    END SHEET
END SCHEMATIC
