# ESP-WROOM-02-RGB
<p>
ESP-WROOM-02を使ってウエブブラウザからRGB-LEDを制御します<br>
切り替えスイッチでステーションモード／アクセスポイントモードを選択<br>
</p>
<p>
アクセスポイントモード時<br>
SSID:ESP8266-RGB PW:12345678でアクセスポイントに接続<br>
ブラウザで http://192.168.1.4 を開く<br>
</p>
<p>
ステーションモード時<br>
既存のアクセスポイントのSSIDとPWをssid.txt、passwd.txtに記述して<br>
SDメモリに保存<br>
起動後液晶に表示されるアドレスをブラウザで開く<br>
（アドレスはアクセスポイントのDHCPにより割り当てられるものとする）<br>
</p>
<p>
制御ページのHTMLデータはSDメモリに格納<br>
</p>
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
