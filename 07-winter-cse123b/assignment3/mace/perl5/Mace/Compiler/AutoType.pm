# 
# AutoType.pm : part of the Mace toolkit for building distributed systems
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
package Mace::Compiler::AutoType;
#XXX hoping to consolidate this with others!
use strict;

#use Mace::Compiler::Method;

my %messageNums;

use Class::MakeMethods::Template::Hash 
    (
     'new' => 'new',
     'string' => "name",
     'array_of_objects' => ["typeOptions" => { class => "Mace::Compiler::TypeOption" }],
     'array_of_objects' => ["fields" => { class => "Mace::Compiler::Param" }],
     'array_of_objects' => ["methods" => { class => "Mace::Compiler::Method" }],
     'array_of_objects' => ["constructors" => { class => "Mace::Compiler::Method" }],
     'number' => 'line',
     'number' => 'includedline',
     
#Auto Type Options
     'boolean' => 'serialize',
     'boolean' => 'node',
     'string' => 'scoreType',
     'string' => 'scoreField',
     'boolean' => 'failDetect',
     'boolean' => 'equalComparable',
     'boolean' => 'lessthanComparable',
     'boolean' => 'hashComparable',
     'boolean' => 'isPrivate',
     'number' => 'messageNum',
     
#     'array_of_objects' => ["methods" => { class => "Mace::Compiler::Method" }],
#     'array_of_objects' => ["constructors" => { class => "Mace::Compiler::Method" }],
#     'object' => ["destructor" => { class => "Mace::Compiler::Method" }],
#     'array' => "superclasses",
     );

sub toString {
    my $this = shift;
    my $s = $this->name()." {\n";
    #XXX add option printing?
    $s .= join("", map { "    ".$_->toString().";\n" } $this->fields());
    $s .= "  }";
    return $s;
} # toString

sub toForwardDeclare {
    my $this = shift;
    return "class ".$this->name.";\n";
}

sub validateAutoTypeOptions {
  my $this = shift;
  $this->serialize(1);
  $this->node(0);
  $this->equalComparable(0);
  $this->lessthanComparable(0);
  $this->hashComparable(0);
  $this->isPrivate(0);

  my %processedOptions;

  foreach my $option ($this->typeOptions()) {
    Mace::Compiler::Globals::error('bad_type_option', $option->line(), "Cannot define option ".$option->name()." more than once!") unless(++$processedOptions{$option->name()} == 1);
    if($option->name() eq 'serialize') {
      while(my ($name, $value) = each(%{$option->options()})) {
        if($name eq 'no') {
          $this->serialize(0);
        }
        elsif(! $name eq 'yes') {
          Mace::Compiler::Globals::error('bad_type_option', $option->line(), "Invalid option with name $name and value $value");
        }
      }
    }
    elsif($option->name() eq 'node') {
      $this->node(1);
      $this->failDetect(0);
      $this->scoreField("0.0");
      $this->scoreType("double");
      while(my ($name, $value) = each(%{$option->options()})) {
        if($name eq 'score') {
          $this->scoreField($value);
          my $fieldFound = 0;
          foreach my $field ($this->fields()) {
            if($field->name() eq $value) {
              $fieldFound = 1;
              $this->scoreType($field->type()->toString());
              last;
            }
          }
          Mace::Compiler::Globals::error('bad_type_option', $option->line(), "Score field '$value' does not match any auto type fields!") unless($fieldFound);
        }
        elsif($name eq 'tracked_services') {
          Mace::Compiler::Globals::warning('unimplemented', $option->line(), "tracked_services not implemented yet");
        }
        elsif($name eq 'fail_detect') {
          $this->failDetect(1) if(!$value eq 'disabled');
        }
        else {
          Mace::Compiler::Globals::error('bad_type_option', $option->line(), "Invalid option with name $name and value $value");
        }
      }
    }
    elsif($option->name() eq 'comparable') {
      while(my ($name, $value) = each(%{$option->options()})) {
        if($name eq 'equals') {
          $this->equalComparable(1);
          if($value ne "default") { 
            #TODO: Implement var list as non-default
            Mace::Compiler::Globals::error('bad_type_option', $option->line(), "non-default equals comparison not yet supported");
          }
        }
        elsif($name eq 'lessthan') {
          $this->lessthanComparable(1);
          $this->equalComparable(1);
          if($value ne "default") { 
            #TODO: Implement var list as non-default
            Mace::Compiler::Globals::error('bad_type_option', $option->line(), "non-default lessthan comparison not yet supported");
          }
        }
        elsif($name eq 'hashof') {
          $this->hashComparable(1);
            #TODO: Implement some hash generator
          Mace::Compiler::Globals::error('bad_type_option', $option->line(), "hashof comparison not yet supported");
        }
        else {
          Mace::Compiler::Globals::error('bad_type_option', $option->line(), "Invalid option with name $name and value $value");
        }
      }
    }
      elsif ($option->name() eq 'private') {
	  $this->isPrivate(1);
	  while(my ($name, $value) = each(%{$option->options()})) {
	      if($name eq 'no') {
		  $this->isPrivate(0);
	      }
	      elsif(! $name eq 'yes') {
		  Mace::Compiler::Globals::error('bad_type_option', $option->line(), "Invalid option with name $name and value $value");
		}
	  }
      }
    else {
      Mace::Compiler::Globals::error('bad_type_option', $option->line(), "Invalid option ".$option->name());
    }
  }

  foreach my $field ($this->fields()) {
    $field->validateTypeOptions({'serialize'=>1});
  }
}

