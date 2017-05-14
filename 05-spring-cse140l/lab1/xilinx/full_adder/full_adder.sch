VERSION 6
BEGIN SCHEMATIC
    BEGIN ATTR DeviceFamilyName "virtex2"
        DELETE all:0
        EDITNAME all:0
        EDITTRAIT all:0
    END ATTR
    BEGIN NETLIST
        SIGNAL A
        SIGNAL B
        SIGNAL Ci
        SIGNAL XLXN_5
        SIGNAL S
        SIGNAL XLXN_10
        SIGNAL XLXN_11
        SIGNAL Co
        PORT Input A
        PORT Input B
        PORT Input Ci
        PORT Output S
        PORT Output Co
        BEGIN BLOCKDEF half_adder
            TIMESTAMP 2005 4 19 0 4 41
            RECTANGLE N 64 -128 320 0 
            LINE N 64 -96 0 -96 
            LINE N 64 -32 0 -32 
            LINE N 320 -96 384 -96 
            LINE N 320 -32 384 -32 
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
        BEGIN BLOCK XLXI_1 half_adder
            PIN A A
            PIN B B
            PIN S XLXN_5
            PIN Co XLXN_11
        END BLOCK
        BEGIN BLOCK XLXI_2 half_adder
            PIN A Ci
            PIN B XLXN_5
            PIN S S
            PIN Co XLXN_10
        END BLOCK
        BEGIN BLOCK XLXI_3 or2
            PIN I0 XLXN_10
            PIN I1 XLXN_11
            PIN O Co
        END BLOCK
    END NETLIST
    BEGIN SHEET 1 3520 2720
        BEGIN INSTANCE XLXI_1 944 992 R0
        END INSTANCE
        BEGIN INSTANCE XLXI_2 944 1296 R0
        END INSTANCE
        BEGIN BRANCH A
            WIRE 832 896 944 896
        END BRANCH
        BEGIN BRANCH B
            WIRE 832 960 944 960
        END BRANCH
        BEGIN BRANCH Ci
            WIRE 832 1200 944 1200
        END BRANCH
        INSTANCE XLXI_3 1424 1136 R0
        BEGIN BRANCH XLXN_5
            WIRE 928 1264 944 1264
            WIRE 928 1264 928 1344
            WIRE 928 1344 944 1344
            WIRE 944 1344 1360 1344
            WIRE 1328 896 1360 896
            WIRE 1360 896 1360 1344
        END BRANCH
        BEGIN BRANCH S
            WIRE 1328 1200 1344 1200
            WIRE 1344 1200 1712 1200
        END BRANCH
        BEGIN BRANCH XLXN_10
            WIRE 1328 1264 1376 1264
            WIRE 1376 1072 1376 1264
            WIRE 1376 1072 1424 1072
        END BRANCH
        BEGIN BRANCH XLXN_11
            WIRE 1328 960 1376 960
            WIRE 1376 960 1376 1008
            WIRE 1376 1008 1424 1008
        END BRANCH
        IOMARKER 832 896 A R180 28
        IOMARKER 832 960 B R180 28
        IOMARKER 832 1200 Ci R180 28
        BEGIN BRANCH Co
            WIRE 1680 1040 1712 1040
        END BRANCH
        IOMARKER 1712 1040 Co R0 28
        IOMARKER 1712 1200 S R0 28
    END SHEET
END SCHEMATIC
