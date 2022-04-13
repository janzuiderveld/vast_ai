""" This script waits for new scan file to appear in the systems FTP folder,
sends them to the sketch to image server, waits for the returned image and 
finally prints it on the Xerox Machine.
"""

import os
import sys
import time
import ftplib
import argparse
import logging
import logging.handlers
import configparser
import datetime
import subprocess
import re
import shutil
import glob
import requests



# make it work 
