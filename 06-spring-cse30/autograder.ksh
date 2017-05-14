#!/usr/bin/ksh

##############################################################################
#                                                                            #
#                                Autograder                                  #
#                                                                            #
# This script is the driver, or overhead program, for running all of the     #
# autograder operations.  While it is possible for a knowledgeable user to   #
# execute the autograder helper scripts individually, the scripts are set up #
# to accept a number of command line arguments which are most easily handled #
# through the autograder driver.                                             #
#                                                                            #
# One of the autograder driver's primary goals is to bring the number of     #
# possible user mistakes made in the grading process to a minimum.  It does  #
# this by disallowing autograder commands to be run when the necessary       #
# directories or files do not exist. Also, the driver lets the user know     #
# exactly what input files and drivers will be used in the autograding       #
# process. If the user notices a discrepancy between the input files to be   #
# used and the one's he wishes to be used, he can elect to not execute the   #
# autograder.                                                                #
#                                                                            #
# collect everyone's assignment:  Choosing this option requires that you have a     #
# file called account_names in your autograder area that contains a list of  #
# all the login names of the students whose assignments you want to collect. #
# After choosing this option, the autograder will ask you if you would like  #
# to change the email message sent out by the collect script. If you answer  #
# yes, the collect script is opened and you can edit the email messages      #
# within it.  The places you are allowed to edit are marked with a large     #
# comment block and explanation.  Next you are asked if the assignment       #
# allows for the students to work with a partner. Once you have answered     #
# this, collection will proceed.  Each student listed in the account_names   #
# file has their assignment moved into the 1/Turnin directory.  An         #
# account_names file with a list of all the students who turned in the       #
# assignment is put into the Testground directory.  This file is used by the #
# autograder, so do not remove it.  Also, based on your answer to the        #
# partners question, either the partner_assng script or the create_listing   #
# script is called to update the graders .students files in the Tutor area   #
# to match who turned in the current assignment. Note that if there already  #
# are turned in files in the Turnin directory, this option will read         #
#                                                                            #
# specify which accounts to collect:  same as above except you tell the      #
# autograder which students you want collected. In this case, the            #
# create_listing script is not invoked, nor is the account_names file in the #
# Testground area created.                                                   #
#                                                                            #
# collect assignment at specified time:  This will call the collect_at script.  It  #
# allows you to specify a day and time you want collection to occur.  You    #
# are informed if the job is successfully set or not.  If so, you can count  #
# on the assignments being collected as per the first option at they day and #
# time you selected.                                                         #
#                                                                            #
# assign students to graders: This option uses the account_names file        #
# in the autograder area and the tas file in the Tutor area.  It is meant    #
# to be run at the beginning of the quarter and handles assigning the        #
# appropriate amount of students to each grader.  It creates a               #
# .students.orig file for each grader in the Tutors area.  This file         #
# contains the list of all of the students each grader is to grade for the   #
# quarter.                                                                   #
#                                                                            #
# Note that if .students.orig files already exist in the Tutors area, this   #
# option will read "reassign students to graders."                           #
#                                                                            #
# send/collect gradesheet from graders:  This option prompts you to enter    #
# what directory, starting from the graders home directory, you want to      #
# send to or collect from.  The autograder will then go to that directory    #
# and check and see if there is .grades file there yet for the grader to     #
# enter the scores of his students into.  If there is, this file will be     #
# collected.  If it is not there, it will be into this directory.  The       #
# .grades file is a file that has a list of the names and logins of each     #
# student the grader is supposed to grade, with a score of 0 next to each    #
# name.  At the top of the file, there is a line that reads: "I am done:     #
# N".  The grader's job then is to change these scores and, when he is       #
# finished, to change the top to read "I am done: Y".  All of the            #
# collected scores are put into a 1.scores.sorted file in the Tutors       #
# area.  All of the graders who are not yet finished grading are put into    #
# an 1.holdouts file.                                                      #
#                                                                            #
# email tutors list of students to grade:  This script emails each grader    #
# the contents of their .students.orig file in the Tutors area, along with   #
# the actual name of each student they are to grade.  This way if there are  #
# any problems, like a graders knows some of the students he is assigned to, #
# he can let the professor know, and the .students list can be changed       #
# accordingly.                                                               #
#                                                                            # 
# grade different assignment:  This option simply changes the homework       #
# assignment directory the autograder deals with.  For example, if you       #
# wanted to grade 3, and then wanted to collect the grades for 2, you    #
# would elect this option in order to start dealing with 2 instead of 3. #
#                                                                            #
# run autograder on 1: To explain this option, you need to understand      #
# how the actual testing of homework assignments work.                       #
#                                                                             #
# The reason the autograder can save you so much time is that you don't      #
# have to sit there compiling each student's program and typing in test      #
# input.  What this does mean, though, is that you must provide to the       #
# autograder all of the input you want to be used to test the student's      #
# programs.  In the homework assignment's Testground/TestFiles area, you     #
# need to put all of your input files.  The naming convention that you must  #
# follow for the input files is [name of executable]_[whatever you want].in. #
# You can also include .desc files corresponding to each .in file that       #
# contain a description of what the test case is testing.                    #
#                                                                            #
# For example, if you are testing a program that runs when you type          #
# "Calculator", an input file might be called Calculator_add.  The file      #
# would contain input you would normally enter at the keyboard to test       #
#                                                                            #
# Another requirement in running the autograder is the existence of a        #
# Makefile.  The Makefile needs to be able to compile the student's program  #
# by typing "make" at the command line prompt.  Also, typing "make clean"    #
# should remove all object and core files.  Having a Makefile allows the     #
# autograder to work with programs of different languages without the        #
# autograder scripts having to be changed each time a different program      #
# is tested.  The Makefile should be put in the homework assignment's        #
# Testground/Provided area. Also in the Provided area should be in files     #
# provided for the student in the homework assignment.  For example, in the  #
# Provided area you may wish to include the Driver program used to run to    #
# test the student's code.  This way, when the autograder runs, you insure   #
# that the same Driver is being used for every student's assignment.         #
#                                                                            #
# So, when you select the option to run the autograder, you are first asked  #
# how many different executables you will be testing for each student.  The  #
# maximum number the autograder supports is five.  Then there is a prompt    #
# that looks like this:                                                      #
#                                                                            #
#          Would you like to:   a) swap out main in the student's file to be #                                        
#                                 replaced with a different provided main    #
#                               b) use drivers in the Provided area          #
#                                                                            #
# Based on your selection here, the autograder tests the assignments in      #
# different ways.  You would want to select option "a" when the student      #
# when the code the student turned in actually included "main" in the file   #
# you want to test, but you don't want to test their main (you want to       #
# test the other functions in the file).  In this case, in the TestFiles     #
# area, every input file must have an equivalently named .driver file.  The  #
# driver file should be a short "main" function that tests whatever it is    #
# you want to test.  When the autograder tests their program then, it swaps  #
# out their main function, and replaces it with the main inside whatever     #
# driver it is currently testing.                                            #
#                                                                            #
# Option "b" is the easier one to use.  If you have a single Driver in the   #
# Provided area for each executable you want to test, you can avoid the      #
# necessity of having a separate .driver file for each .in file.  With this  #
# option, the autograder tests each student's code using whatever Drivers    #
# are in the Provided area.                                                  #
#                                                                            #
# The next prompt you encounter asks if the assignment is done in C, C++,    #
# or Java.  The answer to this question mainly just determines how long the  #
# autograder sleeps between test cases (java is obviously much slower than   #
# C).                                                                        #
#                                                                            #
# Next, you are asked to enter the name of the account to use as the key.    #
# This account should be located in the Turnin area with the rest of the     #
# accounts.  The output of every account tested is compared to the output    #
# of this key account.                                                       #
#                                                                            #
# After this, you are asked if you would like to run the autograder in       #
# no hangup mode.  If you answer yes, the autograder will safely run even    #
# if you logout of your account.  However, none of the autograder's output   #
# will be sent to the terminal screen.                                       #
#                                                                            #
# The last option is: "Start autograder (y/n)?"  Answering yes will start    #
# the testing of each account.  If you are not running in no hangup mode,    #
# you can watch the autograder go through each account, and see which test   #
# case is being tested at what time.                                         #
#                                                                            #
# autograde specified accounts: This option is the same as the above option  #
# except you are asked to specify exactly which accounts are to be tested    #
# instead of the autograder relying on the account_names file created at     #
# collection time for this information.                                      #
#                                                                            #
# distribute autograder results to graders:  This option takes all of the    #
# results in the homework assignment's Results area and distributes them     #
# into the directory you specify in the grader's home directory.  If the     #
# directory you specify does not exist, it will be created for you.  After   #
# this script is run, the Results area will be empty.                        #
#                                                                            #
# quit:  quits out of the autograder, back to the Unix command line prompt.  #
#                                                                            #
##############################################################################

