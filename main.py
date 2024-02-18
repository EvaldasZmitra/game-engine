import subprocess

sh = """
#!/bin/bash

CC=gcc
echo $(echo $CC)
"""

subprocess.run(sh, shell=True)
