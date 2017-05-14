#! /bin/csh -xe

#
# This script compiles the software & hardware, and runs the seamless CVE program
#

set systemdir=p4

# make sure we're in the right directory
if ( `basename $PWD` != $systemdir ) then
echo "ERROR: Not in '$systemdir' directory. Exiting."
exit
endif

# set the $hom variable
setenv hom `pwd`

# 'prompt' must be exported because the prep scripts expects to be executed
# from an interactive shell, but running from a script is noninteractive.
# So we trick the script by setting the prompt variable.
setenv prompt 'prompt> '

# compile software
csh -xec 'prep arm; cd sw; ./compile.sh'

# compile hardware
csh -xec 'prep mentor; prep seamless; cd hw; ./hwcompile'

# run simulation
csh -xec 'prep mentor; prep seamless; cd scripts; ./run'

