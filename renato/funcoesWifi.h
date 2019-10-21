int getQualidadeWifi(int RSSI) {
  int quality = 0;
  if (RSSI <= -100) {
    quality = 0;
  } else if (RSSI >= -50) {
    quality = 100;
  } else {
    quality = 2 * (RSSI + 100);
  }
  return quality;
}

String getConexoesWifi() {
  int n = WiFi.scanNetworks();
  for (int i = 0; i < n; i++) {
    String message = "";
    String select = "";
    Serial.print("scannetworks:");
    int n = WiFi.scanNetworks();
    
    String cor;
    Serial.print(n);
    select += "<form method='GET' action='conectar'>";
    for (int i = 0; i < n; i++) {
      if (i % 2 == 0) {
        cor = "rgba(156, 233, 252, 0.2)";
      }
      else {
        cor = "rgba(156, 233, 252, 0.1)";
      }
      select += "<label style='display: inline-block; width: 98%; margin: 0 auto; font-size: 50px; padding: 4px; text-align: left; background-color: "+cor+"; border-radius: 4px; margin: 2px;'><input name='rede' type='radio' value='"+String(WiFi.SSID(i))+"'> " + String(WiFi.SSID(i)) + " (" + String(getQualidadeWifi(WiFi.RSSI(i))) + "%)</label>";  
    }
    select += "<br/><input style='border-radius: 10px; height: 55px; padding-left: 10px; width: 90%; margin: 0 auto; font-size: 15px; -webkit-appearance: none; -moz-appearance:textfield;' name='senha' type='password' placeholder='Senha'/><hr/><br/>";
    select += "<input style='border-radius: 10px; height: 55px; padding-left: 10px; width: 50%; margin: 0 auto; font-size: 15px;' class='button' type='submit' value='Entrar'>";
    select += "</form>";
    return select;
  }
}
