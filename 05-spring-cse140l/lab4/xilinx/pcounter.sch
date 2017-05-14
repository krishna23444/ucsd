VERSION 6
BEGIN SCHEMATIC
    BEGIN ATTR DeviceFamilyName "virtex2"
        DELETE all:0
        EDITNAME all:0
        EDITTRAIT all:0
    END ATTR
    BEGIN NETLIST
        SIGNAL addr(3)
        SIGNAL addr(2)
        SIGNAL addr(1)
        SIGNAL addr(0)
        SIGNAL addr(3:0)
        SIGNAL clr
        SIGNAL clk
        SIGNAL clk_en
        PORT Output addr(3:0)
        PORT Input clr
        PORT Input clk
        PORT Input clk_en
        BEGIN BLOCKDEF cb4ce
            TIMESTAMP 2001 2 2 12 36 39
            RECTANGLE N 64 -512 320 -64 
            LINE N 0 -32 64 -32 
            LINE N 0 -128 64 -128 
            LINE N 384 -256 320 -256 
            LINE N 384 -320 320 -320 
            LINE N 384 -384 320 -384 
            LINE N 384 -448 320 -448 
            LINE N 80 -128 64 -144 
            LINE N 64 -112 80 -128 
            LINE N 384 -128 320 -128 
            LINE N 192 -32 64 -32 
            LINE N 192 -64 192 -32 
            LINE N 0 -192 64 -192 
            LINE N 384 -192 320 -192 
        END BLOCKDEF
        BEGIN BLOCK XLXI_1 cb4ce
            PIN C clk
            PIN CE clk_en
            PIN CLR clr
            PIN CEO
            PIN Q0 addr(0)
            PIN Q1 addr(1)
            PIN Q2 addr(2)
            PIN Q3 addr(3)
            PIN TC
        END BLOCK
    END NETLIST
    BEGIN SHEET 1 3520 2720
        INSTANCE XLXI_1 1216 1344 R0
        BEGIN BRANCH addr(3)
            WIRE 1600 1088 1616 1088
            WIRE 1616 1088 1632 1088
            BEGIN DISPLAY 1616 1088 ATTR Name
                ALIGNMENT SOFT-BCENTER
            END DISPLAY
        END BRANCH
        BEGIN BRANCH addr(2)
            WIRE 1600 1024 1616 1024
            WIRE 1616 1024 1632 1024
            BEGIN DISPLAY 1616 1024 ATTR Name
                ALIGNMENT SOFT-BCENTER
            END DISPLAY
        END BRANCH
        BEGIN BRANCH addr(1)
            WIRE 1600 960 1616 960
            WIRE 1616 960 1632 960
            BEGIN DISPLAY 1616 960 ATTR Name
                ALIGNMENT SOFT-BCENTER
            END DISPLAY
        END BRANCH
        BEGIN BRANCH addr(0)
            WIRE 1600 896 1616 896
            WIRE 1616 896 1632 896
            BEGIN DISPLAY 1616 896 ATTR Name
                ALIGNMENT SOFT-BCENTER
            END DISPLAY
        END BRANCH
        BEGIN BRANCH addr(3:0)
            WIRE 1728 768 1760 768
            WIRE 1728 768 1728 880
            WIRE 1728 880 1728 896
            WIRE 1728 896 1728 960
            WIRE 1728 960 1728 992
            WIRE 1728 992 1728 1024
            WIRE 1728 1024 1728 1088
            WIRE 1728 1088 1728 1120
            BEGIN DISPLAY 1728 992 ATTR Name
                ALIGNMENT SOFT-TVCENTER
            END DISPLAY
        END BRANCH
        BUSTAP 1728 1088 1632 1088
        BUSTAP 1728 1024 1632 1024
        BUSTAP 1728 960 1632 960
        BUSTAP 1728 896 1632 896
        IOMARKER 1760 768 addr(3:0) R0 28
        BEGIN BRANCH clr
            WIRE 1184 1312 1216 1312
        END BRANCH
        IOMARKER 1184 1312 clr R180 28
        BEGIN BRANCH clk
            WIRE 1184 1216 1216 1216
        END BRANCH
        IOMARKER 1184 1216 clk R180 28
        BEGIN BRANCH clk_en
            WIRE 1184 1152 1216 1152
        END BRANCH
        IOMARKER 1184 1152 clk_en R180 28
    END SHEET
END SCHEMATIC
