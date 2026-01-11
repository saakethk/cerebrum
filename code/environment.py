""" Define environment via class for reinforcement learning """
from __future__ import absolute_import
from __future__ import division
from __future__ import print_function
import os
import abc
import tensorflow as tf
import numpy as np
from tf_agents.environments import py_environment
from tf_agents.environments import tf_environment
from tf_agents.environments import tf_py_environment
from tf_agents.environments import utils
from tf_agents.specs import array_spec
from tf_agents.environments import wrappers
from tf_agents.environments import suite_gym
from tf_agents.trajectories import time_step as ts

from data import Data

# Keep using keras-2 (tf-keras) rather than keras-3 (keras).
os.environ['TF_USE_LEGACY_KERAS'] = '1'

class StockEnv(py_environment.PyEnvironment):

  def __init__(self):
      self._state = None
      self._target = 0
      self._target_reward = 0
      self._episode_ended = True
      self._action_spec = array_spec.BoundedArraySpec(
          shape=(), dtype=np.int32, minimum=0, maximum=2, name='action') # 0 - buy, 1 - hold, 2 - sell
      self._observation_spec = array_spec.BoundedArraySpec(
          shape=(7, ), dtype=np.float32, minimum=0, name='observation') # Past 7 fluctuations

  def action_spec(self): # type: ignore
      return self._action_spec

  def observation_spec(self): # type: ignore
      return self._observation_spec

  def _reset(self): # type: ignore
      # Gets new sample of stock prices - Sets up environment
      observations, expected_value = Data.get_subset(symbol="AAPL", seq_size=7)
      self._state = np.array(observations["pct_change"].values, dtype=np.float32)
      # Determines correct answer
      if expected_value["encoded_buy"] == 1:
          self._target = 0
      elif expected_value["encoded_hold"] == 1:
          self._target = 1
      elif expected_value["encoded_sell"] == 1:
          self._target = 2
      # Determines reward to be issued if correct answer is chosen
      self._target_reward = expected_value["encoded_strength"]
      self._episode_ended = False
      return ts.restart(self._state)

  def _step(self, action): # type: ignore
      if self._episode_ended:
          return self.reset()
      
      if action == self._target_reward:
          # Positive reinforcement
          return ts.termination(self._state, self._target_reward)
      else:
          # Negative reinforcement
          return ts.termination(self._state, -self._target_reward)

    # if self._episode_ended or self._state >= 21:
    #   reward = self._state - 21 if self._state <= 21 else -21
    #   return ts.termination(np.array([self._state], dtype=np.int32), reward)
    # else:
    #   return ts.transition(
    #       np.array([self._state], dtype=np.int32), reward=0.0, discount=1.0)

if __name__ == "__main__":
    environment = StockEnv()
    utils.validate_py_environment(environment, episodes=5)