#include <Arduino.h>
#include <PS4Controller.h>

struct ControlData{
  int8_t left_x; //左スティックX  -128~128の範囲 8bit
  int8_t left_y; //左スティックY -128~128の範囲
  int8_t right_x;//右スティックX 128~128の範囲
  int8_t right_y;//右スティックY 128~128の範囲

  uint8_t left_firing : 1; //左発射、0-1 1bit
  uint8_t right_firing : 1; //右発射 0-1
  uint8_t collect : 1; //回収  0-1
  uint8_t take : 1;    //巻取  0-1

  uint8_t forward : 1; //0-1
  uint8_t back : 1;    //0-1
  uint8_t left : 1 ;   //0-1
  uint8_t right : 1;   //0-1
};


void setup() {
  // シリアル初期化
  Serial.begin(115200);
  Serial2.begin(115200); // TX=16,RX=17 がデフォルト
  while(!Serial2){
    Serial.println("Error!!");
    delay(100);
  };
  PS4.begin("08:B6:1F:ED:5D:A6"); //TODO macアドレスを書き換える
}
void loop(){
  if(!PS4.isConnected()){
    //Serial2.print(0);
    Serial.println("disconnect");
    return;
  }

  // Serial.println("connect");
  ControlData ppps; 
  ppps.left_x = abs(PS4.LStickX()) > 30 ? PS4.LStickX() : 0;
  ppps.left_y = abs(PS4.LStickY()) > 30 ? PS4.LStickY() : 0;
  ppps.right_x = abs(PS4.RStickX()) > 30 ? PS4.RStickX() : 0;
  ppps.right_y = abs(PS4.RStickY()) > 30 ? PS4.RStickY() : 0;

  ppps.right_firing = PS4.Circle();
  ppps.left_firing = PS4.Square();
  ppps.collect = PS4.Triangle();
  ppps.take = PS4.Cross();

  ppps.forward = PS4.Up();
  ppps.back = PS4.Down();
  ppps.left = PS4.Left();
  ppps.right = PS4.Right();
  
   // データを64ビット変数にコピー
  uint64_t txData = 0;
  memcpy(&txData, &ppps, sizeof(ppps));

  Serial.printf("txData: %x\n", txData);

  // 送信
  Serial2.write(reinterpret_cast<uint8_t*>(&txData), sizeof(txData));

  // 100msの遅延を追加 (送信頻度を制御)
  delay(100);
}