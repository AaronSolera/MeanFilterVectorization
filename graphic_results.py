#!/usr/bin/env python
import pandas as pd
import matplotlib.pyplot as plt

GRAPH_TITLE = "Execution Time vs Width"
X_TITLE     = "Width"
Y_TITLE     = "Execution time (s)"

#----------------------------------------------------------------------
# Butterfly image execution time graph
#----------------------------------------------------------------------
butterfly_data = pd.read_csv("butterfly_execution_time.csv")

x          = butterfly_data.iloc[:,0]
y_serial   = butterfly_data.iloc[:,1]
y_parallel = butterfly_data.iloc[:,2]

plt.figure(figsize=(5,3), dpi=100)
plt.gcf().subplots_adjust(bottom=0.16)

plt.plot(x, y_serial, label="Serial time")
plt.plot(x, y_parallel, label="Parallel time")

plt.grid(True)

plt.title(GRAPH_TITLE)
plt.xlabel(X_TITLE)
plt.ylabel(Y_TITLE)
plt.legend()

plt.savefig("butterfly-execution-time-graph.png", dpi=300)

plt.show()

#----------------------------------------------------------------------
# Radiograph image execution time graph
#----------------------------------------------------------------------
radiograph_data = pd.read_csv("radiograph_execution_time.csv")

x          = radiograph_data.iloc[:,0]
y_serial   = radiograph_data.iloc[:,1]
y_parallel = radiograph_data.iloc[:,2]

plt.figure(figsize=(5,3), dpi=100)
plt.gcf().subplots_adjust(bottom=0.16)

plt.plot(x, y_serial, label="Serial time")
plt.plot(x, y_parallel, label="Parallel time")

plt.grid(True)

plt.title(GRAPH_TITLE)
plt.xlabel(X_TITLE)
plt.ylabel(Y_TITLE)
plt.legend()

plt.savefig("radiograph-execution-time-graph.png", dpi=300)

plt.show()
