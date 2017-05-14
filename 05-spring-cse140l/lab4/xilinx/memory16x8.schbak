VERSION 6
BEGIN SCHEMATIC
    BEGIN ATTR DeviceFamilyName "virtex2"
        DELETE all:0
        EDITNAME all:0
        EDITTRAIT all:0
    END ATTR
    BEGIN NETLIST
        SIGNAL addr(0)
        SIGNAL addr(1)
        SIGNAL addr(2)
        SIGNAL addr(3)
        SIGNAL data(7:0)
        SIGNAL data(1)
        SIGNAL data(6)
        SIGNAL data(0)
        SIGNAL data(2)
        SIGNAL data(3)
        SIGNAL data(4)
        SIGNAL data(5)
        SIGNAL data(7)
        SIGNAL addr(3:0)
        PORT Output data(7:0)
        PORT Input addr(3:0)
        BEGIN BLOCKDEF rom16x1
            TIMESTAMP 2001 2 2 12 39 16
            LINE N 0 -128 64 -128 
            LINE N 0 -192 64 -192 
            LINE N 0 -256 64 -256 
            LINE N 0 -320 64 -320 
            RECTANGLE N 64 -384 320 -64 
            LINE N 384 -320 320 -320 
        END BLOCKDEF
        BEGIN BLOCK XLXI_18 rom16x1
            BEGIN ATTR INIT "0404"
                VERILOG all:0 dp:1nosynth wsynop:1 wsynth:1
                VHDL all:0 gm:1nosynth wa:1 wd:1
                VALUETYPE BitVector 16 h
            END ATTR
            PIN A0 addr(0)
            PIN A1 addr(1)
            PIN A2 addr(2)
            PIN A3 addr(3)
            PIN O data(1)
        END BLOCK
        BEGIN BLOCK XLXI_9 rom16x1
            BEGIN ATTR INIT "1A94"
                VERILOG all:0 dp:1nosynth wsynop:1 wsynth:1
                VHDL all:0 gm:1nosynth wa:1 wd:1
                VALUETYPE BitVector 16 h
            END ATTR
            PIN A0 addr(0)
            PIN A1 addr(1)
            PIN A2 addr(2)
            PIN A3 addr(3)
            PIN O data(6)
        END BLOCK
        BEGIN BLOCK XLXI_16 rom16x1
            BEGIN ATTR INIT "0422"
                VERILOG all:0 dp:1nosynth wsynop:1 wsynth:1
                VHDL all:0 gm:1nosynth wa:1 wd:1
                VALUETYPE BitVector 16 h
            END ATTR
            PIN A0 addr(0)
            PIN A1 addr(1)
            PIN A2 addr(2)
            PIN A3 addr(3)
            PIN O data(0)
        END BLOCK
        BEGIN BLOCK XLXI_19 rom16x1
            BEGIN ATTR INIT "0406"
                VERILOG all:0 dp:1nosynth wsynop:1 wsynth:1
                VHDL all:0 gm:1nosynth wa:1 wd:1
                VALUETYPE BitVector 16 h
            END ATTR
            PIN A0 addr(0)
            PIN A1 addr(1)
            PIN A2 addr(2)
            PIN A3 addr(3)
            PIN O data(2)
        END BLOCK
        BEGIN BLOCK XLXI_20 rom16x1
            BEGIN ATTR INIT "0000"
                VERILOG all:0 dp:1nosynth wsynop:1 wsynth:1
                VHDL all:0 gm:1nosynth wa:1 wd:1
                VALUETYPE BitVector 16 h
            END ATTR
            PIN A0 addr(0)
            PIN A1 addr(1)
            PIN A2 addr(2)
            PIN A3 addr(3)
            PIN O data(3)
        END BLOCK
        BEGIN BLOCK XLXI_15 rom16x1
            BEGIN ATTR INIT "0102"
                VERILOG all:0 dp:1nosynth wsynop:1 wsynth:1
                VHDL all:0 gm:1nosynth wa:1 wd:1
                VALUETYPE BitVector 16 h
            END ATTR
            PIN A0 addr(0)
            PIN A1 addr(1)
            PIN A2 addr(2)
            PIN A3 addr(3)
            PIN O data(4)
        END BLOCK
        BEGIN BLOCK XLXI_14 rom16x1
            BEGIN ATTR INIT "17B2"
                VERILOG all:0 dp:1nosynth wsynop:1 wsynth:1
                VHDL all:0 gm:1nosynth wa:1 wd:1
                VALUETYPE BitVector 16 h
            END ATTR
            PIN A0 addr(0)
            PIN A1 addr(1)
            PIN A2 addr(2)
            PIN A3 addr(3)
            PIN O data(5)
        END BLOCK
        BEGIN BLOCK XLXI_1 rom16x1
            BEGIN ATTR INIT "1948"
                VERILOG all:0 dp:1nosynth wsynop:1 wsynth:1
                VHDL all:0 gm:1nosynth wa:1 wd:1
                VALUETYPE BitVector 16 h
            END ATTR
            PIN A0 addr(0)
            PIN A1 addr(1)
            PIN A2 addr(2)
            PIN A3 addr(3)
            PIN O data(7)
        END BLOCK
    END NETLIST
    BEGIN SHEET 1 3520 2720
        IOMARKER 800 432 addr(3:0) R180 28
        BEGIN BRANCH data(7:0)
            WIRE 1664 352 1664 384
            WIRE 1664 384 1664 816
            WIRE 1664 816 1664 1232
            WIRE 1664 1232 1664 1648
            WIRE 1664 1648 1664 1968
            WIRE 1664 1968 2368 1968
            WIRE 2368 336 2432 336
            WIRE 2432 336 2496 336
            WIRE 2368 336 2368 368
            WIRE 2368 368 2368 800
            WIRE 2368 800 2368 1232
            WIRE 2368 1232 2368 1664
            WIRE 2368 1664 2368 1968
            BEGIN DISPLAY 2432 336 ATTR Name
                ALIGNMENT SOFT-BCENTER
            END DISPLAY
        END BRANCH
        IOMARKER 2496 336 data(7:0) R0 28
        BEGIN BRANCH data(1)
            WIRE 2240 800 2256 800
            WIRE 2256 800 2272 800
            BEGIN DISPLAY 2256 800 ATTR Name
                ALIGNMENT SOFT-BCENTER
            END DISPLAY
        END BRANCH
        BUSTAP 2368 800 2272 800
        BUSTAP 1728 992 1824 992
        BUSTAP 1728 928 1824 928
        BUSTAP 1728 864 1824 864
        BUSTAP 1728 800 1824 800
        BEGIN BRANCH addr(3)
            WIRE 1824 992 1840 992
            WIRE 1840 992 1856 992
            BEGIN DISPLAY 1840 992 ATTR Name
                ALIGNMENT SOFT-BCENTER
            END DISPLAY
        END BRANCH
        BEGIN BRANCH addr(2)
            WIRE 1824 928 1840 928
            WIRE 1840 928 1856 928
            BEGIN DISPLAY 1840 928 ATTR Name
                ALIGNMENT SOFT-BCENTER
            END DISPLAY
        END BRANCH
        BEGIN BRANCH addr(1)
            WIRE 1824 864 1840 864
            WIRE 1840 864 1856 864
            BEGIN DISPLAY 1840 864 ATTR Name
                ALIGNMENT SOFT-BCENTER
            END DISPLAY
        END BRANCH
        BEGIN BRANCH addr(0)
            WIRE 1824 800 1840 800
            WIRE 1840 800 1856 800
            BEGIN DISPLAY 1840 800 ATTR Name
                ALIGNMENT SOFT-BCENTER
            END DISPLAY
        END BRANCH
        BEGIN INSTANCE XLXI_18 1856 1120 R0
            BEGIN DISPLAY 0 -456 ATTR INIT
                FONT 28 "Arial"
                DISPLAYFORMAT NAMEEQUALSVALUE
            END DISPLAY
        END INSTANCE
        BEGIN BRANCH data(6)
            WIRE 1536 816 1552 816
            WIRE 1552 816 1568 816
            BEGIN DISPLAY 1552 816 ATTR Name
                ALIGNMENT SOFT-BCENTER
            END DISPLAY
        END BRANCH
        BUSTAP 1664 816 1568 816
        BUSTAP 1024 1008 1120 1008
        BUSTAP 1024 944 1120 944
        BUSTAP 1024 880 1120 880
        BUSTAP 1024 816 1120 816
        BEGIN BRANCH addr(3)
            WIRE 1120 1008 1136 1008
            WIRE 1136 1008 1152 1008
            BEGIN DISPLAY 1136 1008 ATTR Name
                ALIGNMENT SOFT-BCENTER
            END DISPLAY
        END BRANCH
        BEGIN BRANCH addr(2)
            WIRE 1120 944 1136 944
            WIRE 1136 944 1152 944
            BEGIN DISPLAY 1136 944 ATTR Name
                ALIGNMENT SOFT-BCENTER
            END DISPLAY
        END BRANCH
        BEGIN BRANCH addr(1)
            WIRE 1120 880 1136 880
            WIRE 1136 880 1152 880
            BEGIN DISPLAY 1136 880 ATTR Name
                ALIGNMENT SOFT-BCENTER
            END DISPLAY
        END BRANCH
        BEGIN BRANCH addr(0)
            WIRE 1120 816 1136 816
            WIRE 1136 816 1152 816
            BEGIN DISPLAY 1136 816 ATTR Name
                ALIGNMENT SOFT-BCENTER
            END DISPLAY
        END BRANCH
        BEGIN INSTANCE XLXI_9 1152 1136 R0
            BEGIN DISPLAY 4 -456 ATTR INIT
                FONT 28 "Arial"
                DISPLAYFORMAT NAMEEQUALSVALUE
            END DISPLAY
        END INSTANCE
        BEGIN INSTANCE XLXI_16 1856 688 R0
            BEGIN DISPLAY 0 -472 ATTR INIT
                FONT 28 "Arial"
                DISPLAYFORMAT NAMEEQUALSVALUE
            END DISPLAY
        END INSTANCE
        BEGIN BRANCH addr(0)
            WIRE 1824 368 1840 368
            WIRE 1840 368 1856 368
            BEGIN DISPLAY 1840 368 ATTR Name
                ALIGNMENT SOFT-BCENTER
            END DISPLAY
        END BRANCH
        BEGIN BRANCH addr(1)
            WIRE 1824 432 1840 432
            WIRE 1840 432 1856 432
            BEGIN DISPLAY 1840 432 ATTR Name
                ALIGNMENT SOFT-BCENTER
            END DISPLAY
        END BRANCH
        BEGIN BRANCH addr(2)
            WIRE 1824 496 1840 496
            WIRE 1840 496 1856 496
            BEGIN DISPLAY 1840 496 ATTR Name
                ALIGNMENT SOFT-BCENTER
            END DISPLAY
        END BRANCH
        BEGIN BRANCH addr(3)
            WIRE 1824 560 1840 560
            WIRE 1840 560 1856 560
            BEGIN DISPLAY 1840 560 ATTR Name
                ALIGNMENT SOFT-BCENTER
            END DISPLAY
        END BRANCH
        BUSTAP 1728 368 1824 368
        BUSTAP 1728 432 1824 432
        BUSTAP 1728 496 1824 496
        BUSTAP 1728 560 1824 560
        BUSTAP 2368 368 2272 368
        BEGIN BRANCH data(0)
            WIRE 2240 368 2256 368
            WIRE 2256 368 2272 368
            BEGIN DISPLAY 2256 368 ATTR Name
                ALIGNMENT SOFT-BCENTER
            END DISPLAY
        END BRANCH
        BEGIN BRANCH data(2)
            WIRE 2240 1232 2256 1232
            WIRE 2256 1232 2272 1232
            BEGIN DISPLAY 2256 1232 ATTR Name
                ALIGNMENT SOFT-BCENTER
            END DISPLAY
        END BRANCH
        BUSTAP 2368 1232 2272 1232
        BUSTAP 1728 1424 1824 1424
        BUSTAP 1728 1360 1824 1360
        BUSTAP 1728 1296 1824 1296
        BUSTAP 1728 1232 1824 1232
        BEGIN BRANCH addr(3)
            WIRE 1824 1424 1840 1424
            WIRE 1840 1424 1856 1424
            BEGIN DISPLAY 1840 1424 ATTR Name
                ALIGNMENT SOFT-BCENTER
            END DISPLAY
        END BRANCH
        BEGIN BRANCH addr(2)
            WIRE 1824 1360 1840 1360
            WIRE 1840 1360 1856 1360
            BEGIN DISPLAY 1840 1360 ATTR Name
                ALIGNMENT SOFT-BCENTER
            END DISPLAY
        END BRANCH
        BEGIN BRANCH addr(1)
            WIRE 1824 1296 1840 1296
            WIRE 1840 1296 1856 1296
            BEGIN DISPLAY 1840 1296 ATTR Name
                ALIGNMENT SOFT-BCENTER
            END DISPLAY
        END BRANCH
        BEGIN BRANCH addr(0)
            WIRE 1824 1232 1840 1232
            WIRE 1840 1232 1856 1232
            BEGIN DISPLAY 1840 1232 ATTR Name
                ALIGNMENT SOFT-BCENTER
            END DISPLAY
        END BRANCH
        BEGIN INSTANCE XLXI_19 1856 1552 R0
            BEGIN DISPLAY 0 -456 ATTR INIT
                FONT 28 "Arial"
                DISPLAYFORMAT NAMEEQUALSVALUE
            END DISPLAY
        END INSTANCE
        BEGIN INSTANCE XLXI_20 1856 1984 R0
            BEGIN DISPLAY 0 -460 ATTR INIT
                FONT 28 "Arial"
                DISPLAYFORMAT NAMEEQUALSVALUE
            END DISPLAY
            BEGIN DISPLAY 64 -408 ATTR INIT
                FONT 28 "Arial"
                DISPLAYFORMAT NAMEEQUALSVALUE
            END DISPLAY
        END INSTANCE
        BEGIN BRANCH data(3)
            WIRE 2240 1664 2256 1664
            WIRE 2256 1664 2272 1664
            BEGIN DISPLAY 2256 1664 ATTR Name
                ALIGNMENT SOFT-BCENTER
            END DISPLAY
        END BRANCH
        BUSTAP 2368 1664 2272 1664
        BUSTAP 1728 1856 1824 1856
        BUSTAP 1728 1792 1824 1792
        BUSTAP 1728 1728 1824 1728
        BUSTAP 1728 1664 1824 1664
        BEGIN BRANCH addr(3)
            WIRE 1824 1856 1840 1856
            WIRE 1840 1856 1856 1856
            BEGIN DISPLAY 1840 1856 ATTR Name
                ALIGNMENT SOFT-BCENTER
            END DISPLAY
        END BRANCH
        BEGIN BRANCH addr(2)
            WIRE 1824 1792 1840 1792
            WIRE 1840 1792 1856 1792
            BEGIN DISPLAY 1840 1792 ATTR Name
                ALIGNMENT SOFT-BCENTER
            END DISPLAY
        END BRANCH
        BEGIN BRANCH addr(1)
            WIRE 1824 1728 1840 1728
            WIRE 1840 1728 1856 1728
            BEGIN DISPLAY 1840 1728 ATTR Name
                ALIGNMENT SOFT-BCENTER
            END DISPLAY
        END BRANCH
        BEGIN BRANCH addr(0)
            WIRE 1824 1664 1840 1664
            WIRE 1840 1664 1856 1664
            BEGIN DISPLAY 1840 1664 ATTR Name
                ALIGNMENT SOFT-BCENTER
            END DISPLAY
        END BRANCH
        BEGIN BRANCH data(4)
            WIRE 1536 1648 1552 1648
            WIRE 1552 1648 1568 1648
            BEGIN DISPLAY 1552 1648 ATTR Name
                ALIGNMENT SOFT-BCENTER
            END DISPLAY
        END BRANCH
        BUSTAP 1664 1648 1568 1648
        BUSTAP 1024 1840 1120 1840
        BUSTAP 1024 1776 1120 1776
        BUSTAP 1024 1712 1120 1712
        BUSTAP 1024 1648 1120 1648
        BEGIN BRANCH addr(3)
            WIRE 1120 1840 1136 1840
            WIRE 1136 1840 1152 1840
            BEGIN DISPLAY 1136 1840 ATTR Name
                ALIGNMENT SOFT-BCENTER
            END DISPLAY
        END BRANCH
        BEGIN BRANCH addr(2)
            WIRE 1120 1776 1136 1776
            WIRE 1136 1776 1152 1776
            BEGIN DISPLAY 1136 1776 ATTR Name
                ALIGNMENT SOFT-BCENTER
            END DISPLAY
        END BRANCH
        BEGIN BRANCH addr(1)
            WIRE 1120 1712 1136 1712
            WIRE 1136 1712 1152 1712
            BEGIN DISPLAY 1136 1712 ATTR Name
                ALIGNMENT SOFT-BCENTER
            END DISPLAY
        END BRANCH
        BEGIN BRANCH addr(0)
            WIRE 1120 1648 1136 1648
            WIRE 1136 1648 1152 1648
            BEGIN DISPLAY 1136 1648 ATTR Name
                ALIGNMENT SOFT-BCENTER
            END DISPLAY
        END BRANCH
        BEGIN INSTANCE XLXI_15 1152 1968 R0
            BEGIN DISPLAY 0 -456 ATTR INIT
                FONT 28 "Arial"
                DISPLAYFORMAT NAMEEQUALSVALUE
            END DISPLAY
        END INSTANCE
        BEGIN BRANCH data(5)
            WIRE 1536 1232 1552 1232
            WIRE 1552 1232 1568 1232
            BEGIN DISPLAY 1552 1232 ATTR Name
                ALIGNMENT SOFT-BCENTER
            END DISPLAY
        END BRANCH
        BUSTAP 1664 1232 1568 1232
        BUSTAP 1024 1424 1120 1424
        BUSTAP 1024 1360 1120 1360
        BUSTAP 1024 1296 1120 1296
        BUSTAP 1024 1232 1120 1232
        BEGIN BRANCH addr(3)
            WIRE 1120 1424 1136 1424
            WIRE 1136 1424 1152 1424
            BEGIN DISPLAY 1136 1424 ATTR Name
                ALIGNMENT SOFT-BCENTER
            END DISPLAY
        END BRANCH
        BEGIN BRANCH addr(2)
            WIRE 1120 1360 1136 1360
            WIRE 1136 1360 1152 1360
            BEGIN DISPLAY 1136 1360 ATTR Name
                ALIGNMENT SOFT-BCENTER
            END DISPLAY
        END BRANCH
        BEGIN BRANCH addr(1)
            WIRE 1120 1296 1136 1296
            WIRE 1136 1296 1152 1296
            BEGIN DISPLAY 1136 1296 ATTR Name
                ALIGNMENT SOFT-BCENTER
            END DISPLAY
        END BRANCH
        BEGIN BRANCH addr(0)
            WIRE 1120 1232 1136 1232
            WIRE 1136 1232 1152 1232
            BEGIN DISPLAY 1136 1232 ATTR Name
                ALIGNMENT SOFT-BCENTER
            END DISPLAY
        END BRANCH
        BEGIN INSTANCE XLXI_14 1152 1552 R0
            BEGIN DISPLAY 4 -452 ATTR INIT
                FONT 28 "Arial"
                DISPLAYFORMAT NAMEEQUALSVALUE
            END DISPLAY
        END INSTANCE
        BUSTAP 1024 384 1120 384
        BUSTAP 1024 448 1120 448
        BEGIN BRANCH addr(3)
            WIRE 1120 576 1136 576
            WIRE 1136 576 1152 576
            BEGIN DISPLAY 1136 576 ATTR Name
                ALIGNMENT SOFT-BCENTER
            END DISPLAY
        END BRANCH
        BUSTAP 1664 384 1568 384
        BEGIN BRANCH data(7)
            WIRE 1536 384 1552 384
            WIRE 1552 384 1568 384
            BEGIN DISPLAY 1552 384 ATTR Name
                ALIGNMENT SOFT-BCENTER
            END DISPLAY
        END BRANCH
        BEGIN BRANCH addr(1)
            WIRE 1120 448 1136 448
            WIRE 1136 448 1152 448
            BEGIN DISPLAY 1136 448 ATTR Name
                ALIGNMENT SOFT-BCENTER
            END DISPLAY
        END BRANCH
        BEGIN BRANCH addr(0)
            WIRE 1120 384 1136 384
            WIRE 1136 384 1152 384
            BEGIN DISPLAY 1136 384 ATTR Name
                ALIGNMENT SOFT-BCENTER
            END DISPLAY
        END BRANCH
        BEGIN INSTANCE XLXI_1 1152 704 R0
            BEGIN DISPLAY 4 -452 ATTR INIT
                FONT 28 "Arial"
                DISPLAYFORMAT NAMEEQUALSVALUE
            END DISPLAY
        END INSTANCE
        BUSTAP 1024 576 1120 576
        BEGIN BRANCH addr(3:0)
            WIRE 800 432 896 432
            WIRE 896 432 1024 432
            WIRE 1024 432 1024 448
            WIRE 1024 448 1024 512
            WIRE 1024 512 1024 576
            WIRE 1024 576 1024 816
            WIRE 1024 816 1024 880
            WIRE 1024 880 1024 944
            WIRE 1024 944 1024 1008
            WIRE 1024 1008 1024 1232
            WIRE 1024 1232 1024 1296
            WIRE 1024 1296 1024 1360
            WIRE 1024 1360 1024 1424
            WIRE 1024 1424 1024 1648
            WIRE 1024 1648 1024 1712
            WIRE 1024 1712 1024 1776
            WIRE 1024 1776 1024 1840
            WIRE 1024 1840 1024 1888
            WIRE 1024 208 1728 208
            WIRE 1728 208 1728 368
            WIRE 1728 368 1728 432
            WIRE 1728 432 1728 496
            WIRE 1728 496 1728 560
            WIRE 1728 560 1728 800
            WIRE 1728 800 1728 864
            WIRE 1728 864 1728 928
            WIRE 1728 928 1728 992
            WIRE 1728 992 1728 1232
            WIRE 1728 1232 1728 1296
            WIRE 1728 1296 1728 1360
            WIRE 1728 1360 1728 1424
            WIRE 1728 1424 1728 1664
            WIRE 1728 1664 1728 1728
            WIRE 1728 1728 1728 1792
            WIRE 1728 1792 1728 1856
            WIRE 1728 1856 1728 1904
            WIRE 1024 208 1024 384
            WIRE 1024 384 1024 432
            BEGIN DISPLAY 896 432 ATTR Name
                ALIGNMENT SOFT-BCENTER
            END DISPLAY
        END BRANCH
        BEGIN BRANCH addr(2)
            WIRE 1120 512 1136 512
            WIRE 1136 512 1152 512
            BEGIN DISPLAY 1136 512 ATTR Name
                ALIGNMENT SOFT-BCENTER
            END DISPLAY
        END BRANCH
        BUSTAP 1024 512 1120 512
    END SHEET
END SCHEMATIC