#function for checking that necessary directories exist
dir_error_check()
{
   # call other helper functions 
   assn_dir_check 
   assn=$?
   tutor_dir_check
   tutor=$?
   if (( $tutor ))
      then
         tas_file_check
   fi
   ta=$?

   if (( $assn && $tutor && $ta )) 
      then
	 return 1 
      else
	 return 0 
   fi
}

#function for checking that the current  assignment's directory and
#subdirectories exist
assn_dir_check()
{
   if [ -x $AG_home/$assignment ]
      then
	 if [ ! -x $AG_home/$assignment/Results ] 
	    then
	       mkdir $AG_home/$assignment/Results
	 fi

	 if [ ! -x $AG_home/$assignment/Testground ]
	    then
	       mkdir $AG_home/$assignment /Testground
         fi

	 if [ ! -x $AG_home/$assignment/Turnin ]
	    then
	       mkdir $AG_home/$assignment/Turnin
	 fi

	 if [ ! -x $AG_home/$assignment/Testground/TestArea ]
	    then
	       mkdir $AG_home/$assignment/Testground/TestArea 
	 fi

	 if [ ! -x $AG_home/$assignment/Testground/TestFiles ]
	    then
	       mkdir $AG_home/$assignment/Testground/TestFiles 
	 fi

	 if [ ! -x $AG_home/$assignment/Testground/Provided ]
	    then
	       mkdir $AG_home/$assignment/Testground/Provided
	 fi
	  
      return 1
      
      else
	 print ""
         print The needed directories for $assignment do not yet exist.
         print "Would you like to have them created for you (y/n)?  \c"
         read answer
	 print ""
         if [ $answer = y -o $answer = Y ]
	    then
	    print "mkdir $assignment" 
	    mkdir $AG_home/$assignment
	    print "cd $AG_home/$assignment" 
	    cd $AG_home/$assignment
	    print "mkdir Results"
	    print "mkdir Testground"
	    print "mkdir Turnin"
	    mkdir Results
	    mkdir Testground
	    mkdir Turnin
            print "cd Testground"
            print "mkdir TestFiles"
            print "mkdir Provided"
            print "mkdir TestArea"
            cd Testground
            mkdir TestArea
            mkdir Provided
            mkdir TestFiles
	    return 1
         fi
       
       return 0
   fi
}

