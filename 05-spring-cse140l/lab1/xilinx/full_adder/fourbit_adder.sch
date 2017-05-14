VERSION 6
BEGIN SCHEMATIC
    BEGIN ATTR DeviceFamilyName "virtex2"
        DELETE all:0
        EDITNAME all:0
        EDITTRAIT all:0
    END ATTR
    BEGIN NETLIST
        SIGNAL XLXN_3
        SIGNAL Ci
        BEGIN SIGNAL B(0)
        END SIGNAL
        BEGIN SIGNAL A(0)
        END SIGNAL
        SIGNAL XLXN_7
        SIGNAL XLXN_10
        SIGNAL Co
        BEGIN SIGNAL A(2)
        END SIGNAL
        BEGIN SIGNAL B(2)
        END SIGNAL
        BEGIN SIGNAL A(3)
        END SIGNAL
        BEGIN SIGNAL B(3)
        END SIGNAL
        SIGNAL A(3:0)
        BEGIN SIGNAL A(1)
        END SIGNAL
        SIGNAL B(3:0)
        SIGNAL B(1)
        SIGNAL S(3:0)
        SIGNAL S(0)
        SIGNAL S(1)
        SIGNAL S(3)
        SIGNAL S(2)
        PORT Input Ci
        PORT Output Co
        PORT Input A(3:0)
        PORT Input B(3:0)
        PORT Output S(3:0)
        BEGIN BLOCKDEF full_adder
            TIMESTAMP 2005 4 19 1 10 25
            RECTANGLE N 64 -192 320 0 
            LINE N 64 -160 0 -160 
            LINE N 64 -96 0 -96 
            LINE N 64 -32 0 -32 
            LINE N 320 -160 384 -160 
            LINE N 320 -96 384 -96 
        END BLOCKDEF
        BEGIN BLOCK XLXI_1 full_adder
            PIN A A(0)
            PIN B B(0)
            PIN Ci Ci
            PIN Co XLXN_3
            PIN S S(0)
        END BLOCK
        BEGIN BLOCK XLXI_2 full_adder
            PIN A A(1)
            PIN B B(1)
            PIN Ci XLXN_3
            PIN Co XLXN_7
            PIN S S(1)
        END BLOCK
        BEGIN BLOCK XLXI_3 full_adder
            PIN A A(2)
            PIN B B(2)
            PIN Ci XLXN_7
            PIN Co XLXN_10
            PIN S S(2)
        END BLOCK
        BEGIN BLOCK XLXI_4 full_adder
            PIN A A(3)
            PIN B B(3)
            PIN Ci XLXN_10
            PIN Co Co
            PIN S S(3)
        END BLOCK
    END NETLIST
    BEGIN SHEET 1 3520 2720
        BEGIN INSTANCE XLXI_1 2016 800 R90
        END INSTANCE
        BEGIN INSTANCE XLXI_2 1648 800 R90
        END INSTANCE
        BEGIN INSTANCE XLXI_3 1280 800 R90
        END INSTANCE
        BEGIN INSTANCE XLXI_4 928 800 R90
        END INSTANCE
        BEGIN BRANCH XLXN_3
            WIRE 1680 720 2352 720
            WIRE 2352 720 2352 1200
            WIRE 1680 720 1680 800
            WIRE 2176 1184 2176 1200
            WIRE 2176 1200 2352 1200
        END BRANCH
        BEGIN BRANCH Ci
            WIRE 2048 608 2048 800
        END BRANCH
        BEGIN BRANCH B(0)
            WIRE 2112 576 2112 624
            WIRE 2112 624 2112 800
            BEGIN DISPLAY 2112 624 ATTR Name
                ALIGNMENT SOFT-TVCENTER
            END DISPLAY
        END BRANCH
        BEGIN BRANCH A(0)
            WIRE 2176 512 2176 592
            WIRE 2176 592 2176 800
            BEGIN DISPLAY 2176 592 ATTR Name
                ALIGNMENT SOFT-TVCENTER
            END DISPLAY
        END BRANCH
        BEGIN BRANCH XLXN_7
            WIRE 1312 640 1936 640
            WIRE 1936 640 1936 1200
            WIRE 1312 640 1312 800
            WIRE 1808 1184 1808 1200
            WIRE 1808 1200 1936 1200
        END BRANCH
        BEGIN BRANCH XLXN_10
            WIRE 960 720 1568 720
            WIRE 1568 720 1568 1200
            WIRE 960 720 960 800
            WIRE 1440 1184 1440 1200
            WIRE 1440 1200 1568 1200
        END BRANCH
        BEGIN BRANCH Co
            WIRE 784 656 832 656
            WIRE 832 656 1216 656
            WIRE 1216 656 1216 1200
            WIRE 784 656 784 1424
            WIRE 1088 1184 1088 1200
            WIRE 1088 1200 1216 1200
            BEGIN DISPLAY 832 656 ATTR Name
                ALIGNMENT SOFT-BCENTER
            END DISPLAY
        END BRANCH
        IOMARKER 2048 608 Ci R270 28
        BEGIN BRANCH A(2)
            WIRE 1440 512 1440 592
            WIRE 1440 592 1440 800
            BEGIN DISPLAY 1440 592 ATTR Name
                ALIGNMENT SOFT-TVCENTER
            END DISPLAY
        END BRANCH
        BEGIN BRANCH B(2)
            WIRE 1376 576 1376 624
            WIRE 1376 624 1376 800
            BEGIN DISPLAY 1376 624 ATTR Name
                ALIGNMENT SOFT-TVCENTER
            END DISPLAY
        END BRANCH
        BEGIN BRANCH A(3)
            WIRE 1088 512 1088 592
            WIRE 1088 592 1088 800
            BEGIN DISPLAY 1088 592 ATTR Name
                ALIGNMENT SOFT-TVCENTER
            END DISPLAY
        END BRANCH
        BEGIN BRANCH B(3)
            WIRE 1024 576 1024 624
            WIRE 1024 624 1024 800
            BEGIN DISPLAY 1024 624 ATTR Name
                ALIGNMENT SOFT-TVCENTER
            END DISPLAY
        END BRANCH
        BEGIN BRANCH A(3:0)
            WIRE 768 416 816 416
            WIRE 816 416 1088 416
            WIRE 1088 416 1440 416
            WIRE 1440 416 1808 416
            WIRE 1808 416 2176 416
            WIRE 2176 416 2336 416
            BEGIN DISPLAY 816 416 ATTR Name
                ALIGNMENT SOFT-BCENTER
            END DISPLAY
        END BRANCH
        BUSTAP 1088 416 1088 512
        BUSTAP 1440 416 1440 512
        BUSTAP 1808 416 1808 512
        BUSTAP 2176 416 2176 512
        BEGIN BRANCH A(1)
            WIRE 1808 512 1808 592
            WIRE 1808 592 1808 800
            BEGIN DISPLAY 1808 592 ATTR Name
                ALIGNMENT SOFT-TVCENTER
            END DISPLAY
        END BRANCH
        BEGIN BRANCH B(3:0)
            WIRE 768 480 816 480
            WIRE 816 480 1024 480
            WIRE 1024 480 1376 480
            WIRE 1376 480 1744 480
            WIRE 1744 480 2112 480
            WIRE 2112 480 2336 480
            BEGIN DISPLAY 816 480 ATTR Name
                ALIGNMENT SOFT-BCENTER
            END DISPLAY
        END BRANCH
        BUSTAP 1024 480 1024 576
        BUSTAP 1376 480 1376 576
        BUSTAP 2112 480 2112 576
        BUSTAP 1744 480 1744 576
        BEGIN BRANCH B(1)
            WIRE 1744 576 1744 624
            WIRE 1744 624 1744 800
            BEGIN DISPLAY 1744 624 ATTR Name
                ALIGNMENT SOFT-TVCENTER
            END DISPLAY
        END BRANCH
        BEGIN BRANCH S(3:0)
            WIRE 912 1472 1024 1472
            WIRE 1024 1472 1120 1472
            WIRE 1120 1472 1376 1472
            WIRE 1376 1472 1744 1472
            WIRE 1744 1472 2112 1472
            WIRE 2112 1472 2336 1472
            BEGIN DISPLAY 1120 1472 ATTR Name
                ALIGNMENT SOFT-BCENTER
            END DISPLAY
        END BRANCH
        IOMARKER 768 416 A(3:0) R180 28
        IOMARKER 768 480 B(3:0) R180 28
        BUSTAP 2112 1472 2112 1376
        BEGIN BRANCH S(0)
            WIRE 2112 1184 2112 1264
            WIRE 2112 1264 2112 1376
            BEGIN DISPLAY 2112 1264 ATTR Name
                ALIGNMENT SOFT-TVCENTER
            END DISPLAY
        END BRANCH
        BUSTAP 1744 1472 1744 1376
        BEGIN BRANCH S(1)
            WIRE 1744 1184 1744 1264
            WIRE 1744 1264 1744 1376
            BEGIN DISPLAY 1744 1264 ATTR Name
                ALIGNMENT SOFT-TVCENTER
            END DISPLAY
        END BRANCH
        BUSTAP 1024 1472 1024 1376
        BEGIN BRANCH S(3)
            WIRE 1024 1184 1024 1264
            WIRE 1024 1264 1024 1376
            BEGIN DISPLAY 1024 1264 ATTR Name
                ALIGNMENT SOFT-TVCENTER
            END DISPLAY
        END BRANCH
        BUSTAP 1376 1472 1376 1376
        BEGIN BRANCH S(2)
            WIRE 1376 1184 1376 1264
            WIRE 1376 1264 1376 1376
            BEGIN DISPLAY 1376 1264 ATTR Name
                ALIGNMENT SOFT-TVCENTER
            END DISPLAY
        END BRANCH
        IOMARKER 784 1424 Co R90 28
        IOMARKER 2336 1472 S(3:0) R0 28
    END SHEET
END SCHEMATIC
