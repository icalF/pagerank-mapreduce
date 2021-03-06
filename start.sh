#!/usr/bin/env sh

hadoop dfs -rmr /user/ical/output-rank
hadoop dfs -mkdir /user/ical/output-rank

hadoop dfs -copyFromLocal -f init /user/ical/init
hadoop dfs -copyFromLocal -f pr /user/ical/pr
hadoop dfs -copyFromLocal -f fin /user/ical/fin

hadoop pipes -D mapreduce.job.name='[Afrizal] Preprocess' \
-D mapreduce.job.maps=122 \
-D mapreduce.job.reduces=122 \
-D mapreduce.pipes.isjavarecordreader=true \
-D mapreduce.pipes.isjavarecordwriter=true \
-program /user/ical/init \
-input /user/ical/input/example \
-output /user/ical/output-rank/init

hadoop pipes -D mapreduce.job.name='[Afrizal] #1' \
-D mapreduce.job.maps=122 \
-D mapreduce.job.reduces=122 \
-D mapreduce.pipes.isjavarecordreader=true \
-D mapreduce.pipes.isjavarecordwriter=true \
-program /user/ical/pr \
-input /user/ical/output-rank/init/* \
-output /user/ical/output-rank/i1

hadoop pipes -D mapreduce.job.name='[Afrizal] #2' \
-D mapreduce.job.maps=122 \
-D mapreduce.job.reduces=122 \
-D mapreduce.pipes.isjavarecordreader=true \
-D mapreduce.pipes.isjavarecordwriter=true \
-program /user/ical/pr \
-input /user/ical/output-rank/i1/* \
-output /user/ical/output-rank/i2

hadoop pipes -D mapreduce.job.name='[Afrizal] #3' \
-D mapreduce.job.maps=122 \
-D mapreduce.job.reduces=122 \
-D mapreduce.pipes.isjavarecordreader=true \
-D mapreduce.pipes.isjavarecordwriter=true \
-program /user/ical/pr \
-input /user/ical/output-rank/i2/* \
-output /user/ical/output-rank/i3

hadoop pipes -D mapreduce.job.name='[Afrizal] Finalize' \
-D mapreduce.job.maps=122 \
-D mapreduce.pipes.isjavarecordreader=true \
-D mapreduce.pipes.isjavarecordwriter=true \
-program /user/ical/fin \
-input /user/ical/output-rank/i3/* \
-output /user/ical/output-rank/final
