import matplotlib.pyplot as plt
import numpy as np

# Đường dẫn đến file dữ liệu
file_path = "D:\\Python\\Lab\\Raw_data_2_ports.txt"

# Khởi tạo danh sách dữ liệu
raw_data_a = []
raw_data_b = []

# Đọc dữ liệu từ file
with open(file_path, "r") as file:
    for line in file:
        line = line.strip()
        if "Raw Data a:" in line:
            value = int(line.split(":")[1].strip())
            raw_data_a.append(value)
        elif "Raw Data b:" in line:
            value = int(line.split(":")[1].strip())
            raw_data_b.append(value)

# Kiểm tra nếu không có dữ liệu
if not raw_data_a and not raw_data_b:
    print("Không tìm thấy dữ liệu trong file!")
    exit()

# Tạo trục thời gian (giả sử mỗi giá trị tương ứng với một đơn vị thời gian)
time_axis_a = np.arange(len(raw_data_a))
time_axis_b = np.arange(len(raw_data_b))

# Vẽ hai đồ thị trên cùng một cửa sổ
plt.figure(figsize=(12, 8))

# Đồ thị Raw Data a
plt.subplot(2, 1, 1)
plt.plot(time_axis_a, raw_data_a, label="Raw Data a", color="blue", marker="o")
plt.title("Raw Data a")
plt.xlabel("Time (Sample Unit)")
plt.ylabel("Raw data a (Value)")
plt.grid(True)
plt.legend()

# Đồ thị Raw Data b
plt.subplot(2, 1, 2)
plt.plot(time_axis_b, raw_data_b, label="Raw Data b", color="red", marker="x")
plt.title("Raw Data b")
plt.xlabel("Time (Sample Unit)")
plt.ylabel("Raw data b (Value)")
plt.grid(True)
plt.legend()

# Hiển thị đồ thị
plt.tight_layout()
plt.show()