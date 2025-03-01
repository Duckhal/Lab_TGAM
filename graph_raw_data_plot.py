import matplotlib.pyplot as plt
import numpy as np

# Đường dẫn đến file dữ liệu
file_path = "D:\\Python\\Lab\\Raw_data_collected.txt"

# Đọc dữ liệu từ file
raw_data = []
with open(file_path, "r") as file:
    for line in file:
        if "Raw Data:" in line:
            # Trích xuất giá trị rawData từ dòng
            value = int(line.split(":")[1].strip())
            if (value<=32767):
                raw_data.append(value)
            else:
                raw_data.append(value-65536)

# Kiểm tra nếu không có dữ liệu
if not raw_data:
    print("Không tìm thấy dữ liệu rawData trong file!")
    exit()

# Chuyển đổi dữ liệu thành mảng numpy để dễ xử lý
raw_data = np.array(raw_data)

# Tạo trục thời gian (giả sử dữ liệu được thu thập mỗi giây)
time_axis = np.arange(len(raw_data))

# Vẽ đồ thị
plt.figure(figsize=(10, 6))
plt.plot(time_axis, raw_data, label="Raw Data", color="blue", marker="o")
plt.title("Raw Data từ Arduino")
plt.xlabel("Thời gian (s)")
plt.ylabel("Giá trị Raw Data")
plt.grid(True)
plt.legend()
plt.show()