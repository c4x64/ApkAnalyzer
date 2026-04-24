import os
import sys

def analyze_file(path):
    if not os.path.exists(path):
        print(f"Error: {path} not found")
        return

    size = os.path.getsize(path)
    with open(path, 'rb') as f:
        magic = f.read(4)
    
    print(f"File: {os.path.basename(path)}")
    print(f"Size: {size} bytes")
    print(f"Magic: {magic.hex()}")

if __name__ == "__main__":
    if len(sys.argv) > 1:
        analyze_file(sys.argv[1])
    else:
        print("Usage: python file_analyzer.py <file_path>")
