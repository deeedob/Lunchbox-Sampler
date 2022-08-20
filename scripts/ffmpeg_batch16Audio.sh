#!/bin/bash

####################################################################################################
# Copyright (c) 2022. Dennis Oberst
####################################################################################################

# for formats
# ffmpeg -formats | grep PCM

shopt -s globstar nullglob
for f in *.wav **/*.wav
do
  ffmpeg -i "$f" -acodec pcm_s16le -ar 44100 "${f%.wav}.new.wav"
  mv -f "${f%.wav}.new.wav" "$f"
done