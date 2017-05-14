#


prep arm
prep mentor
prep seamless

#setenv CVE_HOME /saumya/seamless/tools/cve_home.${VCO}
#setenv PATH ${CVE_HOME}/bin:${PATH}

#setenv LD_LIBRARY_PATH .:${CVE_HOME}/lib:${LD_LIBRARY_PATH}
setenv SHLIB_PATH ${CVE_HOME}/lib

unlimit

setenv hom `pwd`
