#!/bin/bash
chaine=$( date +%H" heures "%M" minutes")
echo $chaine 
pico2wave -l fr-FR -w /tmp/test.wav "$chaine"
play -q /tmp/test.wav
rm /tmp/test.wav

