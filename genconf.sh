#!/bin/bash

if [[ $1 = "" ]]; then
    echo "Usage: $0 {name}"
    exit -1
fi

echo

SRC=src/$1.cpp
HEADER=headers/$1.h


########################################
# Parses settings.conf and sends var
# names / values to a callback function
#
# Usage: read_settings callback_function
# callback receives key, value as $1, $2
########################################

function parse_settings() {
  cat settings.conf | while read KEY VALUE; do
    [[ $KEY = "#" ]] || [[ $VALUE = "" ]] && continue;
    $1 $KEY $VALUE
  done
}

function debug() {
  echo "  register config var: $1"
}

echo "Generating configuration class"
parse_settings debug

########################################
# callback functions
# for printing from settings.conf
########################################

# defining each var at the beginning of conf.cpp
# defaults to whatever the current settings.conf says
function define_vars () {
  echo "double Conf::$1 = $2;" >>$SRC
}

# this is when settings.conf is being parsed,
# during the file io loop
function print_key_compare() {
  cat >>$SRC <<EOF
        if (key.compare("$1")) {
            settingsFile >> $1;
            continue;
        }
EOF
}


# Log each config val when the app starts
function print_debug() {
  echo >>$SRC
  echo -n "             << \"  $1: \" << $1 << '\n'" >>$SRC
}

# Add each variable to the header
function declare_vars() {
    echo "        static double $1;" >>$HEADER
}


########################################
# generate the header
########################################

echo "Generating header file $HEADER"
cat >$HEADER <<EOF
//
// $( date "+%m-%d-%y" )
// generated $HEADER 
//

#ifndef _Conf
#define _Conf

#include <string>

class Conf {
    public:
        static void init(std::string);
EOF

parse_settings declare_vars

cat >>$HEADER <<EOF
};

#endif
EOF


########################################
# done with the header
# generate the source file
########################################



echo "Generating source file $SRC"

cat >$SRC <<EOF
//
// generated file: $SRC" >>$SRC
// $( date "+%d/%m/%y" )
//

#include <iostream>
#include <fstream>
#include <string>
#include "$1.h"

EOF

parse_settings define_vars

cat >>$SRC <<EOF
void Conf::init(std::string filename) {

    std::ifstream settingsFile(filename.c_str());
    std::string key;

    while (settingsFile >> key) {

        if (key.compare("#")) {
            std::getline (std::cin,key);
            continue;
        }
EOF

parse_settings print_key_compare

echo "}" >>$SRC
echo >> $SRC
echo -n "    std::cout << \"finished reading \" << filename << '\n'" >>$SRC

parse_settings print_debug
echo ";" >>$SRC
cat >>$SRC <<EOF
    settingsFile.close();
}

EOF

echo "Done generating configuration class"
echo
