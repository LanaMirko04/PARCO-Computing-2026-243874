# Tools
This directory contains various tools (scripts) used for matrix generation and result analysis. 
Each tool is implemented as a Python script (or bash script) and can be executed from the command line.

## Usage
The first thing you need to do is to create a virtual environment and install the required dependencies listed in the `requirements.txt` file. You can do this by executing the following commands in your terminal:

```shell
$ python -m venv .venv
$ source .venv/bin/activate  # If you are using fish shell use: source .venv/bin/activate.fish
$ pip install -r tools/requirements.txt
```

Once the dependencies are installed, you can run any of the scripts in this directory using Python. For example, to run the matrix generator script, you would execute:

```shell
$ python tools/mmgen.py
```

## Available Tools
The following tools are included in this directory:

- **`mmgen.py`**: A script to generate sparse matrices in Matrix Market format. You can specify the size and density of the matrix to be generated.
