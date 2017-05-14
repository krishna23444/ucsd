VERSION 6
BEGIN SCHEMATIC
    BEGIN ATTR DeviceFamilyName "virtex2"
        DELETE all:0
        EDITNAME all:0
        EDITTRAIT all:0
    END ATTR
    BEGIN NETLIST
        SIGNAL XLXN_1(4:0)
        SIGNAL clr
        SIGNAL XLXN_5
        SIGNAL clk
        SIGNAL R2_en
        SIGNAL R3_en
        SIGNAL ov_en
        SIGNAL mem_D(4:0)
        SIGNAL dpath_D(4:0)
        SIGNAL OV
        SIGNAL ge_en
        SIGNAL GE
        SIGNAL XLXN_17(4:0)
        SIGNAL R2(4:0)
        SIGNAL R3(4:0)
        SIGNAL OVF
        SIGNAL GEF
        SIGNAL XLXN_23(4:0)
        SIGNAL XLXN_25(4:0)
        SIGNAL R2_sel
        SIGNAL XLXN_27(4:0)
        SIGNAL XLXN_28(4:0)
        SIGNAL R1_en
        SIGNAL R1(4:0)
        SIGNAL XLXN_32(4:0)
        SIGNAL XLXN_33
        SIGNAL XLXN_34
        SIGNAL XLXN_35
        SIGNAL XLXN_36
        SIGNAL XLXN_37
        SIGNAL XLXN_38(4:0)
        SIGNAL XLXN_39(4:0)
        SIGNAL XLXN_40
        SIGNAL XLXN_41(4:0)
        SIGNAL XLXN_42(4:0)
        SIGNAL XLXN_43
        SIGNAL XLXN_44(4:0)
        SIGNAL XLXN_45(4:0)
        SIGNAL XLXN_46
        SIGNAL XLXN_48
        PORT Input clr
        PORT Input clk
        PORT Input R2_en
        PORT Input R3_en
        PORT Input ov_en
        PORT Input mem_D(4:0)
        PORT Input dpath_D(4:0)
        PORT Input OV
        PORT Input ge_en
        PORT Input GE
        PORT Output R2(4:0)
        PORT Output R3(4:0)
        PORT Output OVF
        PORT Output GEF
        PORT Input R2_sel
        PORT Input R1_en
        PORT Output R1(4:0)
        BEGIN BLOCKDEF Register5
            TIMESTAMP 2005 6 1 5 39 49
            RECTANGLE N 64 -256 320 0 
            LINE N 64 -224 0 -224 
            RECTANGLE N 0 -236 64 -212 
            LINE N 64 -160 0 -160 
            LINE N 64 -96 0 -96 
            LINE N 64 -32 0 -32 
            LINE N 320 -224 384 -224 
            RECTANGLE N 320 -236 384 -212 
        END BLOCKDEF
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
        BEGIN BLOCK Reg2 Register5
            PIN D(4:0) XLXN_1(4:0)
            PIN clk_en R2_en
            PIN clk clk
            PIN clr clr
            PIN Q(4:0) R2(4:0)
        END BLOCK
        BEGIN BLOCK XLXI_4 Mux2_5bus
            PIN A(4:0) mem_D(4:0)
            PIN B(4:0) R3(4:0)
            PIN C(4:0) XLXN_1(4:0)
            PIN S R2_sel
        END BLOCK
        BEGIN BLOCK Reg3 Register5
            PIN D(4:0) dpath_D(4:0)
            PIN clk_en R3_en
            PIN clk clk
            PIN clr clr
            PIN Q(4:0) R3(4:0)
        END BLOCK
        BEGIN BLOCK OV_FLG fdre
            PIN C clk
            PIN CE ov_en
            PIN D OV
            PIN R clr
            PIN Q OVF
        END BLOCK
        BEGIN BLOCK GE_FLG fdre
            PIN C clk
            PIN CE ge_en
            PIN D GE
            PIN R clr
            PIN Q GEF
        END BLOCK
        BEGIN BLOCK Reg1 Register5
            PIN D(4:0) mem_D(4:0)
            PIN clk_en R1_en
            PIN clk clk
            PIN clr clr
            PIN Q(4:0) R1(4:0)
        END BLOCK
    END NETLIST
    BEGIN SHEET 1 3520 2720
        BEGIN INSTANCE Reg2 1104 992 R0
            BEGIN DISPLAY 0 -376 ATTR InstName
                FONT 28 "Arial"
            END DISPLAY
        END INSTANCE
        BEGIN BRANCH XLXN_1(4:0)
            WIRE 992 624 1088 624
            WIRE 1088 624 1088 768
            WIRE 1088 768 1104 768
        END BRANCH
        BEGIN INSTANCE Reg3 1104 1408 R0
            BEGIN DISPLAY 0 -376 ATTR InstName
                FONT 28 "Arial"
            END DISPLAY
        END INSTANCE
        BEGIN INSTANCE OV_FLG 1104 1872 R0
            BEGIN DISPLAY 0 -412 ATTR InstName
                FONT 28 "Arial"
            END DISPLAY
        END INSTANCE
        BEGIN INSTANCE GE_FLG 1104 2320 R0
            BEGIN DISPLAY 0 -412 ATTR InstName
                FONT 28 "Arial"
            END DISPLAY
        END INSTANCE
        BEGIN BRANCH clr
            WIRE 576 528 1072 528
            WIRE 1072 528 1104 528
            WIRE 1072 528 1072 960
            WIRE 1072 960 1072 1376
            WIRE 1072 1376 1072 1840
            WIRE 1072 1840 1104 1840
            WIRE 1072 1840 1072 2288
            WIRE 1072 2288 1104 2288
            WIRE 1072 1376 1104 1376
            WIRE 1072 960 1104 960
        END BRANCH
        BEGIN BRANCH clk
            WIRE 576 464 1040 464
            WIRE 1040 464 1104 464
            WIRE 1040 464 1040 896
            WIRE 1040 896 1040 1312
            WIRE 1040 1312 1040 1744
            WIRE 1040 1744 1040 2192
            WIRE 1040 2192 1104 2192
            WIRE 1040 1744 1104 1744
            WIRE 1040 1312 1104 1312
            WIRE 1040 896 1104 896
        END BRANCH
        BEGIN BRANCH R2_en
            WIRE 576 832 1104 832
        END BRANCH
        IOMARKER 576 832 R2_en R180 28
        BEGIN BRANCH R3_en
            WIRE 576 1248 1104 1248
        END BRANCH
        IOMARKER 576 1248 R3_en R180 28
        BEGIN BRANCH ov_en
            WIRE 576 1680 1104 1680
        END BRANCH
        IOMARKER 576 1680 ov_en R180 28
        BEGIN BRANCH mem_D(4:0)
            WIRE 576 336 704 336
            WIRE 704 336 1104 336
            WIRE 704 336 704 624
            WIRE 704 624 736 624
        END BRANCH
        BEGIN BRANCH dpath_D(4:0)
            WIRE 576 1184 1104 1184
        END BRANCH
        IOMARKER 576 1184 dpath_D(4:0) R180 28
        BEGIN BRANCH OV
            WIRE 576 1616 1104 1616
        END BRANCH
        IOMARKER 576 1616 OV R180 28
        BEGIN BRANCH ge_en
            WIRE 576 2128 1104 2128
        END BRANCH
        IOMARKER 576 2128 ge_en R180 28
        BEGIN BRANCH GE
            WIRE 576 2064 1104 2064
        END BRANCH
        IOMARKER 576 2064 GE R180 28
        BEGIN BRANCH R2(4:0)
            WIRE 1488 768 1600 768
        END BRANCH
        IOMARKER 1600 768 R2(4:0) R0 28
        BEGIN BRANCH R3(4:0)
            WIRE 672 688 736 688
            WIRE 672 688 672 1072
            WIRE 672 1072 1536 1072
            WIRE 1536 1072 1536 1184
            WIRE 1536 1184 1600 1184
            WIRE 1488 1184 1536 1184
        END BRANCH
        IOMARKER 1600 1184 R3(4:0) R0 28
        BEGIN BRANCH OVF
            WIRE 1488 1616 1600 1616
        END BRANCH
        IOMARKER 1600 1616 OVF R0 28
        BEGIN BRANCH GEF
            WIRE 1488 2064 1600 2064
        END BRANCH
        IOMARKER 1600 2064 GEF R0 28
        BEGIN INSTANCE Reg1 1104 560 R0
            BEGIN DISPLAY 0 -376 ATTR InstName
                FONT 28 "Arial"
            END DISPLAY
        END INSTANCE
        BEGIN BRANCH R1_en
            WIRE 576 400 1104 400
        END BRANCH
        BEGIN BRANCH R1(4:0)
            WIRE 1488 336 1600 336
        END BRANCH
        IOMARKER 576 464 clk R180 28
        IOMARKER 576 528 clr R180 28
        IOMARKER 576 400 R1_en R180 28
        IOMARKER 576 336 mem_D(4:0) R180 28
        IOMARKER 1600 336 R1(4:0) R0 28
        BEGIN INSTANCE XLXI_4 736 784 R0
        END INSTANCE
        BEGIN BRANCH R2_sel
            WIRE 576 784 896 784
        END BRANCH
        IOMARKER 576 784 R2_sel R180 28
    END SHEET
END SCHEMATIC
