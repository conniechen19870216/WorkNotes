#!/usr/bin/perl

printf("Please select the base variant:\n1. Q.767\n2. ccitt(ITU 92)\n3. ITU 97\n4. ETSI V2\n5. ETSI V3\n6. Other variant\n");
$input1 = <STDIN>;
$input1 =~ s/\n//;
if($input1 eq "1")
{$basevar="Q.767";}
elsif($input1 eq "2")
{$basevar="ccitt";}
elsif($input1 eq "3")
{$basevar="ITU 97";}
elsif($input1 eq "4")
{$basevar="ETSI";}
elsif($input1 eq "5")
{$basevar="ETSI V3";}
elsif($input1 eq "6")
{
   printf("Please type the base variant name(e.g. 'ansi 92', make sure it exits):\n");
   $input1 = <STDIN>;
   $basevar=$input1;
}
else
{
die ("bye.\n");
}
$basevar =~ s/\n//;
###########################################################################################
printf("Please type the new variant name:\n");
$input1 = <STDIN>;
$newvar=$input1;
$newvar =~ s/\n//;

printf("Please specify the db file,(it should exist in current dir)(<cr>=si_db.c):\n");
$input1 = <STDIN>;
if ($input1 eq "\n")
{
$input1 = "si_db.c";
}
$dbfile=$input1;
$dbfile =~ s/\n//;

printf("Please specify the new db file(<cr>=".$dbfile.".new):\n");
$input1 = <STDIN>;

if ($input1 eq "\n")
{
$input1 = $dbfile.".new";
}

$newdbfile=$input1;
$newdbfile =~ s/\n//;
$logfile=$newdbfile.".log";

printf("Base variant is: %s\n", $basevar);
printf("New variant is : %s\n", $newvar);
printf("DB file is: %s\n", $dbfile);
printf("New DB file is: %s\n", $newdbfile);
printf("Log file is: %s\n\n", $logfile);

printf("Continue to generate new db file? y/n, <cr>=n\n");
$input1 = <STDIN>;
if($input1 ne "y\n" )
{
die("bye.\n");
}
########################################################################################
open (INFILE, $dbfile) || die ("Could not open file");
@array = <INFILE>;
$scalar = @array;

open(LOGFILE, ">".$logfile) || die("Cannot create log file\n");
$logstring = ("(scroll down to the end to check warnings.)\n".$dbfile.":line#"." --> ".$newdbfile.":line#\n\n");

printf("File has %d lines.\n",$scalar);
printf("Wait...\n");
#
@newarray;
#$pattern1="\\/\\*.*switch.*".$basevar.".*\\*\\/";
$pattern1="\\/\\*.*switch.*".$basevar." *\\*\\/";
#$pattern1="\\/\\*.*switch.*".$basevar." +\\*\\/"."|"."\\/\\*.*switch.*".$basevar.".*req.*\\*\\/";
$pattern_newvar="\\/\\*.*switch.*".$newvar." *\\*\\/";
$pattern2="^#if +0";
$pattern3="^#else";
$pattern4="^#endif";
$pattern5="^#if +1";
$pattern6="^#ifdef +TELICA";
$pattern7="^\\{";

#$pattern_ccitt="\\/\\*.*switch.*"."russia"." *.*\\*\\/";
$pattern_test="\\/\\*.*switch.*"."test"." *\\*\\/";
#$num_ccitt = 0;
#$num_test = 0;

$count = 0;
$count1 = 0;
$count2 = 0;
$line1 = "";
$line2 = "";
$oldline="";
$newline="";
$found=0;
$addlinesnum=0;
$foundif0=0;
$warnings="";
$warning1="";
#$warning2="\nWarning, at the following places there are multiple choices, normally caused by #if 0(1) statements, please double check whether the choice made is correct.\n";
#$warning3="";
$warning2="\nWarning, for arraies ending at the following lines of ".$newdbfile.", there are multiple lines matching (".$basevar."), this is normally caused by #if 0(1) statements, please check to see whether the new variant is correctly added.\n";
$warning3="\nWarning, for arraies ending at the following lines of ".$newdbfile.", did not add a line for new variant because there is no correct match for (".$basevar.") please check and add it manually.\n";

