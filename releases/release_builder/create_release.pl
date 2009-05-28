#!/usr/bin/perl

use strict;

# Read version number to create
my $version_number=$ARGV[0];

my $SRCDIR="../../src";

if ($version_number!~/[0-9]-[0-9]+/) {
    print STDOUT "Version number '$version_number' invalid!\n";
    print STDOUT "-> use '1-03', for instance\n";
    exit(1);
}

print STDOUT "Before proceeding, make sure that\n";
print STDOUT "   (a) the subdirectory './binaries' contains /up-to-date/ precompiled versions\n";
print STDOUT "       gcx_linux.bin, gcx_macos.bin, gcx_win32.exe for all three operating systems\n";
print STDOUT "   (b) the subdirectory './makefiles' contains Makefile.Linux and Makefile.Windows\n";
print STOUDT "       which match the current src code version and have the path for the executable\n";
print STDOUT "       set to ../bin/gcx\n";
print STDOUT "   (c) the subdirectory './documentation' contains an up-to-date version of\n";
print STDOUT "       class_diagram.pdf and manual.pdf\n\n";
print STDOUT "ARE YOU SURE THAT ALL OF THE ABOVE CRITERIA ARE SATISIFIED [y/n]? ";
my $confirm=<STDIN>;
print STDOUT "\n";

chomp($confirm);
if (!($confirm eq 'y')) {
    print STDOUT "ABORTING...\n See you again soon :-)!\n";
    exit(1);
}

################################################### CONFIG
my %source=(
    os => "all",
    create_bin_dir => 1,
    create_example_dir => 1,
    create_src_dir => 1,
    fill_bin_dir => 0,
    fill_example_dir => 1,
    fill_src_dir => 1);

my %linux_bin=(
    os => "linux",
    create_bin_dir => 1,
    create_example_dir => 1,
    create_src_dir => 0,
    fill_bin_dir => 1,
    fill_example_dir => 1,
    fill_src_dir => 0);

my %windows_bin=(
    os => "win32",
    create_bin_dir => 1,
    create_example_dir => 1,
    create_src_dir => 0,
    fill_bin_dir => 1,
    fill_example_dir => 1,
    fill_src_dir => 0);

my %mac_bin=(
    os => "macos",
    create_bin_dir => 1,
    create_example_dir => 1,
    create_src_dir => 0,
    fill_bin_dir => 1,
    fill_example_dir => 1,
    fill_src_dir => 0);

my @configurations=(\%source,\%linux_bin,\%windows_bin,\%mac_bin);

################################################### MAIN
my $basedir="./release_$version_number";
print STDOUT "##### Creating directory '$basedir'...\n";
system("rm -Rf ./$basedir");
system("mkdir ./$basedir");

my $outdir="$basedir/archives";
print STDOUT "##### Creating directory '$outdir'...\n";
system("mkdir $outdir");

### create configurations
for (my $c=0;$c<=$#configurations;$c++) {
    my $config=$configurations[$c];

    # retrieve configuration values
    my $os=$config->{"os"};
    my $create_bin_dir=$config->{"create_bin_dir"};
    my $create_example_dir=$config->{"create_example_dir"};
    my $create_src_dir=$config->{"create_src_dir"};
    my $fill_bin_dir=$config->{"fill_bin_dir"};
    my $fill_example_dir=$config->{"fill_example_dir"};
    my $fill_src_dir=$config->{"fill_src_dir"};
    my $zipmode=$config->{"zipmode"};

    print STDOUT "### Creating configuration #$c...\n";
    
    # calculate directory name
    my $dir_base="gcx_$version_number" . "_";
    if ($create_src_dir && $fill_src_dir) {
        $dir_base.="src";
    } else {
        $dir_base.="bin_$os";
    }
    my $dirname="$basedir/$dir_base";
    print STDOUT " -> Creating directory '$dirname'...\n";
    system("mkdir $dirname");

    # calculate and create subdirectories
    my $bin_dir;
    if ($create_bin_dir) {
        $bin_dir="$dirname/bin";
        system("mkdir $bin_dir");
        print STDOUT " -> Creating subdirectory '$bin_dir'...\n";
    }
    my $example_dir;
    if ($create_example_dir) {
        $example_dir="$dirname/examples";
        system("mkdir $example_dir");
        print STDOUT " -> Creating subdirectory '$example_dir'...\n";
    }
    my $src_dir;
    if ($create_src_dir) {
        $src_dir="$dirname/src";
        system("mkdir $src_dir");
        print STDOUT " -> Creating subdirectory '$src_dir'...\n";
    }

    # fill subdirectories
    &fillMainDir($dirname);
    if ($fill_bin_dir) {
        &fillBinDir($bin_dir,$os);
    }
    if ($fill_example_dir) {
        &fillExampleDir($example_dir);
    }
    if ($fill_src_dir) {
        &fillSrcDir($src_dir);
    }

    # create and move zip archives
    &createZipArchives($basedir,$dir_base);
    system("mv $basedir/$dir_base.tar.gz $basedir/archives");
    system("mv $basedir/$dir_base.zip $basedir/archives");
}