#function for making the Tutors directory and the necessary symbolic links.
#also prompts for a "tas" file to be created
tutor_dir_check()
{

   if [ -x $AG_home/Tutors ]
      then
	 tas_file_check
	 if (( ! $? ))
	   then
	      return 0
         fi

	 cat $AG_home/Tutors/tas | while read link
	 do
	    if [ ! -h $AG_home/Tutors/$link ] 
	       then  
	          ln -s ~/../$link $AG_home/Tutors/$link
	    fi 
	 done
	 return 1 
   
      else 
	 print ""
	 print "A Tutors directory does not yet exist."
         print "Would you like one to be created for you (y/n)?  \c"
         read answer
         if [ $answer = y -o $answer = Y ]
	    then
               print ""
               print "mkdir $AG_home/Tutors"
               print ""	
	       mkdir $AG_home/Tutors
	
	       tas_file_check
	       if (( ! $? ))
		  then
		     return 0
               fi

	       print Creating links to graders directories...
	       cat $AG_home/Tutors/tas | while read link
	       do
	          ln -s ~/../$link $AG_home/Tutors/$link
	       done
	       return 1 
          fi
   fi

   return 0 

}

# function for making sure input files exist and for notifying the 
# user which input files will be used in autograding.
input_file_check()   
{
   # bug fix this later
   if [ ! -f $AG_home/$assignment/Testground/TestFiles/${1}_*.in -a \
        ! -f $AG_home/$assignment/Testground/TestFiles/${1}_*.purifyin -a \
        ! -f $AG_home/$assignment/Testground/TestFiles/${1}_*.freshin -a \
     	! -f $AG_home/$assignment/Testground/TestFiles/${1}_*.commandin ]
      then  
	 print "$1 has no input files associated with it."
	 print "Would you like to create some now (y/n)?  \c"
	 read temp
	 if [ $temp = y -o $temp = Y ]
	    then
	       make_testcase
	    else
	       return 0
	 fi
   fi

   # bug fix this later
   if [ ! -f $AG_home/$assignment/Testground/TestFiles/${1}_*.in -a \
   	! -f $AG_home/$assignment/Testground/TestFiles/${1}_*.purifyin -a \
	! -f $AG_home/$assignment/Testground/TestFiles/${1}_*.freshin -a \
	! -f $AG_home/$assignment/Testground/TestFiles/${1}_*.commandin ]
      	then
	 print "Autograding will not continue with no input files."
	 print ""
	 return 0
  fi

   print "These are the input files that will be used for $1:"
   cd $AG_home/$assignment/Testground/TestFiles/
   ls ${1}_*.in ${1}_*.freshin ${1}_*.purifyin ${1}_*.commandin 2> /dev/null
   return 1
}

#makes sure that a tas file exists in the appropriate place and 
# allows one to be created if one does not.
tas_file_check()
{
   if [ -f $AG_home/Tutors/tas ]
      then
	 return 1 
      else
	 print "You need to specify all of your graders in a file called"
	 print "$AG_home/Tutors/tas. " 
         print "Would you like to create that file now (y/n)?  \c"
	 read answer
	 if [ $answer = y -o $answer = Y ]
	    then
	       vi $AG_home/Tutors/tas
	       if [ ! -f $AG_home/Tutors/tas ]
		  then
		     return 0 
                  else
		     return 1 
	       fi 
	 fi
   fi
   return 0
}

# function for making input files according to the input file 
# naming convention
make_testcase()
{
   inc=0
   input=y
   while [ $input = y -o $input = Y ]
   do
     if [ $inc = 1 ]
        then
           print "Which driver would you like to do testcases for (type s for"
           print "same as previous driver)?  \c"
           read temp
           if [ $temp != s ]
              then
                 driver=temp
           fi

        else
           print "Which driver would you like to do testcases for?  \c"
           read driver
           inc=1
     fi
     print "What is the name of the testcase?  \c"
     read name
     vi $AG_home/$assignment/Testground/TestFiles/${driver}_${name}.in
     print "Would you like to make a description file for this test case?  \c"
     read desc
     if [ $desc = y -o $desc = Y ]
	then
	   vi $AG_home/$assignment/Testground/TestFiles/${driver}_${name}.desc
     fi
     print "Make another testcase(y/n)?  \c"; read input
   done
}      

