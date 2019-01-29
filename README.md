# mkz4_practice

[MKZ4](https://maker.cerevo.com/ja/mkz4/) を用いた実習のソースコード。

## FAQ

### 複数台が同時に授業時間内で使われるのでWLANが混線する

> To set up password protected network, or to configure additional network parameters, use the following overload:  
> `WiFi.softAP(ssid, password, channel, hidden, max_connection)`

ref: https://github.com/esp8266/Arduino/blob/master/doc/esp8266wifi/soft-access-point-class.rst#softap

* [Wi-Fi Analyzer](https://play.google.com/store/apps/details?id=com.vrem.wifianalyzer&hl=ja) とかを使って空いてるチャンネルを探し、10chを使用するなら `WiFi.softAP(ssid, password, 10);` のように指定してみる

### listen? lesson でなくて？

仕様です。出題者に聞いてください。