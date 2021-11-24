#!/bin/bash

N_CORES=$(nproc)

out_philosophes="philosophes_perf_eval.csv"
out_producer_consumer="producer_consumer_perf_eval.csv"
out_reader_writer="reader_writer_perf_eval.csv"
out_ts="test_and_set_perf_eval.csv"
out_tts="test_and_test_and_set"
all_csv=($out_philosophes $out_producer_consumer $out_reader_writer $out_ts $out_tts)


gcc "philosophes".c -o "philosophes" -lpthread
gcc "producer_consumer".c -o "producer_consumer" -lpthread
gcc "reader_writer".c -o "reader_writer" -lpthread

for csv in ${all_csv[@]}; do
  echo "File created: $csv"
  printf "num_threads, total_time_elapsed\n" > $csv
done

for (( i=1; i<=$N_CORES; i++ )) do
   echo "nb of threads : $i"
   for j in {1..5}; do
      #echo "$j th test with $c threads"
      /usr/bin/time -o $out_ts -a -f "$i,%e" ./test_and_set $i
   done
done

for (( i=1; i<=$N_CORES; i++ )) do
   echo "nb of threads : $i"
   for j in {1..5}; do
      #echo "$j th test with $c threads"
      /usr/bin/time -o $out_tts -a -f "$i,%e" ./test_and_test_and_set $i
   done
done

for (( i=1; i<=$N_CORES; i++ )) do
   echo "nb of threads : $i"
   for j in {1..5}; do
      #echo "$j th test with $c threads"
      /usr/bin/time -o $out_philosophes -a -f "$i,%e" ./philosophes $i
   done
done

for (( i=2; i<=$N_CORES; i++ )) do
   echo "nb of threads : $i"
   for j in {1..5}; do
      echo "$j th test with $i threads"
      nb_producer=$(($i/2))
      echo "prod: $nb_producer"
      if [ $(($i%2)) -eq 0 ]
      then
        echo "cons: $nb_producer"
        /usr/bin/time -o $out_producer_consumer -a -f "$i,%e" ./producer_consumer $nb_producer $nb_producer
      else
        nb_consumer=$(($(($i/2))+1))
        echo "cons: $nb_consumer"
        /usr/bin/time -o $out_producer_consumer -a -f "$i,%e" ./producer_consumer $nb_consumer $nb_producer
      fi
   done
done

for (( i=2; i<=$N_CORES; i++ )) do
   echo "nb of threads : $i"
   for j in {1..5}; do
      echo "$j th test with $i threads"
      nb_writer=$(($i/2))
      echo "writer: $nb_writer"
      if [ $(($i%2)) -eq 0 ]
      then
        echo "reader: $nb_writer"
        /usr/bin/time -o $out_reader_writer -a -f "$i,%e" ./reader_writer $nb_writer $nb_writer
      else
        nb_reader=$(($(($i/2))+1))
        echo "reader: $nb_reader"
        /usr/bin/time -o $out_reader_writer -a -f "$i,%e" ./reader_writer $nb_writer $nb_reader
      fi
   done
done

exit 0
