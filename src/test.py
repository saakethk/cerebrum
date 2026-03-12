""" Tests all data functions """
import unittest

from data.connector import *
from data.retrieval import *
from data.data import *

class TestData(unittest.TestCase):
  db_file: str = "data/duck.db"
  
  

if __name__ == '__main__':
  unittest.main()