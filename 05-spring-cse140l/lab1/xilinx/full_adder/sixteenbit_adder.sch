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
        SIGNAL XLXN_3
        BEGIN SIGNAL A(0)
        END SIGNAL
        BEGIN SIGNAL A(15:12)
        END SIGNAL
        SIGNAL A(15:0)
        SIGNAL A(3:0)
        SIGNAL B(3:0)
        SIGNAL A(7:4)
        SIGNAL B(7:4)
        SIGNAL A(11:8)
        SIGNAL B(11:8)
        SIGNAL B(15:12)
        SIGNAL S(15:0)
        SIGNAL S(3:0)
        SIGNAL S(7:4)
        SIGNAL S(11:8)
        SIGNAL S(15:12)
        SIGNAL B(15:0)
        SIGNAL Co
        SIGNAL Ci
        PORT Input A(15:0)
        PORT Output S(15:0)
        PORT Input B(15:0)
        PORT Output Co
        PORT Input Ci
        BEGIN BLOCKDEF fourbit_adder
            TIMESTAMP 2005 4 19 3 11 18
            RECTANGLE N 64 -192 320 0 
            LINE N 64 -160 0 -160 
            RECTANGLE N 0 -172 64 -148 
            LINE N 64 -96 0 -96 
            RECTANGLE N 0 -108 64 -84 
            LINE N 64 -32 0 -32 
            LINE N 320 -160 384 -160 
            LINE N 320 -96 384 -96 
            RECTANGLE N 320 -108 384 -84 
        END BLOCKDEF
        BEGIN BLOCK XLXI_2 fourbit_adder
            PIN A(3:0) A(3:0)
            PIN B(3:0) B(3:0)
            PIN Ci Ci
            PIN Co XLXN_1
            PIN S(3:0) S(3:0)
        END BLOCK
        BEGIN BLOCK XLXI_3 fourbit_adder
            PIN A(3:0) A(7:4)
            PIN B(3:0) B(7:4)
            PIN Ci XLXN_1
            PIN Co XLXN_2
            PIN S(3:0) S(7:4)
        END BLOCK
        BEGIN BLOCK XLXI_4 fourbit_adder
            PIN A(3:0) A(11:8)
            PIN B(3:0) B(11:8)
            PIN Ci XLXN_2
            PIN Co XLXN_3
            PIN S(3:0) S(11:8)
        END BLOCK
        BEGIN BLOCK XLXI_5 fourbit_adder
            PIN A(3:0) A(15:12)
            PIN B(3:0) B(15:12)
            PIN Ci XLXN_3
            PIN Co Co
            PIN S(3:0) S(15:12)
        END BLOCK
    END NETLIST
    BEGIN SHEET 1 3520 2720
        BEGIN INSTANCE XLXI_2 2464 768 R90
        END INSTANCE
        BEGIN INSTANCE XLXI_3 1952 784 R90
        END INSTANCE
        BEGIN INSTANCE XLXI_4 1440 784 R90
        END INSTANCE
        BEGIN INSTANCE XLXI_5 864 784 R90
        END INSTANCE
        BEGIN BRANCH XLXN_1
            WIRE 1984 624 2800 624
            WIRE 2800 624 2800 1168
            WIRE 1984 624 1984 784
            WIRE 2624 1152 2624 1168
            WIRE 2624 1168 2800 1168
        END BRANCH
        BEGIN BRANCH XLXN_2
            WIRE 1472 688 2272 688
            WIRE 2272 688 2272 1184
            WIRE 1472 688 1472 784
            WIRE 2112 1168 2112 1184
            WIRE 2112 1184 2272 1184
        END BRANCH
        BEGIN BRANCH XLXN_3
            WIRE 896 624 1792 624
            WIRE 1792 624 1792 1184
            WIRE 896 624 896 784
            WIRE 1600 1168 1600 1184
            WIRE 1600 1184 1792 1184
        END BRANCH
        BEGIN BRANCH A(15:12)
            WIRE 1024 416 1024 544
            WIRE 1024 544 1024 784
            BEGIN DISPLAY 1024 544 ATTR Name
                ALIGNMENT SOFT-TVCENTER
            END DISPLAY
        END BRANCH
        BEGIN BRANCH A(15:0)
            WIRE 880 320 960 320
            WIRE 960 320 1024 320
            WIRE 1024 320 1104 320
            WIRE 1104 320 1536 320
            WIRE 1536 320 1600 320
            WIRE 1600 320 2048 320
            WIRE 2048 320 2112 320
            WIRE 2112 320 2544 320
            WIRE 2544 320 2560 320
            WIRE 2560 320 2624 320
            WIRE 2624 320 2736 320
            BEGIN DISPLAY 1104 320 ATTR Name
                ALIGNMENT SOFT-BCENTER
            END DISPLAY
        END BRANCH
        BUSTAP 1024 320 1024 416
        BUSTAP 2624 320 2624 416
        BEGIN BRANCH A(3:0)
            WIRE 2624 416 2624 544
            WIRE 2624 544 2624 768
            BEGIN DISPLAY 2624 544 ATTR Name
                ALIGNMENT SOFT-TVCENTER
            END DISPLAY
        END BRANCH
        BEGIN BRANCH B(3:0)
            WIRE 2560 336 2560 416
            WIRE 2560 416 2560 464
            WIRE 2560 464 2560 544
            WIRE 2560 544 2560 768
            BEGIN DISPLAY 2560 544 ATTR Name
                ALIGNMENT SOFT-TVCENTER
            END DISPLAY
        END BRANCH
        BUSTAP 2112 320 2112 416
        BEGIN BRANCH A(7:4)
            WIRE 2112 416 2112 544
            WIRE 2112 544 2112 784
            BEGIN DISPLAY 2112 544 ATTR Name
                ALIGNMENT SOFT-TVCENTER
            END DISPLAY
        END BRANCH
        BEGIN BRANCH B(7:4)
            WIRE 2048 336 2048 416
            WIRE 2048 416 2048 544
            WIRE 2048 544 2048 784
            BEGIN DISPLAY 2048 544 ATTR Name
                ALIGNMENT SOFT-TVCENTER
            END DISPLAY
        END BRANCH
        BUSTAP 1600 320 1600 416
        BEGIN BRANCH A(11:8)
            WIRE 1600 416 1600 544
            WIRE 1600 544 1600 784
            BEGIN DISPLAY 1600 544 ATTR Name
                ALIGNMENT SOFT-TVCENTER
            END DISPLAY
        END BRANCH
        BEGIN BRANCH B(11:8)
            WIRE 1536 336 1536 416
            WIRE 1536 416 1536 544
            WIRE 1536 544 1536 784
            BEGIN DISPLAY 1536 544 ATTR Name
                ALIGNMENT SOFT-TVCENTER
            END DISPLAY
        END BRANCH
        BEGIN BRANCH B(15:12)
            WIRE 960 336 960 416
            WIRE 960 416 960 544
            WIRE 960 544 960 784
            BEGIN DISPLAY 960 544 ATTR Name
                ALIGNMENT SOFT-TVCENTER
            END DISPLAY
        END BRANCH
        BEGIN BRANCH S(15:0)
            WIRE 880 1392 960 1392
            WIRE 960 1392 1536 1392
            WIRE 1536 1392 2048 1392
            WIRE 2048 1392 2560 1392
            WIRE 2560 1392 2704 1392
        END BRANCH
        BUSTAP 2560 1392 2560 1296
        BEGIN BRANCH S(3:0)
            WIRE 2560 1152 2560 1216
            WIRE 2560 1216 2560 1296
            BEGIN DISPLAY 2560 1216 ATTR Name
                ALIGNMENT SOFT-TVCENTER
            END DISPLAY
        END BRANCH
        BUSTAP 2048 1392 2048 1296
        BEGIN BRANCH S(7:4)
            WIRE 2048 1168 2048 1232
            WIRE 2048 1232 2048 1296
            BEGIN DISPLAY 2048 1232 ATTR Name
                ALIGNMENT SOFT-TVCENTER
            END DISPLAY
        END BRANCH
        BUSTAP 1536 1392 1536 1296
        BEGIN BRANCH S(11:8)
            WIRE 1536 1168 1536 1232
            WIRE 1536 1232 1536 1296
            BEGIN DISPLAY 1536 1232 ATTR Name
                ALIGNMENT SOFT-TVCENTER
            END DISPLAY
        END BRANCH
        BUSTAP 960 1392 960 1296
        BEGIN BRANCH S(15:12)
            WIRE 960 1168 960 1232
            WIRE 960 1232 960 1296
            BEGIN DISPLAY 960 1232 ATTR Name
                ALIGNMENT SOFT-TVCENTER
            END DISPLAY
        END BRANCH
        IOMARKER 880 320 A(15:0) R180 28
        IOMARKER 2704 1392 S(15:0) R0 28
        BEGIN BRANCH B(15:0)
            WIRE 880 240 944 240
            WIRE 944 240 944 240
            WIRE 944 240 960 240
            WIRE 960 240 1104 240
            WIRE 1104 240 1536 240
            WIRE 1536 240 2048 240
            WIRE 2048 240 2560 240
            WIRE 2560 240 2736 240
            BEGIN DISPLAY 1104 240 ATTR Name
                ALIGNMENT SOFT-BCENTER
            END DISPLAY
        END BRANCH
        BUSTAP 960 240 960 336
        BUSTAP 1536 240 1536 336
        BUSTAP 2048 240 2048 336
        BUSTAP 2560 240 2560 336
        IOMARKER 880 240 B(15:0) R180 28
        BEGIN BRANCH Co
            WIRE 592 1280 736 1280
            WIRE 736 1280 1024 1280
            WIRE 1024 1168 1024 1280
            BEGIN DISPLAY 736 1280 ATTR Name
                ALIGNMENT SOFT-BCENTER
            END DISPLAY
        END BRANCH
        IOMARKER 592 1280 Co R180 28
        BEGIN BRANCH Ci
            WIRE 2496 464 2928 464
            WIRE 2496 464 2496 768
        END BRANCH
        IOMARKER 2928 464 Ci R0 28
    END SHEET
END SCHEMATIC
