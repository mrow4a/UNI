####################################################################
# Copyright (c) 2008 Piconomic Design [www.piconomic.co.za]
# All rights reserved.
#
# GDB Script to initialise board to a known state for debugging
#
# If this script file is named ".gdbinit" it will be executed 
# automatically when GDB is started.
#
# Useful GDB commands
#
# source script.gdb : read and execute GDB commands from file "script.gdb"
# c                 : continue running the program
# s                 : step
# n                 : next
# break main        : set a breakpoint function "main"
# thbreak main      : set a temporary hardware assisted breakpoint at function "main"
# info registers    : display CPU register values
# info mem          : display memory regions as defined by OpenOCD
# display/i $pc     : display current assembly instruction
#
# Revision Info     :  $Id: program_and_debug.gdb 1 2008-06-30 08:06:38Z Pieter.Conradie $
#
####################################################################
set confirm off
def run
load
c
end
set confirm on

# Set up the GDB debugging environment
#set output-radix 16
#set input-radix 16

# Change timeout to 10 seconds
set remotetimeout 10

# Specify the TCP/IP port OpenOCD GDB Server is using to listen for gdb commands
target remote localhost:3333

# Reset the target
#monitor reset

# Program flash and verify
#load
#compare-sections

# Force HW breakpoints when GDB request SW breakpoints
#monitor arm7_9 force_hw_bkpts enable

# SDRAM initialization script for the AT91SAM9263
#------------------------------------------------
# Configure master clock
echo Configuring the master clock...\n
# Enable main oscillator
#set *0xFFFFFC20 = 0x00004001
#while ((*0xFFFFFC68 & 0x1) == 0)
#end

# Set PLLA to 200MHz
#set *0xFFFFFC28 = 0x206DBF09
#while ((*0xFFFFFC68 & 0x2) == 0)
#end

# Select prescaler
#set *0xFFFFFC30 = 0x00000100
#while ((*0xFFFFFC68 & 0x8) == 0)
#end

# Select master clock
#set *0xFFFFFC30 = 0x00000102
#while ((*0xFFFFFC68 & 0x8) == 0)
#end

#echo Master clock ok.\n
#echo Configuring the SDRAM controller...\n

# Enable EBI chip select for the SDRAM
set *0xFFFFED20 = 0x2

# SDRAM configuration
set *0xFFFFE208 = 0x852272D9

set *0xFFFFE200 = 0x1
set *0x20000000 = 0

set *0xFFFFE200 = 0x2
set *0x20000000 = 0

set *0xFFFFE200 = 0x4
set *0x20000000 = 0
set *0xFFFFE200 = 0x4
set *0x20000000 = 0
set *0xFFFFE200 = 0x4
set *0x20000000 = 0
set *0xFFFFE200 = 0x4
set *0x20000000 = 0
set *0xFFFFE200 = 0x4
set *0x20000000 = 0
set *0xFFFFE200 = 0x4
set *0x20000000 = 0
set *0xFFFFE200 = 0x4
set *0x20000000 = 0
set *0xFFFFE200 = 0x4
set *0x20000000 = 0

set *0xFFFFE200 = 0x3
set *0x20000000 = 0

set *0xFFFFE200 = 0x0
set *0x20000000 = 0

set *0xFFFFE204 = 0x2B7

#echo SDRAM configuration ok.\n

#echo Configuring ARM internal SRAM for instruction and data\n
set *0xffffed00 = 0x3
#echo SRAM memory configured\n

#load

# Set a temporary breakpoint at "main()"
# break main

# Run to breakpoint
#continue


