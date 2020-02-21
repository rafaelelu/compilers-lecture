#!/usr/bin/bash

# Lab 01
# Rafael Elu
# A01634309

instr_regex='[0-9a-f]{4}:\s+([0-9a-f]{2} )+\s+(\w+)'
func_regex='^([0-9a-f]+) <(.*)>:$'

declare -A instructions
declare -A functions

while read line
do
        if [[ $line =~ $func_regex ]]; then
                if [[ ! -v functions[${BASH_REMATCH[2]}] ]]; then
                        functions[${BASH_REMATCH[2]}]=${BASH_REMATCH[1]}
                fi
        elif [[ $line =~ $instr_regex ]]; then
                if [[ -v instructions[${BASH_REMATCH[2]}] ]]; then
                        (( instructions[${BASH_REMATCH[2]}]++ ))
                else
                        instructions[${BASH_REMATCH[2]}]=1
                fi
        fi
done < $1

echo 'Hi, this is the output of the analysis:'
echo '    You have' ${#instructions[@]} 'kind(s) of instructions in this object file:'

for key in "${!instructions[@]}"
do
        printf '        %-9s: Executed %d times\n' $key ${instructions[$key]}
done

echo '    You have' ${#functions[@]} 'functions:'

for key in "${!functions[@]}"
do
        printf '        %-25s: Located at %s addr\n' $key ${functions[$key]}
done
