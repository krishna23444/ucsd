VERSION 6
BEGIN SCHEMATIC
    BEGIN ATTR DeviceFamilyName "virtex2"
        DELETE all:0
        EDITNAME all:0
        EDITTRAIT all:0
    END ATTR
    BEGIN NETLIST
        SIGNAL XLXN_2
        SIGNAL CLR
        SIGNAL VCC
        SIGNAL XLXN_6
        SIGNAL XLXN_8
        SIGNAL XLXN_11
        SIGNAL D3
        SIGNAL CLK_CE
        SIGNAL CLK
        SIGNAL XLXN_16
        SIGNAL XLXN_20
        SIGNAL D1
        SIGNAL XLXN_22
        SIGNAL D2
        SIGNAL XLXN_24
        SIGNAL XLXN_25
        SIGNAL XLXN_27
        SIGNAL XLXN_28
        SIGNAL D0
        SIGNAL XLXN_31
        PORT Input CLR
        PORT Input VCC
        PORT Output D3
        PORT Input CLK_CE
        PORT Input CLK
        PORT Output D1
        PORT Output D2
        PORT Output D0
        BEGIN BLOCKDEF ftce
            TIMESTAMP 2001 2 2 12 37 14
            LINE N 192 -32 64 -32 
            LINE N 192 -64 192 -32 
            LINE N 80 -128 64 -144 
            LINE N 64 -112 80 -128 
            LINE N 384 -256 320 -256 
            LINE N 0 -256 64 -256 
            LINE N 0 -32 64 -32 
            LINE N 0 -192 64 -192 
            LINE N 0 -128 64 -128 
            RECTANGLE N 64 -320 320 -64 
        END BLOCKDEF
        BEGIN BLOCKDEF inv
            TIMESTAMP 2001 2 2 12 38 38
            LINE N 0 -32 64 -32 
            LINE N 224 -32 160 -32 
            LINE N 64 -64 128 -32 
            LINE N 128 -32 64 0 
            LINE N 64 0 64 -64 
            CIRCLE N 128 -48 160 -16 
        END BLOCKDEF
        BEGIN BLOCK XLXI_2 ftce
            PIN C CLK
            PIN CE CLK_CE
            PIN CLR CLR
            PIN T VCC
            PIN Q D0
        END BLOCK
        BEGIN BLOCK XLXI_3 ftce
            PIN C XLXN_31
            PIN CE CLK_CE
            PIN CLR CLR
            PIN T VCC
            PIN Q D1
        END BLOCK
        BEGIN BLOCK XLXI_4 ftce
            PIN C XLXN_22
            PIN CE CLK_CE
            PIN CLR CLR
            PIN T VCC
            PIN Q D2
        END BLOCK
        BEGIN BLOCK XLXI_5 ftce
            PIN C XLXN_24
            PIN CE CLK_CE
            PIN CLR CLR
            PIN T VCC
            PIN Q D3
        END BLOCK
        BEGIN BLOCK XLXI_7 inv
            PIN I D1
            PIN O XLXN_22
        END BLOCK
        BEGIN BLOCK XLXI_8 inv
            PIN I D2
            PIN O XLXN_24
        END BLOCK
        BEGIN BLOCK XLXI_9 inv
            PIN I D0
            PIN O XLXN_31
        END BLOCK
    END NETLIST
    BEGIN SHEET 1 3520 2720
        INSTANCE XLXI_2 896 704 R0
        INSTANCE XLXI_5 896 2144 R0
        INSTANCE XLXI_3 896 1168 R0
        INSTANCE XLXI_4 896 1648 R0
        BEGIN BRANCH CLR
            WIRE 656 2112 768 2112
            WIRE 768 2112 896 2112
            WIRE 768 672 768 1136
            WIRE 768 1136 896 1136
            WIRE 768 1136 768 1616
            WIRE 768 1616 768 2112
            WIRE 768 1616 896 1616
            WIRE 768 672 896 672
        END BRANCH
        IOMARKER 656 2112 CLR R180 28
        BEGIN BRANCH VCC
            WIRE 416 448 512 448
            WIRE 512 448 576 448
            WIRE 576 448 896 448
            WIRE 512 448 512 912
            WIRE 512 912 512 1392
            WIRE 512 1392 512 1888
            WIRE 512 1888 896 1888
            WIRE 512 1392 896 1392
            WIRE 512 912 896 912
        END BRANCH
        BEGIN BRANCH D3
            WIRE 1280 1888 1440 1888
        END BRANCH
        BEGIN BRANCH CLK_CE
            WIRE 416 512 672 512
            WIRE 672 512 896 512
            WIRE 672 512 672 976
            WIRE 672 976 672 1456
            WIRE 672 1456 672 1952
            WIRE 672 1952 896 1952
            WIRE 672 1456 896 1456
            WIRE 672 976 896 976
        END BRANCH
        BEGIN BRANCH CLK
            WIRE 416 576 576 576
            WIRE 576 576 896 576
        END BRANCH
        IOMARKER 416 512 CLK_CE R180 28
        IOMARKER 416 576 CLK R180 28
        IOMARKER 416 448 VCC R180 28
        IOMARKER 1440 448 D0 R0 28
        IOMARKER 1440 912 D1 R0 28
        IOMARKER 1440 1392 D2 R0 28
        IOMARKER 1440 1888 D3 R0 28
        INSTANCE XLXI_7 1168 1168 R180
        INSTANCE XLXI_8 1168 1664 R180
        BEGIN BRANCH D1
            WIRE 1168 1200 1296 1200
            WIRE 1280 912 1296 912
            WIRE 1296 912 1296 1200
            WIRE 1296 912 1440 912
        END BRANCH
        BEGIN BRANCH XLXN_22
            WIRE 880 1200 880 1520
            WIRE 880 1520 896 1520
            WIRE 880 1200 944 1200
        END BRANCH
        BEGIN BRANCH D2
            WIRE 1168 1696 1296 1696
            WIRE 1280 1392 1296 1392
            WIRE 1296 1392 1296 1696
            WIRE 1296 1392 1440 1392
        END BRANCH
        BEGIN BRANCH XLXN_24
            WIRE 880 1696 880 2016
            WIRE 880 2016 896 2016
            WIRE 880 1696 944 1696
        END BRANCH
        INSTANCE XLXI_9 1200 704 R180
        BEGIN BRANCH D0
            WIRE 1200 736 1296 736
            WIRE 1280 448 1296 448
            WIRE 1296 448 1296 736
            WIRE 1296 448 1440 448
        END BRANCH
        BEGIN BRANCH XLXN_31
            WIRE 880 736 880 1040
            WIRE 880 1040 896 1040
            WIRE 880 736 976 736
        END BRANCH
    END SHEET
END SCHEMATIC
