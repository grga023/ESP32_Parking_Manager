import struct
import tkinter as tk
from tkinter import filedialog, messagebox
from tkinter import ttk
import subprocess

# Constants
NAME_SIZE = 20
ID_SIZE = 4
PADDING_SIZE = 8
RECORD_SIZE = NAME_SIZE + ID_SIZE + PADDING_SIZE
END_SEGMENT_PATTERN = b'\xFF' * 32

# Function to read and parse the bin file
def read_bin_file(file_path):
    with open(file_path, 'rb') as f:
        data = f.read()

    persons = []
    for i in range(0, len(data), RECORD_SIZE):
        segment = data[i:i+RECORD_SIZE]

        if segment == END_SEGMENT_PATTERN:
            break  # Stop reading at the end segment

        raw_name = segment[:NAME_SIZE]
        name = raw_name.split(b'\x00', 1)[0].decode('utf-8', errors='replace')
        person_id_bytes = segment[NAME_SIZE:NAME_SIZE+ID_SIZE]
        person_id = ', '.join(f'0x{byte:02X}' for byte in person_id_bytes)
        persons.append((name, person_id, i))  # Include the position
    return persons

# Function to save a new entry to the bin file
def add_entry(file_path, name, person_id_hex):
    if len(name) > NAME_SIZE:
        messagebox.showerror("Error", "Name too long!")
        return

    # Convert the hexadecimal string to bytes
    try:
        person_id = bytes(int(byte, 16) for byte in person_id_hex.split(', '))
    except ValueError:
        messagebox.showerror("Error", "Invalid ID format! Use format like 0x55, 0x35, 0xA7, 0x01")
        return

    if len(person_id) != ID_SIZE:
        messagebox.showerror("Error", "ID must be 4 bytes (e.g., 0x55, 0x35, 0xA7, 0x01)")
        return

    # Check if the end segment exists and adjust where the new data will be added
    with open(file_path, 'rb') as f:
        data = f.read()

    insert_position = data.find(END_SEGMENT_PATTERN)

    if insert_position == -1:
        insert_position = len(data)
    else:
        data = data[:insert_position]  # Truncate the file at the end segment

    name_bytes = name.encode('utf-8').ljust(NAME_SIZE, b'\x00')
    padding = b'\x00' * PADDING_SIZE

    entry = name_bytes + person_id + padding

    with open(file_path, 'wb') as f:
        f.write(data + entry + END_SEGMENT_PATTERN)  # Add the new entry and the end segment back

    messagebox.showinfo("Success", "Entry added successfully!")
    update_table()

# Function to update the table with current bin data
def update_table():
    for row in table.get_children():
        table.delete(row)

    persons = read_bin_file(file_path)
    for idx, person in enumerate(persons):
        table.insert("", "end", values=(person[0], person[1]), iid=idx)

    count_label.config(text=f"Person Count: {len(persons)}")

# Function to browse and load the .bin file
def browse_file():
    global file_path
    file_path = filedialog.askopenfilename(filetypes=[("Binary files", "*.bin")])
    if file_path:
        update_table()

# Function to delete a person from the bin file
def delete_entry():
    selected_item = table.selection()
    if not selected_item:
        messagebox.showerror("Error", "No person selected for deletion.")
        return

    selected_index = int(selected_item[0])
    persons = read_bin_file(file_path)
    _, _, position = persons[selected_index]

    with open(file_path, 'rb') as f:
        data = f.read()

    # Remove the selected entry by slicing the data
    new_data = data[:position] + data[position + RECORD_SIZE:]

    # Write the modified data back to the file
    with open(file_path, 'wb') as f:
        f.write(new_data)

    messagebox.showinfo("Success", "Entry deleted successfully!")
    update_table()

# Function to edit a selected person in the bin file
def edit_entry():
    selected_item = table.selection()
    if not selected_item:
        messagebox.showerror("Error", "No person selected for editing.")
        return

    selected_index = int(selected_item[0])
    persons = read_bin_file(file_path)
    name, _, position = persons[selected_index]

    new_name = name_entry.get()
    new_id_hex = id_entry.get()

    if len(new_name) > NAME_SIZE:
        messagebox.showerror("Error", "Name too long!")
        return

    try:
        new_id = bytes(int(byte, 16) for byte in new_id_hex.split(', '))
    except ValueError:
        messagebox.showerror("Error", "Invalid ID format! Use format like 0x55, 0x35, 0xA7, 0x01")
        return

    if len(new_id) != ID_SIZE:
        messagebox.showerror("Error", "ID must be 4 bytes (e.g., 0x55, 0x35, 0xA7, 0x01)")
        return

    name_bytes = new_name.encode('utf-8').ljust(NAME_SIZE, b'\x00')
    padding = b'\x00' * PADDING_SIZE

    entry = name_bytes + new_id + padding

    with open(file_path, 'rb+') as f:
        f.seek(position)
        f.write(entry)

    messagebox.showinfo("Success", "Entry edited successfully!")
    update_table()

def run_esptool(operation):
    if not file_path:
        messagebox.showerror("Error", "No file selected.")
        return

    exe_path = "esptool.exe" 
    port = "COM12"
    baud = "921600"
    size = "0x1000"
    flash_address = "0x3FA000"

    if operation == "Dump":
        command = [exe_path, '--port', port, '--baud', baud, 'read_flash', flash_address, size, file_path]
    elif operation == "Flash":
        command = [exe_path, '--port', port, '--baud', baud, 'write_flash', flash_address, file_path]
    
    try:
        # Run the command
        subprocess.run(command, check=True)
        messagebox.showinfo("Success", "Executable ran successfully!")
    except subprocess.CalledProcessError as e:
        messagebox.showerror("Error", f"Error running executable: {e}")

# GUI setup
root = tk.Tk()
root.title("Person Bin Manager")

frame = tk.Frame(root)
frame.pack(pady=20)

# Table to display the data
columns = ("Name", "ID")
table = ttk.Treeview(frame, columns=columns, show="headings", height=10)
table.heading("Name", text="Name")
table.heading("ID", text="ID")
table.pack()

# Input fields for name and id
name_label = tk.Label(root, text="Name:")
name_label.pack(pady=5)
name_entry = tk.Entry(root)
name_entry.pack(pady=5)

id_label = tk.Label(root, text="ID (e.g., 0x55, 0x35, 0xA7, 0x01):")
id_label.pack(pady=5)
id_entry = tk.Entry(root)
id_entry.pack(pady=5)

# Add entry button
add_button = tk.Button(root, text="Add Entry", command=lambda: add_entry(file_path, name_entry.get(), id_entry.get()))
add_button.pack(pady=5)

# Edit entry button
edit_button = tk.Button(root, text="Edit Entry", command=edit_entry)
edit_button.pack(pady=5)

# Delete entry button
delete_button = tk.Button(root, text="Delete Entry", command=delete_entry)
delete_button.pack(pady=5)

# File browser button
browse_button = tk.Button(root, text="Browse .bin File", command=browse_file)
browse_button.pack(pady=5)

# Run esptool.py button
flash_esptool_button = tk.Button(root, text="Flash", command=lambda: run_esptool("Flash"))
flash_esptool_button.pack(pady=5)

Dump_esptool_button = tk.Button(root, text="Dump", command=lambda: run_esptool("Dump"))
Dump_esptool_button.pack(pady=5)

# Label to display the person count
count_label = tk.Label(root, text="Person Count: 0")
count_label.pack(pady=5)

root.mainloop()
