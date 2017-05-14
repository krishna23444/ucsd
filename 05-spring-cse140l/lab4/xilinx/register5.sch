VERSION 6
BEGIN SCHEMATIC
    BEGIN ATTR DeviceFamilyName "virtex2"
        DELETE all:0
        EDITNAME all:0
        EDITTRAIT all:0
    END ATTR
    BEGIN NETLIST
        SIGNAL clk_en
        SIGNAL clk
        SIGNAL clr
        SIGNAL D(4)
        SIGNAL D(4:0)
        SIGNAL D(3)
        SIGNAL D(2)
        SIGNAL D(1)
        SIGNAL D(0)
        SIGNAL Q(4:0)
        SIGNAL Q(0)
        SIGNAL Q(1)
        SIGNAL Q(2)
        SIGNAL Q(3)
        SIGNAL Q(4)
        PORT Input clk_en
        PORT Input clk
        PORT Input clr
        PORT Input D(4:0)
        PORT Output Q(4:0)
        BEGIN BLOCKDEF fdre
            TIMESTAMP 2001 2 2 12 37 14
            LINE N 0 -128 64 -128 
            LINE N 0 -192 64 -192 
            LINE N 0 -256 64 -256 
            LINE N 384 -256 320 -256 
            LINE N 0 -32 64 -32 
            RECTANGLE N 64 -320 320 -64 
            LINE N 192 -64 192 -32 
            LINE N 192 -32 64 -32 
            LINE N 64 -112 80 -128 
            LINE N 80 -128 64 -144 
        END BLOCKDEF
        BEGIN BLOCK XLXI_1 fdre
            PIN C clk
            PIN CE clk_en
            PIN D D(1)
            PIN R clr
            PIN Q Q(1)
        END BLOCK
        BEGIN BLOCK XLXI_2 fdre
            PIN C clk
            PIN CE clk_en
            PIN D D(0)
            PIN R clr
            PIN Q Q(0)
        END BLOCK
        BEGIN BLOCK XLXI_3 fdre
            PIN C clk
            PIN CE clk_en
            PIN D D(2)
            PIN R clr
            PIN Q Q(2)
        END BLOCK
        BEGIN BLOCK XLXI_4 fdre
            PIN C clk
            PIN CE clk_en
            PIN D D(3)
            PIN R clr
            PIN Q Q(3)
        END BLOCK
        BEGIN BLOCK XLXI_5 fdre
            PIN C clk
            PIN CE clk_en
            PIN D D(4)
            PIN R clr
            PIN Q Q(4)
        END BLOCK
    END NETLIST
    BEGIN SHEET 1 3520 2720
        INSTANCE XLXI_1 528 992 R0
        INSTANCE XLXI_3 528 1376 R0
        INSTANCE XLXI_4 528 1744 R0
        INSTANCE XLXI_2 528 608 R0
        BEGIN BRANCH clk
            WIRE 400 480 496 480
            WIRE 496 480 528 480
            WIRE 496 480 496 864
            WIRE 496 864 528 864
            WIRE 496 864 496 1248
            WIRE 496 1248 528 1248
            WIRE 496 1248 496 1616
            WIRE 496 1616 528 1616
            WIRE 496 1616 496 1984
            WIRE 496 1984 528 1984
        END BRANCH
        INSTANCE XLXI_5 528 2112 R0
        BEGIN BRANCH clk_en
            WIRE 400 416 464 416
            WIRE 464 416 528 416
            WIRE 464 416 464 800
            WIRE 464 800 528 800
            WIRE 464 800 464 1184
            WIRE 464 1184 528 1184
            WIRE 464 1184 464 1552
            WIRE 464 1552 464 1920
            WIRE 464 1920 528 1920
            WIRE 464 1552 528 1552
        END BRANCH
        BEGIN BRANCH clr
            WIRE 400 576 432 576
            WIRE 432 576 528 576
            WIRE 432 576 432 960
            WIRE 432 960 528 960
            WIRE 432 960 432 1344
            WIRE 432 1344 528 1344
            WIRE 432 1344 432 1712
            WIRE 432 1712 432 2080
            WIRE 432 2080 528 2080
            WIRE 432 1712 528 1712
        END BRANCH
        BEGIN BRANCH D(4)
            WIRE 384 1856 400 1856
            WIRE 400 1856 528 1856
            BEGIN DISPLAY 400 1856 ATTR Name
                ALIGNMENT SOFT-BCENTER
            END DISPLAY
        END BRANCH
        BUSTAP 288 1856 384 1856
        BEGIN BRANCH D(4:0)
            WIRE 208 256 288 256
            WIRE 288 256 288 336
            WIRE 288 336 288 352
            WIRE 288 352 288 736
            WIRE 288 736 288 1120
            WIRE 288 1120 288 1488
            WIRE 288 1488 288 1856
            WIRE 288 1856 288 1904
        END BRANCH
        BUSTAP 288 1488 384 1488
        BEGIN BRANCH D(3)
            WIRE 384 1488 400 1488
            WIRE 400 1488 528 1488
            BEGIN DISPLAY 400 1488 ATTR Name
                ALIGNMENT SOFT-BCENTER
            END DISPLAY
        END BRANCH
        BUSTAP 288 1120 384 1120
        BEGIN BRANCH D(2)
            WIRE 384 1120 400 1120
            WIRE 400 1120 528 1120
            BEGIN DISPLAY 400 1120 ATTR Name
                ALIGNMENT SOFT-BCENTER
            END DISPLAY
        END BRANCH
        BUSTAP 288 736 384 736
        BEGIN BRANCH D(1)
            WIRE 384 736 400 736
            WIRE 400 736 528 736
            BEGIN DISPLAY 400 736 ATTR Name
                ALIGNMENT SOFT-BCENTER
            END DISPLAY
        END BRANCH
        BUSTAP 288 352 384 352
        BEGIN BRANCH D(0)
            WIRE 384 352 400 352
            WIRE 400 352 528 352
            BEGIN DISPLAY 400 352 ATTR Name
                ALIGNMENT SOFT-BCENTER
            END DISPLAY
        END BRANCH
        IOMARKER 208 256 D(4:0) R180 28
        IOMARKER 400 416 clk_en R180 28
        IOMARKER 400 480 clk R180 28
        IOMARKER 400 576 clr R180 28
        BUSTAP 1040 352 944 352
        BEGIN BRANCH Q(4:0)
            WIRE 1040 224 1136 224
            WIRE 1040 224 1040 352
            WIRE 1040 352 1040 736
            WIRE 1040 736 1040 1120
            WIRE 1040 1120 1040 1488
            WIRE 1040 1488 1040 1856
            WIRE 1040 1856 1040 1888
        END BRANCH
        IOMARKER 1136 224 Q(4:0) R0 28
        BEGIN BRANCH Q(0)
            WIRE 912 352 928 352
            WIRE 928 352 944 352
            BEGIN DISPLAY 928 352 ATTR Name
                ALIGNMENT SOFT-BCENTER
            END DISPLAY
        END BRANCH
        BEGIN BRANCH Q(1)
            WIRE 912 736 928 736
            WIRE 928 736 944 736
            BEGIN DISPLAY 928 736 ATTR Name
                ALIGNMENT SOFT-BCENTER
            END DISPLAY
        END BRANCH
        BUSTAP 1040 736 944 736
        BEGIN BRANCH Q(2)
            WIRE 912 1120 928 1120
            WIRE 928 1120 944 1120
            BEGIN DISPLAY 928 1120 ATTR Name
                ALIGNMENT SOFT-BCENTER
            END DISPLAY
        END BRANCH
        BUSTAP 1040 1120 944 1120
        BEGIN BRANCH Q(3)
            WIRE 912 1488 928 1488
            WIRE 928 1488 944 1488
            BEGIN DISPLAY 928 1488 ATTR Name
                ALIGNMENT SOFT-BCENTER
            END DISPLAY
        END BRANCH
        BUSTAP 1040 1488 944 1488
        BEGIN BRANCH Q(4)
            WIRE 912 1856 928 1856
            WIRE 928 1856 944 1856
            BEGIN DISPLAY 928 1856 ATTR Name
                ALIGNMENT SOFT-BCENTER
            END DISPLAY
        END BRANCH
        BUSTAP 1040 1856 944 1856
    END SHEET
END SCHEMATIC