sub setNumber {
  my $this = shift;
  my $last = shift;
  my $name = $this->name();

  if ($this->messageNum() != -1) {
    $$last = $this->messageNum();
  } else {
    $$last++;
    $this->messageNum($$last);
  }
  if ($messageNums{$$last}) {
    Mace::Compiler::Globals::error('message_num_error', $this->line(), "Message '$name' with number '$$last' duplicates the number for message '".$messageNums{$$last}."'");
  }
  $messageNums{$$last} = $name;

  return $$last;
}

sub validateMessageOptions {
  my $this = shift;
  #XXX Need to parse message options

  my %processedOptions;

  $this->serialize(1);
  $this->node(0);
  $this->messageNum(-1);

  foreach my $option ($this->typeOptions()) {
    Mace::Compiler::Globals::error('bad_type_option', $option->line(), "Cannot define option ".$option->name()." more than once!") unless(++$processedOptions{$option->name()} == 1);
    if ($option->name() eq "number") {
      while(my ($name, $value) = each(%{$option->options()})) {
        if($this->messageNum() == -1 and $name =~ /^\d*$/) {
          $this->messageNum($name);
        }
        else {
          Mace::Compiler::Globals::error('bad_type_option', $option->line(), "The number option must be set only to a number, invalid at '$name'");
        }
      }
    }
    else {
      Mace::Compiler::Globals::error('bad_type_option', $option->line(), "Invalid option ".$option->name()." (no options yet supported for messages)");
    }
  }
  foreach my $field ($this->fields()) {
    $field->validateTypeOptions({'serialize'=>0});
  }
}

