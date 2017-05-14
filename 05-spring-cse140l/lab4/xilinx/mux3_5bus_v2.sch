VERSION 6
BEGIN SCHEMATIC
    BEGIN ATTR DeviceFamilyName "virtex2"
        DELETE all:0
        EDITNAME all:0
        EDITTRAIT all:0
    END ATTR
    BEGIN NETLIST
        SIGNAL XLXN_3(4:0)
        SIGNAL A(4:0)
        SIGNAL B(4:0)
        SIGNAL O(4:0)
        SIGNAL XLXN_12(4:0)
        SIGNAL sel(1)
        SIGNAL sel(0)
        SIGNAL sel(1:0)
        SIGNAL XLXN_19(4:0)
        SIGNAL C(4:0)
        PORT Input A(4:0)
        PORT Input B(4:0)
        PORT Output O(4:0)
        PORT Input sel(1:0)
        PORT Input C(4:0)
        BEGIN BLOCKDEF Mux2_5bus
            TIMESTAMP 2005 6 1 5 41 22
            RECTANGLE N 64 -192 192 -64 
            LINE N 64 -160 0 -160 
            RECTANGLE N 0 -172 64 -148 
            LINE N 64 -96 0 -96 
            RECTANGLE N 0 -108 64 -84 
            LINE N 192 -160 256 -160 
            RECTANGLE N 192 -172 256 -148 
            LINE N 160 -64 160 0 
        END BLOCKDEF
        BEGIN BLOCK XLXI_1 Mux2_5bus
            PIN A(4:0) A(4:0)
            PIN B(4:0) B(4:0)
            PIN C(4:0) XLXN_19(4:0)
            PIN S sel(0)
        END BLOCK
        BEGIN BLOCK XLXI_2 Mux2_5bus
            PIN A(4:0) XLXN_19(4:0)
            PIN B(4:0) C(4:0)
            PIN C(4:0) O(4:0)
            PIN S sel(1)
        END BLOCK
    END NETLIST
    BEGIN SHEET 1 3520 2720
        BEGIN INSTANCE XLXI_1 1120 1024 R0
        END INSTANCE
        BEGIN BRANCH A(4:0)
            WIRE 1088 864 1120 864
        END BRANCH
        IOMARKER 1088 864 A(4:0) R180 28
        BEGIN BRANCH B(4:0)
            WIRE 1088 928 1120 928
        END BRANCH
        IOMARKER 1088 928 B(4:0) R180 28
        BEGIN BRANCH O(4:0)
            WIRE 1808 800 1824 800
            WIRE 1824 800 1872 800
        END BRANCH
        BEGIN INSTANCE XLXI_2 1552 960 R0
        END INSTANCE
        BUSTAP 1280 1152 1280 1056
        BUSTAP 1712 1152 1712 1056
        BEGIN BRANCH sel(1)
            WIRE 1712 960 1712 1008
            WIRE 1712 1008 1712 1056
            BEGIN DISPLAY 1712 1008 ATTR Name
                ALIGNMENT SOFT-TVCENTER
            END DISPLAY
        END BRANCH
        BEGIN BRANCH sel(0)
            WIRE 1280 1024 1280 1040
            WIRE 1280 1040 1280 1056
            BEGIN DISPLAY 1280 1040 ATTR Name
                ALIGNMENT SOFT-TVCENTER
            END DISPLAY
        END BRANCH
        BEGIN BRANCH sel(1:0)
            WIRE 1120 1152 1488 1152
            WIRE 1488 1152 1744 1152
            BEGIN DISPLAY 1488 1152 ATTR Name
                ALIGNMENT SOFT-BCENTER
            END DISPLAY
        END BRANCH
        IOMARKER 1120 1152 sel(1:0) R180 28
        IOMARKER 1872 800 O(4:0) R0 28
        BEGIN BRANCH XLXN_19(4:0)
            WIRE 1376 864 1456 864
            WIRE 1456 800 1456 864
            WIRE 1456 800 1552 800
        END BRANCH
        BEGIN BRANCH C(4:0)
            WIRE 1456 944 1488 944
            WIRE 1488 864 1552 864
            WIRE 1488 864 1488 944
        END BRANCH
        IOMARKER 1456 944 C(4:0) R180 28
    END SHEET
END SCHEMATIC
