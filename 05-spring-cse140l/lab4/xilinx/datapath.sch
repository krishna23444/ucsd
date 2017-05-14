VERSION 6
BEGIN SCHEMATIC
    BEGIN ATTR DeviceFamilyName "virtex2"
        DELETE all:0
        EDITNAME all:0
        EDITTRAIT all:0
    END ATTR
    BEGIN NETLIST
        SIGNAL XLXN_33
        SIGNAL GE_FLG
        SIGNAL A(7:0)
        SIGNAL A(7:5)
        SIGNAL A(4:0)
        SIGNAL R1(4:0)
        SIGNAL B(7:0)
        SIGNAL R2(4:0)
        SIGNAL B(4:0)
        SIGNAL B(7:5)
        SIGNAL OV_FLG
        SIGNAL XLXN_95
        SIGNAL XLXN_5
        SIGNAL shf_of(1)
        SIGNAL shf_of(0)
        SIGNAL S(7:0)
        SIGNAL dpath_D(4:0)
        SIGNAL shf_of(1:0)
        SIGNAL R2(0)
        SIGNAL R2(1)
        SIGNAL R2(2)
        SIGNAL R2(3)
        SIGNAL R2(4)
        SIGNAL O(0)
        SIGNAL O(1)
        SIGNAL O(2)
        SIGNAL O(3)
        SIGNAL O(4)
        SIGNAL XLXN_249
        SIGNAL XLXN_253(1:0)
        SIGNAL sel(1:0)
        SIGNAL O(4:0)
        SIGNAL S(4:0)
        SIGNAL XLXN_257
        SIGNAL C(4:0)
        PORT Output GE_FLG
        PORT Input R1(4:0)
        PORT Input R2(4:0)
        PORT Output OV_FLG
        PORT Output dpath_D(4:0)
        PORT Input shf_of(1:0)
        PORT Input sel(1:0)
        BEGIN BLOCKDEF add8
            TIMESTAMP 2001 2 2 12 35 41
            LINE N 336 -128 336 -148 
            LINE N 384 -128 336 -128 
            LINE N 64 -288 64 -432 
            LINE N 128 -256 64 -288 
            LINE N 64 -224 128 -256 
            LINE N 64 -80 64 -224 
            LINE N 384 -160 64 -80 
            LINE N 384 -336 384 -160 
            LINE N 384 -352 384 -336 
            LINE N 64 -432 384 -352 
            LINE N 128 -448 64 -448 
            LINE N 128 -416 128 -448 
            LINE N 0 -448 64 -448 
            RECTANGLE N 384 -268 448 -244 
            LINE N 448 -256 384 -256 
            LINE N 0 -192 64 -192 
            LINE N 0 -320 64 -320 
            RECTANGLE N 0 -332 64 -308 
            RECTANGLE N 0 -204 64 -180 
            LINE N 240 -64 384 -64 
            LINE N 240 -124 240 -64 
            LINE N 448 -64 384 -64 
            LINE N 448 -128 384 -128 
        END BLOCKDEF
        BEGIN BLOCKDEF brlshft8
            TIMESTAMP 2001 2 2 12 39 57
            LINE N 0 -192 64 -192 
            RECTANGLE N 64 -896 320 -64 
            LINE N 384 -576 320 -576 
            LINE N 0 -576 64 -576 
            LINE N 0 -512 64 -512 
            LINE N 384 -512 320 -512 
            LINE N 384 -448 320 -448 
            LINE N 0 -448 64 -448 
            LINE N 0 -384 64 -384 
            LINE N 384 -384 320 -384 
            LINE N 384 -640 320 -640 
            LINE N 384 -704 320 -704 
            LINE N 384 -768 320 -768 
            LINE N 384 -832 320 -832 
            LINE N 0 -832 64 -832 
            LINE N 0 -768 64 -768 
            LINE N 0 -704 64 -704 
            LINE N 0 -640 64 -640 
            LINE N 0 -128 64 -128 
            LINE N 0 -256 64 -256 
        END BLOCKDEF
        BEGIN BLOCKDEF compm8
            TIMESTAMP 2001 2 2 12 36 26
            LINE N 0 -128 64 -128 
            RECTANGLE N 0 -140 64 -116 
            LINE N 0 -320 64 -320 
            RECTANGLE N 0 -332 64 -308 
            RECTANGLE N 64 -384 320 -64 
            LINE N 384 -192 320 -192 
            LINE N 384 -256 320 -256 
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
        BEGIN BLOCKDEF Mask5
            TIMESTAMP 2005 6 1 3 33 56
            RECTANGLE N 64 -128 320 0 
            LINE N 64 -96 0 -96 
            RECTANGLE N 0 -108 64 -84 
            LINE N 64 -32 0 -32 
            RECTANGLE N 0 -44 64 -20 
            LINE N 320 -96 384 -96 
            RECTANGLE N 320 -108 384 -84 
        END BLOCKDEF
        BEGIN BLOCKDEF inv
            TIMESTAMP 2001 2 2 12 38 38
            LINE N 0 -32 64 -32 
            LINE N 224 -32 160 -32 
            LINE N 64 -64 128 -32 
            LINE N 128 -32 64 0 
            LINE N 64 0 64 -64 
            CIRCLE N 128 -48 160 -16 
        END BLOCKDEF
        BEGIN BLOCKDEF buf
            TIMESTAMP 2001 2 2 12 35 54
            LINE N 0 -32 64 -32 
            LINE N 224 -32 128 -32 
            LINE N 64 0 128 -32 
            LINE N 128 -32 64 -64 
            LINE N 64 -64 64 0 
        END BLOCKDEF
        BEGIN BLOCKDEF Mux3_5bus_v2
            TIMESTAMP 2005 6 1 8 24 23
            RECTANGLE N 64 -256 256 -64 
            LINE N 64 -224 0 -224 
            RECTANGLE N 0 -236 64 -212 
            LINE N 64 -160 0 -160 
            RECTANGLE N 0 -172 64 -148 
            LINE N 64 -96 0 -96 
            RECTANGLE N 0 -108 64 -84 
            LINE N 256 -224 320 -224 
            RECTANGLE N 256 -236 320 -212 
            LINE N 160 -64 160 0 
            RECTANGLE N 148 -64 172 0 
        END BLOCKDEF
        BEGIN BLOCK XLXI_4 compm8
            PIN A(7:0) A(7:0)
            PIN B(7:0) B(7:0)
            PIN GT
            PIN LT XLXN_33
        END BLOCK
        BEGIN BLOCK XLXI_7 inv
            PIN I XLXN_33
            PIN O GE_FLG
        END BLOCK
        BEGIN BLOCK XLXI_9(4:0) buf
            PIN I R1(4:0)
            PIN O A(4:0)
        END BLOCK
        BEGIN BLOCK XLXI_16(4:0) buf
            PIN I R2(4:0)
            PIN O B(4:0)
        END BLOCK
        BEGIN BLOCK gndB(2:0) gnd
            PIN G B(7:5)
        END BLOCK
        BEGIN BLOCK gndA(2:0) gnd
            PIN G A(7:5)
        END BLOCK
        BEGIN BLOCK XLXI_6 Mask5
            PIN A(4:0) R1(4:0)
            PIN B(4:0) R2(4:0)
            PIN C(4:0) C(4:0)
        END BLOCK
        BEGIN BLOCK XLXI_3 brlshft8
            PIN I0 R2(4)
            PIN I1 R2(3)
            PIN I2 R2(2)
            PIN I3 R2(1)
            PIN I4 R2(0)
            PIN I5 XLXN_249
            PIN I6 XLXN_249
            PIN I7 XLXN_249
            PIN S0 shf_of(0)
            PIN S1 shf_of(1)
            PIN S2 XLXN_5
            PIN O0 O(4)
            PIN O1 O(3)
            PIN O2 O(2)
            PIN O3 O(1)
            PIN O4 O(0)
            PIN O5
            PIN O6
            PIN O7
        END BLOCK
        BEGIN BLOCK XLXI_5 gnd
            PIN G XLXN_249
        END BLOCK
        BEGIN BLOCK XLXI_33 gnd
            PIN G XLXN_95
        END BLOCK
        BEGIN BLOCK XLXI_54 gnd
            PIN G XLXN_5
        END BLOCK
        BEGIN BLOCK XLXI_2 add8
            PIN A(7:0) A(7:0)
            PIN B(7:0) B(7:0)
            PIN CI XLXN_95
            PIN CO
            PIN OFL OV_FLG
            PIN S(7:0) S(7:0)
        END BLOCK
        BEGIN BLOCK XLXI_55 Mux3_5bus_v2
            PIN C(4:0) C(4:0)
            PIN A(4:0) O(4:0)
            PIN B(4:0) S(4:0)
            PIN O(4:0) dpath_D(4:0)
            PIN sel(1:0) sel(1:0)
        END BLOCK
    END NETLIST
    BEGIN SHEET 1 3520 2720
        INSTANCE XLXI_4 1136 624 R0
        BEGIN BRANCH XLXN_33
            WIRE 1520 432 1552 432
        END BRANCH
        INSTANCE XLXI_7 1552 464 R0
        BEGIN BRANCH GE_FLG
            WIRE 1776 432 2192 432
        END BRANCH
        BEGIN BRANCH A(7:0)
            WIRE 1040 416 1104 416
            WIRE 1040 416 1040 1072
            WIRE 1040 1072 1120 1072
            WIRE 1104 176 1104 208
            WIRE 1104 208 1104 272
            WIRE 1104 272 1104 304
            WIRE 1104 304 1136 304
            WIRE 1104 304 1104 368
            WIRE 1104 368 1104 416
            BEGIN DISPLAY 1104 368 ATTR Name
                ALIGNMENT SOFT-TVCENTER
            END DISPLAY
        END BRANCH
        BEGIN BRANCH A(7:5)
            WIRE 992 272 1008 272
            WIRE 992 272 992 320
            BEGIN DISPLAY 992 272 ATTR Name
                ALIGNMENT SOFT-BCENTER
            END DISPLAY
        END BRANCH
        BEGIN BRANCH A(4:0)
            WIRE 976 208 992 208
            WIRE 992 208 1008 208
            BEGIN DISPLAY 992 208 ATTR Name
                ALIGNMENT SOFT-BCENTER
            END DISPLAY
        END BRANCH
        BUSTAP 1104 272 1008 272
        BUSTAP 1104 208 1008 208
        INSTANCE XLXI_9(4:0) 752 240 R0
        BEGIN BRANCH R1(4:0)
            WIRE 608 128 704 128
            WIRE 704 128 752 128
            WIRE 752 128 752 208
            WIRE 752 208 752 1456
            WIRE 752 1456 1136 1456
            BEGIN DISPLAY 704 128 ATTR Name
                ALIGNMENT SOFT-BCENTER
            END DISPLAY
        END BRANCH
        BEGIN BRANCH B(7:0)
            WIRE 1088 688 1088 1200
            WIRE 1088 1200 1120 1200
            WIRE 1088 688 1104 688
            WIRE 1104 496 1104 528
            WIRE 1104 528 1104 592
            WIRE 1104 592 1104 688
            WIRE 1104 496 1120 496
            WIRE 1120 496 1136 496
            BEGIN DISPLAY 1120 496 ATTR Name
                ALIGNMENT SOFT-BCENTER
            END DISPLAY
        END BRANCH
        BUSTAP 1104 528 1008 528
        BEGIN BRANCH R2(4:0)
            WIRE 608 176 704 176
            WIRE 704 176 704 528
            WIRE 704 528 736 528
            WIRE 704 528 704 1520
            WIRE 704 1520 1136 1520
            WIRE 704 1520 704 1648
            WIRE 704 1648 1008 1648
            WIRE 1008 1648 1008 1728
            WIRE 1008 1728 1008 1792
            WIRE 1008 1792 1008 1856
            WIRE 1008 1856 1008 1920
            WIRE 1008 1920 1008 1984
            WIRE 1008 1984 1008 2032
            BEGIN DISPLAY 704 176 ATTR Name
                ALIGNMENT SOFT-BCENTER
            END DISPLAY
        END BRANCH
        INSTANCE XLXI_16(4:0) 736 560 R0
        BEGIN BRANCH B(4:0)
            WIRE 960 528 1008 528
        END BRANCH
        BUSTAP 1104 592 1008 592
        INSTANCE gndB(2:0) 928 784 R0
        BEGIN BRANCH B(7:5)
            WIRE 992 592 1008 592
            WIRE 992 592 992 656
            BEGIN DISPLAY 992 592 ATTR Name
                ALIGNMENT SOFT-BCENTER
            END DISPLAY
        END BRANCH
        INSTANCE gndA(2:0) 928 448 R0
        BEGIN BRANCH OV_FLG
            WIRE 1568 1264 2208 1264
        END BRANCH
        BEGIN BRANCH XLXN_95
            WIRE 1120 784 1216 784
            WIRE 1120 784 1120 944
        END BRANCH
        BEGIN INSTANCE XLXI_6 1136 1552 R0
        END INSTANCE
        BUSTAP 1008 2304 1104 2304
        BUSTAP 1008 2368 1104 2368
        BEGIN BRANCH XLXN_5
            WIRE 1104 2432 1136 2432
        END BRANCH
        BEGIN BRANCH shf_of(1)
            WIRE 1104 2368 1120 2368
            WIRE 1120 2368 1136 2368
            BEGIN DISPLAY 1120 2368 ATTR Name
                ALIGNMENT SOFT-BCENTER
            END DISPLAY
        END BRANCH
        IOMARKER 608 128 R1(4:0) R180 28
        IOMARKER 608 176 R2(4:0) R180 28
        BUSTAP 1632 1200 1728 1200
        BEGIN BRANCH S(7:0)
            WIRE 1568 1136 1632 1136
            WIRE 1632 1136 1632 1200
            WIRE 1632 1200 1632 1232
        END BRANCH
        BEGIN BRANCH dpath_D(4:0)
            WIRE 2176 1392 2208 1392
        END BRANCH
        IOMARKER 2208 1392 dpath_D(4:0) R0 28
        INSTANCE XLXI_54 1040 2560 R0
        BEGIN BRANCH shf_of(1:0)
            WIRE 640 2448 896 2448
            WIRE 896 2448 1008 2448
            WIRE 1008 2272 1008 2304
            WIRE 1008 2304 1008 2368
            WIRE 1008 2368 1008 2448
            BEGIN DISPLAY 896 2448 ATTR Name
                ALIGNMENT SOFT-BCENTER
            END DISPLAY
        END BRANCH
        INSTANCE XLXI_2 1120 1392 R0
        INSTANCE XLXI_33 1152 912 R0
        IOMARKER 2208 1264 OV_FLG R0 28
        IOMARKER 1872 1664 sel(1:0) R180 28
        IOMARKER 640 2448 shf_of(1:0) R180 28
        IOMARKER 2192 432 GE_FLG R0 28
        BEGIN BRANCH shf_of(0)
            WIRE 1104 2304 1120 2304
            WIRE 1120 2304 1136 2304
            BEGIN DISPLAY 1120 2304 ATTR Name
                ALIGNMENT SOFT-BCENTER
            END DISPLAY
        END BRANCH
        INSTANCE XLXI_3 1136 2560 R0
        BUSTAP 1008 1984 1104 1984
        BEGIN BRANCH R2(0)
            WIRE 1104 1984 1120 1984
            WIRE 1120 1984 1136 1984
            BEGIN DISPLAY 1120 1984 ATTR Name
                ALIGNMENT SOFT-BCENTER
            END DISPLAY
        END BRANCH
        BEGIN BRANCH R2(1)
            WIRE 1104 1920 1120 1920
            WIRE 1120 1920 1136 1920
            BEGIN DISPLAY 1120 1920 ATTR Name
                ALIGNMENT SOFT-BCENTER
            END DISPLAY
        END BRANCH
        BUSTAP 1008 1920 1104 1920
        BUSTAP 1008 1856 1104 1856
        BEGIN BRANCH R2(2)
            WIRE 1104 1856 1120 1856
            WIRE 1120 1856 1136 1856
            BEGIN DISPLAY 1120 1856 ATTR Name
                ALIGNMENT SOFT-BCENTER
            END DISPLAY
        END BRANCH
        BEGIN BRANCH R2(3)
            WIRE 1104 1792 1120 1792
            WIRE 1120 1792 1136 1792
            BEGIN DISPLAY 1120 1792 ATTR Name
                ALIGNMENT SOFT-BCENTER
            END DISPLAY
        END BRANCH
        BUSTAP 1008 1792 1104 1792
        BUSTAP 1008 1728 1104 1728
        BEGIN BRANCH R2(4)
            WIRE 1104 1728 1120 1728
            WIRE 1120 1728 1136 1728
            BEGIN DISPLAY 1120 1728 ATTR Name
                ALIGNMENT SOFT-BCENTER
            END DISPLAY
        END BRANCH
        BUSTAP 1648 1984 1552 1984
        BEGIN BRANCH O(0)
            WIRE 1520 1984 1536 1984
            WIRE 1536 1984 1552 1984
            BEGIN DISPLAY 1536 1984 ATTR Name
                ALIGNMENT SOFT-BCENTER
            END DISPLAY
        END BRANCH
        BUSTAP 1648 1920 1552 1920
        BEGIN BRANCH O(1)
            WIRE 1520 1920 1536 1920
            WIRE 1536 1920 1552 1920
            BEGIN DISPLAY 1536 1920 ATTR Name
                ALIGNMENT SOFT-BCENTER
            END DISPLAY
        END BRANCH
        BUSTAP 1648 1856 1552 1856
        BEGIN BRANCH O(2)
            WIRE 1520 1856 1536 1856
            WIRE 1536 1856 1552 1856
            BEGIN DISPLAY 1536 1856 ATTR Name
                ALIGNMENT SOFT-BCENTER
            END DISPLAY
        END BRANCH
        BUSTAP 1648 1792 1552 1792
        BEGIN BRANCH O(3)
            WIRE 1520 1792 1536 1792
            WIRE 1536 1792 1552 1792
            BEGIN DISPLAY 1536 1792 ATTR Name
                ALIGNMENT SOFT-BCENTER
            END DISPLAY
        END BRANCH
        BUSTAP 1648 1728 1552 1728
        BEGIN BRANCH O(4)
            WIRE 1520 1728 1536 1728
            WIRE 1536 1728 1552 1728
            BEGIN DISPLAY 1536 1728 ATTR Name
                ALIGNMENT SOFT-BCENTER
            END DISPLAY
        END BRANCH
        INSTANCE XLXI_5 1024 2272 R0
        BEGIN BRANCH XLXN_249
            WIRE 1088 2112 1120 2112
            WIRE 1120 2112 1120 2176
            WIRE 1120 2176 1136 2176
            WIRE 1120 2112 1136 2112
            WIRE 1088 2112 1088 2144
            WIRE 1120 2048 1136 2048
            WIRE 1120 2048 1120 2112
        END BRANCH
        BEGIN INSTANCE XLXI_55 1856 1616 R0
        END INSTANCE
        BEGIN BRANCH sel(1:0)
            WIRE 1872 1664 2016 1664
            WIRE 2016 1616 2016 1664
        END BRANCH
        BEGIN BRANCH O(4:0)
            WIRE 1648 1456 1808 1456
            WIRE 1808 1456 1856 1456
            WIRE 1648 1456 1648 2016
            BEGIN DISPLAY 1808 1456 ATTR Name
                ALIGNMENT SOFT-BCENTER
            END DISPLAY
        END BRANCH
        BEGIN BRANCH S(4:0)
            WIRE 1728 1200 1728 1520
            WIRE 1728 1520 1808 1520
            WIRE 1808 1520 1856 1520
            BEGIN DISPLAY 1808 1520 ATTR Name
                ALIGNMENT SOFT-BCENTER
            END DISPLAY
        END BRANCH
        BEGIN BRANCH C(4:0)
            WIRE 1520 1456 1600 1456
            WIRE 1600 1392 1600 1456
            WIRE 1600 1392 1808 1392
            WIRE 1808 1392 1856 1392
            BEGIN DISPLAY 1808 1392 ATTR Name
                ALIGNMENT SOFT-BCENTER
            END DISPLAY
        END BRANCH
    END SHEET
END SCHEMATIC
