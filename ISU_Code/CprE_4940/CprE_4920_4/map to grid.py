import cv2
import os
import sys

# ---------- Dir checks ----------

if not os.path.exists('Screenshots'):
    os.makedirs('Screenshots')
    print("Created 'Screenshots' folder. Put images in there")
    sys.exit(1)

if not os.path.exists('Grids'):
    os.makedirs('Grids')
    print("Created 'Grids' folder.")

files_names = os.listdir('Screenshots')
if len(files_names) == 0:
    print("No images found in the 'Screenshots' folder.")
    sys.exit(1)

# ---------- SELECT THE IMAGE ----------
print("List of all Screenshots to convert: ")
sc_list = []
for number, file_name in enumerate(files_names, start=1):
    sc_list.append(file_name)
    print(f"{number}.) {file_name}")

selected_image = 0
while True:
    try:
        selected_image = int(input("\nEnter which screenshot you would like to convert: "))
        if 0 < selected_image <= len(sc_list):
            break
        print(f"Please enter a number between 1 and {len(sc_list)}")
    except ValueError:
        print(f"Invalid input. Enter a number between 1 and {len(sc_list)}")

image_path = 'Screenshots/' + sc_list[selected_image - 1]

# ---------- GET REAL-WORLD MEASUREMENTS ----------
print("\n--- Map Scale Configuration ---")
print("Use Google Maps 'Measure Distance' tool to find these values.")
try:
    real_width_ft = float(input("Enter the real-world WIDTH of this image in feet: "))
    real_height_ft = float(input("Enter the real-world HEIGHT of this image in feet: "))
    cell_size_ft = float(input("Enter the cell size resolution (e.g., 2.5 for 2.5ft x 2.5ft cells): "))
except ValueError:
    print("Error: You must enter valid numbers. Exiting.")
    sys.exit(1)

# ------------------- Logic -----------------------
def process_map(image_path, output_path, real_w, real_h, cell_size):
    print(f"\nLoading Ground Truth Mask: {image_path}")
    
    # Read the image in grayscale
    img = cv2.imread(image_path, cv2.IMREAD_GRAYSCALE)
    if img is None: 
        sys.exit("Error: Image not found.")

    # Calculate the exact grid dimensions based on the physical measurements
    grid_width = int(real_w / cell_size)
    grid_height = int(real_h / cell_size)

    # Force strictly binary (cleans up any blurry/gray edges)
    _, binary_img = cv2.threshold(img, 127, 255, cv2.THRESH_BINARY)

    # Let OpenCV handle the mathematical downsampling automatically!
    small_grid = cv2.resize(binary_img, (grid_width, grid_height), interpolation=cv2.INTER_AREA)

    # Export to text file
    with open(output_path, 'w') as f:
        # Write the Resolution Header for the C++ code to intercept
        f.write(f"RESOLUTION:{cell_size:.2f}\n")
        
        for y in range(grid_height):
            # White pixel (>127) = drivable (.), Black pixel = wall (#)
            row = ["." if small_grid[y, x] > 127 else "#" for x in range(grid_width)]
            f.write("".join(row) + "\n")
    
    print(f"SUCCESS: Map scaled to {real_w}x{real_h} feet.")
    print(f"Flawless {grid_width}x{grid_height} grid saved to {output_path}")

# --- EXECUTION ---
if __name__ == "__main__":
    name_without_ext = os.path.splitext(sc_list[selected_image - 1])[0]
    output_filepath = "Grids/" + name_without_ext + ".txt"
    
    process_map(image_path, output_filepath, real_width_ft, real_height_ft, cell_size_ft)