import math
import numpy as np

def rankine_hugoniot(state_1, state_2):
  """
  Applies Rankine-Hugoniot relations to relate the state of the flow at point 1 to the state of the flow at point 2

  Args:
    state_1: The state of the flow at point 1
    state_2: The state of the flow at point 2

  Returns:
    The state of the flow at point 2


    Copyright Christopher Jessop, DSTL, 2023
  """

  p1 = state_1["pressure"]
  p2 = state_2["pressure"]
  u1 = state_1["velocity"]
  u2 = state_2["velocity"]
  rho1 = state_1["density"]
  rho2 = state_2["density"]

  # Calculate shock velocity
  us = (p2 - p1) / (rho1 + rho2)

  # Calculate  state of flow at point 2
  state_2["velocity"] = u2 + us
  state_2["density"] = rho2 * (p2 / p1)

  return state_2

if __name__ == "__main__":
  # Set state of flow at point 1
  state_1 = {
    "pressure": 1000,
    "velocity": 100,
    "density": 1.0,
  }

  # Set state of flow at point 2
  state_2 = {
    "pressure": 500,
    "velocity": 0,
    "density": 0.5,
  }

  # Apply the Rankine-Hugoniot relations
  state_2 = rankine_hugoniot(state_1, state_2)

  # Print the state of the flow at point 2
  print(state_2)