#!/bin/bash

tags_list="./tags_list"

find . -name "*.h" -o -name "*.c" -o -name "*.cc" -o -name "*.cpp" -o -name "*.hh" > ${tags_list}