sub toAutoTypeString {
    my $this = shift;
    my $name = $this->name();
    my %args = @_;
  
    my $s = "class $name : public mace::PrintPrintable";
    if($this->serialize()) { $s .= ", public mace::Serializable"; }
    $s .= " {\n";
    my $initSerialize = "";
    if($this->serialize()) {
	$s .= "private:
    mutable size_t serializedByteSize;
    ";
	$initSerialize = "serializedByteSize(0)";
	if ($this->node() || $this->count_fields()) {
	    $initSerialize .= ",";
	}
    }

    if($this->node()) {
      my $scoreType = $this->scoreType();
      my $scoreField = $this->scoreField();
      $s .= <<END;
  private:
    MaceKey _id;
  public:
END
      if($this->failDetect()) {
        $s .= <<END;
    double _time_last_heard;
END
      }
      $s .= <<END;
    const MaceKey& getId() const { return _id; }
    $scoreType getScore() const { return $scoreField; }
END
    }
    $s .= <<END;
  public:
END

    if ($this->isPrivate()) {
	$s .= "private:\n";
    }
    
    $s .= join("", map { "    ".$_->toString(nodefaults => 1).";\n    ".$_->type()->toString()." get_".$_->name()."() const { return ".$_->name()."; }\n" } $this->fields());

    if ($this->isPrivate()) {
	$s .= "public:\n";
    }
    

    if ((not $this->count_fields()) or grep(/0/, map{$_->hasDefault()} $this->fields())) {
	#Basic Constructor
	if($this->node()) {
	    $s .= "    $name(MaceKey __id = MaceKey()) : $initSerialize _id(__id)";
	    if($this->failDetect) {
		$s .= q{, _time_last_heard(0.0)};
	    }
	    if(scalar(@{$this->fields})) {
		$s .= q{, };
	    }
	} else {
	    $s .= "    $name()";
	    if(scalar(@{$this->fields})) {
		$s .= q{ : };
	    }
	}
	$s .= join(', ', map {
	    $_->name."(".($_->hasDefault() ? $_->default() : "") . ")"
	    } @{$this->fields()} );
	$s .= " {}\n";
    }

    #Constructor with parameters
    if($this->count_fields()) {
      if($this->node()) {
        $s .= "    $name(MaceKey __id";
	if (!grep(/0/, map{$_->hasDefault()} $this->fields())) {
	    $s .= " = MaceKey()";
	}
        if(scalar(@{$this->fields})) {
          $s .= ', ';
        }
      } else {
        $s .= "    $name(";
      }
      $s .= join(', ', map {$_->type->toString(paramconst=>1, paramref=>1) . " _".$_->name().  ($_->hasDefault() ? " = " . $_->default() : "") } @{$this->fields()} );
      $s .= ") ";
      if($this->node()) {
        $s .= ": $initSerialize _id(__id)";
        if($this->failDetect()) {
          $s .= ", _time_last_heard(0.0)";
        }
        if(scalar(@{$this->fields})) {
          $s .= ", ";
        }
      }
      else {
        if(scalar(@{$this->fields})) {
          $s .= ": $initSerialize ";
        }
      }
      $s .= join(', ', map { $_->name."(_".$_->name.")" } @{$this->fields()} );
      $s .= " {}\n";
    }

    for my $m ($this->constructors) {
      $s .= $m->toString(noreturn => 1, body=>1) . "\n";
    }

    #Deconstructor
    $s .= "virtual ~$name() {}\n";

    $s .= join("\n", map { $_->toString(body=>0).";" } $this->methods());
    if ($args{tracelevel} > 1) {
      $s .= "\n".join("\n", grep(/./, map{if($_->returnType()->type() ne "void") { $_->toString(methodprefix=>"__mace_log_").";"}} $this->methods()));
    }

    #Print method and state method
    my $nodeString = "";
    my $failDetectString = "";
    if($this->node) {
      $nodeString .= " << _id";
      if(scalar(@{$this->fields})) {
        $nodeString .= qq{ << ", "};
      }
      if($this->failDetect) {
        $failDetectString .= qq{<< ", time_last_heard=" << _time_last_heard};
      }
    }
    my $printFields = join(qq{\n__out << ", ";\n}, grep(/./, map{ $_->toPrint("__out") } $this->fields()));
    my $printFieldState = join(qq{\n__out << ", ";\n}, grep(/./, map{ $_->toPrintState("__out") } $this->fields()));
    $s .= <<END;
    void print(std::ostream& __out) const {
      __out << "$name(" $nodeString $failDetectString;
      $printFields
      __out << ")";
    }
    void printState(std::ostream& __out) const {
      __out << "$name(" $nodeString;
      $printFieldState
      __out << ")";
    }
END

    if($this->serialize()) {
      #serialize method
      $s .= <<END;
    void serialize(std::string& str) const {
      serializedByteSize = str.size();
END
      if($this->node()) {
        $s .= <<END;
      mace::serialize(str, &_id, _id);
END
      }
      map { $s .= $_->toSerialize("str")."\n" } $this->fields();
      $s .= <<END;
      serializedByteSize = str.size() - serializedByteSize;
    }
END
      #deserialize method
      $s .= <<END;
    int deserialize(std::istream& in) throw(mace::SerializationException) {
      serializedByteSize = 0;
END
      if($this->node()) {
        $s .= <<END;
      serializedByteSize += mace::deserialize(in, &_id, _id);
END
      }
      map { $s .= $_->toDeserialize("in", prefix => "serializedByteSize += ")."\n" } $this->fields();
      $s .= <<END;
      return serializedByteSize;
    }

    size_t getSerializedSize() const {
      return serializedByteSize;
    }
END
    }

    if($this->equalComparable()) {
      my $fieldscompare = join("\n", map{ "if(this->".$_->name." != other.".$_->name.") { return false; }" } $this->fields())."\n";
      $s .= <<END;
    bool operator== (const $name& other) const {
      $fieldscompare
      return true;
    }
    bool operator!= (const $name& other) const {
      return !(*this == other);
    }
END
    }

    if($this->lessthanComparable()) {
      my $fieldscompare = join("\n", map{ "if(this->".$_->name." < other.".$_->name.") { return true; }
                                           if(this->".$_->name." != other.".$_->name.") { return false; }" } $this->fields())."\n";
      $s .= <<END;
    bool operator< (const $name& other) const {
      $fieldscompare
      return false; //they are actually equal
    }
END
    }

    $s .= "  };";
    return $s;
} # toAutoTypeString

sub defineAutoTypeMethods {
    my $this = shift;
    my %opts = @_;
    my $name = $this->name();
  
    my $s = "";
    for my $m ($this->methods()) {
        my $under = "";
        my $fnName = $m->name();
        if ($opts{logparams} and $m->returnType->type() ne "void") {
          my $paramlist = join(",", map{ $_->toString(notype=>1, nodefaults=>1) } $m->params());
          $s .= $m->toString(methodprefix=>"${name}::", nodefaults=>1);
          $s .= qq/ {
            ADD_SELECTORS("${name}::$fnName");
            return mace::logVal(__mace_log_$fnName($paramlist), macecompiler(0), "Return value: ");
          }
          /;
          $under = "__mace_log_";
        }
        $s .= $m->toString(methodprefix=>"${name}::${under}", body=>1, prepare=>1, nodefaults=>1, add_selectors=>$name, %opts);
    }
    return $s;
} # defineAutoTypeMethods

sub toMessageString {
  my $this = shift;
  my $servicename = shift;
  my $messagenum = $this->messageNum();

  my $s;

  $s .= $this->toMessageStructString();
  $s .= $this->toMessageClassString($servicename, $messagenum);

} # toMessageString

sub toMessageStructString {
  my $this = shift;
  my $msgName = $this->name();
  my $fieldStr = "";
  if(scalar(@{$this->fields()})) {
    $fieldStr = "\n" . join("\n", map { $_->toString().';' } $this->fields() ) . "\n";
  }
  my $s = qq{
    struct ${\$this->name}_struct { $fieldStr };
  };
}

sub toMessageClassString {
  my $this = shift;
  my $servicename = shift;
  my $messagenum = shift;
  my $msgName = $this->name();

  my $constructorTwo = "";

  my $fieldsOne = join("", map { ', ' . $_->name() . '(_data_store_->' . $_->name() . ')' } $this->fields());
  if(scalar(@{$this->fields()})) {
    my $fieldsTwoA= join(", ", map { $_->type->toString(paramconst=>1, paramref=>1).' my_'.$_->name() } $this->fields());
    my $fieldsTwoB= join(", ", map { $_->name.'(my_'.$_->name().')' } $this->fields());
    $constructorTwo = qq{$msgName($fieldsTwoA) : _data_store_(NULL), serializedByteSize(0), $fieldsTwoB {}};
  }
  my $fields = "\n".join('', map { $_->type()->toString(paramconst=>1, paramref=>1).' '.$_->name().";\n" } $this->fields());
  my $fieldPrint = join(qq{\n__out << ", ";\n}, grep(/./, map{ $_->toPrint("__out") } $this->fields()) );
  my $serializeFields = join("\n", map{ $_->toSerialize("str") } $this->fields());
  my $deserializeFields = join("\n", map{ $_->toDeserialize("in", prefix => "serializedByteSize += ", 'idprefix' => '_data_store_->') } $this->fields());

  my $s = qq/
    class ${servicename}Service::$msgName : public Message, public mace::PrintPrintable {
      private:
      ${msgName}_struct* _data_store_;
      mutable size_t serializedByteSize;
      mutable std::string serializedCache;
      public:
      $msgName() : _data_store_(new ${msgName}_struct()), serializedByteSize(0) $fieldsOne {}
      $constructorTwo
      virtual ~$msgName() { delete _data_store_; _data_store_ = NULL; }
      $fields
      static const uint8_t messageType = $messagenum;
      static uint8_t getMsgType() { return messageType; }
      uint8_t getType() const { return ${msgName}::getMsgType(); }
      std::string toString() const { return mace::PrintPrintable::toString(); }
      void print(std::ostream& __out) const {
        __out << "${msgName}(";
        $fieldPrint 
        __out << ")";
      }
      size_t getSerializedSize() const {
	if (serializedByteSize == 0 && serializedCache.empty()) {
	  serialize(serializedCache);
	}
	return serializedByteSize;
      }
      void serialize(std::string& str) const {
	if (!serializedCache.empty()) {
	  str.append(serializedCache);
	  return;
	}
        uint8_t messaget = messageType;
	size_t initsize = str.size();
        mace::serialize(str, &messaget, messaget);
        ${serializeFields}
	serializedByteSize = str.size() - initsize;
      }
      int deserialize(std::istream& in) throw (mace::SerializationException) {
	serializedByteSize = 0;
        uint8_t messaget = 255;
        serializedByteSize += mace::deserialize(in, &messaget, messaget);
        ASSERT(messaget == messageType);
        ${deserializeFields}
        return serializedByteSize;
      }
    };
  /;
}

1;
