#!/bin/bash

FILE="$1"

grep :102400 $FILE | sed -e 's/^:10240000\(..\)\(..\)\(..\)\(..\).*$/\4\3\2\1/g'