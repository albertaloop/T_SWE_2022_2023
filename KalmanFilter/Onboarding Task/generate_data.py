"""
Instantiate filter class, and initialize x, F, H, P, R, and Q  https://github.com/rlabbe/filterpy
"""
from filterpy.kalman import KalmanFilter, predict, update
from filterpy.common.discretization import Q_discrete_white_noise
import numpy as np
import pandas as pd

sample_space = 100
state = []
measurements = []
estimates = []

# * To Do *


def generate_state():

    # * To Do *
    """
    Create a list of "states" consisting of data points from a curve.  This curve can represent the trajectory
    of a moving vehicle (an example of a curve could be y = 2x).
    """


def generate_measurements():

    # * To Do *
    """
    Create a list of "measurements" by adding a list of random floating point numbers to the "state" list,
    resulting in a new list of noisy measurements.
    https://docs.scipy.org/doc/numpy-1.15.1/reference/generated/numpy.random.randn.html
    """


def filter_measurements():

    # * To Do *
    """
    Create a list of "estimates", by using the predict and update methods of the kf class.
    https://github.com/rlabbe/filterpy
    """


def output_data():

    # * To Do *
    """
    Create a dictionary using the measurements and state lists.  Create a pandas dataframe and save
    the dataframe to a CSV file. https://www.geeksforgeeks.org/saving-a-pandas-dataframe-as-a-csv/
    """


if __name__ == "__main__":
    generate_state()
    generate_measurements()
    filter_measurements()
    output_data()
