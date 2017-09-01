#!/bin/bash
# hadd recursive merger by Andre Goerres and Andreas Herten
# 14.3.2015
 
function fileToArray {
    output=""
    while read line; do
        output=$output" "$line
    done < $1
    echo $output
}
 
tempFile="temp.root"
 
printHelp() {
    echo "./mergeRootFilesByTxt.sh <listOfFiles.txt> [filesAtOnce] [output.root]"
    echo
    echo "Merges ROOT files from <listOfFiles.txt> with hadd."
    echo "Files are recursively merged in sets of [filesAtOnce], default: 1."
    echo "The output file is [output.root], defaulting to "
    echo "<listOfFiles.txt>_merged.root."
    exit
}

printEntryMesssage() {
    echo "> ./mergeRootFilesByTxt.sh started at $(date)"
    echo ">   List of files: $1."
    echo ">   Merging $2 files at once."
    echo ">   Output: $3."
}

if test "$1" == "-h"; then
    printHelp
fi

if test "$1" != ""; then
    listOfFiles=$1
else
    printHelp
fi
 
filesAtOnce="1"
if test "$2" != ""; then
    if [[ $2 =~ ^[0-9]+$ ]]; then
        filesAtOnce=$2
    fi
fi

outputFileName=$listOfFiles"_merged.root"
if test "$3" != ""; then
    outputFileName=$3
fi

printEntryMesssage $listOfFiles $filesAtOnce $outputFileName

listOfFiles=$(fileToArray $listOfFiles)
filesArray=( $listOfFiles )
arraylength=${#filesArray[@]}

master=$outputFileName


currentEntryList=()
for (( i = 0; i < ${arraylength}; i+=1)); do
    currentEntry=${filesArray[0]}
    currentEntryList+=($currentEntry)
    if [[ ${#currentEntryList[@]} -ge $filesAtOnce || $((i+1)) -eq $arraylength ]]; then
        if [[ "$i" -lt $filesAtOnce ]]; then
            echo "> hadd -f $master ${currentEntryList[@]}"
            hadd -f $master ${currentEntryList[@]}
        else
            # echo "mv $master 'temp.root'"
            mv $master "temp.root"
            echo "> hadd -f $master ${currentEntryList[@]} $tempFile"
            hadd -f $master ${currentEntryList[@]} "temp.root"
        fi

        currentEntryList=()
    fi

    filesArray=( ${filesArray[@]/$currentEntry} )
done

if [[ -e $tempFile ]]; then
    rm $tempFile
fi

echo "> Merging done. Final file written: ${master}"
echo ">"
echo "> ------------------------------------------"

