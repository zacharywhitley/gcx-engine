#!/usr/bin/perl

my $in=$ARGV[0];
open (IN,"<$in");
my @f=<IN>;
close (IN);


open (OUT,">$in.fixed");
for (my $i=0;$i<=$#f;$i++) {
	my $line=$f[$i];
	$line=~s/GCX\/license\.html/GCX\/license.php/;
	print OUT $line;
}
close (OUT);
