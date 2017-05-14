VERSION 6
BEGIN SCHEMATIC
    BEGIN ATTR DeviceFamilyName "virtex2"
        DELETE all:0
        EDITNAME all:0
        EDITTRAIT all:0
    END ATTR
    BEGIN NETLIST
        SIGNAL XLXN_1
        SIGNAL XLXN_2
        SIGNAL B
        SIGNAL A
        SIGNAL S
        SIGNAL Co
        PORT Input B
        PORT Input A
        PORT Output S
        PORT Output Co
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
        BEGIN BLOCKDEF and2
            TIMESTAMP 2001 2 2 12 38 38
            LINE N 0 -64 64 -64 
            LINE N 0 -128 64 -128 
            LINE N 256 -96 192 -96 
            ARC N 96 -144 192 -48 144 -48 144 -144 
            LINE N 144 -48 64 -48 
            LINE N 64 -144 144 -144 
            LINE N 64 -48 64 -144 
        END BLOCKDEF
        BEGIN BLOCK XLXI_1 xor2
            PIN I0 B
            PIN I1 A
            PIN O S
        END BLOCK
        BEGIN BLOCK XLXI_2 and2
            PIN I0 B
            PIN I1 A
            PIN O Co
        END BLOCK
    END NETLIST
    BEGIN SHEET 1 3520 2720
        INSTANCE XLXI_1 1408 1056 R0
        INSTANCE XLXI_2 1408 1328 R0
        BEGIN BRANCH B
            WIRE 1168 992 1344 992
            WIRE 1344 992 1408 992
            WIRE 1344 992 1344 1264
            WIRE 1344 1264 1408 1264
        END BRANCH
        BEGIN BRANCH A
            WIRE 1168 928 1280 928
            WIRE 1280 928 1408 928
            WIRE 1280 928 1280 1200
            WIRE 1280 1200 1408 1200
        END BRANCH
        BEGIN BRANCH S
            WIRE 1664 960 1792 960
        END BRANCH
        BEGIN BRANCH Co
            WIRE 1664 1232 1792 1232
        END BRANCH
        IOMARKER 1168 928 A R180 28
        IOMARKER 1168 992 B R180 28
        IOMARKER 1792 960 S R0 28
        IOMARKER 1792 1232 Co R0 28
    END SHEET
END SCHEMATIC
