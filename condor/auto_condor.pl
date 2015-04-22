#! /usr/bin/perl -w
use strict;
use warnings;
use Cwd;
use POSIX qw(ceil);

#Setup Environment (I can move this to a sourced script)
my $CondorBase     = "/direct/phenix+spin2/beaumim/WAnalysisRun13/AnalysisCode/condor/";
my $ListsDirectory = $CondorBase."lists/";
my $RunOverFile    = $ListsDirectory."27Jan2014RalfTaxiRun13MuonOT.txt";
#my $RunOverFile    = $ListsDirectory."run13_reproduce_rpc1.txt";
my $FinishedJobs   = $CondorBase."output/basic_cut/"; 
my $LogDirectory   = $CondorBase."logs/";
my $JobsDirectory  = $CondorBase."jobs/";
my $LibraryDirectory = "/direct/phenix+spin2/beaumim/WAnalysisRun13/AnalysisCode/condor/lib/";
my $LibraryName      = "libSingleMuonsWAnalysis.so";
my $Library = $LibraryDirectory.$LibraryName;

system("mkdir -p $FinishedJobs");
system("mkdir -p $LogDirectory");
system("mkdir -p $JobsDirectory");

#Variables
my $User     = "beaumim";          # submit jobs as this user
my $JobSplit = 15;                 # split list of files to run over into 10 lists
my $Macro    = "Run_Reduction.C";
my $Email    = 'michael.beaumier@gmail.com';
my $logTag   = "MuonDataBasicCut"; # name of condor log file
my $jobName  = "MuonDataBasicCut"; # a unique tag to use to identify job files

# Job File Template 
my %JobFile = (
		Universe     => "vanilla"       , 
		Notification => "Error"         , 
		Initialdir   => ""              , # Update with $JobDirectory 
		Executable   => ""              , # Update with $JobDirectory.$Macro
		Arguments    => ""              , 
		Log          => ""              , # set to $LogDirectory.$logTag.".log" when defined
		Output       => ""              , # set to $LogDirectory.$logTag.".out" when defined
		Error        => ""              , # set to $LogDirectory.$logTag.".err" when defined
		Notify_user  => "$Email"        , 
		GetEnv       => "True"          , 
		JobType      => "\"cas\""       , 
		Queue        => ""                
		);

# User Feedback
print "---ENVIRONMENT SETUP---\n";
print "  BASE: ".$CondorBase."\n";
print "  LIST: ".$RunOverFile."\n";
print "  USER: ".$User."\n";
print "  JOBS: ".$JobSplit."\n";
print "  DEPO: ".$FinishedJobs."\n";
print " MACRO: ".$Macro."\n";
print "   LIB: ".$Library."\n";
print "\n";

# Show user the template used to create job files
print "---JOB FILE TEMPLATE---\n";
my $href = "" ; # hash reference
my $key  = "" ; # hash key
my $val  = "" ; # hash val
# loop over @JobFile, getting a reference to the has for each entry, setting it to $href
for $key ( keys %JobFile ) 
{
	$val = $JobFile{$key};
	print "  $key = $val\n";
}
print "\n";

# Dump <FILELIST> into an array so that we can split the files into jobs
open FILELIST, "<", $RunOverFile or die $!;
my @files = ();
my $file = "";
my $numberOfFiles = 0;
chomp( @files = <FILELIST> ); 
$numberOfFiles = @files;
print "Splitting $numberOfFiles from $RunOverFile into $JobSplit jobs.\n";

# Loop over file list and create a list of bash calls to root
my $rootFileName = "";
my @bashCallList = ();
my $bashRootCall = "";
my $MacroArgList = "";
my $ralfDirectory = "";
my $lastGoodFile = "";
foreach $file (@files)
{
	# Extract the file name (not directory) to use for output
	if( $file =~ /(\/.*\/)(\d{6}_\d\.root)/m )
	#if( $file =~ /(\/direct\/phenix\+spin2\/rseidl\/Run13pp510Muon\/2103\/data\/)(\d{6}_\d\.root)/m )
	#if( $file =~ /(\/direct\/phenix\+spin\/phnxsp01\/rseidl\/taxi\/Run13pp510OT\/2765\/data\/)(\d{6}_\d\.root)/m ) 
	{
		$ralfDirectory = $1;
		$rootFileName  = $2;
		$lastGoodFile = $file;
		$MacroArgList = "\\(\\\"$ralfDirectory\\\",\\\"$FinishedJobs\\\",\\\"$rootFileName\\\",\\\"$Library\\\"\\)";
		$bashRootCall = "root.exe -b -q $CondorBase"."$Macro"."$MacroArgList";
		#print $bashRootCall."\n";
		push(@bashCallList,$bashRootCall);
	}
	else
	{
		print "irregularly formatted file  : [$file]\n";
		print "last matched file looks like: [$lastGoodFile]\n";
	}
}
close FILELIST;

