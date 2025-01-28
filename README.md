
# FastFrames

This repository is designed for producing metadata and running the FastFrames package for analysis tasks. Below are the steps to set up and run the package on LXPlus.

## Setup

Before running any commands, ensure you have sourced the `setup.sh` script to set up the environment:

```bash
source setup.sh
```

## Producing Metadata

To produce metadata files, use the following Python command. This will generate the necessary metadata files from the input ROOT files:

```bash
python3 FastFrames/python/produce_metadata_files.py --root_files_folder <input path> --output_path <output path>
```

Replace `<input path>` with the folder containing the input ROOT files and `<output path>` with the desired output directory for the metadata files.

## Running the Package

To run the FastFrames package, use the following command with the appropriate configuration file and step number:

```bash
python3 FastFrames/python/FastFrames.py --config <path to config file> --step n
```

Here, replace `n` with the necessary step you wish to run.

## Example Usage

1. Source the environment setup:
   ```bash
   source setup.sh
   ```

2. Produce metadata files (assuming the input files are in `/path/to/root_files` and you want to output metadata to `/path/to/output`):
   ```bash
   python3 python/produce_metadata_files.py --root_files_folder /path/to/root_files --output_path /path/to/output
   ```

3. Run the FastFrames package (assuming step `1` for example):
   ```bash
   python3 python/FastFrames.py --config /path/to/config.yml --step n
   ```

## Notes

- Ensure you have all dependencies installed as specified in the `setup.sh` file.
- Modify the paths and configuration files according to your specific setup.
