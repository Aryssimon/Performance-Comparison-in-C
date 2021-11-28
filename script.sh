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

echo "Start compiling C files"

gcc "main_philosophes".c "src/philosophes".c "src/test_and_test_and_set".c -o "out/philosophes" -lpthread
gcc "main_producer_consumer".c "src/producer_consumer".c "src/test_and_test_and_set".c "src/our_semaphore".c -o "out/producer_consumer" -lpthread
gcc "main_reader_writer".c "src/reader_writer".c "src/test_and_test_and_set".c "src/our_semaphore".c -o "out/reader_writer" -lpthread
gcc "main_our_philosophes".c "src/philosophes".c "src/test_and_test_and_set".c -o "out/our_philosophes" -lpthread
gcc "main_our_producer_consumer".c "src/producer_consumer".c "src/test_and_test_and_set".c "src/our_semaphore".c -o "out/our_producer_consumer" -lpthread
gcc "main_our_reader_writer".c "src/reader_writer".c "src/test_and_test_and_set".c "src/our_semaphore".c -o "out/our_reader_writer" -lpthread
gcc "main_test_and_set".c "src/test_and_set".c -o "out/test_and_set" -lpthread
gcc "main_test_and_test_and_set".c "src/test_and_test_and_set".c -o "out/test_and_test_and_set" -lpthread

echo "Files successfully compiled"
echo " "

for csv in ${all_csv[@]}; do
  echo "File created: $csv"
  printf "num_threads, total_time_elapsed\n" > $csv
done

echo " "
echo "Start computing the executions times"

echo " "
echo "Philosophes:"
for (( i=1; i<=$N_CORES; i++ )) do
   echo "#Threads = $i"
   for j in {1..5}; do
      /usr/bin/time -o $out_philosophes -a -f "$i,%e" ./out/philosophes $i
   done
done

echo " "
echo "Our_Philosophes:"
for (( i=1; i<=$N_CORES; i++ )) do
   echo "#Threads = $i"
   for j in {1..5}; do
      /usr/bin/time -o $out_our_philosophes -a -f "$i,%e" ./out/our_philosophes $i
   done
done

echo " "
echo "Test_and_set:"
for (( i=1; i<=$N_CORES; i++ )) do
   echo "#Threads = $i"
   for j in {1..5}; do
      /usr/bin/time -o $out_ts -a -f "$i,%e" ./out/test_and_set $i
   done
done

echo " "
echo "Test_and_test_and_set:"
for (( i=1; i<=$N_CORES; i++ )) do
   echo "#Threads = $i"
   for j in {1..5}; do
      /usr/bin/time -o $out_tts -a -f "$i,%e" ./out/test_and_test_and_set $i
   done
done

echo " "
echo "Producer_Consumer"
for (( i=2; i<=$N_CORES; i++ )) do
   echo "#Threads = $i"
   for j in {1..5}; do
      nb_producer=$(($i/2))
      if [ $(($i%2)) -eq 0 ]
      then
        /usr/bin/time -o $out_producer_consumer -a -f "$i,%e" ./out/producer_consumer $nb_producer $nb_producer
      else
        nb_consumer=$(($(($i/2))+1))
        /usr/bin/time -o $out_producer_consumer -a -f "$i,%e" ./out/producer_consumer $nb_consumer $nb_producer
      fi
   done
done

echo " "
echo "Our_Producer_Consumer"
for (( i=2; i<=$N_CORES; i++ )) do
   echo "#Threads = $i"
   for j in {1..5}; do
      nb_producer=$(($i/2))
      if [ $(($i%2)) -eq 0 ]
      then
        /usr/bin/time -o $out_our_producer_consumer -a -f "$i,%e" ./out/our_producer_consumer $nb_producer $nb_producer
      else
        nb_consumer=$(($(($i/2))+1))
        /usr/bin/time -o $out_our_producer_consumer -a -f "$i,%e" ./out/our_producer_consumer $nb_consumer $nb_producer
      fi
   done
done

echo " "
echo "Reader_Writer"
for (( i=2; i<=$N_CORES; i++ )) do
   echo "#Threads = $i"
   for j in {1..5}; do
      nb_writer=$(($i/2))
      if [ $(($i%2)) -eq 0 ]
      then
        /usr/bin/time -o $out_reader_writer -a -f "$i,%e" ./out/reader_writer $nb_writer $nb_writer
      else
        nb_reader=$(($(($i/2))+1))
        /usr/bin/time -o $out_reader_writer -a -f "$i,%e" ./out/reader_writer $nb_writer $nb_reader
      fi
   done
done

echo " "
echo "Our_Reader_Writer"
for (( i=2; i<=$N_CORES; i++ )) do
   echo "#Threads = $i"
   for j in {1..5}; do
      nb_writer=$(($i/2))
      if [ $(($i%2)) -eq 0 ]
      then
        /usr/bin/time -o $out_our_reader_writer -a -f "$i,%e" ./out/our_reader_writer $nb_writer $nb_writer
      else
        nb_reader=$(($(($i/2))+1))
        /usr/bin/time -o $out_our_reader_writer -a -f "$i,%e" ./out/our_reader_writer $nb_writer $nb_reader
      fi
   done
done

echo " "
echo "Times successfully computed"
echo " "

exit 0
