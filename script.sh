#!/bin/bash

N_CORES=$(nproc)

out_philosophes="out/philosophes_perf_eval.csv"
out_our_philosophes="out/our_philosophes_perf_eval.csv"
out_producer_consumer="out/producer_consumer_perf_eval.csv"
out_our_producer_consumer="out/our_producer_consumer_perf_eval.csv"
out_reader_writer="out/reader_writer_perf_eval.csv"
out_our_reader_writer="out/our_reader_writer_perf_eval.csv"
out_ts="out/test_and_set_perf_eval.csv"
out_tts="out/test_and_test_and_set_perf_eval.csv"
all_csv=($out_philosophes $out_our_philosophes $out_producer_consumer $out_our_producer_consumer $out_reader_writer $out_our_reader_writer $out_ts $out_tts)


gcc "main_philosophes".c -o "out/philosophes" -lpthread
gcc "main_producer_consumer".c -o "out/producer_consumer" -lpthread
gcc "main_reader_writer".c -o "out/reader_writer" -lpthread
gcc "main_our_philosophes".c "src/philosophes".c "src/test_and_test_and_set".c -o "out/our_reader_writer" -lpthread
gcc "main_our_producer_consumer".c "src/producer_consumer".c "src/test_and_test_and_set".c "src/our_semaphore".c -o "out/our_producer_consumer" -lpthread
gcc "main_our_reader_writer".c "src/reader_writer".c "src/test_and_test_and_set".c "src/our_semaphore".c -o "out/our_reader_writer" -lpthread
gcc "main_test_and_set".c -o "out/test_and_set" -lpthread
gcc "main_test_and_test_and_set".c -o "out/test_and_test_and_set" -lpthread

for csv in ${all_csv[@]}; do
  echo "File created: $csv"
  printf "num_threads, total_time_elapsed\n" > $csv
done


for (( i=1; i<=$N_CORES; i++ )) do
   echo "nb of threads : $i"
   for j in {1..5}; do
      #echo "$j th test with $c threads"
      /usr/bin/time -o $out_our_philosophes -a -f "$i,%e" ./out/our_philosophes $i
      /usr/bin/time -o $out_philosophes -a -f "$i,%e" ./out/philosophes $i
      /usr/bin/time -o $out_tts -a -f "$i,%e" ./out/test_and_test_and_set $i
      /usr/bin/time -o $out_ts -a -f "$i,%e" ./out/test_and_set $i
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
        /usr/bin/time -o $out_producer_consumer -a -f "$i,%e" ./out/producer_consumer $nb_producer $nb_producer
        /usr/bin/time -o $out_our_producer_consumer -a -f "$i,%e" ./out/our_producer_consumer $nb_producer $nb_producer
      else
        nb_consumer=$(($(($i/2))+1))
        echo "cons: $nb_consumer"
        /usr/bin/time -o $out_producer_consumer -a -f "$i,%e" ./out/producer_consumer $nb_consumer $nb_producer
        /usr/bin/time -o $out_our_producer_consumer -a -f "$i,%e" ./out/our_producer_consumer $nb_consumer $nb_producer
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
        /usr/bin/time -o $out_reader_writer -a -f "$i,%e" ./out/reader_writer $nb_writer $nb_writer
        /usr/bin/time -o $out_our_reader_writer -a -f "$i,%e" ./out/our_reader_writer $nb_writer $nb_writer
      else
        nb_reader=$(($(($i/2))+1))
        echo "reader: $nb_reader"
        /usr/bin/time -o $out_reader_writer -a -f "$i,%e" ./out/reader_writer $nb_writer $nb_reader
        /usr/bin/time -o $out_our_reader_writer -a -f "$i,%e" ./out/our_reader_writer $nb_writer $nb_reader
      fi
   done
done

exit 0
