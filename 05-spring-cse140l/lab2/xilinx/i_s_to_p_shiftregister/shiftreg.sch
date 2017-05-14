VERSION 6
BEGIN SCHEMATIC
    BEGIN ATTR DeviceFamilyName "virtex2"
        DELETE all:0
        EDITNAME all:0
        EDITTRAIT all:0
    END ATTR
    BEGIN NETLIST
        SIGNAL Din
        SIGNAL CLK_EN
        SIGNAL CLK
        SIGNAL CLR
        SIGNAL D0
        SIGNAL D1
        SIGNAL XLXN_10
        SIGNAL D2
        SIGNAL D3
        SIGNAL XLXN_13
        SIGNAL XLXN_14
        PORT Input Din
        PORT Input CLK_EN
        PORT Input CLK
        PORT Input CLR
        PORT Output D0
        PORT Output D1
        PORT Output D2
        PORT Output D3
        BEGIN BLOCKDEF fdce
            TIMESTAMP 2001 2 2 12 37 14
            LINE N 0 -128 64 -128 
            LINE N 0 -192 64 -192 
            LINE N 0 -32 64 -32 
            LINE N 0 -256 64 -256 
            LINE N 384 -256 320 -256 
            LINE N 64 -112 80 -128 
            LINE N 80 -128 64 -144 
            LINE N 192 -64 192 -32 
            LINE N 192 -32 64 -32 
            RECTANGLE N 64 -320 320 -64 
        END BLOCKDEF
        BEGIN BLOCK XLXI_1 fdce
            PIN C CLK
            PIN CE CLK_EN
            PIN CLR CLR
            PIN D Din
            PIN Q D0
        END BLOCK
        BEGIN BLOCK XLXI_3 fdce
            PIN C CLK
            PIN CE CLK_EN
            PIN CLR CLR
            PIN D D0
            PIN Q D1
        END BLOCK
        BEGIN BLOCK XLXI_4 fdce
            PIN C CLK
            PIN CE CLK_EN
            PIN CLR CLR
            PIN D D1
            PIN Q D2
        END BLOCK
        BEGIN BLOCK XLXI_5 fdce
            PIN C CLK
            PIN CE CLK_EN
            PIN CLR CLR
            PIN D D2
            PIN Q D3
        END BLOCK
    END NETLIST
    BEGIN SHEET 1 3520 2720
        INSTANCE XLXI_1 1136 912 R0
        INSTANCE XLXI_3 1136 1344 R0
        INSTANCE XLXI_5 1136 2176 R0
        INSTANCE XLXI_4 1136 1760 R0
        BEGIN BRANCH Din
            WIRE 848 656 1136 656
        END BRANCH
        IOMARKER 848 656 Din R180 28
        BEGIN BRANCH CLK_EN
            WIRE 848 720 864 720
            WIRE 864 720 896 720
            WIRE 896 720 1136 720
            WIRE 896 720 896 1152
            WIRE 896 1152 896 1568
            WIRE 896 1568 896 1984
            WIRE 896 1984 1136 1984
            WIRE 896 1568 1136 1568
            WIRE 896 1152 1136 1152
        END BRANCH
        IOMARKER 848 720 CLK_EN R180 28
        BEGIN BRANCH CLK
            WIRE 848 784 960 784
            WIRE 960 784 1136 784
            WIRE 960 784 960 1216
            WIRE 960 1216 960 1632
            WIRE 960 1632 960 2048
            WIRE 960 2048 1136 2048
            WIRE 960 1632 1136 1632
            WIRE 960 1216 1136 1216
        END BRANCH
        IOMARKER 848 784 CLK R180 28
        BEGIN BRANCH CLR
            WIRE 800 2144 1024 2144
            WIRE 1024 2144 1136 2144
            WIRE 1024 880 1136 880
            WIRE 1024 880 1024 1312
            WIRE 1024 1312 1136 1312
            WIRE 1024 1312 1024 1728
            WIRE 1024 1728 1024 2144
            WIRE 1024 1728 1136 1728
        END BRANCH
        BEGIN BRANCH D0
            WIRE 1072 960 1072 1088
            WIRE 1072 1088 1136 1088
            WIRE 1072 960 1600 960
            WIRE 1520 656 1600 656
            WIRE 1600 656 1600 960
            WIRE 1600 656 1728 656
        END BRANCH
        BEGIN BRANCH D1
            WIRE 1056 1376 1056 1504
            WIRE 1056 1504 1136 1504
            WIRE 1056 1376 1600 1376
            WIRE 1520 1088 1600 1088
            WIRE 1600 1088 1600 1376
            WIRE 1600 1088 1728 1088
        END BRANCH
        BEGIN BRANCH D2
            WIRE 1072 1776 1136 1776
            WIRE 1136 1776 1520 1776
            WIRE 1520 1776 1536 1776
            WIRE 1072 1776 1072 1920
            WIRE 1072 1920 1136 1920
            WIRE 1520 1504 1536 1504
            WIRE 1536 1504 1536 1776
            WIRE 1536 1504 1728 1504
        END BRANCH
        BEGIN BRANCH D3
            WIRE 1520 1920 1536 1920
            WIRE 1536 1920 1728 1920
        END BRANCH
        IOMARKER 1728 656 D0 R0 28
        IOMARKER 1728 1088 D1 R0 28
        IOMARKER 800 2144 CLR R180 28
        IOMARKER 1728 1920 D3 R0 28
        IOMARKER 1728 1504 D2 R0 28
    END SHEET
END SCHEMATIC
