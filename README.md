comp_arch2_cache
================
ReadMe Mem_Sim PE313

A cache and memory structure + debug functions

Cache allows for unaligned and variable length load/stores

Debug can print cache/memory state or foce values into cache

Allows for invalid/malformed inputs or invalid addresses
	gives errors messages but continues reading input

Allows for capitalised instructions

Exits on end-of-file or ctrl-z in console

Frees all dynamiccaly allocated memory

Instructions::
	read-req	reads a byte from the given byte address
	write-req	stores a word in the given byte address 	- if input is too short = undefined behaviour
								- if input is too long = truncated storage
	flush-req	flushes all dirty data into memory
	debug-req	prints all instructions and cache states since last debug request - limited to 20 instructions
