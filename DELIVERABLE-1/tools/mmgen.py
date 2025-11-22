import argparse
import csv
from typing import List, Tuple

import scipy as sp
from rich.prompt import Confirm, FloatPrompt, IntPrompt


def generate_mm_matrix(
    m: int, n: int, density: float, dtype: type = float
) -> sp.sparse.coo_matrix:
    """
    Generate a random sparse matrix in COO format.

    Parameters:
    m (int): Number of rows.
    n (int): Number of columns.
    density (float): Density of the sparse matrix (between 0 and 1).

    Returns:
    sp.sparse.coo_matrix: Generated sparse matrix in COO format.
    """
    matrix = sp.sparse.random(m, n, density=density, format="coo", dtype=dtype)
    return matrix


def save_mm_matrix(matrix: sp.sparse.coo_matrix, filename: str) -> None:
    """
    Save a sparse matrix in Matrix Market format.

    Parameters:
    matrix (sp.sparse.coo_matrix): Sparse matrix to save.
    filename (str): Path to the output file.
    """
    sp.io.mmwrite(filename, matrix)


def make_parser() -> argparse.ArgumentParser:
    """
    Create an argument parser for the script.

    Returns:
    argparse.ArgumentParser: Configured argument parser.
    """
    parser = argparse.ArgumentParser(
        description="Generate a random sparse matrix in Matrix Market format."
    )
    parser.add_argument(
        "-i",
        "--input",
        type=str,
        required=False,
        help="Path to file that describes the matricies to generate.",
    )
    return parser


def parse_csv_file_prompt(filename: str) -> List[dict]:
    """
    Parse a CSV file containing matrix specifications.

    Parameters:
    filename (str): Path to the CSV file.

    Returns:
    List[dict]: List of matrix specifications.
    """
    specs = []
    with open(filename, "r") as file:
        csv_reader = csv.DictReader(file)
        for row in csv_reader:
            specs.append(row)
    return specs


def main() -> None:
    """
    Main function to run the script.
    """
    parser = make_parser()
    args = parser.parse_args()

    if args.input:
        matricies_to_generate = parse_csv_file_prompt(args.input)
        for spec in matricies_to_generate:
            print(spec)
            m = int(spec["row"])
            n = int(spec["col"])
            density = float(spec["density"])
            filename = spec.get(
                "filename", f"mm_matrix_{m}x{n}_density{density:.2f}.mtx"
            )
            save_mm_matrix(generate_mm_matrix(m, n, density), filename)

    else:
        is_running = True

        while is_running:
            density = FloatPrompt.ask(
                "Enter the density of the sparse matrix (0.01 <= density <= 1)",
                default=0.1,
            )
            density = max(0.01, min(1.0, density))  # Clamp density between 0.01 and 1

            m = IntPrompt.ask("Enter the number of rows", default="1000000")
            n = IntPrompt.ask("Enter the number of columns", default="1000000")
            filename = f"mm_matrix_{m}x{n}_density{density:.2f}.mtx"
            save_mm_matrix(generate_mm_matrix(int(m), int(n), density), filename)

            is_running = Confirm.ask(
                "Do you want to generate another matrix?", default=False
            )


if __name__ == "__main__":
    main()
