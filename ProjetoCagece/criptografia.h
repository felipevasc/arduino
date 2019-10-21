String chave = "FraseDeSegurancaTeste";
  
String criptografar(String mensagem) {
  String resultado = "";
  int letra, letraTmp, key, p = 0;
  char novaLetra;
  
  int tamanho = strlen(mensagem.c_str());
  for (int i = 0; i < tamanho; i++) {
    letra = (int)(mensagem[i]);
    
    key = (int)(chave[p]);
    letraTmp = letra + key;
    while (letraTmp > 126) {
      letraTmp = letraTmp - 94;
    }
    novaLetra = (char)(letraTmp);
    resultado = resultado + novaLetra + (char)(random(33, 126));
    p++;
    if (p >= strlen(chave.c_str())) {
      p = 0;
    }
  }
  return resultado;
}

String descriptografar(String cript_men) {
  String resultado = "";
  int letra, letraTmp, key, p = 0;
  char novaLetra;

  int tamanho = strlen(cript_men.c_str());
  for (int i = 0; i < tamanho; i++) {
    if (i % 2 != 0) {
      continue;
    }
    letra = (int)(cript_men[i]);
    key = (int)(chave[p]);
    letraTmp = letra - key;
    while (letraTmp < 33) {
      letraTmp = letraTmp + 94;
    }
    novaLetra = (char)(letraTmp);
    resultado = resultado + novaLetra;
    p++;
    if (p >= strlen(chave.c_str())) {
      p = 0;
    }
  }
  return resultado;
}