#checks to make sure that there is a driver or desc file for every 
#input file and notifies the user if there is not.
test_case_comp()
{
   count=0
   INDEX=0
   compdir=$AG_home/$assignment/Testground/TestFiles
   ls $compdir/*.in $compdir/*.freshin $compdir/*.commandin 2> /dev/null | while read in 
   do
      temp=0 
      ls $AG_home/$assignment/Testground/TestFiles/*.$1 2> /dev/null | while read driver 
      do
	 if [ ${in%%.in} = ${driver%%.$1} ]
	    then
	       temp=1 
	       break

            elif [ ${in%%.freshin} = ${driver%%.$1} ]
	       then
	       temp=1 
	       break

	    elif [ ${in%%.commandin} = ${driver%%.$1} ]
	       then 
	       temp=1 
	       break

         fi

      done

      if [ $temp = 0 ]
         then
	    NOINPUT[$(($INDEX))]=`basename $in` 
	    let INDEX=$INDEX+1
	    count=1
      fi
   done

   if [ $count = 1 ]
      then
         print "The following input files do not have an associated $1 file: "
         print ${NOINPUT[*]}
	 return 0
   fi

   return 1

}

#start of autograder driver:

tput clear
print "************************************************************************"
print "*                                                                      *"
print "*                     Welcome to the Autograder!                       *"
print "*                     	   (version 3.0)                               *"
print "************************************************************************"
print ""

# check for installation having been run
if [ ! -f ~/.autograder.rc ]
   then
      print "You must run the autograder installer before continuing."
      exit 1
fi

#have access to global variables
. ~/.autograder.rc

# assign variables based on .autograder.rc file created by installer
#home=`head -1 ~/.autograder.rc`
#scripts=`sed -n '2 p' ~/.autograder.rc`
while :
do
print "Please enter the directory name for the assignment that "
print "you want to grade:  \c"; read assignment
if [ ! -z $assignment ]
   then
      break
   else 
      print "Invalid entry!"
      print ""
fi
done

if [ $assignment = q ]
   then
      exit
fi

# make sure all directories exist before continuing
dir_error_check
temp=$?

if [ $temp = 0 ]
   then
      print You need to have all directories in place in order to continue.
      print Exiting autograder...
      exit 1
fi

# program only ends with input of q at switch statement
while :
do
   print "" 
   print "Please select an option: "
   if [ -f ~/.collection_job ]
      then
	 set `cat ~/.collection_job`
	 print "                         re(m)ove collection job set for ${10} at ${13},"
   fi

   if [  -f $AG_home/$assignment/Turnin/* ]
      then
	 print "                         re(c)ollect everyone's $assignment assignment,"  
      else
	 print "                         (c)ollect everyone's $assignment assignment, "
   fi

   print "                         s(p)ecify which accounts to collect,"

   if [ ! -f ~/.collection_job ]
      then
         print "                         collect $assignment at specified (t)ime,"
   fi

   if [ -f $AG_home/Tutors/*.students.orig ]
      then
         print "                         re(a)ssign students to graders,"
      else
         print "                         (a)ssign students to graders,"
   fi
   print "                         (e)mail tutors list of students to grade,"
   print "                         (s)end/collect gradesheet from graders,"
   print "                         (g)rade different assignment,"
   print "                         (r)un autograder on $assignment assignment,"
   print "                         a(u)tograde specified accounts,"
   print "                         (d)istribute autograder results to graders,"
   print "			 archi(v)e students work," 
   print "			 send m(o)ss email to instructor,"
   print "			 estimate grading (h)ours,"
				
   if [ -f $AG_home/$assignment/Turnin/* ]
      then
         print "                         create (n)ew listing of students for grading,"
   fi
   print "                         (q)uit" 
   print "Enter your choice> \c" 

   read option

   case "$option" in
 
      h|H)
	$AG_scripts/estimate_hours $assignment
	;;

      o|O)
	
	print "Who should get the ready to moss email: \c"; read moss_emailer
        print "If you want to moss goto the archive machine and run the script on the line below" > $AG_home/moss_email
	print "exec ksh" >> $AG_home/moss_email
	print "wai=\`whoami\`;cd \$HOME/archive/\$wai/moss/\$wai;moss_script" >> $AG_home/moss_email
	mailx -s "To Initiate Mossing" $moss_emailer < $AG_home/moss_email
        rm $AG_home/moss_email

      ;;

      v|V)
	##run the archive script##
        $AG_scripts/archive_script

	;;  

      m|M)
	 set `cat ~/.collection_job`
	 at -r ${8} 2> /dev/null
	 if [ $? = 0 ]
	    then
	       print "Collection job removed successfully!"
         fi
	 rm ~/.collection_job 2> /dev/null
	 ;;

      # collect
      c|C|p|P|t|T)
         
	 dir_error_check
	 temp=$?
	 if [ $temp = 0 ]
	    then
	       print You need to have all directories in place in order to continue.
	       print Exiting autograder...
	       exit 1
     fi

	#creating account names dynamically

	
		count_name_acc=${#AG_name_acc}

		currDir=`pwd`
		echo cd to $AG_umbrella

		#create all files in $AG_home directory, in case of permission 
		#problems

		ls -1 $AG_umbrella > $AG_home/account_names.temp 

		echo name_acc is $AG_name_acc

		print "\c" > $AG_home/account_names
  cat $AG_home/account_names.temp | grep -v 'SUSPEND' | grep $AG_name_acc | while \
read account 
	    do

			#don't include the class dir itself eg. cs8f or cs12x
			if [ ${#account} = $count_name_acc ]
			   then
				  continue
			fi	

			#figure out how to separate from TA's accounts
			isTA=${account#${account%%?}}
			
			#checking if TA
		    if [ $isTA = 1 ]
			   then
				  continue
			elif [ $isTA = 2 ]
			   then
				  continue
			elif [ $isTA = 3 ]
			   then
				  continue
			elif [ $isTA = 4 ]
			   then
				  continue
			elif [ $isTA = 5 ]
			   then
				  continue
			elif [ $isTA = 6 ]
			   then
				  continue
			elif [ $isTA = 7 ]
			   then
				  continue
			elif [ $isTA = 8 ]
			   then
				  continue
			elif [ $isTA = 9 ]
			   then
				  continue
			elif [ $isTA = 0 ]
			   then
				  continue
			else
				echo $account to file $AG_home/account_names	
				print "$account" >> $AG_home/account_names	

		    fi
		

		done 	#end of loop dynamically create accoun_names file

		rm $AG_home/account_names.temp
		cd $currDir

	# update current_assignment status file in public directory
         if [ $option = t -o $option = T -o $option = c -o $option = C ]
	    then
	       print ""
	       print "Please enter the next assignment to be collected"
	       print "(enter none for no next assignment):"
	       read n_assn
	       if [ $n_assn = "none" -o $n_assn = "NONE" -o $n_assn = "None" ] 
		  then
		     rm -f $AG_pub/current_assignment
	       else
	             print $n_assn > $AG_pub/current_assignment
	       fi
	 fi

         if [ $option = t -o $option = T ]
	    then
	       $AG_scripts/collect_at $assignment
	       continue
         fi

	 if [ $option = p -o $option = P ]
             then
               print ""
	       print "Enter the account name you need to collect."
	       print "Hit enter when you have entered all of the account names."
	       print "" 
	       inc=1
	       print "Enter the name of account $inc to collect:  \c"
	       read account_name
	       if [ -z $account_name ]
		  then
		     continue
		  elif [ $account_name = q -o $account_name = Q ]
		  then
		     continue
		  else
	             mv $AG_home/account_names $AG_home/account_names.tmp 2> /dev/null
		     print $account_name > $AG_home/account_names
		     let inc=$inc+1
		     while :
		     do 
		        print "Enter the name of account $inc to collect:  \c"
		        read account_name
  	                if [ -z $account_name ]
			   then
			      break
			   elif [ $account_name = q - o $account_name = Q ]
			   then
			      break
		           else
			      print $account_name >> $AG_home/account_names
		        fi
		        let inc=$inc+1
		     done
               fi
         fi

	 if [ ! -z $account_name ]
	    then
		if [ $account_name = q -o $account_name = Q ]
		   then
		      continue
                fi
         fi

	 print "" 
	 wc -l ~/.autograder.rc | grep '2' > /dev/null
	 if [ "$acs_course" = "" ]
	    then
               print "What is the name of the file you want to collect (e.g. ..hw1.tar.gz)?  \c"
	       read file

			#to collect?
	       export file
	       print ""
         fi
	 print "Would you like to change the email message sent out by"
	 print "the collect script before continuing (y/n)?  \c"
         read choice
         if [ $choice = q -o $choice = Q ]
	    then
	       if [ -f $AG_home/account_names.tmp ]
		  then
		     mv $AG_home/account_names.tmp $AG_home/account_names
               fi
               continue
         fi
	 
	 if [ $choice = y -o $choice = Y ]
            then
	       wc -l ~/.autograder.rc | grep '2' > /dev/null
	       if [ $? = 0 ]
		  then
		     vi $AG_home/email_turnin $AG_home/email_noturnin $AG_home/email_badpermissions
		  else
		     vi $AG_home/email_turnin $AG_home/email_noturnin
               fi
	       print ""
               print "Would you like to proceed with the collection now (y/n)?  \c"
               read choice
	       
	       if [ $choice = q -o $choice = Q ]
		    then

	            if [ -f $AG_home/account_names.tmp ]
		        then
		           mv $AG_home/account_names.tmp $AG_home/account_names
                fi
		     continue 
           fi

           if [ $choice = y -o $choice = Y ]
             then
		     print ""
                     print Collecting $assignment...
                     $AG_scripts/gethw $assignment
               fi
             else
		print ""
                print Collecting $assignment...
                $AG_scripts/gethw $assignment           
         fi
	 
	 if [ $option = p -o $option = P ]
	    then
	       mv $AG_home/account_names.tmp $AG_home/account_names
	 fi
         ;; 

      a|A)
         dir_error_check
	 temp=$?
	 if [ $temp = 0 ]
	    then
	       print You must have all directories in place in order to continue.
	       print Exiting autograder...
	       exit 1
         fi

	 print "This option should only be selected approximately once a "
         print "quarter, are you sure you want to "
	 print "assign students to graders? (y/n) \c"
	 read ans

	 if [ $ans = n -o $ans = N ]
	   then
	     #don't reassing students to graders
	     continue
         fi

	 print Assigning students to graders...
         $AG_scripts/grader_assng 
         ;; 

      s|S)
         dir_error_check
	 temp=$?
	 if [ $temp = 0 ]
	    then 
	       print You must have all directories in place in order to continue.
	       print Exiting autograder...
	       exit 1
         fi
         print ""
         print "Please specify the pathname (from the graders' home directory) of"
	 print "where you want to send/collect from:  \c"
	 read path
         if [ $path = q ]
            then
               continue
         fi

         print $path | grep '~'
         if [ $? = 0 ]
            then
               path=${path##~/}
         fi


         if [ $? = 0 ]
            then
               path=${path##~/}
         fi

	 print Sending/collecting gradesheet from graders
         $AG_scripts/gradescript $path $assignment
         ;;

      e|E)
         dir_error_check
	 temp=$?
	 if [ $temp = 0 ]
	    then
	       print You must have all directories in place in order to continue.
	       print Exiting autograder...
	       exit 1
         fi

         if [ ! -f $AG_home/Tutors/*.students.orig ]
            then
	       print "You do not have the necessary /".students.orig/" files in your"
               print "Tutors directory.  Please select the /"assign students to graders/""
	       print "option from the main menu."
	       continue
         fi

	 print Emailing tutors list of students to grade...
         $AG_scripts/emailtutors
         ;;

      u|U|r|R)
         
         print "" 
	 dir_error_check
	 temp=$?
	 if [ $temp = 0 ]
	    then
	       print You must have all directories in place in order to continue.
	       print Exiting autograder...
	       exit 1
         fi

	 #if [ ! -f $AG_home/$assignment/Turnin/* ]
	 #   then
	 #      print You need to collect the assignment before running the \
	 #      autograder.
	 #      continue
	 #fi

         if [[ ! -f $AG_home/$assignment/Testground/account_names && $option != u && $option != U ]] 
	    then
               print "In order for autograding to take place, you must have a file named"
               print "account_names in $AG_home/$assignment/Testground"
               print "that lists the names of accounts to collect from."             
	       continue
	 fi

	 if [ $option = u -o $option = U ]
	    then
	       print "Enter the account name you need to grade."
	       print "Hit enter at the prompt when you have entered all of the account names."
	       print "" 
	       inc=1
	       print "Enter the name of account $inc to grade:  \c"
	       read account_name
	       if [ -z $account_name ] 
		  then
		     continue
		  elif [ $account_name = q -o $account_name = Q ]
		  then
		     continue
                  elif [ ! -f $AG_home/$assignment/Turnin/${account_name}* ]
                  then
                     print "You must collect this student's assignment before proceeding."
                     continue
		  else
                     cp $AG_home/$assignment/Testground/account_names $AG_home/$assignment/Testground/account_names.tmp 2> /dev/null
		     print $account_name > $AG_home/$assignment/Testground/account_names
		     let inc=$inc+1
		     while :
		     do 
		        print "Enter the name of account $inc to grade:  \c"
		        read account_name
		        if [ -z $account_name ]
			   then
			      break
			   elif [ $account_name = q -o $account_name = Q ]
			   then
			      break
                           elif [ ! -f $AG_home/$assignment/Turnin/${account_name}* ]
                           then
                              print "You must collect this student's assignment before proceeding."
                              account_name=q
                              break
		           else
			      print $account_name >> $AG_home/$assignment/Testground/account_names
		        fi
		        let inc=$inc+1
		     done
               fi
	       print ""
         fi

	 if [ ! -z $account_name ]
	    then
	    if [ $account_name = q  -o $account_name = Q ]
	       then
	          mv $AG_home/$assignment/Testground/account_names.tmp $AG_home/$assignment/Testground/account_names 2> /dev/null
	       continue
	    fi
	 fi
	 
	 temp=0 
	 while [ $temp = 0 ] 
	 do 
            print "How many executables do you need to test (max is 5)?  \c";  read execnum

	    if [ -z $execnum ]
	       then
		  continue
            fi

	    if [ $execnum = q - o $execnum = Q ]
	       then
	         break 
	    fi
	    
	    if [[ $execnum != 1 && $execnum != 2 && $execnum != 3 && $execnum != 4 && \
	          $execnum != 5 ]] 
               then
                  print "Number of executables must be between one and five..."
               else
	          temp=1 
            fi

	 done

	 if [ $execnum = q -o $execnum = Q ]
	    then
	       if [ -f $AG_home/$assignment/Testground/account_names.tmp ]
	          then
	             mv $AG_home/$assignment/Testground/account_names.tmp $AG_home/$assignment/Testground/account_names 2> /dev/null
               fi
	       continue
         fi 
	
	 choice=0

         while [[ $choice != a && $choice != A && $choice != b && $choice != B ]] 
         do
            print ""
            print "Would you like to:   a) swap out main in the student's file to be"                                         
            print "                        replaced with a different provided main"
            print "                     b) use drivers in the Provided area"
            print ""
            print "Enter a, or b:  \c"; read choice
	    if [ $choice = q ]
	       then
		  break
	    fi
            if [[ $choice != a && $choice !=  A && $choice != b &&  $choice != B ]]
               then
                  print "Choice needs to be a or b."
            fi
         done

	 if [ $choice = q ]
	    then
	       if [ -f $AG_home/$assignment/Testground/account_names.tmp ]
	          then
	             mv $AG_home/$assignment/Testground/account_names.tmp $AG_home/$assignment/Testground/account_names
               fi
	       continue
	 fi

	 print ""
	 count=1
         while [ $count -le $execnum ]
	 do
	    print "What is the name of executable $count?  \c" ; read exec
	    print ""
	    if [ $exec = q -o $exec = Q ]
	       then
		  break
	    fi

	    ex[$(($count))]=$exec
            if [ $choice = a -o $choice = A ]
               then
		  print "What is the name of file $count corresponding to ${ex[$(($count))]} in"
                  print "which you want to swap out main?  \c";  read file$count
		  print ""
            fi
            let count=$count+1 
	 done

	 if [ $exec = q -o $exec = Q ]
	    then
	       if [ -f $AG_home/$assignment/Testground/account_names.tmp ]
	          then
	             mv $AG_home/$assignment/Testground/account_names.tmp $AG_home/$assignment/Testground/account_names
               fi
	       continue
	 fi

	 print "Which account would you like to use as the key?  \c"
         read key
	 print ""
	 if [ $key = q ]
	    then
	       if [ -f $AG_home/$assignment/Testground/account_names.tmp ]
	          then
	             mv $AG_home/$assignment/Testground/account_names.tmp $AG_home/$assignment/Testground/account_names
               fi
	       continue
         fi

	 if [ ! -f $AG_home/$assignment/Turnin/$key* ]
	    then
	       print "You must collect $key's account or move its file to the"
	       print "Turnin directory before proceeding."
	       continue
         fi

	 #wc -l ~/.autograder.rc | grep '3' > /dev/null
	 if [ ! "acs_course" = "" ]
	    then
	       print "Before each assignment is compiled during testing, would you like"
	       print "to rename the file to something other than the student's account name (y/n)?  \c"
	       read rename
	       if [ $rename = y -o $rename = Y ]
		  then
		     print "What would you like to rename the file to be?  \c"
		     read file_rename
		     export file_rename
		     print ""
               fi
         fi

	 index=1
	 while [ $count -gt 1 ]
	 do
	    input_file_check ${ex[$(($index))]}
	    result=$?
	    if (( ! $result ))
	       then
		  break
	    fi

	    let index=$index+1
	    let count=$count-1
	    print ""
         done

	 if (( ! $result )) 
	    then
	       continue
	 fi
	 
	 if [ $choice = a -o $choice = A ]
	    then
	       if [ ! -f $AG_home/$assignment/Testground/TestFiles/*.driver ]
		  then
		     print "Autograder cannot continue without any driver files in your"
		     print "TestFiles directory..." 
		     continue
		  else
	             test_case_comp driver
		     result=$?
		     if (( ! $result ))
			then
			   print "Would you like to make drivers now(y/n)?\c  "
			   read input
			   if [ $input = y -o $input = Y ]
			      then
				 make test_case driver
			   
			      else
                                 if [ $input = q ]
                                    then
		                       mv $AG_home/$assignment/Testground/account_names.tmp \
                                       $AG_home/$assignment/Testground/account_names 2> /dev/null
                                       continue
                                 fi
                                 print "You must have all drivers present in \
                                 order to continue"
				 continue
			   fi
		     fi
	       fi 
         fi
         
	 test_case_comp desc
	 if [ $? = 0 ]
	    then
	       print ""
         fi

	 if [ -f $AG_home/$assignment/Testground/TestFiles/*.freshin ]
	    then
	       print "What command would you like to have run after each of your "
	       print ""freshin" test cases?  \c"
	       read freshin
	       export freshin
	       print ""
         fi

               print "Would you like to run the autograder in no hangup mode (y/n)?  \c"
               read hangup
               if [ $hangup = q -o $hangup = Q ]
                  then
		     mv $AG_home/$assignment/Testground/account_names.tmp \
                         $AG_home/$assignment/Testground/account_names 2> /dev/null
                     continue
               fi

	       print ""
	       print "Would you like to be notified when it's done (y/n)? \c"
	       read notify
	       if [ -z $notify ]
	       then
		  email="none"
	       elif [ $notify = y -o $notify = Y ]
	       then
		  print "Please enter your email address: \c"
		  read email
	       elif [ $notify = q -o $notify = Q ]
	       then
		     continue
               elif [ $notify = n -o $notify = N ]
	       then
		  email="none"
	       fi

               print ""      
               print "Start autograder(y/n)?  \c"; read start
               if [ $start = y -o $start = Y ]
                  then
                     case "$execnum" in
                        1)
                           if [ $choice = a -o $choice = A ]
                              then
                                 if [ $hangup = y -o $hangup = Y ]
                                    then
                                    nohup $AG_scripts/test_all $email $assignment $key $choice ${ex[1]} $file1 > /dev/null &
                                    print ""
                                    print "The Autograder is running in no hangup mode.  You can safely logout"
                                    print "and autograding will continue."
                                    sleep 2
                                    else
                                    $AG_scripts/test_all $email $assignment $key $choice ${ex[1]} $file1
                                 fi
                              else
                                 if [ $hangup = y -o $hangup = Y ]
                                    then
                                    nohup $AG_scripts/test_all $email $assignment $key $choice ${ex[1]} > /dev/null &
                                    print ""
                                    print "The Autograder is running in no hangup mode.  You can safely logout"
                                    print "and autograding will continue."
                                    sleep 2
                                    else
                                    $AG_scripts/test_all $email $assignment $key $choice ${ex[1]}
                                 fi
                           fi
                           ;;
                        2)
                           if [ $choice = a -o $choice = A ]
                              then
                                 if [ $hangup = y -o $hangup = Y ]
                                    then
                                    nohup $AG_scripts/test_all $email $assignment $key $choice ${ex[1]} \ 
                                     $file1 ${ex[2]} $file2 > grader &
                                    print ""
                                    print "The Autograder is running in no hangup mode.  You can safely logout"
                                    print "and autograding will continue."
                                    sleep 2
                                    else
                                    $AG_scripts/test_all $email $assignment $key $choice ${ex[1]} \ 
                                     $file1 ${ex[2]} $file2
                                 fi
                              else
                                 if [ $hangup = y -o $hangup = Y ]
                                    then
                                    nohup $AG_scripts/test_all $email $assignment $key $choice ${ex[1]} ${ex[2]}  > /dev/null &
                                    print ""
                                    print "The Autograder is running in no hangup mode.  You can safely logout"
                                    print "and autograding will continue."
                                    sleep 2
                                    else
                                    $AG_scripts/test_all $email $assignment $key $choice ${ex[1]} ${ex[2]}
                                 fi				 
                           fi
                           ;;
                        3)
                           if [ $choice = a -o $choice = A ]
                              then
                                 if [ $hangup = y -o $hangup = Y ]
                                    then                                 
                                    nohup $AG_scripts/test_all $email $assignment $key $choice ${ex[1]} \
                                      $file1 ${ex[2]} $file2 ${ex[3]} $file3 > /dev/null &
                                    print ""
                                    print "The Autograder is running in no hangup mode.  You can safely logout"
                                    print "and autograding will continue."
                                    sleep 2
                                    else
                                    $AG_scripts/test_all $email $assignment $key $choice ${ex[1]} \
                                    $file1 ${ex[2]} $file2 ${ex[3]} $file3
                                 fi
                              else
                                 if [ $hangup = y -o $hangup = Y ]
                                    then                                                                       
                                    nohup $AG_scripts/test_all $email $assignment $key $choice ${ex[1]} \
                                      ${ex[2]} ${ex[3]} > /dev/null &
                                    print ""
                                    print "The Autograder is running in no hangup mode.  You can safely logout"
                                    print "and autograding will continue."
                                    sleep 2
                                    else
                                    $AG_scripts/test_all $email $assignment $key $choice ${ex[1]} \
                                    ${ex[2]} ${ex[3]}
                                 fi
                           fi
                           ;;
                        4)
                           if [ $choice = a -o $choice = A ]
                              then
                                 if [ $hangup = y -o $hangup = Y ]
                                    then                                                                                                                                    
                                    nohup $AG_scripts/test_all $email $assignment $key $choice ${ex[1]} \ 
                                      $file1 ${ex[2]} $file2 ${ex[3]} $file3 ${ex[4]} $file4 > /dev/null &
                                    print ""
                                    print "The Autograder is running in no hangup mode.  You can safely logout"
                                    print "and autograding will continue."
                                    sleep 2
                                    else
                                    $AG_scripts/test_all $email $assignment $key $choice ${ex[1]} \ 
                                      $file1 ${ex[2]} $file2 ${ex[3]} $file3 ${ex[4]} $file4
                                 fi
                              else
                                 if [ $hangup = y -o $hangup = Y ]
                                    then                                                                    
                                    nohup $AG_scripts/test_all $email $assignment $key $choice ${ex[1]} \
                                      ${ex[2]} ${ex[3]} ${ex[4]} > /dev/null &
                                    print ""
                                    print "The Autograder is running in no hangup mode.  You can safely logout"
                                    print "and autograding will continue."
                                    sleep 2
                                    else
                                       $AG_scripts/test_all $email $assignment $key $choice ${ex[1]} \
                                       ${ex[2]} ${ex[3]} ${ex[4]}
                                 fi
                           fi
                           ;;
                        5)
                           if [ $choice = a -o $choice = A ]
                              then
                                 if [ $hangup = y -o $hangup = Y ]
                                    then                                                                                                                                                                     
                                    nohup $AG_scripts/test_all $email $assignment $key $choice ${ex[1]} \
				      $file1 ${ex[2]} $file2 ${ex[3]} $file3 ${ex[4]} \ 
                                          $file4 ${ex[5]} $file5 > /dev/null &
                                    print ""
                                    print "The Autograder is running in no hangup mode.  You can safely logout"
                                    print "and autograding will continue."
                                    sleep 2
                                    else
                                       $AG_scripts/test_all $email $assignment $key $choice ${ex[1]} \
                                          $file1 ${ex[2]} $file2 ${ex[3]} $file3 ${ex[4]} \
                                            $file4 ${ex[5]} $file5
                                 fi
                              else
                                 if [ $hangup = y -o $hangup = Y ]
                                    then                                                                                                                                                                                                      
                                    nohup $AG_scripts/test_all $email $assignment $key $choice ${ex[1]} \
                                          ${ex[2]} ${ex[3]} ${ex[4]} ${ex[5]} > /dev/null &
                                    print ""
                                    print "The Autograder is running in no hangup mode.  You can safely logout"
                                    print "and autograding will continue."
                                    sleep 2
                                    else
                                    $AG_scripts/test_all $email $assignment $key $choice ${ex[1]} \
                                     ${ex[2]} ${ex[3]} ${ex[4]} ${ex[5]}
                                 fi
                           fi         
			   ;;
                     esac   
                  else
		     continue
	       fi

	       if [ $option = u - o $option = U ]
		  then
		     mv $AG_home/$assignment/Testground/account_names.tmp \
                         $AG_home/$assignment/Testground/account_names 2> /dev/null
               fi
	 ;;

      g|G)
         print "Please enter the directory name for the assignment that "
         print "you want to grade:  \c"
         read assignment
	 assn_dir_check $assignment
	 ;;

      d|D)
         dir_error_check 
	 temp=$?
	 if [ $temp = 0 ]
	    then  
	       print You must have all directories in place in order to continue.
	       print Exiting autograder...
	       exit 1
         fi

         if [ ! -f $AG_home/Tutors/*.students.orig ]
            then
               $AG_scripts/grader_assng
         fi

	 if [ -f $AG_home/.distribute.date ]
	     then
	       cat $AG_home/.distribute.date | grep $assignment > /dev/null/
	       if [ $? = 0 ]
	       	  then
	    print "You have already distributed assignment $assignment "
	    print "to graders at a different time ... "
	    print "would you like to save the old directory? (y/n) \c"
	    read resp
	  
	    typeset -i jkcount
            jkcount=0

	    if [ $resp = y -o $resp = Y ]
	       then
	 	 cat $AG_home/Tutors/tas | while read jk_tas 
		 do
	    		while (( 1 ))
            		do
			if [ ! -d $AG_home/Tutors/$jk_tas/$assignment.$jkcount ]
		   	then
		      	  break
			else
			  jkcount=$jkcount+1
			fi
	    	 	done
		     mv $AG_home/Tutors/$jk_tas/$assignment $AG_home/Tutors/$jk_tas/$assignment.$jkcount 2> /dev/null/ 
		 done
	    fi
	   print "Would you like to continue with the distribution? (y/n) \c"	
	   read resp
	        ###############################
	    	if [ $resp = n -o $resp = N ]
	            then
		    continue
		 fi
	        ###############################
	       fi
	 fi  	  

         print ""
         print "Please specify the pathname (from the graders' home directory) of"
         print "where you want to distribute to:  \c"
	 read path
         print $path | grep '~'
         if [ $? = 0 ]
            then
               path=${path##~/}
         fi

         print ""
	 print "Which account was used as the key?  \c"
 	 read key

         $AG_scripts/distribute $assignment $path $key

	 print "$assignment   `date`" >> $AG_home/.distribute.date

         ;;

      n|N)
	 print ""
	 print "Did the $assignment assignment allow the students to work"
	 print "with a partner (y/n)?  \c"
	 read partner
	 if [ $partner = y -o $partner = Y ]
	    then
	       print "Assigning students to graders..."
	       $AG_scripts/partner_assng $assignment
            else
	       $AG_scripts/create_listing $assignment
         fi
	 ;;

      q|Q)
         login=`whoami`
	 set `ufinger $login`
         name=${11}
	 print "Goodbye, $name!"
         print ""
         exit 0
         ;;

        *)
         print Please enter a valid letter!
         ;;
   esac
done
exit 0
