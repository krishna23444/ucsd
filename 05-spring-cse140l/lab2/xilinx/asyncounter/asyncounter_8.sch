VERSION 6
BEGIN SCHEMATIC
    BEGIN ATTR DeviceFamilyName "virtex2"
        DELETE all:0
        EDITNAME all:0
        EDITTRAIT all:0
    END ATTR
    BEGIN NETLIST
        SIGNAL CLR
        SIGNAL VCC
        SIGNAL CLK_CE
        SIGNAL CLK
        SIGNAL D(1)
        SIGNAL XLXN_22
        SIGNAL D(2)
        SIGNAL XLXN_24
        SIGNAL D(0)
        SIGNAL XLXN_31
        SIGNAL XLXN_33
        SIGNAL D(7)
        SIGNAL D(5)
        SIGNAL XLXN_37
        SIGNAL D(6)
        SIGNAL XLXN_40
        SIGNAL D(4)
        SIGNAL XLXN_43
        SIGNAL XLXN_47
        SIGNAL D(7:0)
        SIGNAL D(3)
        PORT Input CLR
        PORT Input VCC
        PORT Input CLK_CE
        PORT Input CLK
        PORT Output D(7:0)
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
            PIN Q D(0)
        END BLOCK
        BEGIN BLOCK XLXI_3 ftce
            PIN C XLXN_31
            PIN CE CLK_CE
            PIN CLR CLR
            PIN T VCC
            PIN Q D(1)
        END BLOCK
        BEGIN BLOCK XLXI_4 ftce
            PIN C XLXN_22
            PIN CE CLK_CE
            PIN CLR CLR
            PIN T VCC
            PIN Q D(2)
        END BLOCK
        BEGIN BLOCK XLXI_5 ftce
            PIN C XLXN_24
            PIN CE CLK_CE
            PIN CLR CLR
            PIN T VCC
            PIN Q D(3)
        END BLOCK
        BEGIN BLOCK XLXI_7 inv
            PIN I D(1)
            PIN O XLXN_22
        END BLOCK
        BEGIN BLOCK XLXI_8 inv
            PIN I D(2)
            PIN O XLXN_24
        END BLOCK
        BEGIN BLOCK XLXI_9 inv
            PIN I D(0)
            PIN O XLXN_31
        END BLOCK
        BEGIN BLOCK XLXI_10 ftce
            PIN C XLXN_47
            PIN CE CLK_CE
            PIN CLR CLR
            PIN T VCC
            PIN Q D(4)
        END BLOCK
        BEGIN BLOCK XLXI_11 ftce
            PIN C XLXN_40
            PIN CE CLK_CE
            PIN CLR CLR
            PIN T VCC
            PIN Q D(7)
        END BLOCK
        BEGIN BLOCK XLXI_12 ftce
            PIN C XLXN_43
            PIN CE CLK_CE
            PIN CLR CLR
            PIN T VCC
            PIN Q D(5)
        END BLOCK
        BEGIN BLOCK XLXI_13 ftce
            PIN C XLXN_37
            PIN CE CLK_CE
            PIN CLR CLR
            PIN T VCC
            PIN Q D(6)
        END BLOCK
        BEGIN BLOCK XLXI_14 inv
            PIN I D(5)
            PIN O XLXN_37
        END BLOCK
        BEGIN BLOCK XLXI_15 inv
            PIN I D(6)
            PIN O XLXN_40
        END BLOCK
        BEGIN BLOCK XLXI_16 inv
            PIN I D(4)
            PIN O XLXN_43
        END BLOCK
        BEGIN BLOCK XLXI_17 inv
            PIN I D(3)
            PIN O XLXN_47
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
            WIRE 768 2112 768 2160
            WIRE 768 2160 1088 2160
            WIRE 1088 2160 2080 2160
            WIRE 768 672 768 1136
            WIRE 768 1136 896 1136
            WIRE 768 1136 768 1616
            WIRE 768 1616 768 2112
            WIRE 768 1616 896 1616
            WIRE 768 672 896 672
            WIRE 2080 672 2080 1136
            WIRE 2080 1136 2208 1136
            WIRE 2080 1136 2080 1616
            WIRE 2080 1616 2080 2112
            WIRE 2080 2112 2208 2112
            WIRE 2080 2112 2080 2160
            WIRE 2080 1616 2208 1616
            WIRE 2080 672 2208 672
        END BRANCH
        IOMARKER 656 2112 CLR R180 28
        BEGIN BRANCH VCC
            WIRE 416 448 512 448
            WIRE 512 448 512 912
            WIRE 512 912 512 1392
            WIRE 512 1392 512 1888
            WIRE 512 1888 896 1888
            WIRE 512 1392 896 1392
            WIRE 512 912 896 912
            WIRE 512 448 896 448
            WIRE 512 272 2032 272
            WIRE 2032 272 2032 448
            WIRE 2032 448 2208 448
            WIRE 2032 448 2032 912
            WIRE 2032 912 2208 912
            WIRE 2032 912 2032 1392
            WIRE 2032 1392 2032 1888
            WIRE 2032 1888 2208 1888
            WIRE 2032 1392 2208 1392
            WIRE 512 272 512 448
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
            WIRE 672 352 672 512
            WIRE 672 352 2128 352
            WIRE 2128 352 2128 512
            WIRE 2128 512 2208 512
            WIRE 2128 512 2128 976
            WIRE 2128 976 2208 976
            WIRE 2128 976 2128 1456
            WIRE 2128 1456 2128 1952
            WIRE 2128 1952 2208 1952
            WIRE 2128 1456 2208 1456
        END BRANCH
        BEGIN BRANCH CLK
            WIRE 416 576 896 576
        END BRANCH
        IOMARKER 416 512 CLK_CE R180 28
        IOMARKER 416 576 CLK R180 28
        IOMARKER 416 448 VCC R180 28
        INSTANCE XLXI_7 1168 1168 R180
        INSTANCE XLXI_8 1168 1664 R180
        BEGIN BRANCH D(1)
            WIRE 1168 1200 1296 1200
            WIRE 1280 912 1296 912
            WIRE 1296 912 1296 1200
            WIRE 1296 912 1600 912
            WIRE 1600 192 1600 912
        END BRANCH
        BEGIN BRANCH XLXN_22
            WIRE 880 1200 880 1520
            WIRE 880 1520 896 1520
            WIRE 880 1200 944 1200
        END BRANCH
        BEGIN BRANCH D(2)
            WIRE 1168 1696 1296 1696
            WIRE 1280 1392 1296 1392
            WIRE 1296 1392 1296 1696
            WIRE 1296 1392 1712 1392
            WIRE 1712 192 1712 1392
        END BRANCH
        BEGIN BRANCH XLXN_24
            WIRE 880 1696 880 2016
            WIRE 880 2016 896 2016
            WIRE 880 1696 928 1696
            WIRE 928 1696 944 1696
        END BRANCH
        INSTANCE XLXI_9 1200 704 R180
        BEGIN BRANCH D(0)
            WIRE 1200 736 1296 736
            WIRE 1280 448 1296 448
            WIRE 1296 448 1296 736
            WIRE 1296 448 1504 448
            WIRE 1504 192 1504 448
        END BRANCH
        BEGIN BRANCH XLXN_31
            WIRE 880 736 880 1040
            WIRE 880 1040 896 1040
            WIRE 880 736 976 736
        END BRANCH
        INSTANCE XLXI_10 2208 704 R0
        INSTANCE XLXI_11 2208 2144 R0
        INSTANCE XLXI_12 2208 1168 R0
        INSTANCE XLXI_13 2208 1648 R0
        BEGIN BRANCH D(7)
            WIRE 2592 1888 2880 1888
            WIRE 2880 1888 2896 1888
        END BRANCH
        INSTANCE XLXI_14 2480 1168 R180
        INSTANCE XLXI_15 2480 1664 R180
        BEGIN BRANCH D(5)
            WIRE 2480 1200 2608 1200
            WIRE 2592 912 2608 912
            WIRE 2608 912 2608 1200
            WIRE 2608 912 2880 912
        END BRANCH
        BEGIN BRANCH XLXN_37
            WIRE 2192 1200 2192 1520
            WIRE 2192 1520 2208 1520
            WIRE 2192 1200 2256 1200
        END BRANCH
        BEGIN BRANCH D(6)
            WIRE 2480 1696 2608 1696
            WIRE 2592 1392 2608 1392
            WIRE 2608 1392 2608 1696
            WIRE 2608 1392 2880 1392
        END BRANCH
        BEGIN BRANCH XLXN_40
            WIRE 2192 1696 2192 2016
            WIRE 2192 2016 2208 2016
            WIRE 2192 1696 2256 1696
        END BRANCH
        INSTANCE XLXI_16 2512 704 R180
        BEGIN BRANCH D(4)
            WIRE 2512 736 2608 736
            WIRE 2592 448 2608 448
            WIRE 2608 448 2608 736
            WIRE 2608 448 2880 448
        END BRANCH
        BEGIN BRANCH XLXN_43
            WIRE 2192 736 2192 1040
            WIRE 2192 1040 2208 1040
            WIRE 2192 736 2288 736
        END BRANCH
        BEGIN BRANCH XLXN_47
            WIRE 2048 576 2208 576
        END BRANCH
        BEGIN BRANCH D(7:0)
            WIRE 1424 96 1504 96
            WIRE 1504 96 1600 96
            WIRE 1600 96 1712 96
            WIRE 1712 96 1808 96
            WIRE 1808 96 2976 96
            WIRE 2976 96 2976 448
            WIRE 2976 448 2976 912
            WIRE 2976 912 2976 1392
            WIRE 2976 1392 2976 1888
            WIRE 2976 1888 2976 2000
        END BRANCH
        BUSTAP 2976 448 2880 448
        BUSTAP 2976 1392 2880 1392
        BUSTAP 2976 1888 2880 1888
        BUSTAP 2976 912 2880 912
        BUSTAP 1504 96 1504 192
        BUSTAP 1600 96 1600 192
        BUSTAP 1712 96 1712 192
        BUSTAP 1808 96 1808 192
        BEGIN BRANCH D(3)
            WIRE 1280 1888 1808 1888
            WIRE 1808 192 1808 576
            WIRE 1808 576 1824 576
            WIRE 1808 576 1808 1888
        END BRANCH
        INSTANCE XLXI_17 1824 608 R0
        IOMARKER 2976 2000 D(7:0) R90 28
    END SHEET
END SCHEMATIC
