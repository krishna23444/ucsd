VERSION 6
BEGIN SCHEMATIC
    BEGIN ATTR DeviceFamilyName "virtex2"
        DELETE all:0
        EDITNAME all:0
        EDITTRAIT all:0
    END ATTR
    BEGIN NETLIST
        SIGNAL B(0)
        SIGNAL A(0)
        SIGNAL XLXN_9
        SIGNAL B(4:0)
        SIGNAL B(2)
        SIGNAL A(2)
        SIGNAL B(3)
        SIGNAL A(3)
        SIGNAL B(4)
        SIGNAL A(4)
        SIGNAL C(0)
        SIGNAL C(1)
        SIGNAL C(2)
        SIGNAL C(3)
        SIGNAL C(4)
        SIGNAL A(4:0)
        SIGNAL C(4:0)
        SIGNAL B(1)
        SIGNAL A(1)
        PORT Input B(4:0)
        PORT Input A(4:0)
        PORT Output C(4:0)
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
        BEGIN BLOCK XLXI_3 and2
            PIN I0 B(0)
            PIN I1 A(0)
            PIN O C(0)
        END BLOCK
        BEGIN BLOCK XLXI_4 and2
            PIN I0 B(1)
            PIN I1 A(1)
            PIN O C(1)
        END BLOCK
        BEGIN BLOCK XLXI_5 and2
            PIN I0 B(2)
            PIN I1 A(2)
            PIN O C(2)
        END BLOCK
        BEGIN BLOCK XLXI_6 and2
            PIN I0 B(3)
            PIN I1 A(3)
            PIN O C(3)
        END BLOCK
        BEGIN BLOCK XLXI_7 and2
            PIN I0 B(4)
            PIN I1 A(4)
            PIN O C(4)
        END BLOCK
    END NETLIST
    BEGIN SHEET 1 3520 2720
        INSTANCE XLXI_3 1264 992 R0
        INSTANCE XLXI_4 1264 1136 R0
        INSTANCE XLXI_5 1264 1280 R0
        INSTANCE XLXI_6 1264 1424 R0
        INSTANCE XLXI_7 1264 1568 R0
        BUSTAP 1136 928 1232 928
        BEGIN BRANCH B(0)
            WIRE 1232 928 1248 928
            WIRE 1248 928 1264 928
            BEGIN DISPLAY 1248 928 ATTR Name
                ALIGNMENT SOFT-BCENTER
            END DISPLAY
        END BRANCH
        BUSTAP 1088 864 1184 864
        BEGIN BRANCH A(0)
            WIRE 1184 864 1216 864
            WIRE 1216 864 1264 864
            BEGIN DISPLAY 1216 864 ATTR Name
                ALIGNMENT SOFT-BCENTER
            END DISPLAY
        END BRANCH
        BEGIN BRANCH B(4:0)
            WIRE 976 816 1040 816
            WIRE 1040 816 1136 816
            WIRE 1136 816 1136 1072
            WIRE 1136 1072 1136 1216
            WIRE 1136 1216 1136 1360
            WIRE 1136 1360 1136 1504
            WIRE 1136 1504 1136 1568
            BEGIN DISPLAY 1040 816 ATTR Name
                ALIGNMENT SOFT-BCENTER
            END DISPLAY
        END BRANCH
        BUSTAP 1136 1216 1232 1216
        BEGIN BRANCH B(2)
            WIRE 1232 1216 1248 1216
            WIRE 1248 1216 1264 1216
            BEGIN DISPLAY 1248 1216 ATTR Name
                ALIGNMENT SOFT-BCENTER
            END DISPLAY
        END BRANCH
        BEGIN BRANCH A(2)
            WIRE 1184 1152 1216 1152
            WIRE 1216 1152 1264 1152
            BEGIN DISPLAY 1216 1152 ATTR Name
                ALIGNMENT SOFT-BCENTER
            END DISPLAY
        END BRANCH
        BUSTAP 1088 1152 1184 1152
        BUSTAP 1136 1360 1232 1360
        BEGIN BRANCH B(3)
            WIRE 1232 1360 1248 1360
            WIRE 1248 1360 1264 1360
            BEGIN DISPLAY 1248 1360 ATTR Name
                ALIGNMENT SOFT-BCENTER
            END DISPLAY
        END BRANCH
        BEGIN BRANCH A(3)
            WIRE 1184 1296 1216 1296
            WIRE 1216 1296 1264 1296
            BEGIN DISPLAY 1216 1296 ATTR Name
                ALIGNMENT SOFT-BCENTER
            END DISPLAY
        END BRANCH
        BUSTAP 1088 1296 1184 1296
        BUSTAP 1136 1504 1232 1504
        BEGIN BRANCH B(4)
            WIRE 1232 1504 1248 1504
            WIRE 1248 1504 1264 1504
            BEGIN DISPLAY 1248 1504 ATTR Name
                ALIGNMENT SOFT-BCENTER
            END DISPLAY
        END BRANCH
        BEGIN BRANCH A(4)
            WIRE 1184 1440 1216 1440
            WIRE 1216 1440 1264 1440
            BEGIN DISPLAY 1216 1440 ATTR Name
                ALIGNMENT SOFT-BCENTER
            END DISPLAY
        END BRANCH
        BUSTAP 1088 1440 1184 1440
        BUSTAP 1648 896 1552 896
        BEGIN BRANCH C(0)
            WIRE 1520 896 1536 896
            WIRE 1536 896 1552 896
            BEGIN DISPLAY 1536 896 ATTR Name
                ALIGNMENT SOFT-BCENTER
            END DISPLAY
        END BRANCH
        BUSTAP 1648 1040 1552 1040
        BEGIN BRANCH C(1)
            WIRE 1520 1040 1536 1040
            WIRE 1536 1040 1552 1040
            BEGIN DISPLAY 1536 1040 ATTR Name
                ALIGNMENT SOFT-BCENTER
            END DISPLAY
        END BRANCH
        BUSTAP 1648 1184 1552 1184
        BEGIN BRANCH C(2)
            WIRE 1520 1184 1536 1184
            WIRE 1536 1184 1552 1184
            BEGIN DISPLAY 1536 1184 ATTR Name
                ALIGNMENT SOFT-BCENTER
            END DISPLAY
        END BRANCH
        BUSTAP 1648 1328 1552 1328
        BEGIN BRANCH C(3)
            WIRE 1520 1328 1536 1328
            WIRE 1536 1328 1552 1328
            BEGIN DISPLAY 1536 1328 ATTR Name
                ALIGNMENT SOFT-BCENTER
            END DISPLAY
        END BRANCH
        BUSTAP 1648 1472 1552 1472
        BEGIN BRANCH C(4)
            WIRE 1520 1472 1536 1472
            WIRE 1536 1472 1552 1472
            BEGIN DISPLAY 1536 1472 ATTR Name
                ALIGNMENT SOFT-BCENTER
            END DISPLAY
        END BRANCH
        BEGIN BRANCH A(4:0)
            WIRE 976 752 1040 752
            WIRE 1040 752 1088 752
            WIRE 1088 752 1088 1008
            WIRE 1088 1008 1088 1152
            WIRE 1088 1152 1088 1296
            WIRE 1088 1296 1088 1440
            WIRE 1088 1440 1088 1568
            BEGIN DISPLAY 1040 752 ATTR Name
                ALIGNMENT SOFT-BCENTER
            END DISPLAY
        END BRANCH
        IOMARKER 976 752 A(4:0) R180 28
        IOMARKER 976 816 B(4:0) R180 28
        BEGIN BRANCH C(4:0)
            WIRE 1648 768 1648 784
            WIRE 1648 768 1648 1568
            WIRE 1648 768 1696 768
            WIRE 1696 768 1760 768
            BEGIN DISPLAY 1696 768 ATTR Name
                ALIGNMENT SOFT-BCENTER
            END DISPLAY
        END BRANCH
        IOMARKER 1760 768 C(4:0) R0 28
        BUSTAP 1136 1072 1232 1072
        BEGIN BRANCH B(1)
            WIRE 1232 1072 1248 1072
            WIRE 1248 1072 1264 1072
            BEGIN DISPLAY 1248 1072 ATTR Name
                ALIGNMENT SOFT-BCENTER
            END DISPLAY
        END BRANCH
        BUSTAP 1088 1008 1184 1008
        BEGIN BRANCH A(1)
            WIRE 1184 1008 1216 1008
            WIRE 1216 1008 1264 1008
            BEGIN DISPLAY 1216 1008 ATTR Name
                ALIGNMENT SOFT-BCENTER
            END DISPLAY
        END BRANCH
    END SHEET
END SCHEMATIC
