#include <Arduino.h>
#include <PS4Controller.h>

struct ControlData{
  int8_t left_x; //左スティックX  -128~128の範囲 8bit
  int8_t left_y; //左スティックY -128~128の範囲

  uint8_t catching : 1; //  ボール回収 0-1 1bit
  uint8_t r_launching : 1; //右発射 0-1
  uint8_t l_launching : 1; //左発射  0-1

  uint8_t left : 1 ;   //左旋回 0-1
  uint8_t right : 1;   //右旋回 0-1
};

void printControlData(const ControlData& data) {
  Serial.println("Control Data:");
  Serial.print("Left Stick X: "); Serial.println(data.left_x);
  Serial.print("Left Stick Y: "); Serial.println(data.left_y);
  
  Serial.print("Square: "); Serial.println(data.catching ? "Pressed" : "Not Pressed");
  Serial.print("Circle: "); Serial.println(data.r_launching ? "Pressed" : "Not Pressed");
  Serial.print("Triangle: "); Serial.println(data.l_launching ? "Pressed" : "Not Pressed");

  Serial.print("Left: "); Serial.println(data.left ? "Pressed" : "Not Pressed");
  Serial.print("Right: "); Serial.println(data.right ? "Pressed" : "Not Pressed");
}

void setup() {
  // シリアル初期化
  Serial.begin(115200);
  Serial2.begin(115200); // TX=16,RX=17 がデフォルト
  PS4.begin("08:B6:1F:ED:5D:A6"); 
}

void loop(){
  if(!PS4.isConnected()){
    Serial.println("disconnect");
    return;
  }

  // Serial.println("connect");
  ControlData ps; 
  ps.left_x = abs(PS4.LStickX()) > 30 ? PS4.LStickX() : 0;
  ps.left_y = abs(PS4.LStickY()) > 30 ? PS4.LStickY() : 0;

  ps.catching = PS4.Square();
  ps.r_launching = PS4.R1();
  ps.l_launching = PS4.L1();

  ps.left = PS4.Left();
  ps.right = PS4.Right();
  
  uint32_t tx_data = 0;
  memcpy(&tx_data, &ps, sizeof(ps));// データを64ビット変数にコピー

  printControlData(ps);
  // 送信
  Serial2.write("\n");//開始
  Serial2.write(reinterpret_cast<uint8_t*>(&tx_data), sizeof(tx_data));
  Serial2.write("\n");//終了
  // 100msの遅延を追加 (送信頻度を制御)
  delay(100);
}