all: bashScript pythonPlots

bashScript:
	./script.sh

pythonPlots:
	python3 plot_graph.py	

clean:
	rm -f out/*
	rm -f plots/*

.PHONY: clean
