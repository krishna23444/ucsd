#!/usr/bin/perl -w
# 
# gen-extradefaults.pl : part of the Mace toolkit for building distributed systems
# 
# Copyright (c) 2006, Charles Killian, James W. Anderson, Ryan Braud, Adolfo Rodriguez
# All rights reserved.
# 
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
# 
#    * Redistributions of source code must retain the above copyright
#      notice, this list of conditions and the following disclaimer.
#    * Redistributions in binary form must reproduce the above copyright
#      notice, this list of conditions and the following disclaimer in
#      the documentation and/or other materials provided with the
#      distribution.
#    * Neither the names of Duke University nor The University of
#      California, San Diego, nor the names of the authors or contributors
#      may be used to endorse or promote products derived from
#      this software without specific prior written permission.
# 
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
# DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE
# FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
# DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
# SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
# CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
# OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE
# USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
# 
# ----END-OF-LEGAL-STUFF----

use strict;
use File::Basename;
use lib ((dirname($0) || "."), (dirname($0) || ".")."/../mace-extras/perl5");
use Mace::Compiler::GeneratedOn;

print Mace::Compiler::GeneratedOn::generatedOnHeader("Extension Specific Makefile Variables and Rules", "commentLinePrefix" => "#");

my $tgts = join(' ', map { '\1.'.$_.'.o' } @ARGV);

print "\n";
print q|DEPENDCCRULE=@set -e; rm -f $@; $(CXX) -MM $(CXXFLAGS) $< > $@.$$$$; sed 's,\($*\)\.o[ :]*,|.$tgts.q| $@ : ,g' < $@.$$$$ > $@; rm -f $@.$$$$|."\n";
print q|DEPENDCRULE=@set -e; rm -f $@; $(CC) -MM $(CXXFLAGS) $< > $@.$$$$; sed 's,\($*\)\.o[ :]*,|.$tgts.q| $@ : ,g' < $@.$$$$ > $@; rm -f $@.$$$$|."\n";

for my $ext (@ARGV) {

#$(PLAINAPPS) : % : %.o $(LIB) $(SERVICE_LIBFILES) $(OTHER_LIBFILES) $(EXTRA_DEP)
#	$(CXX) -pthread -o $@ $< $(LIBS) $(GLOBAL_LINK_FLAGS)

  print "\n# processing extension $ext\n";
  print "${ext}_OBJS = \$(BASE_OBJS:.o=.$ext.o)\n";
  print "${ext}_LINK_LIBS = \$(addsuffix .$ext,\$(addprefix -l,\$(LINK_LIBS)))\n";
  print "${ext}_LINK_LIBFILES = \$(addsuffix .$ext.a,\$(LINK_LIBFILESPRE))\n";
  print "%.$ext.o : %.cc\n\t" . '$(CXX) -c $(CPPFLAGS) $(CXXFLAGS) $('.$ext.'_FLAGS) $< -o $@'."\n";
  print "%.$ext.o : %.c\n\t" . '$(CC) -c $(CPPFLAGS) $(CFLAGS) $('.$ext.'_FLAGS) $< -o $@'."\n";
  print "\$(LIBDIR)lib\$(LIBNAME).$ext.a : \$(${ext}_OBJS)\n\tar rc \$@ \$^\n";
  print "${ext}_APPS = \$(addsuffix .${ext},\$(APPS))\n";
  print "${ext}_APP_OBJS = \$(addsuffix .${ext}.o,\$(APPS))\n";
  print "\$(${ext}_APPS) : %.$ext : %.$ext.o \$(${ext}_LINK_LIBFILES)\n\t\$(CXX) \$(GENERIC_LINK_FLAGS_PRE) \$(${ext}_LINK_FLAGS) -o \$@ \$< \$(${ext}_LINK_LIBS) \$(GLOBAL_LINK_FLAGS) \$(GENERIC_LINK_FLAGS_POST)\n";
  print "CLEAN_LIBS+=\$(LIBDIR)lib\$(LIBNAME).$ext.a\n";
  print "CLEAN_OBJS+=\$(${ext}_OBJS) \$(${ext}_APP_OBJS)\n";
  print "CLEAN_APPS+=\$(${ext}_APPS)\n";
  print qq|ifeq "\$(USE_$ext)" "1"\nTYPES+=${ext}\nALL_OBJS+=\$(${ext}_OBJS) \$(${ext}_APP_OBJS)\nALL_LIBS+=\$(LIBDIR)lib\$(LIBNAME).$ext.a\nALL_APPS+=\$(addsuffix .${ext},\$(APPS))\nendif\n|;

}

