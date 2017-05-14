VERSION 6
BEGIN SCHEMATIC
    BEGIN ATTR DeviceFamilyName "virtex2"
        DELETE all:0
        EDITNAME all:0
        EDITTRAIT all:0
    END ATTR
    BEGIN NETLIST
        SIGNAL PRE
        SIGNAL CLK
        SIGNAL XLXN_3
        SIGNAL D(0)
        SIGNAL D(1)
        SIGNAL D(2)
        SIGNAL XLXN_8
        SIGNAL D(3)
        SIGNAL D(3:0)
        SIGNAL CLK_CE
        PORT Input PRE
        PORT Input CLK
        PORT Output D(3:0)
        PORT Input CLK_CE
        BEGIN BLOCKDEF fdpe
            TIMESTAMP 2001 2 2 12 37 14
            LINE N 64 -112 80 -128 
            LINE N 80 -128 64 -144 
            LINE N 192 -320 192 -352 
            LINE N 192 -352 64 -352 
            RECTANGLE N 64 -320 320 -64 
            LINE N 384 -256 320 -256 
            LINE N 0 -352 64 -352 
            LINE N 0 -256 64 -256 
            LINE N 0 -192 64 -192 
            LINE N 0 -128 64 -128 
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
        BEGIN BLOCK XLXI_2 fdpe
            PIN C CLK
            PIN CE CLK_CE
            PIN D XLXN_8
            PIN PRE PRE
            PIN Q D(0)
        END BLOCK
        BEGIN BLOCK XLXI_3 fdpe
            PIN C CLK
            PIN CE CLK_CE
            PIN D D(0)
            PIN PRE PRE
            PIN Q D(1)
        END BLOCK
        BEGIN BLOCK XLXI_4 fdpe
            PIN C CLK
            PIN CE CLK_CE
            PIN D D(1)
            PIN PRE PRE
            PIN Q D(2)
        END BLOCK
        BEGIN BLOCK XLXI_5 fdpe
            PIN C CLK
            PIN CE CLK_CE
            PIN D D(2)
            PIN PRE PRE
            PIN Q D(3)
        END BLOCK
        BEGIN BLOCK XLXI_9 xor2
            PIN I0 D(3)
            PIN I1 D(2)
            PIN O XLXN_8
        END BLOCK
    END NETLIST
    BEGIN SHEET 1 3520 2720
        INSTANCE XLXI_3 992 1264 R0
        INSTANCE XLXI_4 992 1760 R0
        INSTANCE XLXI_5 992 2224 R0
        INSTANCE XLXI_2 992 752 R0
        BEGIN BRANCH PRE
            WIRE 784 400 912 400
            WIRE 912 400 992 400
            WIRE 912 400 912 912
            WIRE 912 912 992 912
            WIRE 912 912 912 1408
            WIRE 912 1408 992 1408
            WIRE 912 1408 912 1872
            WIRE 912 1872 992 1872
        END BRANCH
        IOMARKER 784 400 PRE R180 28
        BEGIN BRANCH CLK
            WIRE 784 448 864 448
            WIRE 864 448 864 624
            WIRE 864 624 992 624
            WIRE 864 624 864 1136
            WIRE 864 1136 992 1136
            WIRE 864 1136 864 1632
            WIRE 864 1632 864 2096
            WIRE 864 2096 992 2096
            WIRE 864 1632 992 1632
        END BRANCH
        IOMARKER 784 448 CLK R180 28
        BEGIN BRANCH D(0)
            WIRE 992 800 1360 800
            WIRE 992 800 992 1008
            WIRE 1360 496 1360 800
            WIRE 1360 496 1376 496
            WIRE 1376 496 2016 496
            WIRE 2016 496 2048 496
            BEGIN DISPLAY 2016 496 ATTR Name
                ALIGNMENT SOFT-BCENTER
            END DISPLAY
        END BRANCH
        BEGIN BRANCH D(1)
            WIRE 992 1296 992 1504
            WIRE 992 1296 1376 1296
            WIRE 1376 1008 1376 1296
            WIRE 1376 1008 2016 1008
            WIRE 2016 1008 2048 1008
            BEGIN DISPLAY 2016 1008 ATTR Name
                ALIGNMENT SOFT-BCENTER
            END DISPLAY
        END BRANCH
        BEGIN BRANCH D(2)
            WIRE 992 1792 1376 1792
            WIRE 992 1792 992 1968
            WIRE 1376 1504 1376 1792
            WIRE 1376 1504 1840 1504
            WIRE 1840 1504 2016 1504
            WIRE 2016 1504 2048 1504
            WIRE 1840 848 1840 1504
            BEGIN DISPLAY 2016 1504 ATTR Name
                ALIGNMENT SOFT-BCENTER
            END DISPLAY
        END BRANCH
        BEGIN BRANCH XLXN_8
            WIRE 992 304 992 496
            WIRE 992 304 1584 304
            WIRE 1584 304 1584 816
        END BRANCH
        INSTANCE XLXI_9 1840 720 R180
        BEGIN BRANCH D(3)
            WIRE 1376 1968 1920 1968
            WIRE 1920 1968 2016 1968
            WIRE 2016 1968 2048 1968
            WIRE 1840 784 1920 784
            WIRE 1920 784 1920 1968
            BEGIN DISPLAY 2016 1968 ATTR Name
                ALIGNMENT SOFT-BCENTER
            END DISPLAY
        END BRANCH
        BEGIN BRANCH D(3:0)
            WIRE 2144 448 2144 496
            WIRE 2144 496 2144 1008
            WIRE 2144 1008 2144 1504
            WIRE 2144 1504 2144 1968
            WIRE 2144 1968 2144 2064
        END BRANCH
        BUSTAP 2144 496 2048 496
        BUSTAP 2144 1008 2048 1008
        BUSTAP 2144 1504 2048 1504
        BUSTAP 2144 1968 2048 1968
        IOMARKER 2144 448 D(3:0) R270 28
        BEGIN BRANCH CLK_CE
            WIRE 784 512 800 512
            WIRE 800 512 800 560
            WIRE 800 560 992 560
            WIRE 800 560 800 1072
            WIRE 800 1072 992 1072
            WIRE 800 1072 800 1568
            WIRE 800 1568 800 2032
            WIRE 800 2032 992 2032
            WIRE 800 1568 992 1568
        END BRANCH
        IOMARKER 784 512 CLK_CE R180 28
    END SHEET
END SCHEMATIC
