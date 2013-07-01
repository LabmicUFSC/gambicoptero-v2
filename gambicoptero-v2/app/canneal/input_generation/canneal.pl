#!/usr/bin/perl

# Copyright 2009, Princeton University
# All rights reserved.
#
# A simple script to generate inputs for the canneal workload of the PARSEC
# Benchmark Suite.

# Modified to print the output in two files: canneal_input.txt and canneal_epos_input.txt
# canneal_input.txt -> original output format
# canneal_epos_input.txt -> output format that EPOS uses as input
#copy the output lines to $EPOS/app/canneal/include/input.h

use strict;
my @names;

my $x = shift;
my $y = shift;
my $num_elements = shift;
($x > 1) or die "x is invalid: $y";
($y >1) or die "y is invalid: $y";
($num_elements < ($x * $y) )or die;
my $num_connections = 5;

open (MYFILE, '>canneal_input.txt');
open (EPOS, '>canneal_epos_input.txt');

print EPOS "const int input_num_elements = $num_elements;\n";
print EPOS "const int input_max_x = $x;\n";
print EPOS "const int input_max_y = $y;\n\n";
print EPOS "const char input_array[input_num_elements][Y_SIZE] = \n{ \n";
print MYFILE "$num_elements	$x	$y\n";

#create a set of names.  Use the ++ operator gives meaningless names, but thats 
#all I really need
my $name = "a";
foreach my $i (0..$num_elements-1){
	$names[$i] = $name;
	$name++;
}

foreach my $i (0..$num_elements-1){
	print MYFILE "$names[$i]\t";
	print EPOS '"';
	print EPOS "$names[$i] ";
	#type is either reg or comb  For now my program makes no distinction
	my $type = 1+ int(rand(2));
	print MYFILE "$type\t";
	print EPOS "$type ";
	foreach my $j (0..$num_connections-1){
		#get a random element
		my $random_connection = int(rand($num_elements));
		print MYFILE $names[$random_connection];
		print EPOS $names[$random_connection];
		print MYFILE "\t";
		print EPOS " "
	}
	print MYFILE "END\n";
	print EPOS "END";
	print EPOS '",';
	print EPOS "\n";
}
print EPOS "};\n"
