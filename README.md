# ESP-WROOM-02-RGB
ESP-WROOM-02を使ってウエブブラウザからRGB-LEDを制御します
切り替えスイッチでステーションモード／アクセスポイントモードを選択

アクセスポイントモード時
SSID:ESP8266-RGB PW:12345678でアクセスポイントに接続
ブラウザで http://192.168.1.4 を開く

ステーションモード時
既存のアクセスポイントのSSIDとPWをssid.txt、passwd.txtと記述して
SDメモリに保存
起動後液晶に表示されるアドレスをブラウザで開く（アクセスポイントのDHCPにより割り当てられる）

制御ページのHTMLデータはSDメモリに格納

<p>
試作回路画像<br>
<img src="./image/board1.png" width="40%" hight="40%" ><br>
</p>
<p>
ブラウザ画面<br>
<img src="./image/browser.png" width="40%" hight="40%" ><br>
点灯画像<br>
<img src="./image/board2.png" width="40%" hight="40%" ><br>
</p>
