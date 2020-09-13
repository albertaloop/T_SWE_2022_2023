import pandas as pd
import time
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation
from itertools import count

index = count()
time_step = []
estimates = []
states = []

sample_space = 100


def animate(i):
    # Clear the graph after 40 iterations
    x = next(index)
    counter = x
    print(counter)
    time_step.append(x)
    if counter > 40:
        time_step.pop(0)
        estimates.pop(0)
        states.pop(0)
        counter = 0
        plt.cla()

    # * To Do *

    '''
    Append to "states" and "estimates" with respective values from pandas dataframe, then
    call the "plot" function
    https://pythonprogramming.net/live-graphs-matplotlib-tutorial/
    https://www.tutorialspoint.com/python_pandas/python_pandas_dataframe.htm
    '''

    ax.legend(["Actual State", "Estimation"])
    ax.set_xlabel("Time Step")
    ax.set_ylabel("Position")
    plt.title('Kalman Filter Estimation')
    time.sleep(.05)  # keep refresh rate of 0.25 seconds


if __name__ == "__main__":
    # * To Do *

    '''
    Read CSV file.
    '''

    # Plot results
    fig, ax = plt.subplots()
    ani = FuncAnimation(plt.gcf(), animate, sample_space)
    plt.tight_layout()
    plt.show()
