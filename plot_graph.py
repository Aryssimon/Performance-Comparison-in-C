from sys import argv
import numpy as np
import matplotlib.pyplot as plt
from numpy.lib.function_base import average

def show_plots(n_threads, averages, stdevs, name, labels):
  """
  calculer la moyenne
  calculer les ecarts-types
  plot le tout avec matplotlib
  """
  fig1 = plt.figure()
  std_scaling = 1
  colors = ["red", "green"]
  for i in range(len(averages)):
      plt.errorbar(n_threads, averages[i], yerr=[std * std_scaling for std in stdevs[i]], ecolor=colors[i], color=colors[i], capsize=10.0, label=labels[i])

  plt.legend()
  plt.xlim(0, max(n_threads) + 1)

  plt.xlabel("Nombre de threads")
  plt.ylabel("Temps d'exécution en secondes")
  plt.title("Temps moyens observés lors de l'exécution de: "+name)
  plt.grid(True)

  plt.savefig(name+"_plot.png")
  plt.savefig(name+"_plot.pdf")

  plt.show()

  plt.close()

def main(input_names):
    for names in input_names:
        averages, stdevs = [], []
        for name in names:
            xs, ys = [], []
            with open(name+"_perf_eval.csv", "r") as f:
                lines = f.readlines()
            if len(lines) <= 1:
                continue
            for l in lines[1:]:
                elems = l.split(",")
                threads, time = int(elems[0]), float(elems[1])
                xs.append(threads)
                ys.append(time)
            curr_averages, curr_stdevs = [], []
            n_threads = np.arange(1, int(lines[-1][0]) + 1)
            for n in n_threads:
                filtered_y = [ys[i] for i in range(len(ys)) if xs[i] == n]
                curr_averages.append(np.average(filtered_y))
                curr_stdevs.append(np.std(filtered_y))

            averages.append(curr_averages)
            stdevs.append(curr_stdevs)
        if(names[0] == "test_and_set"):
            labels = ["test_and_set", "test_and_test_and_set"]
        else:
            labels = ["POSIX", "Test_and_test_and_set"]
        show_plots(n_threads, averages, stdevs, name, labels)


if __name__ == "__main__":
  input_names=[["out/philosophes","out/our_philosophes"],
               ["out/producer_consumer","out/our_producer_consumer"],
               ["out/reader_writer","out/our_reader_writer"],
               ["test_and_set","test_and_test_and_set"]]

  main(input_names)
