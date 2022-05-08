#!/bin/python

# Standard Library
import sys
from typing import (Optional)

def open_output_handle(filename:Optional[str] = None, mode:str = "w+"):
    """
    Opens file and returns handle, or returns handle to stdout

    Parameters
    ----------
    filename : Optional[str]
        path to file, or None (use stdout)
    mode : str
        file mode

    Returns
    -------
    filehandle:
        handle to opened file, or sys.stdout
    """
    if filename:
        return open(filename, mode)
    else:
        return sys.stdout