# Create directories for job file + job script
my $fileCounter = 0;
my $jobCounter  = 1;
print "\n---SANITY CHECK ?(ROOT FILES == BASH ROOT CALLS)?---\n";
print "       ROOT FILES: ".@files."\n";
print "  BASH ROOT CALLS: ".@bashCallList."\n";

my $i = 0;
my $j = 0;

my $bashCallsTotalFile = $LogDirectory."bashCalls.txt";
open BASHCALL_SUMMARY, ">", $bashCallsTotalFile;
for( $i = 0; $i < @bashCallList; $i++)
{
	print BASHCALL_SUMMARY $bashCallList[$i]."\n";
}
close BASHCALL_SUMMARY;

my $jobCountChunk = ceil($numberOfFiles/$JobSplit)+1;
my $jobDir = "";
my $BashScript = "";
my $mkdirCmd = "";
my $chmodCmd = "";
my $firstCall = 0;
my $job_number = 0;
my $lastJob = @bashCallList % $jobCountChunk; # remaining jobs

$jobDir = $JobsDirectory."job_$job_number/";
$BashScript = $jobDir."$jobName"."_$job_number.sh";
$mkdirCmd = "mkdir -p $jobDir";
system($mkdirCmd);
open BASH_SCRIPT, ">", $BashScript;
print BASH_SCRIPT "#! /bin/bash\n";
print BASH_SCRIPT "# THIS FILE IS AUTOMATICALLY GENERATED\n";
print BASH_SCRIPT "# $jobName"."_$job_number.sh\n\n";

for($i = 0; $i < @bashCallList; $i++)
{
	print BASH_SCRIPT $bashCallList[$i]."\n";
	if( (($i % $jobCountChunk == 0 ) && $i > 0) || ($i == @bashCallList - 1))
	{
		close BASH_SCRIPT; # finished first set
		system("chmod a+rwx $BashScript");

		# make a job file by modifying hash
		$JobFile{'Initialdir' } = $jobDir;
		$JobFile{'Executable'} = $BashScript;
		$JobFile{'Log'       } = $LogDirectory.$logTag."_$job_number".".log";
		$JobFile{'Output'    } = $LogDirectory.$logTag."_$job_number".".out";
		$JobFile{'Error'     } = $LogDirectory.$logTag."_$job_number".".err";

		open JOB_FILE, ">", $jobDir."$jobName"."_$job_number".".job";
		print JOB_FILE 'Universe     = '.$JobFile{'Universe'    }."\n";
		print JOB_FILE 'Notification = '.$JobFile{'Notification'}."\n";
		print JOB_FILE 'Initialdir   = '.$JobFile{'Initialdir'  }."\n";
		print JOB_FILE 'Executable   = '.$JobFile{'Executable'  }."\n";
		print JOB_FILE 'Arguments    = '.$JobFile{'Arguments'   }."\n";
		print JOB_FILE 'Log          = '.$JobFile{'Log'         }."\n";
		print JOB_FILE 'Output       = '.$JobFile{'Output'      }."\n";
		print JOB_FILE 'Error        = '.$JobFile{'Error'       }."\n";
		print JOB_FILE 'Notify_user  = '.$JobFile{'Notify_user' }."\n";
		print JOB_FILE 'GetEnv       = '.$JobFile{'GetEnv'      }."\n";
		print JOB_FILE '+JobType     = '.$JobFile{'JobType'     }."\n";
		print JOB_FILE 'Queue'."\n";
		close JOB_FILE;
		$job_number++;

		if( $i < @bashCallList - 1)
		{
			$jobDir = $JobsDirectory."job_$job_number/";
			$mkdirCmd = "mkdir -p $jobDir";
			system($mkdirCmd);
			$BashScript = $jobDir."$jobName"."_$job_number.sh";
			open BASH_SCRIPT, ">", $BashScript;
			print BASH_SCRIPT "#! /bin/bash\n";
			print BASH_SCRIPT "# THIS FILE IS AUTOMATICALLY GENERATED\n";
			print BASH_SCRIPT "# $jobName"."_$job_number.sh\n\n";
		}
	}
}