while ($count <= $scalar-1)
{
   #printf($count);
   $line1 = $array[$count];
   #printf($newarray[$count]);

   #if ($line1 =~ /$pattern_test/i )
   #{
   #  $num_test++;
   #}
   #if ($line1 =~ /$pattern_ccitt/i )
   #{
   #  $num_ccitt++;
   #}

   if($found==0)
   {
     if ($line1 =~ /$pattern1/i )
     {

       for ($count4=1; $count4 <= 5; $count4++)
       {
            $line2 = $array[$count-$count4];
            if($line2 =~ /$pattern2/)  # #if 0
            {
               #printf($array[$count-$count4]);
               #printf($array[$count-$count4+1]);
               #printf($array[$count-$count4+2]);
               $found=0;
               last;
            }
            if($line2 =~ /$pattern3/ || $line2 =~ /$pattern5/ || $line2 =~ /$pattern6/)  # #else, #endif, #if 1, #ifdef TELICA
            {
               $found=1;
               last;
            }
            if($count4==5 || $line2 =~ /$pattern4/ || $line2 =~ /$pattern7/) # #endif \{
            {
               $found=1;
               last;
            }
       }

       if($found==1)
       {
          $oldline=$line1;
          $newline=$line1;
          #printf($newline);
          $newline =~ s/\b$basevar\b/$newvar/ig;
          $oldlinenum=$count+1;

       }
     }

   }
   else
   {
      #printf("found a line.\n");
      if ($line1 =~ /\};/)
      {

         $newarray[$count1]=$newline;
         #printf($newarray[$count1]);
         $count1++;
         $addlinesnum++;
         $found=0;

         $logstring=$logstring.($oldlinenum."  ".$oldline);
         $logstring=$logstring.(($count1)."  ".$newline."\n");
         #print LOGFILE ($oldlinenum."  ".$oldline);
         #print LOGFILE (($count1)."  ".$newline."\n");

      }

      if ($line1 =~ /^\{/)
      {
         $found=0;
         $warning1 = ("Warning! found a match line at\n".$oldlinenum."  ".$oldline." But did not add a corresponding line for new variant here.\n\n").$warning1;
      }
   }

   $newarray[$count1]=$line1;

   #printf($newarray[$count1]);
   $count1++;
   $count++;

}

open(OUTFILE, ">".$newdbfile) || die("Cannot create new file\n");
$newscal=@newarray;
$count5=0;
while ($count5 <= $newscal-1)
{
print OUTFILE ($newarray[$count5]);
$count5++;
}

 $count7=0;
 $count8=0;
 $found1=0;
 #$found2=0;
 while ($count8 <= $newscal-1)
 {
   $line4 = $newarray[$count8];
   if($found1==0)
   {
      if ($line4 =~ /$pattern_test/i )
      {
         $found1=1;
         #$found2=0;
         $count7=0;
      }
      if($line4 =~ /$pattern1/i)
      {
         $count7++;
      }
   }
   else
   {
      #if ($line4 =~ /$pattern_newvar/i )
      #{
      #   $found2=1;
      #}
      if ($line4 =~ /$pattern1/i )
      {
         $count7++;
      }
      if ($line4 =~ /\};/)
      {
         #if($found2==0)
         #{
         #   $warning3=$warning3."--".($count8+1)."\n";
         #}
         if (!($newarray[$count8-1] =~ /$pattern_newvar/i ))
         {
            $warning3=$warning3."--".($count8+1)."\n";
         }
         if($count7>1)
         {
           $warning2=$warning2."--".($count8+1)."\n";
         }
         $found1=0;
         #$found2=0;
         $count7=0;
      }
      if ($line4 =~ /^\{/)
      {
         $found1=0;
         #$found2=0;
         $count7=0;
      }
   }
   $count8++;
 }

print LOGFILE ($logstring);
print LOGFILE ("Totally add ".$addlinesnum." lines.\n\n");
#print LOGFILE ("Find ".$num_test." lines for variant test\n");
#print LOGFILE ("Find ".$num_ccitt." lines for variant ccitt\n");
print LOGFILE ($warning1);
$twominus="\\-\\-";
if($warning2 =~ /$twominus/)
{
   print LOGFILE ($warning2);
}
if($warning3 =~ /$twominus/)
{
   print LOGFILE ($warning3);
}

printf("Add %s lines based on base variant.\n", $addlinesnum);
printf("Done generating new file!\n");
printf("Log file is ".$logfile.". Please check log!\n\n");

close(OUTFILE);
close(INFILE);
close(LOGFILE);

