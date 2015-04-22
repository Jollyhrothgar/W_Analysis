#! /usr/bin/perl -w 
use strict;
use warnings;
use POSIX;

if (@ARGV != 1) {
  die "Usage is $0 <tag> to create a backup\n"
}

my $ana_base = "/direct/phenix+spin2/beaumim/WAnalysisRun13/AnalysisCode";
my $rcf_home = "/direct/phenix+u/beaumim";  

my @log_files = `find $ana_base/logs/ -maxdepth 1 | grep -P \"(.*\.txt)|(.*\.csv)\"`;
my @root_trees = `find $ana_base/macros/rootTrees/Run13Wness* -maxdepth 1`; 
my @pdf_files = `find $ana_base/macros/rootTrees/ -maxdepth 1 | grep -i pdf`;

my $now_string = strftime "%e%b%Y", localtime;
print $now_string."\n";

my $mkdircmd = "mkdir -p $ana_base/macros/rootTrees/compare/$now_string"."_"."$ARGV[0]";
system($mkdircmd);
system("rm -v $rcf_home/WanaTrees");
system("ln -sv $ana_base/macros/rootTrees/compare/$now_string $rcf_home/WanaTrees");

my @copy_list = (@log_files,@root_trees,@pdf_files);
foreach(@copy_list) {
  chomp $_;
  my $copy_cmd = "rsync --progress --update --verbose $_ $rcf_home/WanaTrees";
  system("$copy_cmd\n");
}
