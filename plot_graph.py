from sys import argv
import numpy as np
import matplotlib.pyplot as plt
from numpy.lib.function_base import average

def show_plots(xs, ys, n_threads, averages, stdevs, name):
  """
  calculer la moyenne
  calculer les ecarts-types
  plot le tout avec matplotlib
  """
  fig1 = plt.figure()
  std_scaling = 4
  #plt.plot(n_threads,averages,color="red",linewidth=1.0, linestyle="-")
  plt.errorbar(n_threads, averages, yerr=[std * std_scaling for std in stdevs],ecolor="black",capsize=10.0)
  plt.xlim(0, max(n_threads) + 1)

  plt.xlabel("Nombre de threads")
  plt.ylabel("Temps d'exécution en secondes")
  plt.title("Temps moyens observés lors de l'exécution de: "+name)
  #plt.legend(['Bruxelles','Marseille'], loc = 'upper right')
  plt.grid(True)

  plt.savefig(name+"_plot.png")
  plt.savefig(name+"_plot.pdf")

  plt.show()

  plt.close()

def main(input_names):
    for name in input_names:
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
      averages, stdevs = [], []
      n_threads = np.arange(1, int(lines[-1][0]) + 1)
      print(n_threads)
      for n in n_threads:
        filtered_y = [ys[i] for i in range(len(ys)) if xs[i] == n]
        averages.append(np.average(filtered_y))
        stdevs.append(np.std(filtered_y))
      print(averages)
      print(stdevs)
      show_plots(xs, ys, n_threads, averages, stdevs, name)

if __name__ == "__main__":
  input_names=["philosophes", "producer_consumer", "reader_writer"]
  main(input_names)
