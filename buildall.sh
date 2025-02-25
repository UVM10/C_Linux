#!/usr/bin/env bash
# build.sh - Biên dịch các file C thành file thực thi tùy chọn

# Cách dùng:
#   ./build.sh [tên_file_thực_thi] [danh_sách_file_C...]
# Nếu không truyền gì, mặc định tên file thực thi = "program"
# và biên dịch tất cả *.c trong thư mục.

# 1) Đặt tên file thực thi (mặc định = "program")
OUTPUT="${1:-program}"

# 2) Bỏ tham số đầu (vì đã lấy làm OUTPUT), còn lại là danh sách file C
shift

# 3) Nếu không truyền file C, mặc định là tất cả *.c
if [ $# -gt 0 ]; then
    SRC_FILES="$@"
else
    SRC_FILES=$(ls *.c 2>/dev/null)
fi

CFLAGS="-Wall -Wextra -O2"

if [ -z "$SRC_FILES" ]; then
    echo "❌ Không tìm thấy file C nào để biên dịch!"
    exit 1
fi

echo "🚀 Đang biên dịch các file: $SRC_FILES"
gcc $CFLAGS $SRC_FILES -o "$OUTPUT"

if [ $? -eq 0 ]; then
    echo "✅ Biên dịch thành công! File thực thi: ./$OUTPUT"
else
    echo "❌ Lỗi biên dịch!"
    exit 1
fi

