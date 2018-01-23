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
使用するAPのSSIDとPASSが未設定の場合USBシリアルポートに接続した<br>
PCから設定を行い内部フラッシュに保存される<br>
起動後液晶に表示されるアドレスをブラウザで開く<br>
（アドレスはアクセスポイントのDHCPにより割り当てられるものとする）<br>
動作中にUSBシリアルポートに接続したPCで"x"キーを押すと<br>
設定クリアするかを聞いてくるので"y"キーを押すと設定がクリアされる<br>
PCはTeraTermで115200bps8bitパリティ無しストップ1フロー無しで接続
</p>
<p>
制御ページのHTMLデータはSDメモリに格納<br>
</p>
<p>
開発環境は　Arduino IDE + ESP8266アドオン　を使用
</p>
<p>
</p>
<p>
</p>
<p>
試作回路画像<br>
<img src="./image/board1.png" width="40%" hight="40%" ><br>
</p>
<p>
ブラウザ画面<br>
<img src="./image/browser.png" width="40%" hight="40%" ><br>
</p>
<p>
点灯画像<br>
<img src="./image/board2.png" width="40%" hight="40%" ><br>
</p>
