VERSION 6
BEGIN SCHEMATIC
    BEGIN ATTR DeviceFamilyName "virtex2"
        DELETE all:0
        EDITNAME all:0
        EDITTRAIT all:0
    END ATTR
    BEGIN NETLIST
        SIGNAL pc(3:0)
        SIGNAL clk_en
        SIGNAL clk
        SIGNAL rst
        SIGNAL R1(4:0)
        SIGNAL R2(4:0)
        SIGNAL R3(4:0)
        SIGNAL OV_FLG
        SIGNAL GE_FLG
        SIGNAL shf_of(1:0)
        SIGNAL sel(1:0)
        SIGNAL XLXN_23
        SIGNAL XLXN_24
        SIGNAL XLXN_25
        SIGNAL XLXN_26
        SIGNAL XLXN_27(4:0)
        SIGNAL XLXN_28
        SIGNAL XLXN_29
        SIGNAL XLXN_30
        SIGNAL XLXN_31
        SIGNAL XLXN_32
        SIGNAL inst(7:0)
        SIGNAL inst(4:0)
        SIGNAL inst(7:3)
        PORT Output pc(3:0)
        PORT Input clk_en
        PORT Input clk
        PORT Input rst
        PORT Output R1(4:0)
        PORT Output R2(4:0)
        PORT Output R3(4:0)
        PORT Output OV_FLG
        PORT Output GE_FLG
        PORT Output inst(7:0)
        BEGIN BLOCKDEF datapath
            TIMESTAMP 2005 6 1 5 58 23
            RECTANGLE N 64 -256 320 0 
            LINE N 64 -224 0 -224 
            RECTANGLE N 0 -236 64 -212 
            LINE N 64 -160 0 -160 
            RECTANGLE N 0 -172 64 -148 
            LINE N 64 -96 0 -96 
            RECTANGLE N 0 -108 64 -84 
            LINE N 64 -32 0 -32 
            RECTANGLE N 0 -44 64 -20 
            LINE N 320 -224 384 -224 
            LINE N 320 -160 384 -160 
            LINE N 320 -96 384 -96 
            RECTANGLE N 320 -108 384 -84 
        END BLOCKDEF
        BEGIN BLOCKDEF regs
            TIMESTAMP 2005 6 1 5 58 1
            RECTANGLE N 64 -768 320 0 
            LINE N 64 -736 0 -736 
            RECTANGLE N 0 -748 64 -724 
            LINE N 64 -672 0 -672 
            LINE N 64 -608 0 -608 
            LINE N 64 -544 0 -544 
            LINE N 64 -480 0 -480 
            LINE N 64 -416 0 -416 
            LINE N 64 -288 0 -288 
            LINE N 64 -224 0 -224 
            LINE N 64 -160 0 -160 
            LINE N 64 -96 0 -96 
            LINE N 64 -32 0 -32 
            LINE N 320 -736 384 -736 
            RECTANGLE N 320 -748 384 -724 
            LINE N 320 -672 384 -672 
            RECTANGLE N 320 -684 384 -660 
            LINE N 320 -608 384 -608 
            RECTANGLE N 320 -620 384 -596 
            LINE N 320 -544 384 -544 
            LINE N 320 -480 384 -480 
            LINE N 64 -352 0 -352 
            RECTANGLE N 0 -364 64 -340 
        END BLOCKDEF
        BEGIN BLOCKDEF pcounter
            TIMESTAMP 2005 6 1 5 56 55
            RECTANGLE N 64 -192 320 0 
            LINE N 64 -160 0 -160 
            LINE N 64 -96 0 -96 
            LINE N 64 -32 0 -32 
            LINE N 320 -160 384 -160 
            RECTANGLE N 320 -172 384 -148 
        END BLOCKDEF
        BEGIN BLOCKDEF memory16x8
            TIMESTAMP 2005 6 1 6 0 22
            RECTANGLE N 64 -64 320 0 
            LINE N 64 -32 0 -32 
            RECTANGLE N 0 -44 64 -20 
            LINE N 320 -32 384 -32 
            RECTANGLE N 320 -44 384 -20 
        END BLOCKDEF
        BEGIN BLOCKDEF truthtbl
            TIMESTAMP 2005 6 1 7 53 30
            LINE N 320 32 384 32 
            LINE N 320 96 384 96 
            LINE N 320 160 384 160 
            LINE N 320 224 384 224 
            LINE N 320 288 384 288 
            LINE N 320 352 384 352 
            LINE N 320 416 384 416 
            RECTANGLE N 320 468 384 492 
            LINE N 320 480 384 480 
            RECTANGLE N 320 532 384 556 
            LINE N 320 544 384 544 
            LINE N 0 44 64 44 
            RECTANGLE N 0 32 64 56 
            RECTANGLE N 68 0 324 560 
        END BLOCKDEF
        BEGIN BLOCK XLXI_2 regs
            PIN mem_D(4:0) inst(4:0)
            PIN R1_en XLXN_29
            PIN clk clk
            PIN clr XLXN_28
            PIN R2_sel XLXN_31
            PIN R2_en XLXN_30
            PIN R3_en XLXN_32
            PIN OV XLXN_26
            PIN ov_en XLXN_24
            PIN GE XLXN_25
            PIN ge_en XLXN_23
            PIN R1(4:0) R1(4:0)
            PIN R2(4:0) R2(4:0)
            PIN R3(4:0) R3(4:0)
            PIN OVF OV_FLG
            PIN GEF GE_FLG
            PIN dpath_D(4:0) XLXN_27(4:0)
        END BLOCK
        BEGIN BLOCK XLXI_3 pcounter
            PIN clk_en clk_en
            PIN clk clk
            PIN clr rst
            PIN addr(3:0) pc(3:0)
        END BLOCK
        BEGIN BLOCK XLXI_4 memory16x8
            PIN addr(3:0) pc(3:0)
            PIN data(7:0) inst(7:0)
        END BLOCK
        BEGIN BLOCK XLXI_5 truthtbl
            PIN D(7:3) inst(7:3)
            PIN clr XLXN_28
            PIN R1_en XLXN_29
            PIN R2_en XLXN_30
            PIN R2_sel XLXN_31
            PIN R3_en XLXN_32
            PIN ov_en XLXN_24
            PIN ge_en XLXN_23
            PIN sel(1:0) sel(1:0)
            PIN shf_of(1:0) shf_of(1:0)
        END BLOCK
        BEGIN BLOCK XLXI_1 datapath
            PIN R1(4:0) R1(4:0)
            PIN R2(4:0) R2(4:0)
            PIN sel(1:0) sel(1:0)
            PIN shf_of(1:0) shf_of(1:0)
            PIN GE_FLG XLXN_25
            PIN OV_FLG XLXN_26
            PIN dpath_D(4:0) XLXN_27(4:0)
        END BLOCK
    END NETLIST
    BEGIN SHEET 1 3520 2720
        BEGIN INSTANCE XLXI_3 864 864 R0
        END INSTANCE
        BEGIN BRANCH pc(3:0)
            WIRE 592 1152 592 1296
            WIRE 592 1296 656 1296
            WIRE 592 1152 1008 1152
            WIRE 1008 1152 1280 1152
            WIRE 1248 704 1280 704
            WIRE 1280 704 1312 704
            WIRE 1280 704 1280 1152
            BEGIN DISPLAY 1008 1152 ATTR Name
                ALIGNMENT SOFT-BCENTER
            END DISPLAY
        END BRANCH
        BEGIN BRANCH clk_en
            WIRE 816 704 864 704
        END BRANCH
        BEGIN BRANCH clk
            WIRE 816 768 848 768
            WIRE 848 768 864 768
            WIRE 848 576 848 768
            WIRE 848 576 1408 576
            WIRE 1408 576 2144 576
            BEGIN DISPLAY 1408 576 ATTR Name
                ALIGNMENT SOFT-BCENTER
            END DISPLAY
        END BRANCH
        BEGIN BRANCH rst
            WIRE 816 832 864 832
        END BRANCH
        IOMARKER 1312 704 pc(3:0) R0 28
        BEGIN BRANCH R3(4:0)
            WIRE 2528 576 2624 576
        END BRANCH
        BEGIN BRANCH OV_FLG
            WIRE 2528 640 2624 640
        END BRANCH
        BEGIN BRANCH GE_FLG
            WIRE 2528 704 2624 704
        END BRANCH
        IOMARKER 2624 448 R1(4:0) R0 28
        IOMARKER 2624 512 R2(4:0) R0 28
        IOMARKER 2624 576 R3(4:0) R0 28
        IOMARKER 2624 640 OV_FLG R0 28
        IOMARKER 2624 704 GE_FLG R0 28
        BEGIN BRANCH shf_of(1:0)
            WIRE 992 2176 1408 2176
            WIRE 1408 2176 1904 2176
            BEGIN DISPLAY 1408 2176 ATTR Name
                ALIGNMENT SOFT-BCENTER
            END DISPLAY
        END BRANCH
        BEGIN BRANCH sel(1:0)
            WIRE 992 2112 1408 2112
            WIRE 1408 2112 1904 2112
            BEGIN DISPLAY 1408 2112 ATTR Name
                ALIGNMENT SOFT-BCENTER
            END DISPLAY
        END BRANCH
        BEGIN INSTANCE XLXI_5 608 1632 R0
        END INSTANCE
        BEGIN BRANCH XLXN_28
            WIRE 992 1664 1552 1664
            WIRE 1552 640 1552 1664
            WIRE 1552 640 2144 640
        END BRANCH
        BEGIN BRANCH XLXN_29
            WIRE 992 1728 1152 1728
            WIRE 1152 1728 1536 1728
            WIRE 1536 512 1536 1728
            WIRE 1536 512 2144 512
        END BRANCH
        BEGIN BRANCH XLXN_30
            WIRE 992 1792 1568 1792
            WIRE 1568 768 1568 1792
            WIRE 1568 768 2144 768
        END BRANCH
        BEGIN BRANCH XLXN_31
            WIRE 992 1856 1520 1856
            WIRE 1520 704 1520 1856
            WIRE 1520 704 2144 704
        END BRANCH
        BEGIN BRANCH XLXN_32
            WIRE 992 1920 1584 1920
            WIRE 1584 896 1584 1920
            WIRE 1584 896 2144 896
        END BRANCH
        BEGIN BRANCH XLXN_24
            WIRE 992 1984 1600 1984
            WIRE 1600 1024 1600 1984
            WIRE 1600 1024 2144 1024
        END BRANCH
        BEGIN BRANCH XLXN_23
            WIRE 992 2048 1616 2048
            WIRE 1616 1152 1616 2048
            WIRE 1616 1152 2144 1152
        END BRANCH
        BEGIN INSTANCE XLXI_1 1904 2208 R0
        END INSTANCE
        BEGIN BRANCH R2(4:0)
            WIRE 1856 2048 1904 2048
            WIRE 1856 2048 1856 2272
            WIRE 1856 2272 2592 2272
            WIRE 2528 512 2592 512
            WIRE 2592 512 2624 512
            WIRE 2592 512 2592 2272
        END BRANCH
        BEGIN BRANCH R1(4:0)
            WIRE 1840 1984 1904 1984
            WIRE 1840 1984 1840 2288
            WIRE 1840 2288 2560 2288
            WIRE 2528 448 2560 448
            WIRE 2560 448 2624 448
            WIRE 2560 448 2560 2288
        END BRANCH
        BEGIN BRANCH XLXN_25
            WIRE 2128 1088 2128 1248
            WIRE 2128 1248 2352 1248
            WIRE 2352 1248 2352 1984
            WIRE 2128 1088 2144 1088
            WIRE 2288 1984 2352 1984
        END BRANCH
        BEGIN BRANCH XLXN_26
            WIRE 2112 960 2112 1200
            WIRE 2112 1200 2336 1200
            WIRE 2336 1200 2336 2048
            WIRE 2112 960 2144 960
            WIRE 2288 2048 2336 2048
        END BRANCH
        BEGIN BRANCH XLXN_27(4:0)
            WIRE 2096 832 2096 1280
            WIRE 2096 1280 2608 1280
            WIRE 2608 1280 2608 2112
            WIRE 2096 832 2144 832
            WIRE 2288 2112 2608 2112
        END BRANCH
        BEGIN INSTANCE XLXI_2 2144 1184 R0
        END INSTANCE
        IOMARKER 816 768 clk R180 28
        IOMARKER 816 832 rst R180 28
        IOMARKER 816 704 clk_en R180 28
        BEGIN INSTANCE XLXI_4 656 1328 R0
        END INSTANCE
        BEGIN BRANCH inst(7:0)
            WIRE 1040 1296 1216 1296
            WIRE 1216 1296 1216 1344
            WIRE 1216 1344 1216 1376
            WIRE 1216 1296 1280 1296
            WIRE 1216 1216 1216 1248
            WIRE 1216 1248 1216 1296
        END BRANCH
        BUSTAP 1216 1248 1312 1248
        BEGIN BRANCH inst(4:0)
            WIRE 1312 1248 1728 1248
            WIRE 1728 448 2144 448
            WIRE 1728 448 1728 832
            WIRE 1728 832 1728 1248
            BEGIN DISPLAY 1728 832 ATTR Name
                ALIGNMENT SOFT-TVCENTER
            END DISPLAY
        END BRANCH
        BEGIN BRANCH inst(7:3)
            WIRE 560 1440 832 1440
            WIRE 832 1440 1072 1440
            WIRE 560 1440 560 1680
            WIRE 560 1680 608 1680
            WIRE 1072 1344 1072 1440
            WIRE 1072 1344 1120 1344
            BEGIN DISPLAY 832 1440 ATTR Name
                ALIGNMENT SOFT-BCENTER
            END DISPLAY
        END BRANCH
        BUSTAP 1216 1344 1120 1344
        IOMARKER 1280 1296 inst(7:0) R0 28
    END SHEET
END SCHEMATIC
