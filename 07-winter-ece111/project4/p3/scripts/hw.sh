#!/bin/sh -ev
########################################################################
##                    Px Generated File                               ##
##              Platform Express, Version 3.3.1 (BuildId: 200607071143)##
##              System Level Engineering Division                     ##
##              Mentor Graphics Corporation                           ##
##                                                                    ##
## Generated on: February 18, 2007 7:19:01 PM PST                     ##
## Generated by: saumya                                               ##
## Modelsim logic simulator invocation script                         ##
########################################################################

#if [ -f ../pxenv.sh ] ; then . ../pxenv.sh; fi
 vmap work $hom/work
 vsim -i   -t 1ps -l transcript -do $hom/scripts/wave.do  -pli " ${CVE_HOME}/lib/arm946e_sr1_vsim_vlog.slib "\
 	$hom/work.pxdefault_tb
