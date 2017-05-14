////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 1995-2005 Xilinx, Inc.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////
//   ____  ____
//  /   /\/   /
// /___/  \  /    Vendor: Xilinx
// \   \   \/     Version: H.39
//  \   \         Application: netgen
//  /   /         Filename: sixteenbit_adder_sta.v
// /___/   /\     Timestamp: Mon Apr 18 20:46:27 2005
// \   \  /  \ 
//  \___\/\___\
//             
// Command	: -sta -w sixteenbit_adder.ncd sixteenbit_adder_sta.v 
// Device	: 2v80cs144-5 (PRODUCTION 1.121 2005-02-23)
// Input file	: sixteenbit_adder.ncd
// Output file	: sixteenbit_adder_sta.v
// # of Modules	: 1
// Design Name	: sixteenbit_adder
// Xilinx	: G:/xilinx/ise71
//             
// Purpose:    
//     This verilog netlist is a verification model and uses library 
//     primitives which may not represent the true implementation of 
//     the device, however the netlist is functionally correct and 
//     should not be modified. This file cannot be synthesized and 
//     should only be used with supported static timing analysis tools.
//             
// Reference:  
//     Development System Reference Guide, Chapter 23
//             
////////////////////////////////////////////////////////////////////////////////

`timescale 1 ns/1 ps

module sixteenbit_adder (
  Ci, Co, A, B, S
);
  input Ci;
  output Co;
  input [15 : 0] A;
  input [15 : 0] B;
  output [15 : 0] S;
  wire A_0_IBUF_0;
  wire A_1_IBUF_1;
  wire A_2_IBUF_2;
  wire A_3_IBUF_3;
  wire B_0_IBUF_4;
  wire A_4_IBUF_5;
  wire B_1_IBUF_6;
  wire A_5_IBUF_7;
  wire B_2_IBUF_8;
  wire A_6_IBUF_9;
  wire B_3_IBUF_10;
  wire A_7_IBUF_11;
  wire B_4_IBUF_12;
  wire A_8_IBUF_13;
  wire B_5_IBUF_14;
  wire A_9_IBUF_15;
  wire B_6_IBUF_16;
  wire B_7_IBUF_17;
  wire B_8_IBUF_18;
  wire B_9_IBUF_19;
  wire Ci_IBUF_20;
  wire A_10_IBUF_21;
  wire A_11_IBUF_22;
  wire A_12_IBUF_23;
  wire A_13_IBUF_24;
  wire A_14_IBUF_25;
  wire A_15_IBUF_26;
  wire B_10_IBUF_27;
  wire B_11_IBUF_28;
  wire B_12_IBUF_29;
  wire B_13_IBUF_30;
  wire B_14_IBUF_31;
  wire B_15_IBUF_32;
  wire \XLXI_4/XLXN_3 ;
  wire XLXN_2_0;
  wire \XLXI_4/XLXN_7_0 ;
  wire \XLXI_4/XLXI_2/XLXN_5_0 ;
  wire \XLXI_4/XLXI_1/XLXN_5_0 ;
  wire \XLXI_3/XLXI_4/XLXN_5_0 ;
  wire \XLXI_3/XLXN_7_0 ;
  wire \XLXI_3/XLXI_3/XLXN_5_0 ;
  wire \XLXI_2/XLXN_10 ;
  wire \XLXI_2/XLXN_7_0 ;
  wire XLXN_1_0;
  wire \XLXI_5/XLXI_3/XLXN_5_0 ;
  wire \XLXI_5/XLXN_3 ;
  wire \XLXI_5/XLXI_2/XLXN_5_0 ;
  wire \XLXI_3/XLXI_2/XLXN_5_0 ;
  wire \XLXI_3/XLXI_1/XLXN_5_0 ;
  wire \XLXI_5/XLXI_1/XLXN_5_0 ;
  wire \XLXI_4/XLXN_10 ;
  wire \XLXI_4/XLXI_4/XLXN_5_0 ;
  wire \XLXI_2/XLXI_4/XLXN_5_0 ;
  wire \XLXI_2/XLXI_3/XLXN_5_0 ;
  wire XLXN_3_0;
  wire \XLXI_5/XLXN_7_0 ;
  wire \XLXI_4/XLXI_3/XLXN_5_0 ;
  wire \XLXI_3/XLXN_10 ;
  wire \XLXI_5/XLXI_4/XLXN_5_0 ;
  wire \XLXI_3/XLXN_3 ;
  wire \XLXI_2/XLXI_2/XLXN_5 ;
  wire \XLXI_2/XLXN_3 ;
  wire \XLXI_5/XLXN_10 ;
  wire \B<11>/INBUF ;
  wire \B<12>/INBUF ;
  wire \B<13>/INBUF ;
  wire \B<14>/INBUF ;
  wire \B<15>/INBUF ;
  wire \XLXI_4/XLXN_7 ;
  wire \XLXI_4/XLXN_3_pack_1 ;
  wire S_9_OBUF_33;
  wire \XLXI_4/XLXI_1/XLXN_5 ;
  wire \XLXI_3/XLXI_3/XLXN_5 ;
  wire S_7_OBUF_34;
  wire XLXN_1;
  wire \XLXI_2/XLXN_10_pack_1 ;
  wire S_14_OBUF_35;
  wire \XLXI_5/XLXI_2/XLXN_5 ;
  wire S_5_OBUF_36;
  wire \XLXI_3/XLXI_1/XLXN_5 ;
  wire \XLXI_4/XLXI_4/XLXN_5 ;
  wire S_12_OBUF_37;
  wire \XLXI_2/XLXI_3/XLXN_5 ;
  wire S_3_OBUF_38;
  wire \XLXI_5/XLXN_7 ;
  wire \XLXI_5/XLXN_3_pack_1 ;
  wire S_10_OBUF_39;
  wire \XLXI_4/XLXI_2/XLXN_5 ;
  wire XLXN_2;
  wire \XLXI_3/XLXN_10_pack_1 ;
  wire \XLXI_3/XLXI_4/XLXN_5 ;
  wire S_8_OBUF_40;
  wire \A<0>/INBUF ;
  wire \A<1>/INBUF ;
  wire \A<2>/INBUF ;
  wire \A<3>/INBUF ;
  wire \B<0>/INBUF ;
  wire \A<4>/INBUF ;
  wire \B<1>/INBUF ;
  wire \A<5>/INBUF ;
  wire \B<2>/INBUF ;
  wire \A<6>/INBUF ;
  wire \B<3>/INBUF ;
  wire \A<7>/INBUF ;
  wire \B<4>/INBUF ;
  wire \A<8>/INBUF ;
  wire \B<5>/INBUF ;
  wire \A<9>/INBUF ;
  wire \B<6>/INBUF ;
  wire \B<7>/INBUF ;
  wire \B<8>/INBUF ;
  wire \B<9>/INBUF ;
  wire \Ci/INBUF ;
  wire S_15_OBUF_41;
  wire \XLXI_5/XLXI_3/XLXN_5 ;
  wire S_6_OBUF_42;
  wire \XLXI_3/XLXI_2/XLXN_5 ;
  wire \XLXI_5/XLXI_1/XLXN_5 ;
  wire S_13_OBUF_43;
  wire \XLXI_2/XLXI_4/XLXN_5 ;
  wire S_4_OBUF_44;
  wire S_11_OBUF_45;
  wire \XLXI_4/XLXI_3/XLXN_5 ;
  wire S_1_OBUF_46;
  wire \XLXI_2/XLXI_2/XLXN_5_pack_1 ;
  wire XLXN_3;
  wire \XLXI_4/XLXN_10_pack_1 ;
  wire S_0_OBUF_47;
  wire \XLXI_2/XLXN_7 ;
  wire \XLXI_2/XLXN_3_pack_1 ;
  wire \XLXI_5/XLXI_4/XLXN_5 ;
  wire Co_OBUF_48;
  wire \XLXI_5/XLXN_10_pack_1 ;
  wire \XLXI_3/XLXN_7 ;
  wire \XLXI_3/XLXN_3_pack_1 ;
  wire S_2_OBUF_49;
  wire \A<10>/INBUF ;
  wire \A<11>/INBUF ;
  wire \A<12>/INBUF ;
  wire \A<13>/INBUF ;
  wire \A<14>/INBUF ;
  wire \A<15>/INBUF ;
  wire \B<10>/INBUF ;
  wire VCC;
  X_BUF B_11_IBUF (
    .I(B[11]),
    .O(\B<11>/INBUF )
  );
  X_BUF \B<11>/IFF/IMUX  (
    .I(\B<11>/INBUF ),
    .O(B_11_IBUF_28)
  );
  X_BUF B_12_IBUF (
    .I(B[12]),
    .O(\B<12>/INBUF )
  );
  X_BUF \B<12>/IFF/IMUX  (
    .I(\B<12>/INBUF ),
    .O(B_12_IBUF_29)
  );
  X_BUF B_13_IBUF (
    .I(B[13]),
    .O(\B<13>/INBUF )
  );
  X_BUF \B<13>/IFF/IMUX  (
    .I(\B<13>/INBUF ),
    .O(B_13_IBUF_30)
  );
  X_BUF B_14_IBUF (
    .I(B[14]),
    .O(\B<14>/INBUF )
  );
  X_BUF \B<14>/IFF/IMUX  (
    .I(\B<14>/INBUF ),
    .O(B_14_IBUF_31)
  );
  X_BUF B_15_IBUF (
    .I(B[15]),
    .O(\B<15>/INBUF )
  );
  X_BUF \B<15>/IFF/IMUX  (
    .I(\B<15>/INBUF ),
    .O(B_15_IBUF_32)
  );
  X_BUF \XLXI_4/XLXN_7/XUSED  (
    .I(\XLXI_4/XLXN_7 ),
    .O(\XLXI_4/XLXN_7_0 )
  );
  X_BUF \XLXI_4/XLXN_7/YUSED  (
    .I(\XLXI_4/XLXN_3_pack_1 ),
    .O(\XLXI_4/XLXN_3 )
  );
  X_LUT4MUX16 \XLXI_4/XLXI_1/XLXI_3  (
    .I0(1'b0),
    .I1(1'b0),
    .I2(1'b0),
    .I3(1'b1),
    .I4(1'b0),
    .I5(1'b1),
    .I6(1'b1),
    .I7(1'b1),
    .I8(1'b0),
    .I9(1'b0),
    .I10(1'b0),
    .I11(1'b1),
    .I12(1'b0),
    .I13(1'b1),
    .I14(1'b1),
    .I15(1'b1),
    .ADR0(B_8_IBUF_18),
    .ADR1(XLXN_2_0),
    .ADR2(A_8_IBUF_13),
    .ADR3(VCC),
    .O(\XLXI_4/XLXN_3_pack_1 )
  );
  X_BUF \S_9_OBUF/YUSED  (
    .I(\XLXI_4/XLXI_1/XLXN_5 ),
    .O(\XLXI_4/XLXI_1/XLXN_5_0 )
  );
  X_LUT4MUX16 \XLXI_4/XLXI_1/XLXI_1/XLXI_1  (
    .I0(1'b0),
    .I1(1'b0),
    .I2(1'b1),
    .I3(1'b1),
    .I4(1'b0),
    .I5(1'b0),
    .I6(1'b1),
    .I7(1'b1),
    .I8(1'b1),
    .I9(1'b1),
    .I10(1'b0),
    .I11(1'b0),
    .I12(1'b1),
    .I13(1'b1),
    .I14(1'b0),
    .I15(1'b0),
    .ADR0(VCC),
    .ADR1(A_8_IBUF_13),
    .ADR2(VCC),
    .ADR3(B_8_IBUF_18),
    .O(\XLXI_4/XLXI_1/XLXN_5 )
  );
  X_BUF \XLXI_3/XLXI_3/XLXN_5/XUSED  (
    .I(\XLXI_3/XLXI_3/XLXN_5 ),
    .O(\XLXI_3/XLXI_3/XLXN_5_0 )
  );
  X_LUT4MUX16 \XLXI_3/XLXI_4/XLXI_2/XLXI_1  (
    .I0(1'b0),
    .I1(1'b0),
    .I2(1'b0),
    .I3(1'b1),
    .I4(1'b0),
    .I5(1'b1),
    .I6(1'b1),
    .I7(1'b1),
    .I8(1'b1),
    .I9(1'b1),
    .I10(1'b1),
    .I11(1'b0),
    .I12(1'b1),
    .I13(1'b0),
    .I14(1'b0),
    .I15(1'b0),
    .ADR0(\XLXI_3/XLXN_7_0 ),
    .ADR1(A_6_IBUF_9),
    .ADR2(B_6_IBUF_16),
    .ADR3(\XLXI_3/XLXI_4/XLXN_5_0 ),
    .O(S_7_OBUF_34)
  );
  X_BUF \XLXN_1/XUSED  (
    .I(XLXN_1),
    .O(XLXN_1_0)
  );
  X_BUF \XLXN_1/YUSED  (
    .I(\XLXI_2/XLXN_10_pack_1 ),
    .O(\XLXI_2/XLXN_10 )
  );
  X_LUT4MUX16 \XLXI_2/XLXI_3/XLXI_3  (
    .I0(1'b0),
    .I1(1'b0),
    .I2(1'b0),
    .I3(1'b1),
    .I4(1'b0),
    .I5(1'b0),
    .I6(1'b0),
    .I7(1'b1),
    .I8(1'b0),
    .I9(1'b1),
    .I10(1'b1),
    .I11(1'b1),
    .I12(1'b0),
    .I13(1'b1),
    .I14(1'b1),
    .I15(1'b1),
    .ADR0(\XLXI_2/XLXN_7_0 ),
    .ADR1(A_2_IBUF_2),
    .ADR2(VCC),
    .ADR3(B_2_IBUF_8),
    .O(\XLXI_2/XLXN_10_pack_1 )
  );
  X_BUF \S_14_OBUF/YUSED  (
    .I(\XLXI_5/XLXI_2/XLXN_5 ),
    .O(\XLXI_5/XLXI_2/XLXN_5_0 )
  );
  X_LUT4MUX16 \XLXI_5/XLXI_2/XLXI_1/XLXI_1  (
    .I0(1'b0),
    .I1(1'b1),
    .I2(1'b0),
    .I3(1'b1),
    .I4(1'b1),
    .I5(1'b0),
    .I6(1'b1),
    .I7(1'b0),
    .I8(1'b0),
    .I9(1'b1),
    .I10(1'b0),
    .I11(1'b1),
    .I12(1'b1),
    .I13(1'b0),
    .I14(1'b1),
    .I15(1'b0),
    .ADR0(B_13_IBUF_30),
    .ADR1(VCC),
    .ADR2(A_13_IBUF_24),
    .ADR3(VCC),
    .O(\XLXI_5/XLXI_2/XLXN_5 )
  );
  X_BUF \S_5_OBUF/YUSED  (
    .I(\XLXI_3/XLXI_1/XLXN_5 ),
    .O(\XLXI_3/XLXI_1/XLXN_5_0 )
  );
  X_LUT4MUX16 \XLXI_3/XLXI_1/XLXI_1/XLXI_1  (
    .I0(1'b0),
    .I1(1'b0),
    .I2(1'b0),
    .I3(1'b0),
    .I4(1'b1),
    .I5(1'b1),
    .I6(1'b1),
    .I7(1'b1),
    .I8(1'b1),
    .I9(1'b1),
    .I10(1'b1),
    .I11(1'b1),
    .I12(1'b0),
    .I13(1'b0),
    .I14(1'b0),
    .I15(1'b0),
    .ADR0(VCC),
    .ADR1(VCC),
    .ADR2(B_4_IBUF_12),
    .ADR3(A_4_IBUF_5),
    .O(\XLXI_3/XLXI_1/XLXN_5 )
  );
  X_BUF \XLXI_4/XLXI_4/XLXN_5/XUSED  (
    .I(\XLXI_4/XLXI_4/XLXN_5 ),
    .O(\XLXI_4/XLXI_4/XLXN_5_0 )
  );
  X_LUT4MUX16 \XLXI_5/XLXI_1/XLXI_2/XLXI_1  (
    .I0(1'b0),
    .I1(1'b0),
    .I2(1'b1),
    .I3(1'b1),
    .I4(1'b0),
    .I5(1'b1),
    .I6(1'b1),
    .I7(1'b0),
    .I8(1'b0),
    .I9(1'b1),
    .I10(1'b1),
    .I11(1'b0),
    .I12(1'b1),
    .I13(1'b1),
    .I14(1'b0),
    .I15(1'b0),
    .ADR0(B_11_IBUF_28),
    .ADR1(\XLXI_5/XLXI_1/XLXN_5_0 ),
    .ADR2(\XLXI_4/XLXN_10 ),
    .ADR3(A_11_IBUF_22),
    .O(S_12_OBUF_37)
  );
  X_BUF \XLXI_2/XLXI_3/XLXN_5/XUSED  (
    .I(\XLXI_2/XLXI_3/XLXN_5 ),
    .O(\XLXI_2/XLXI_3/XLXN_5_0 )
  );
  X_LUT4MUX16 \XLXI_2/XLXI_4/XLXI_2/XLXI_1  (
    .I0(1'b0),
    .I1(1'b0),
    .I2(1'b1),
    .I3(1'b1),
    .I4(1'b0),
    .I5(1'b1),
    .I6(1'b1),
    .I7(1'b0),
    .I8(1'b0),
    .I9(1'b1),
    .I10(1'b1),
    .I11(1'b0),
    .I12(1'b1),
    .I13(1'b1),
    .I14(1'b0),
    .I15(1'b0),
    .ADR0(B_2_IBUF_8),
    .ADR1(\XLXI_2/XLXI_4/XLXN_5_0 ),
    .ADR2(A_2_IBUF_2),
    .ADR3(\XLXI_2/XLXN_7_0 ),
    .O(S_3_OBUF_38)
  );
  X_BUF \XLXI_5/XLXN_7/XUSED  (
    .I(\XLXI_5/XLXN_7 ),
    .O(\XLXI_5/XLXN_7_0 )
  );
  X_BUF \XLXI_5/XLXN_7/YUSED  (
    .I(\XLXI_5/XLXN_3_pack_1 ),
    .O(\XLXI_5/XLXN_3 )
  );
  X_LUT4MUX16 \XLXI_5/XLXI_1/XLXI_3  (
    .I0(1'b0),
    .I1(1'b0),
    .I2(1'b0),
    .I3(1'b0),
    .I4(1'b0),
    .I5(1'b0),
    .I6(1'b1),
    .I7(1'b1),
    .I8(1'b0),
    .I9(1'b0),
    .I10(1'b1),
    .I11(1'b1),
    .I12(1'b1),
    .I13(1'b1),
    .I14(1'b1),
    .I15(1'b1),
    .ADR0(VCC),
    .ADR1(B_12_IBUF_29),
    .ADR2(XLXN_3_0),
    .ADR3(A_12_IBUF_23),
    .O(\XLXI_5/XLXN_3_pack_1 )
  );
  X_BUF \S_10_OBUF/YUSED  (
    .I(\XLXI_4/XLXI_2/XLXN_5 ),
    .O(\XLXI_4/XLXI_2/XLXN_5_0 )
  );
  X_LUT4MUX16 \XLXI_4/XLXI_2/XLXI_1/XLXI_1  (
    .I0(1'b0),
    .I1(1'b0),
    .I2(1'b0),
    .I3(1'b0),
    .I4(1'b1),
    .I5(1'b1),
    .I6(1'b1),
    .I7(1'b1),
    .I8(1'b1),
    .I9(1'b1),
    .I10(1'b1),
    .I11(1'b1),
    .I12(1'b0),
    .I13(1'b0),
    .I14(1'b0),
    .I15(1'b0),
    .ADR0(VCC),
    .ADR1(VCC),
    .ADR2(A_9_IBUF_15),
    .ADR3(B_9_IBUF_19),
    .O(\XLXI_4/XLXI_2/XLXN_5 )
  );
  X_BUF \XLXN_2/XUSED  (
    .I(XLXN_2),
    .O(XLXN_2_0)
  );
  X_BUF \XLXN_2/YUSED  (
    .I(\XLXI_3/XLXN_10_pack_1 ),
    .O(\XLXI_3/XLXN_10 )
  );
  X_LUT4MUX16 \XLXI_3/XLXI_3/XLXI_3  (
    .I0(1'b0),
    .I1(1'b0),
    .I2(1'b0),
    .I3(1'b0),
    .I4(1'b0),
    .I5(1'b0),
    .I6(1'b1),
    .I7(1'b1),
    .I8(1'b0),
    .I9(1'b0),
    .I10(1'b1),
    .I11(1'b1),
    .I12(1'b1),
    .I13(1'b1),
    .I14(1'b1),
    .I15(1'b1),
    .ADR0(VCC),
    .ADR1(B_6_IBUF_16),
    .ADR2(A_6_IBUF_9),
    .ADR3(\XLXI_3/XLXN_7_0 ),
    .O(\XLXI_3/XLXN_10_pack_1 )
  );
  X_BUF \XLXI_3/XLXI_4/XLXN_5/XUSED  (
    .I(\XLXI_3/XLXI_4/XLXN_5 ),
    .O(\XLXI_3/XLXI_4/XLXN_5_0 )
  );
  X_BUF A_0_IBUF (
    .I(A[0]),
    .O(\A<0>/INBUF )
  );
  X_BUF \A<0>/IFF/IMUX  (
    .I(\A<0>/INBUF ),
    .O(A_0_IBUF_0)
  );
  X_BUF A_1_IBUF (
    .I(A[1]),
    .O(\A<1>/INBUF )
  );
  X_BUF \A<1>/IFF/IMUX  (
    .I(\A<1>/INBUF ),
    .O(A_1_IBUF_1)
  );
  X_BUF A_2_IBUF (
    .I(A[2]),
    .O(\A<2>/INBUF )
  );
  X_BUF \A<2>/IFF/IMUX  (
    .I(\A<2>/INBUF ),
    .O(A_2_IBUF_2)
  );
  X_BUF A_3_IBUF (
    .I(A[3]),
    .O(\A<3>/INBUF )
  );
  X_BUF \A<3>/IFF/IMUX  (
    .I(\A<3>/INBUF ),
    .O(A_3_IBUF_3)
  );
  X_BUF B_0_IBUF (
    .I(B[0]),
    .O(\B<0>/INBUF )
  );
  X_BUF \B<0>/IFF/IMUX  (
    .I(\B<0>/INBUF ),
    .O(B_0_IBUF_4)
  );
  X_BUF A_4_IBUF (
    .I(A[4]),
    .O(\A<4>/INBUF )
  );
  X_BUF \A<4>/IFF/IMUX  (
    .I(\A<4>/INBUF ),
    .O(A_4_IBUF_5)
  );
  X_BUF B_1_IBUF (
    .I(B[1]),
    .O(\B<1>/INBUF )
  );
  X_BUF \B<1>/IFF/IMUX  (
    .I(\B<1>/INBUF ),
    .O(B_1_IBUF_6)
  );
  X_BUF A_5_IBUF (
    .I(A[5]),
    .O(\A<5>/INBUF )
  );
  X_BUF \A<5>/IFF/IMUX  (
    .I(\A<5>/INBUF ),
    .O(A_5_IBUF_7)
  );
  X_BUF B_2_IBUF (
    .I(B[2]),
    .O(\B<2>/INBUF )
  );
  X_BUF \B<2>/IFF/IMUX  (
    .I(\B<2>/INBUF ),
    .O(B_2_IBUF_8)
  );
  X_BUF A_6_IBUF (
    .I(A[6]),
    .O(\A<6>/INBUF )
  );
  X_BUF \A<6>/IFF/IMUX  (
    .I(\A<6>/INBUF ),
    .O(A_6_IBUF_9)
  );
  X_BUF B_3_IBUF (
    .I(B[3]),
    .O(\B<3>/INBUF )
  );
  X_BUF \B<3>/IFF/IMUX  (
    .I(\B<3>/INBUF ),
    .O(B_3_IBUF_10)
  );
  X_BUF A_7_IBUF (
    .I(A[7]),
    .O(\A<7>/INBUF )
  );
  X_BUF \A<7>/IFF/IMUX  (
    .I(\A<7>/INBUF ),
    .O(A_7_IBUF_11)
  );
  X_BUF B_4_IBUF (
    .I(B[4]),
    .O(\B<4>/INBUF )
  );
  X_BUF \B<4>/IFF/IMUX  (
    .I(\B<4>/INBUF ),
    .O(B_4_IBUF_12)
  );
  X_BUF A_8_IBUF (
    .I(A[8]),
    .O(\A<8>/INBUF )
  );
  X_BUF \A<8>/IFF/IMUX  (
    .I(\A<8>/INBUF ),
    .O(A_8_IBUF_13)
  );
  X_BUF B_5_IBUF (
    .I(B[5]),
    .O(\B<5>/INBUF )
  );
  X_BUF \B<5>/IFF/IMUX  (
    .I(\B<5>/INBUF ),
    .O(B_5_IBUF_14)
  );
  X_BUF A_9_IBUF (
    .I(A[9]),
    .O(\A<9>/INBUF )
  );
  X_BUF \A<9>/IFF/IMUX  (
    .I(\A<9>/INBUF ),
    .O(A_9_IBUF_15)
  );
  X_BUF B_6_IBUF (
    .I(B[6]),
    .O(\B<6>/INBUF )
  );
  X_BUF \B<6>/IFF/IMUX  (
    .I(\B<6>/INBUF ),
    .O(B_6_IBUF_16)
  );
  X_BUF B_7_IBUF (
    .I(B[7]),
    .O(\B<7>/INBUF )
  );
  X_BUF \B<7>/IFF/IMUX  (
    .I(\B<7>/INBUF ),
    .O(B_7_IBUF_17)
  );
  X_BUF B_8_IBUF (
    .I(B[8]),
    .O(\B<8>/INBUF )
  );
  X_BUF \B<8>/IFF/IMUX  (
    .I(\B<8>/INBUF ),
    .O(B_8_IBUF_18)
  );
  X_BUF B_9_IBUF (
    .I(B[9]),
    .O(\B<9>/INBUF )
  );
  X_BUF \B<9>/IFF/IMUX  (
    .I(\B<9>/INBUF ),
    .O(B_9_IBUF_19)
  );
  X_BUF Ci_IBUF (
    .I(Ci),
    .O(\Ci/INBUF )
  );
  X_BUF \Ci/IFF/IMUX  (
    .I(\Ci/INBUF ),
    .O(Ci_IBUF_20)
  );
  X_BUF S_10_OBUF (
    .I(S_10_OBUF_39),
    .O(S[10])
  );
  X_LUT4MUX16 \XLXI_4/XLXI_1/XLXI_2/XLXI_1  (
    .I0(1'b0),
    .I1(1'b0),
    .I2(1'b0),
    .I3(1'b1),
    .I4(1'b1),
    .I5(1'b1),
    .I6(1'b1),
    .I7(1'b0),
    .I8(1'b0),
    .I9(1'b1),
    .I10(1'b1),
    .I11(1'b1),
    .I12(1'b1),
    .I13(1'b0),
    .I14(1'b0),
    .I15(1'b0),
    .ADR0(B_7_IBUF_17),
    .ADR1(\XLXI_3/XLXN_10 ),
    .ADR2(\XLXI_4/XLXI_1/XLXN_5_0 ),
    .ADR3(A_7_IBUF_11),
    .O(S_8_OBUF_40)
  );
  X_BUF \S_15_OBUF/YUSED  (
    .I(\XLXI_5/XLXI_3/XLXN_5 ),
    .O(\XLXI_5/XLXI_3/XLXN_5_0 )
  );
  X_LUT4MUX16 \XLXI_5/XLXI_3/XLXI_1/XLXI_1  (
    .I0(1'b0),
    .I1(1'b0),
    .I2(1'b1),
    .I3(1'b1),
    .I4(1'b1),
    .I5(1'b1),
    .I6(1'b0),
    .I7(1'b0),
    .I8(1'b0),
    .I9(1'b0),
    .I10(1'b1),
    .I11(1'b1),
    .I12(1'b1),
    .I13(1'b1),
    .I14(1'b0),
    .I15(1'b0),
    .ADR0(VCC),
    .ADR1(B_14_IBUF_31),
    .ADR2(A_14_IBUF_25),
    .ADR3(VCC),
    .O(\XLXI_5/XLXI_3/XLXN_5 )
  );
  X_BUF \S_6_OBUF/YUSED  (
    .I(\XLXI_3/XLXI_2/XLXN_5 ),
    .O(\XLXI_3/XLXI_2/XLXN_5_0 )
  );
  X_LUT4MUX16 \XLXI_3/XLXI_2/XLXI_1/XLXI_1  (
    .I0(1'b0),
    .I1(1'b0),
    .I2(1'b0),
    .I3(1'b0),
    .I4(1'b1),
    .I5(1'b1),
    .I6(1'b1),
    .I7(1'b1),
    .I8(1'b1),
    .I9(1'b1),
    .I10(1'b1),
    .I11(1'b1),
    .I12(1'b0),
    .I13(1'b0),
    .I14(1'b0),
    .I15(1'b0),
    .ADR0(VCC),
    .ADR1(VCC),
    .ADR2(A_5_IBUF_7),
    .ADR3(B_5_IBUF_14),
    .O(\XLXI_3/XLXI_2/XLXN_5 )
  );
  X_BUF \XLXI_5/XLXI_1/XLXN_5/XUSED  (
    .I(\XLXI_5/XLXI_1/XLXN_5 ),
    .O(\XLXI_5/XLXI_1/XLXN_5_0 )
  );
  X_LUT4MUX16 \XLXI_5/XLXI_2/XLXI_2/XLXI_1  (
    .I0(1'b0),
    .I1(1'b0),
    .I2(1'b1),
    .I3(1'b1),
    .I4(1'b0),
    .I5(1'b1),
    .I6(1'b1),
    .I7(1'b0),
    .I8(1'b0),
    .I9(1'b1),
    .I10(1'b1),
    .I11(1'b0),
    .I12(1'b1),
    .I13(1'b1),
    .I14(1'b0),
    .I15(1'b0),
    .ADR0(A_12_IBUF_23),
    .ADR1(\XLXI_5/XLXI_2/XLXN_5_0 ),
    .ADR2(XLXN_3_0),
    .ADR3(B_12_IBUF_29),
    .O(S_13_OBUF_43)
  );
  X_BUF \XLXI_2/XLXI_4/XLXN_5/XUSED  (
    .I(\XLXI_2/XLXI_4/XLXN_5 ),
    .O(\XLXI_2/XLXI_4/XLXN_5_0 )
  );
  X_LUT4MUX16 \XLXI_3/XLXI_1/XLXI_2/XLXI_1  (
    .I0(1'b0),
    .I1(1'b0),
    .I2(1'b0),
    .I3(1'b1),
    .I4(1'b0),
    .I5(1'b1),
    .I6(1'b1),
    .I7(1'b1),
    .I8(1'b1),
    .I9(1'b1),
    .I10(1'b1),
    .I11(1'b0),
    .I12(1'b1),
    .I13(1'b0),
    .I14(1'b0),
    .I15(1'b0),
    .ADR0(A_3_IBUF_3),
    .ADR1(\XLXI_2/XLXN_10 ),
    .ADR2(B_3_IBUF_10),
    .ADR3(\XLXI_3/XLXI_1/XLXN_5_0 ),
    .O(S_4_OBUF_44)
  );
  X_BUF \S_11_OBUF/YUSED  (
    .I(\XLXI_4/XLXI_3/XLXN_5 ),
    .O(\XLXI_4/XLXI_3/XLXN_5_0 )
  );
  X_LUT4MUX16 \XLXI_4/XLXI_3/XLXI_1/XLXI_1  (
    .I0(1'b0),
    .I1(1'b0),
    .I2(1'b0),
    .I3(1'b0),
    .I4(1'b1),
    .I5(1'b1),
    .I6(1'b1),
    .I7(1'b1),
    .I8(1'b1),
    .I9(1'b1),
    .I10(1'b1),
    .I11(1'b1),
    .I12(1'b0),
    .I13(1'b0),
    .I14(1'b0),
    .I15(1'b0),
    .ADR0(VCC),
    .ADR1(VCC),
    .ADR2(B_10_IBUF_27),
    .ADR3(A_10_IBUF_21),
    .O(\XLXI_4/XLXI_3/XLXN_5 )
  );
  X_BUF \S_1_OBUF/YUSED  (
    .I(\XLXI_2/XLXI_2/XLXN_5_pack_1 ),
    .O(\XLXI_2/XLXI_2/XLXN_5 )
  );
  X_LUT4MUX16 \XLXI_2/XLXI_2/XLXI_1/XLXI_1  (
    .I0(1'b0),
    .I1(1'b0),
    .I2(1'b1),
    .I3(1'b1),
    .I4(1'b0),
    .I5(1'b0),
    .I6(1'b1),
    .I7(1'b1),
    .I8(1'b1),
    .I9(1'b1),
    .I10(1'b0),
    .I11(1'b0),
    .I12(1'b1),
    .I13(1'b1),
    .I14(1'b0),
    .I15(1'b0),
    .ADR0(VCC),
    .ADR1(A_1_IBUF_1),
    .ADR2(VCC),
    .ADR3(B_1_IBUF_6),
    .O(\XLXI_2/XLXI_2/XLXN_5_pack_1 )
  );
  X_BUF \XLXN_3/XUSED  (
    .I(XLXN_3),
    .O(XLXN_3_0)
  );
  X_BUF \XLXN_3/YUSED  (
    .I(\XLXI_4/XLXN_10_pack_1 ),
    .O(\XLXI_4/XLXN_10 )
  );
  X_LUT4MUX16 \XLXI_4/XLXI_3/XLXI_3  (
    .I0(1'b0),
    .I1(1'b0),
    .I2(1'b0),
    .I3(1'b1),
    .I4(1'b0),
    .I5(1'b1),
    .I6(1'b1),
    .I7(1'b1),
    .I8(1'b0),
    .I9(1'b0),
    .I10(1'b0),
    .I11(1'b1),
    .I12(1'b0),
    .I13(1'b1),
    .I14(1'b1),
    .I15(1'b1),
    .ADR0(\XLXI_4/XLXN_7_0 ),
    .ADR1(A_10_IBUF_21),
    .ADR2(B_10_IBUF_27),
    .ADR3(VCC),
    .O(\XLXI_4/XLXN_10_pack_1 )
  );
  X_LUT4MUX16 \XLXI_2/XLXI_1/XLXI_2/XLXI_1  (
    .I0(1'b0),
    .I1(1'b1),
    .I2(1'b1),
    .I3(1'b0),
    .I4(1'b1),
    .I5(1'b0),
    .I6(1'b0),
    .I7(1'b1),
    .I8(1'b0),
    .I9(1'b1),
    .I10(1'b1),
    .I11(1'b0),
    .I12(1'b1),
    .I13(1'b0),
    .I14(1'b0),
    .I15(1'b1),
    .ADR0(A_0_IBUF_0),
    .ADR1(B_0_IBUF_4),
    .ADR2(Ci_IBUF_20),
    .ADR3(VCC),
    .O(S_0_OBUF_47)
  );
  X_BUF \XLXI_2/XLXN_7/XUSED  (
    .I(\XLXI_2/XLXN_7 ),
    .O(\XLXI_2/XLXN_7_0 )
  );
  X_BUF \XLXI_2/XLXN_7/YUSED  (
    .I(\XLXI_2/XLXN_3_pack_1 ),
    .O(\XLXI_2/XLXN_3 )
  );
  X_LUT4MUX16 \XLXI_2/XLXI_1/XLXI_3  (
    .I0(1'b0),
    .I1(1'b0),
    .I2(1'b0),
    .I3(1'b0),
    .I4(1'b0),
    .I5(1'b0),
    .I6(1'b1),
    .I7(1'b1),
    .I8(1'b0),
    .I9(1'b0),
    .I10(1'b1),
    .I11(1'b1),
    .I12(1'b1),
    .I13(1'b1),
    .I14(1'b1),
    .I15(1'b1),
    .ADR0(VCC),
    .ADR1(B_0_IBUF_4),
    .ADR2(A_0_IBUF_0),
    .ADR3(Ci_IBUF_20),
    .O(\XLXI_2/XLXN_3_pack_1 )
  );
  X_BUF \XLXI_5/XLXI_4/XLXN_5/XUSED  (
    .I(\XLXI_5/XLXI_4/XLXN_5 ),
    .O(\XLXI_5/XLXI_4/XLXN_5_0 )
  );
  X_LUT4MUX16 \XLXI_5/XLXI_4/XLXI_1/XLXI_1  (
    .I0(1'b0),
    .I1(1'b1),
    .I2(1'b0),
    .I3(1'b1),
    .I4(1'b1),
    .I5(1'b0),
    .I6(1'b1),
    .I7(1'b0),
    .I8(1'b0),
    .I9(1'b1),
    .I10(1'b0),
    .I11(1'b1),
    .I12(1'b1),
    .I13(1'b0),
    .I14(1'b1),
    .I15(1'b0),
    .ADR0(A_15_IBUF_26),
    .ADR1(VCC),
    .ADR2(B_15_IBUF_32),
    .ADR3(VCC),
    .O(\XLXI_5/XLXI_4/XLXN_5 )
  );
  X_BUF \Co_OBUF/YUSED  (
    .I(\XLXI_5/XLXN_10_pack_1 ),
    .O(\XLXI_5/XLXN_10 )
  );
  X_LUT4MUX16 \XLXI_5/XLXI_3/XLXI_3  (
    .I0(1'b0),
    .I1(1'b0),
    .I2(1'b0),
    .I3(1'b0),
    .I4(1'b0),
    .I5(1'b0),
    .I6(1'b1),
    .I7(1'b1),
    .I8(1'b0),
    .I9(1'b0),
    .I10(1'b1),
    .I11(1'b1),
    .I12(1'b1),
    .I13(1'b1),
    .I14(1'b1),
    .I15(1'b1),
    .ADR0(VCC),
    .ADR1(A_14_IBUF_25),
    .ADR2(B_14_IBUF_31),
    .ADR3(\XLXI_5/XLXN_7_0 ),
    .O(\XLXI_5/XLXN_10_pack_1 )
  );
  X_BUF \XLXI_3/XLXN_7/XUSED  (
    .I(\XLXI_3/XLXN_7 ),
    .O(\XLXI_3/XLXN_7_0 )
  );
  X_BUF \XLXI_3/XLXN_7/YUSED  (
    .I(\XLXI_3/XLXN_3_pack_1 ),
    .O(\XLXI_3/XLXN_3 )
  );
  X_LUT4MUX16 \XLXI_3/XLXI_1/XLXI_3  (
    .I0(1'b0),
    .I1(1'b0),
    .I2(1'b0),
    .I3(1'b1),
    .I4(1'b0),
    .I5(1'b1),
    .I6(1'b1),
    .I7(1'b1),
    .I8(1'b0),
    .I9(1'b0),
    .I10(1'b0),
    .I11(1'b1),
    .I12(1'b0),
    .I13(1'b1),
    .I14(1'b1),
    .I15(1'b1),
    .ADR0(A_4_IBUF_5),
    .ADR1(B_4_IBUF_12),
    .ADR2(XLXN_1_0),
    .ADR3(VCC),
    .O(\XLXI_3/XLXN_3_pack_1 )
  );
  X_LUT4MUX16 \XLXI_2/XLXI_3/XLXI_2/XLXI_1  (
    .I0(1'b0),
    .I1(1'b0),
    .I2(1'b0),
    .I3(1'b1),
    .I4(1'b0),
    .I5(1'b1),
    .I6(1'b1),
    .I7(1'b1),
    .I8(1'b1),
    .I9(1'b1),
    .I10(1'b1),
    .I11(1'b0),
    .I12(1'b1),
    .I13(1'b0),
    .I14(1'b0),
    .I15(1'b0),
    .ADR0(B_1_IBUF_6),
    .ADR1(A_1_IBUF_1),
    .ADR2(\XLXI_2/XLXN_3 ),
    .ADR3(\XLXI_2/XLXI_3/XLXN_5_0 ),
    .O(S_2_OBUF_49)
  );
  X_BUF S_11_OBUF (
    .I(S_11_OBUF_45),
    .O(S[11])
  );
  X_BUF Co_OBUF (
    .I(Co_OBUF_48),
    .O(Co)
  );
  X_BUF S_12_OBUF (
    .I(S_12_OBUF_37),
    .O(S[12])
  );
  X_BUF S_13_OBUF (
    .I(S_13_OBUF_43),
    .O(S[13])
  );
  X_BUF S_14_OBUF (
    .I(S_14_OBUF_35),
    .O(S[14])
  );
  X_BUF S_15_OBUF (
    .I(S_15_OBUF_41),
    .O(S[15])
  );
  X_BUF S_0_OBUF (
    .I(S_0_OBUF_47),
    .O(S[0])
  );
  X_BUF S_1_OBUF (
    .I(S_1_OBUF_46),
    .O(S[1])
  );
  X_BUF S_2_OBUF (
    .I(S_2_OBUF_49),
    .O(S[2])
  );
  X_BUF A_10_IBUF (
    .I(A[10]),
    .O(\A<10>/INBUF )
  );
  X_BUF \A<10>/IFF/IMUX  (
    .I(\A<10>/INBUF ),
    .O(A_10_IBUF_21)
  );
  X_BUF S_3_OBUF (
    .I(S_3_OBUF_38),
    .O(S[3])
  );
  X_BUF A_11_IBUF (
    .I(A[11]),
    .O(\A<11>/INBUF )
  );
  X_BUF \A<11>/IFF/IMUX  (
    .I(\A<11>/INBUF ),
    .O(A_11_IBUF_22)
  );
  X_BUF S_4_OBUF (
    .I(S_4_OBUF_44),
    .O(S[4])
  );
  X_BUF A_12_IBUF (
    .I(A[12]),
    .O(\A<12>/INBUF )
  );
  X_BUF \A<12>/IFF/IMUX  (
    .I(\A<12>/INBUF ),
    .O(A_12_IBUF_23)
  );
  X_BUF S_5_OBUF (
    .I(S_5_OBUF_36),
    .O(S[5])
  );
  X_BUF A_13_IBUF (
    .I(A[13]),
    .O(\A<13>/INBUF )
  );
  X_BUF \A<13>/IFF/IMUX  (
    .I(\A<13>/INBUF ),
    .O(A_13_IBUF_24)
  );
  X_BUF S_6_OBUF (
    .I(S_6_OBUF_42),
    .O(S[6])
  );
  X_BUF A_14_IBUF (
    .I(A[14]),
    .O(\A<14>/INBUF )
  );
  X_BUF \A<14>/IFF/IMUX  (
    .I(\A<14>/INBUF ),
    .O(A_14_IBUF_25)
  );
  X_BUF S_7_OBUF (
    .I(S_7_OBUF_34),
    .O(S[7])
  );
  X_BUF A_15_IBUF (
    .I(A[15]),
    .O(\A<15>/INBUF )
  );
  X_BUF \A<15>/IFF/IMUX  (
    .I(\A<15>/INBUF ),
    .O(A_15_IBUF_26)
  );
  X_BUF S_8_OBUF (
    .I(S_8_OBUF_40),
    .O(S[8])
  );
  X_BUF S_9_OBUF (
    .I(S_9_OBUF_33),
    .O(S[9])
  );
  X_BUF \B<10>/IFF/IMUX  (
    .I(\B<10>/INBUF ),
    .O(B_10_IBUF_27)
  );
  X_BUF B_10_IBUF (
    .I(B[10]),
    .O(\B<10>/INBUF )
  );
  X_LUT4MUX16 \XLXI_3/XLXI_3/XLXI_2/XLXI_1  (
    .I0(1'b0),
    .I1(1'b0),
    .I2(1'b1),
    .I3(1'b1),
    .I4(1'b0),
    .I5(1'b1),
    .I6(1'b1),
    .I7(1'b0),
    .I8(1'b0),
    .I9(1'b1),
    .I10(1'b1),
    .I11(1'b0),
    .I12(1'b1),
    .I13(1'b1),
    .I14(1'b0),
    .I15(1'b0),
    .ADR0(\XLXI_3/XLXN_3 ),
    .ADR1(\XLXI_3/XLXI_3/XLXN_5_0 ),
    .ADR2(A_5_IBUF_7),
    .ADR3(B_5_IBUF_14),
    .O(S_6_OBUF_42)
  );
  X_LUT4MUX16 \XLXI_5/XLXI_1/XLXI_1/XLXI_1  (
    .I0(1'b0),
    .I1(1'b0),
    .I2(1'b1),
    .I3(1'b1),
    .I4(1'b0),
    .I5(1'b0),
    .I6(1'b1),
    .I7(1'b1),
    .I8(1'b1),
    .I9(1'b1),
    .I10(1'b0),
    .I11(1'b0),
    .I12(1'b1),
    .I13(1'b1),
    .I14(1'b0),
    .I15(1'b0),
    .ADR0(VCC),
    .ADR1(B_12_IBUF_29),
    .ADR2(VCC),
    .ADR3(A_12_IBUF_23),
    .O(\XLXI_5/XLXI_1/XLXN_5 )
  );
  X_LUT4MUX16 \XLXI_2/XLXI_4/XLXI_1/XLXI_1  (
    .I0(1'b0),
    .I1(1'b1),
    .I2(1'b0),
    .I3(1'b1),
    .I4(1'b1),
    .I5(1'b0),
    .I6(1'b1),
    .I7(1'b0),
    .I8(1'b0),
    .I9(1'b1),
    .I10(1'b0),
    .I11(1'b1),
    .I12(1'b1),
    .I13(1'b0),
    .I14(1'b1),
    .I15(1'b0),
    .ADR0(A_3_IBUF_3),
    .ADR1(VCC),
    .ADR2(B_3_IBUF_10),
    .ADR3(VCC),
    .O(\XLXI_2/XLXI_4/XLXN_5 )
  );
  X_LUT4MUX16 \XLXI_4/XLXI_4/XLXI_2/XLXI_1  (
    .I0(1'b0),
    .I1(1'b1),
    .I2(1'b0),
    .I3(1'b1),
    .I4(1'b0),
    .I5(1'b1),
    .I6(1'b1),
    .I7(1'b0),
    .I8(1'b0),
    .I9(1'b1),
    .I10(1'b1),
    .I11(1'b0),
    .I12(1'b1),
    .I13(1'b0),
    .I14(1'b1),
    .I15(1'b0),
    .ADR0(\XLXI_4/XLXI_4/XLXN_5_0 ),
    .ADR1(\XLXI_4/XLXN_7_0 ),
    .ADR2(B_10_IBUF_27),
    .ADR3(A_10_IBUF_21),
    .O(S_11_OBUF_45)
  );
  X_LUT4MUX16 \XLXI_2/XLXI_2/XLXI_2/XLXI_1  (
    .I0(1'b0),
    .I1(1'b0),
    .I2(1'b0),
    .I3(1'b1),
    .I4(1'b0),
    .I5(1'b1),
    .I6(1'b1),
    .I7(1'b1),
    .I8(1'b1),
    .I9(1'b1),
    .I10(1'b1),
    .I11(1'b0),
    .I12(1'b1),
    .I13(1'b0),
    .I14(1'b0),
    .I15(1'b0),
    .ADR0(Ci_IBUF_20),
    .ADR1(A_0_IBUF_0),
    .ADR2(B_0_IBUF_4),
    .ADR3(\XLXI_2/XLXI_2/XLXN_5 ),
    .O(S_1_OBUF_46)
  );
  X_LUT4MUX16 \XLXI_4/XLXI_4/XLXI_3  (
    .I0(1'b0),
    .I1(1'b0),
    .I2(1'b0),
    .I3(1'b0),
    .I4(1'b0),
    .I5(1'b1),
    .I6(1'b0),
    .I7(1'b1),
    .I8(1'b0),
    .I9(1'b1),
    .I10(1'b0),
    .I11(1'b1),
    .I12(1'b1),
    .I13(1'b1),
    .I14(1'b1),
    .I15(1'b1),
    .ADR0(B_11_IBUF_28),
    .ADR1(VCC),
    .ADR2(\XLXI_4/XLXN_10 ),
    .ADR3(A_11_IBUF_22),
    .O(XLXN_3)
  );
  X_LUT4MUX16 \XLXI_2/XLXI_2/XLXI_3  (
    .I0(1'b0),
    .I1(1'b0),
    .I2(1'b0),
    .I3(1'b0),
    .I4(1'b0),
    .I5(1'b1),
    .I6(1'b0),
    .I7(1'b1),
    .I8(1'b0),
    .I9(1'b1),
    .I10(1'b0),
    .I11(1'b1),
    .I12(1'b1),
    .I13(1'b1),
    .I14(1'b1),
    .I15(1'b1),
    .ADR0(B_1_IBUF_6),
    .ADR1(VCC),
    .ADR2(A_1_IBUF_1),
    .ADR3(\XLXI_2/XLXN_3 ),
    .O(\XLXI_2/XLXN_7 )
  );
  X_LUT4MUX16 \XLXI_5/XLXI_4/XLXI_3  (
    .I0(1'b0),
    .I1(1'b0),
    .I2(1'b0),
    .I3(1'b0),
    .I4(1'b0),
    .I5(1'b1),
    .I6(1'b0),
    .I7(1'b1),
    .I8(1'b0),
    .I9(1'b1),
    .I10(1'b0),
    .I11(1'b1),
    .I12(1'b1),
    .I13(1'b1),
    .I14(1'b1),
    .I15(1'b1),
    .ADR0(A_15_IBUF_26),
    .ADR1(VCC),
    .ADR2(B_15_IBUF_32),
    .ADR3(\XLXI_5/XLXN_10 ),
    .O(Co_OBUF_48)
  );
  X_LUT4MUX16 \XLXI_5/XLXI_3/XLXI_2/XLXI_1  (
    .I0(1'b0),
    .I1(1'b0),
    .I2(1'b0),
    .I3(1'b1),
    .I4(1'b0),
    .I5(1'b1),
    .I6(1'b1),
    .I7(1'b1),
    .I8(1'b1),
    .I9(1'b1),
    .I10(1'b1),
    .I11(1'b0),
    .I12(1'b1),
    .I13(1'b0),
    .I14(1'b0),
    .I15(1'b0),
    .ADR0(B_13_IBUF_30),
    .ADR1(\XLXI_5/XLXN_3 ),
    .ADR2(A_13_IBUF_24),
    .ADR3(\XLXI_5/XLXI_3/XLXN_5_0 ),
    .O(S_14_OBUF_35)
  );
  X_LUT4MUX16 \XLXI_3/XLXI_2/XLXI_2/XLXI_1  (
    .I0(1'b0),
    .I1(1'b0),
    .I2(1'b1),
    .I3(1'b1),
    .I4(1'b0),
    .I5(1'b1),
    .I6(1'b1),
    .I7(1'b0),
    .I8(1'b0),
    .I9(1'b1),
    .I10(1'b1),
    .I11(1'b0),
    .I12(1'b1),
    .I13(1'b1),
    .I14(1'b0),
    .I15(1'b0),
    .ADR0(XLXN_1_0),
    .ADR1(\XLXI_3/XLXI_2/XLXN_5_0 ),
    .ADR2(B_4_IBUF_12),
    .ADR3(A_4_IBUF_5),
    .O(S_5_OBUF_36)
  );
  X_LUT4MUX16 \XLXI_4/XLXI_4/XLXI_1/XLXI_1  (
    .I0(1'b0),
    .I1(1'b1),
    .I2(1'b0),
    .I3(1'b1),
    .I4(1'b0),
    .I5(1'b1),
    .I6(1'b0),
    .I7(1'b1),
    .I8(1'b1),
    .I9(1'b0),
    .I10(1'b1),
    .I11(1'b0),
    .I12(1'b1),
    .I13(1'b0),
    .I14(1'b1),
    .I15(1'b0),
    .ADR0(B_11_IBUF_28),
    .ADR1(VCC),
    .ADR2(VCC),
    .ADR3(A_11_IBUF_22),
    .O(\XLXI_4/XLXI_4/XLXN_5 )
  );
  X_LUT4MUX16 \XLXI_2/XLXI_3/XLXI_1/XLXI_1  (
    .I0(1'b0),
    .I1(1'b1),
    .I2(1'b0),
    .I3(1'b1),
    .I4(1'b1),
    .I5(1'b0),
    .I6(1'b1),
    .I7(1'b0),
    .I8(1'b0),
    .I9(1'b1),
    .I10(1'b0),
    .I11(1'b1),
    .I12(1'b1),
    .I13(1'b0),
    .I14(1'b1),
    .I15(1'b0),
    .ADR0(B_2_IBUF_8),
    .ADR1(VCC),
    .ADR2(A_2_IBUF_2),
    .ADR3(VCC),
    .O(\XLXI_2/XLXI_3/XLXN_5 )
  );
  X_LUT4MUX16 \XLXI_5/XLXI_2/XLXI_3  (
    .I0(1'b0),
    .I1(1'b0),
    .I2(1'b0),
    .I3(1'b0),
    .I4(1'b0),
    .I5(1'b0),
    .I6(1'b1),
    .I7(1'b1),
    .I8(1'b0),
    .I9(1'b0),
    .I10(1'b1),
    .I11(1'b1),
    .I12(1'b1),
    .I13(1'b1),
    .I14(1'b1),
    .I15(1'b1),
    .ADR0(VCC),
    .ADR1(A_13_IBUF_24),
    .ADR2(\XLXI_5/XLXN_3 ),
    .ADR3(B_13_IBUF_30),
    .O(\XLXI_5/XLXN_7 )
  );
  X_LUT4MUX16 \XLXI_4/XLXI_3/XLXI_2/XLXI_1  (
    .I0(1'b0),
    .I1(1'b1),
    .I2(1'b0),
    .I3(1'b1),
    .I4(1'b0),
    .I5(1'b1),
    .I6(1'b1),
    .I7(1'b0),
    .I8(1'b0),
    .I9(1'b1),
    .I10(1'b1),
    .I11(1'b0),
    .I12(1'b1),
    .I13(1'b0),
    .I14(1'b1),
    .I15(1'b0),
    .ADR0(\XLXI_4/XLXI_3/XLXN_5_0 ),
    .ADR1(\XLXI_4/XLXN_3 ),
    .ADR2(A_9_IBUF_15),
    .ADR3(B_9_IBUF_19),
    .O(S_10_OBUF_39)
  );
  X_LUT4MUX16 \XLXI_3/XLXI_4/XLXI_3  (
    .I0(1'b0),
    .I1(1'b0),
    .I2(1'b0),
    .I3(1'b0),
    .I4(1'b0),
    .I5(1'b1),
    .I6(1'b0),
    .I7(1'b1),
    .I8(1'b0),
    .I9(1'b1),
    .I10(1'b0),
    .I11(1'b1),
    .I12(1'b1),
    .I13(1'b1),
    .I14(1'b1),
    .I15(1'b1),
    .ADR0(A_7_IBUF_11),
    .ADR1(VCC),
    .ADR2(\XLXI_3/XLXN_10 ),
    .ADR3(B_7_IBUF_17),
    .O(XLXN_2)
  );
  X_LUT4MUX16 \XLXI_3/XLXI_4/XLXI_1/XLXI_1  (
    .I0(1'b0),
    .I1(1'b0),
    .I2(1'b1),
    .I3(1'b1),
    .I4(1'b0),
    .I5(1'b0),
    .I6(1'b1),
    .I7(1'b1),
    .I8(1'b1),
    .I9(1'b1),
    .I10(1'b0),
    .I11(1'b0),
    .I12(1'b1),
    .I13(1'b1),
    .I14(1'b0),
    .I15(1'b0),
    .ADR0(VCC),
    .ADR1(B_7_IBUF_17),
    .ADR2(VCC),
    .ADR3(A_7_IBUF_11),
    .O(\XLXI_3/XLXI_4/XLXN_5 )
  );
  X_LUT4MUX16 \XLXI_5/XLXI_4/XLXI_2/XLXI_1  (
    .I0(1'b0),
    .I1(1'b0),
    .I2(1'b0),
    .I3(1'b1),
    .I4(1'b1),
    .I5(1'b1),
    .I6(1'b1),
    .I7(1'b0),
    .I8(1'b0),
    .I9(1'b1),
    .I10(1'b1),
    .I11(1'b1),
    .I12(1'b1),
    .I13(1'b0),
    .I14(1'b0),
    .I15(1'b0),
    .ADR0(\XLXI_5/XLXN_7_0 ),
    .ADR1(B_14_IBUF_31),
    .ADR2(\XLXI_5/XLXI_4/XLXN_5_0 ),
    .ADR3(A_14_IBUF_25),
    .O(S_15_OBUF_41)
  );
  X_LUT4MUX16 \XLXI_4/XLXI_2/XLXI_3  (
    .I0(1'b0),
    .I1(1'b0),
    .I2(1'b0),
    .I3(1'b1),
    .I4(1'b0),
    .I5(1'b1),
    .I6(1'b1),
    .I7(1'b1),
    .I8(1'b0),
    .I9(1'b0),
    .I10(1'b0),
    .I11(1'b1),
    .I12(1'b0),
    .I13(1'b1),
    .I14(1'b1),
    .I15(1'b1),
    .ADR0(B_9_IBUF_19),
    .ADR1(A_9_IBUF_15),
    .ADR2(\XLXI_4/XLXN_3 ),
    .ADR3(VCC),
    .O(\XLXI_4/XLXN_7 )
  );
  X_LUT4MUX16 \XLXI_4/XLXI_2/XLXI_2/XLXI_1  (
    .I0(1'b0),
    .I1(1'b1),
    .I2(1'b0),
    .I3(1'b1),
    .I4(1'b0),
    .I5(1'b1),
    .I6(1'b1),
    .I7(1'b0),
    .I8(1'b0),
    .I9(1'b1),
    .I10(1'b1),
    .I11(1'b0),
    .I12(1'b1),
    .I13(1'b0),
    .I14(1'b1),
    .I15(1'b0),
    .ADR0(\XLXI_4/XLXI_2/XLXN_5_0 ),
    .ADR1(A_8_IBUF_13),
    .ADR2(XLXN_2_0),
    .ADR3(B_8_IBUF_18),
    .O(S_9_OBUF_33)
  );
  X_LUT4MUX16 \XLXI_3/XLXI_3/XLXI_1/XLXI_1  (
    .I0(1'b0),
    .I1(1'b0),
    .I2(1'b1),
    .I3(1'b1),
    .I4(1'b1),
    .I5(1'b1),
    .I6(1'b0),
    .I7(1'b0),
    .I8(1'b0),
    .I9(1'b0),
    .I10(1'b1),
    .I11(1'b1),
    .I12(1'b1),
    .I13(1'b1),
    .I14(1'b0),
    .I15(1'b0),
    .ADR0(VCC),
    .ADR1(A_6_IBUF_9),
    .ADR2(B_6_IBUF_16),
    .ADR3(VCC),
    .O(\XLXI_3/XLXI_3/XLXN_5 )
  );
  X_LUT4MUX16 \XLXI_2/XLXI_4/XLXI_3  (
    .I0(1'b0),
    .I1(1'b0),
    .I2(1'b0),
    .I3(1'b1),
    .I4(1'b0),
    .I5(1'b0),
    .I6(1'b0),
    .I7(1'b1),
    .I8(1'b0),
    .I9(1'b1),
    .I10(1'b1),
    .I11(1'b1),
    .I12(1'b0),
    .I13(1'b1),
    .I14(1'b1),
    .I15(1'b1),
    .ADR0(A_3_IBUF_3),
    .ADR1(B_3_IBUF_10),
    .ADR2(VCC),
    .ADR3(\XLXI_2/XLXN_10 ),
    .O(XLXN_1)
  );
  X_LUT4MUX16 \XLXI_3/XLXI_2/XLXI_3  (
    .I0(1'b0),
    .I1(1'b0),
    .I2(1'b0),
    .I3(1'b0),
    .I4(1'b0),
    .I5(1'b1),
    .I6(1'b0),
    .I7(1'b1),
    .I8(1'b0),
    .I9(1'b1),
    .I10(1'b0),
    .I11(1'b1),
    .I12(1'b1),
    .I13(1'b1),
    .I14(1'b1),
    .I15(1'b1),
    .ADR0(\XLXI_3/XLXN_3 ),
    .ADR1(VCC),
    .ADR2(A_5_IBUF_7),
    .ADR3(B_5_IBUF_14),
    .O(\XLXI_3/XLXN_7 )
  );
  X_ONE NlwBlock_sixteenbit_adder_VCC (
    .O(VCC)
  );
endmodule

