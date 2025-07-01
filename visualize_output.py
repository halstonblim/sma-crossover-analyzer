import argparse
import pandas as pd
import matplotlib.pyplot as plt
from pathlib import Path


def parse_args():
    """Parse command-line arguments."""
    parser = argparse.ArgumentParser(
        description="Visualize price and SMA series from a CSV output file produced by the SMA crossover analyzer.")
    parser.add_argument(
        "file",
        nargs="?",
        default="AAPL_daily_out.csv",
        help="Path to the _out.csv (CSV) file to visualize. Defaults to 'AAPL_daily_out.csv'.",
    )
    return parser.parse_args()


def main():
    args = parse_args()
    csv_path = Path(args.file)

    if not csv_path.exists():
        raise FileNotFoundError(f"File not found: {csv_path}")

    # Derive the crossover file path by replacing the suffix that denotes the SMA output file
    # Accepted variants that users might pass in: "*_out.csv" (produced by main.cpp) or "*.out.csv" (legacy)
    suffix_replacements = [
        ("_out.csv", "_crossovers.csv"),
        (".out.csv", "_crossovers.csv"),
    ]
    crossover_path = None
    for old, new in suffix_replacements:
        if str(csv_path).endswith(old):
            crossover_path = Path(str(csv_path)[: -len(old)] + new)
            break
    # Fallback: append "_crossovers.csv" if no pattern matched
    if crossover_path is None:
        crossover_path = csv_path.with_name(csv_path.stem + "_crossovers.csv")

    # Read crossover information if the file exists
    cross_df = None
    if crossover_path.exists():
        try:
            cross_df = pd.read_csv(crossover_path, parse_dates=["date"])
        except Exception as e:
            print(f"Warning: Failed to read crossover file {crossover_path}: {e}")
    else:
        print(f"Crossover file not found: {crossover_path}. Vertical lines for crosses will not be plotted.")

    # Read the CSV, parsing the first column as dates
    df = pd.read_csv(csv_path, parse_dates=["Date"])

    if "Price" not in df.columns:
        raise ValueError("Expected a 'Price' column in the input file.")

    # Set the date column as index for easier plotting
    df.set_index("Date", inplace=True)

    # Prepare the plot
    plt.figure(figsize=(12, 6))
    plt.plot(df.index, df["Price"], label="Price", linewidth=1.5)

    # Plot any columns that start with 'SMA'
    sma_cols = [col for col in df.columns if col.upper().startswith("SMA")]
    for col in sma_cols:
        plt.plot(df.index, df[col], label=col)

    plt.title(f"Price and SMA Visualization: {csv_path.name}")
    plt.xlabel("Date")
    plt.ylabel("Price")
    plt.legend()
    plt.grid(True, linestyle="--", alpha=0.6)

    # Plot vertical lines for golden and death crosses if available
    if cross_df is not None:
        # Ensure mandatory columns are present
        if {"date", "cross_type"}.issubset({col.lower().strip() for col in cross_df.columns}):
            # Standardize column access using stripped lower-case names
            cross_df.columns = [col.lower().strip() for col in cross_df.columns]
            ymin, ymax = plt.ylim()
            for _, row in cross_df.iterrows():
                color = "green" if row["cross_type"].lower() == "golden" else "red"
                plt.axvline(x=row["date"], color=color, linestyle="--", alpha=0.7)
            # Restore y limits if they were altered
            plt.ylim(ymin, ymax)
        else:
            print(
                f"Crossover file {crossover_path} does not contain required columns 'date' and 'cross_type'. Skipping vertical lines."
            )

    plt.tight_layout()
    # Save the figure to a PNG file next to the CSV
    png_path = csv_path.with_suffix('.png')
    plt.savefig(png_path, dpi=300)
    print(f"Figure saved to {png_path}")
    plt.show()


if __name__ == "__main__":
    main() 