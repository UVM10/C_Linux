#include "BluetoothSerial.h"

BluetoothSerial SerialBT;

// Khai báo chân kết nối với TB6612FNG
const int pwmA = 18;  // PWM cho Motor A
const int in1  = 19;  // Điều khiển Motor A
const int in2  = 21;  // Điều khiển Motor A

const int pwmB = 23;  // PWM cho Motor B
const int in3  = 22;  // Điều khiển Motor B
const int in4  = 5;   // Điều khiển Motor B

const int STANDBY = 4;  // Chân STANDBY của TB6612FNG
a
// Cấu hình PWM cho ESP32
const int pwmFreq = 5000;       // Tần số PWM (Hz)
const int pwmResolution = 8;    // Độ phân giải PWM (8-bit: 0-255)
const int motorA_channel = 0;   
const int motorB_channel = 1;   

void setup() {
  Serial.begin(115200);
  SerialBT.begin("ESP32Car"); // Tên Bluetooth của thiết bị

  // Cấu hình các chân output
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  pinMode(STANDBY, OUTPUT);
  digitalWrite(STANDBY, HIGH);  // Bật driver

  // Thiết lập PWM cho các kênh motor
  ledcSetup(motorA_channel, pwmFreq, pwmResolution);
  ledcAttachPin(pwmA, motorA_channel);
  
  ledcSetup(motorB_channel, pwmFreq, pwmResolution);
  ledcAttachPin(pwmB, motorB_channel);
}

void loop() {
  if (SerialBT.available()) {
    char cmd = SerialBT.read();
    Serial.print("Nhận lệnh: ");
    Serial.println(cmd);
    
    switch (cmd) {
      case 'F': // Tiến
        forward();
        break;
      case 'B': // Lùi
        backward();
        break;
      case 'L': // Rẽ trái
        turnLeft();
        break;
      case 'R': // Rẽ phải
        turnRight();
        break;
      case 'S': // Dừng xe
        stopMotors();
        break;
      default:
        Serial.println("Lệnh không xác định");
        break;
    }
  }
}

// Hàm điều khiển xe tiến
void forward() {
  // Motor A
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  ledcWrite(motorA_channel, 200); // Tốc độ 200/255
  
  // Motor B
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  ledcWrite(motorB_channel, 200);
}

// Hàm điều khiển xe lùi
void backward() {
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  ledcWrite(motorA_channel, 200);
  
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
  ledcWrite(motorB_channel, 200);
}

// Hàm điều khiển xe rẽ trái
void turnLeft() {
  // Tạm dừng Motor A để tạo hiệu ứng quay trái
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  ledcWrite(motorA_channel, 0);
  
  // Motor B tiến
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  ledcWrite(motorB_channel, 200);
}

// Hàm điều khiển xe rẽ phải
void turnRight() {
  // Motor A tiến
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  ledcWrite(motorA_channel, 200);
  
  // Tạm dừng Motor B
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
  ledcWrite(motorB_channel, 0);
}

// Hàm dừng xe
void stopMotors() {
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  ledcWrite(motorA_channel, 0);
  
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
  ledcWrite(motorB_channel, 0);
}
