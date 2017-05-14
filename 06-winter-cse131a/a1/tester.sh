#!/bin/bash

make new
for i in Tests/*; do
  ./scan $i;
  xmldiff -nu $i.xml Outputs/${i/Tests\//}.xml
done
