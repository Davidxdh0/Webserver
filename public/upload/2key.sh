#!/bin/bash
while true; do
read -rsn1 var
if [[ $var == [a-x] ]] ; then
    /System/Library/CoreServices/Menu\ Extras/User.menu/Contents/Resources/CGSession -suspend
fi
if [[ $var == "z" ]] ; then
    echo "hello"
fi
done
#/System/Library/CoreServices/Menu\ Extras/User.menu/Contents/Resources/CGSession -suspend
#caffeinate -iu -t 25600#!/bin/bash
while true; do
read -rsn1 var
if [[ $var == [a-x] ]] ; then
    /System/Library/CoreServices/Menu\ Extras/User.menu/Contents/Resources/CGSession -suspend
fi
if [[ $var == "z" ]] ; then
    echo "hello"
fi
done
#/System/Library/CoreServices/Menu\ Extras/User.menu/Contents/Resources/CGSession -suspend
#caffeinate -iu -t 25600