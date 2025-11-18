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


def main() -> None:
    is_running = True

    while is_running:
        density = FloatPrompt.ask(
            "Enter the density of the sparse matrix (0.01 <= density <= 1)", default=0.1
        )
        density = max(0.01, min(1.0, density))  # Clamp density between 0.01 and 1

        m = IntPrompt.ask("Enter the number of rows", default="1000000")
        n = IntPrompt.ask("Enter the number of columns", default="1000000")
        filename = f"mm_matrix_{m}x{n}_density{density:.2f}.mtx"

        # Idk why, but  
        save_mm_matrix(generate_mm_matrix(int(m), int(n), density), filename)

        is_running = Confirm.ask(
            "Do you want to generate another matrix?", default=False
        )


if __name__ == "__main__":
    main()
