#include <Arduino.h>
#include <PS4Controller.h>

struct ControlData{
  int8_t left_x; //左スティックX  -128~128の範囲 8bit
  int8_t left_y; //左スティックY -128~128の範囲
  int8_t right_x;//右スティックX 128~128の範囲
  int8_t right_y;//右スティックY 128~128の範囲

  uint8_t square : 1; //四角ボタン、0-1 1bit
  uint8_t circle : 1; //丸ボタン 0-1
  uint8_t triangle : 1; //三角ボタン  0-1
  uint8_t cross : 1;    //バツボタン  0-1
  
  uint8_t up : 1; //0-1
  uint8_t down : 1;    //0-1
  uint8_t left : 1 ;   //0-1
  uint8_t right : 1;   //0-1
};

void printControlData(const ControlData& data) {
  Serial.println("Control Data:");
  Serial.print("Left Stick X: "); Serial.println(data.left_x);
  Serial.print("Left Stick Y: "); Serial.println(data.left_y);
  Serial.print("Right Stick X: "); Serial.println(data.right_x);
  Serial.print("Right Stick Y: "); Serial.println(data.right_y);

  Serial.print("Square: "); Serial.println(data.square ? "Pressed" : "Not Pressed");
  Serial.print("Circle: "); Serial.println(data.circle ? "Pressed" : "Not Pressed");
  Serial.print("Triangle: "); Serial.println(data.triangle ? "Pressed" : "Not Pressed");
  Serial.print("Cross: "); Serial.println(data.cross ? "Pressed" : "Not Pressed");

  Serial.print("Up: "); Serial.println(data.up ? "Pressed" : "Not Pressed");
  Serial.print("Down: "); Serial.println(data.down ? "Pressed" : "Not Pressed");
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
  ps.right_x = abs(PS4.RStickX()) > 30 ? PS4.RStickX() : 0;
  ps.right_y = abs(PS4.RStickY()) > 30 ? PS4.RStickY() : 0;

  ps.square = PS4.Square();
  ps.circle = PS4.Circle();
  ps.triangle = PS4.Triangle();
  ps.cross = PS4.Cross();

  ps.up = PS4.Up();
  ps.down = PS4.Down();
  ps.left = PS4.Left();
  ps.right = PS4.Right();
  
  uint64_t txData = 0;
  memcpy(&txData, &ps, sizeof(ps));// データを64ビット変数にコピー

  printControlData(ps);
  // 送信
  Serial2.write("\n");//開始
  Serial2.write(reinterpret_cast<uint8_t*>(&txData), sizeof(txData));
  Serial2.write("\n");//終了
  // 100msの遅延を追加 (送信頻度を制御)
  delay(100);
}