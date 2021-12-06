all: compile execute plots

compile:
	@echo "Start compiling C files"
	@gcc "main_philosophes".c "src/philosophes".c "src/test_and_test_and_set".c "src/test_and_set".c -o "out/philosophes" -lpthread
	@gcc "main_producer_consumer".c "src/producer_consumer".c "src/test_and_test_and_set".c "src/test_and_set".c "src/tts_semaphore".c "src/ts_semaphore".c -o "out/producer_consumer" -lpthread
	@gcc "main_reader_writer".c "src/reader_writer".c "src/test_and_test_and_set".c "src/tts_semaphore".c "src/test_and_set".c "src/ts_semaphore".c -o "out/reader_writer" -lpthread
	@gcc "main_ts_philosophes".c "src/philosophes".c "src/test_and_set".c "src/test_and_test_and_set".c -o "out/ts_philosophes" -lpthread
	@gcc "main_ts_producer_consumer".c "src/producer_consumer".c "src/test_and_set".c "src/ts_semaphore".c "src/test_and_test_and_set".c "src/tts_semaphore".c -o "out/ts_producer_consumer" -lpthread
	@gcc "main_ts_reader_writer".c "src/reader_writer".c "src/test_and_set".c "src/ts_semaphore".c "src/test_and_test_and_set".c "src/tts_semaphore".c -o "out/ts_reader_writer" -lpthread
	@gcc "main_tts_philosophes".c "src/philosophes".c "src/test_and_test_and_set".c "src/test_and_set".c -o "out/tts_philosophes" -lpthread
	@gcc "main_tts_producer_consumer".c "src/producer_consumer".c "src/test_and_test_and_set".c "src/tts_semaphore".c "src/test_and_set".c "src/ts_semaphore".c -o "out/tts_producer_consumer" -lpthread
	@gcc "main_tts_reader_writer".c "src/reader_writer".c "src/test_and_test_and_set".c "src/tts_semaphore".c "src/test_and_set".c "src/ts_semaphore".c -o "out/tts_reader_writer" -lpthread
	@gcc "main_test_and_set".c "src/test_and_set".c -o "out/test_and_set" -lpthread
	@gcc "main_test_and_test_and_set".c "src/test_and_test_and_set".c -o "out/test_and_test_and_set" -lpthread
	@echo "Files successfully compiled"
	@echo " "

execute:
	@./script.sh

plots:
	@python3 plot_graph.py

clean:
	@rm -f out/*
	@rm -f plots/*
	@echo "out/ and plots/ folders cleaned"

.PHONY: clean plots