### create manual
print STDOUT "##### Creating manual zip file...\n";
print STDOUT " -> Zipping manual and class diagram...\n";
# create .tar.gz for manual and class diagram
system("cd ./documentation; cp manual.pdf gcxman_$version_number.pdf; tar --create --file=gcxman_$version_number.tar gcxman_$version_number.pdf; gzip gcxman_$version_number.tar; cd ..");
system("cd ./documentation; cp class_diagram.pdf gcxuml_$version_number.pdf; tar --create --file=gcxuml_$version_number.tar gcxuml_$version_number.pdf; gzip gcxuml_$version_number.tar; cd ..");
# create .zip for manual and class diagram
system("cd ./documentation; cp manual.pdf gcxman_$version_number.pdf; zip -r gcxman_$version_number.zip gcxman_$version_number.pdf; cd ..");
system("cd ./documentation; cp class_diagram.pdf gcxuml_$version_number.pdf; zip -r gcxuml_$version_number.zip gcxuml_$version_number.pdf; cd ..");

print STDOUT " -> Moving manual and class diagram to archive folder...\n";
# moving .zip and .tar.gz manual and class diagram
system("mv ./documentation/gcxman_$version_number.tar.gz $basedir/archives");
system("mv ./documentation/gcxuml_$version_number.tar.gz $basedir/archives");
system("mv ./documentation/gcxman_$version_number.zip $basedir/archives");
system("mv ./documentation/gcxuml_$version_number.zip $basedir/archives");






################################################### SUBS 
sub fillMainDir {
    my $main_dir=shift;
    print STDOUT " -> Copying README.txt, INSTALL.txt, and LICENSE.txt...\n";

    system("cp $SRCDIR/README.txt $main_dir");
    system("cp $SRCDIR/INSTALL.txt $main_dir");
    system("cp $SRCDIR/LICENSE.txt $main_dir");
}

sub fillBinDir {
    my $bin_dir=shift;
    my $os=shift;

    print STDOUT " -> Filling bin directory '$bin_dir'...\n";
    my $suffix=$os eq "win32"?"exe":"bin";
    system("cp ./binaries/gcx_$os.$suffix $bin_dir/gcx.$suffix");
}

sub fillExampleDir {
    my $example_dir=shift;

    print STDOUT " -> Filling example directory '$example_dir'...\n";

    # create dblp, nasa, xmark, and xmp directory
    my $sgml_dir="$example_dir/sgml";
    my $tree_dir="$example_dir/tree";
    my $xmark_dir="$example_dir/xmark";
    my $xmp_dir="$example_dir/xmp";
    system("mkdir $sgml_dir");
    system("mkdir $tree_dir");
    system("mkdir $xmark_dir");
    system("mkdir $xmp_dir");

    # fill the above directories
    open(README,">$example_dir/README.txt");
    print README "The XML documents and example queries in the subdirectories are\n";
    print README "taken from:\n\n";
    print README "tree  -> Galax (http://www.galaxquery.org/)\n";
    print README "sgml  -> Galax (http://www.galaxquery.org/)\n";
    print README "xmark -> XMark - An XML Benchmark Project (http://www.xml-benchmark.org/)\n";
    print README "xmp   -> Galax (http://www.galaxquery.org/)";
    close(README);
    system("cp $SRCDIR/testsuite/queries/_final_sample_queries_v2-1/sgml*xq $sgml_dir");
    system("cp $SRCDIR/testsuite/queries/_final_sample_queries_v2-1/tree*xq $tree_dir");
    system("cp $SRCDIR/testsuite/queries/_final_sample_queries_v2-1/xmark*xq $xmark_dir");
    system("cp $SRCDIR/testsuite/queries/_final_sample_queries_v2-1/xmp*xq $xmp_dir");
    system("cp $SRCDIR/testsuite/xml/_final_sample_queries_v2-1/sgml_q01.xml $sgml_dir/sgml.xml");
    system("cp $SRCDIR/testsuite/xml/_final_sample_queries_v2-1/tree_q02.xml $tree_dir/tree.xml");
    system("cp $SRCDIR/testsuite/xml/_final_sample_queries_v2-1/xmark_q01.xml $xmark_dir/xmark.xml");
    system("cp $SRCDIR/testsuite/xml/_final_sample_queries_v2-1/xmp_q01.xml $xmp_dir/xmp.xml");
}

sub fillSrcDir {
    my $src_dir=shift;

    print STDOUT " -> Filling src directory '$src_dir'...\n";
    system("cp $SRCDIR/*.cpp $src_dir");
    system("cp $SRCDIR/*.h $src_dir");
    system("cp $SRCDIR/*.y $src_dir");
    system("cp $SRCDIR/*.l $src_dir");
    system("cp $SRCDIR/LICENSE.txt $src_dir");
    system("cp $SRCDIR/FRAGMENT.txt $src_dir");
    system("cp $SRCDIR/README.txt $src_dir");
    system("cp $SRCDIR/INSTALL.txt $src_dir");
    system("cp ./makefiles/Makefile.Linux $src_dir");
    system("cp ./makefiles/Makefile.Windows $src_dir");
    system("mkdir $src_dir/util");
    system("cp $SRCDIR/util/*cpp $src_dir/util");
}

sub createZipArchives {
    my $dir=shift;
    my $subdir=shift;

    print STDOUT " -> Zipping directory '$subdir'...\n";
    # create .tar.gz
    system("cd $dir; tar --create --file=$subdir.tar $subdir; gzip $subdir.tar; cd ..");
    # create .zip
    system("cd $dir; zip -r $subdir.zip $subdir; cd ..");
}
