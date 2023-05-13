#!/bin/bash
####################################################################################################
# Copyright (c) 2022. Dennis Oberst
####################################################################################################

# This script is batch converting all wav files inside a directory to 16 bit, as this is the maximum bit depth we
# can support

shopt -s globstar nullglob
for f in *.wav **/*.wav
do
  ffmpeg -i "$f" -acodec pcm_s16le -ar 44100 -ac 1  "${f%.wav}.new.wav"
  mv -f "${f%.wav}.new.wav" "$f"
done