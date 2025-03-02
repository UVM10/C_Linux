#!/bin/bash

# Script khởi động SSH Agent, thêm khóa SSH, và kiểm tra danh sách khóa đã nạp.

# 1. Khởi động SSH Agent
eval "$(ssh-agent -s)" 1>/dev/null

# 2. Đường dẫn đến khóa SSH
# Bạn có thể thay thế "../../.ssh/myLgithub" bằng đường dẫn thật của bạn
KEY_PATH="../../.ssh/myLgithub"

# 3. Kiểm tra khóa SSH có tồn tại không
if [ -f "$KEY_PATH" ]; then
    echo "Đang thêm khóa SSH: $KEY_PATH"
    ssh-add "$KEY_PATH"
else
    echo "Không tìm thấy khóa SSH tại đường dẫn: $KEY_PATH"
    echo "Vui lòng kiểm tra lại đường dẫn hoặc tạo khóa mới."
    exit 1
fi

# 4. Kiểm tra khóa đã được nạp
echo "Các khóa SSH đã nạp:"
ssh-add -l

echo "Hoàn tất! Giờ bạn có thể dùng Git qua SSH mà không cần nhập lại mật khẩu."
