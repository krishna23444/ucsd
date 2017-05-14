# 
# ServiceVar.pm : part of the Mace toolkit for building distributed systems
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
package Mace::Compiler::ServiceVar;

use strict;

use Mace::Util qw(:all);

use Class::MakeMethods::Template::Hash 
    (
     'new' => 'new',
     'string' => 'name',
     'string' => 'serviceclass', #Later, this should be a reference to a ServiceClass
     'string' => 'service', #If set, generate a header file inclusion . . .
     'array' => 'constructionparams',
     'number' => 'line',
     'number' => 'defineLine',
     'boolean' => 'intermediate',
     'boolean' => 'final',
     'boolean' => 'allHandlers',
     'boolean' => 'doDynamicCast',
     'string' => 'dynamicCastSource',
     'array' => 'handlerList',
     'scalar' => 'registrationUid',
     );

sub toString {
    my $this = shift;
    my $s = "ServiceVariable: ".$this->name()." of type ".$this->serviceclass();
    if ( defined($this->service()) ) {
      $s .= " is initialized by default with a service named ".$this->service()." with parameters (";
      $s .= join(", ", $this->constructionparams());
      $s .= ")";
    }
    return $s;
} # toString

sub toNewMethodCall {
  my $this = shift;
  my $lineno = $this->defineLine();
  if ( $this->doDynamicCast() ) {
      return qq{
      #line $lineno "$Mace::Compiler::Globals::filename"
      dynamic_cast<${\$this->serviceclass()}ServiceClass&>(${\$this->dynamicCastSource()})
      // __INSERT_LINE_HERE__
      }
  }
  return qq{\n#line $lineno "$Mace::Compiler::Globals::filename"\n}.$this->service."_namespace::new_".$this->service."_".$this->serviceclass."(".join(",", $this->constructionparams()).")\n // __INSERT_LINE_HERE__\n";
}

sub returnSCInclude {
    my $this = shift;
    return qq{#include "}.$this->serviceclass().qq{ServiceClass.h"\n}
}

sub toServiceVarDeclares {
  my $this = shift;
  if($this->intermediate()) {
    return "";
  }
  my $serviceClass = $this->serviceclass();
  my $name = $this->name();
  my $lineno = $this->line();
  my $s = qq{ 
              #line $lineno "$Mace::Compiler::Globals::filename" 
              ${serviceClass}ServiceClass& _$name;
              // __INSERT_LINE_HERE__
              registration_uid_t $name; };
}

sub toPrintState {
  my $this = shift;
  if($this->intermediate()) {
    return "";
  }
  my $out = shift;
  my $s = qq{ _${\$this->name}.printState($out); };
  return $s;
}

sub doRegister {
  my $this = shift;
  my $handler = shift;
  if ($this->intermediate()) {
    return 0;
  }
  if ($this->allHandlers) { return 1; }
  elsif(grep($handler =~ /^$_(Handler)?$/, $this->handlerList())) {
    return 1;
  } else {
    return 0;
  }
}

1;
